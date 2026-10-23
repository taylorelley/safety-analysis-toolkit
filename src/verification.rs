use crate::{
    adapters,
    config::{Config, inside, relative},
    model::Run,
};
use anyhow::{Context, Result};
use serde::{Deserialize, Serialize};
use std::path::{Path, PathBuf};
thread_local! {static CONTEXT:std::cell::RefCell<serde_json::Value>=const{std::cell::RefCell::new(serde_json::Value::Null)};}
pub fn context() -> serde_json::Value {
    CONTEXT.with(|c| c.borrow().clone())
}
fn with_context<T>(value: serde_json::Value, f: impl FnOnce() -> T) -> T {
    CONTEXT.with(|c| *c.borrow_mut() = value);
    struct Reset;
    impl Drop for Reset {
        fn drop(&mut self) {
            CONTEXT.with(|c| *c.borrow_mut() = serde_json::Value::Null);
        }
    }
    let _reset = Reset;
    f()
}
#[derive(Serialize)]
pub struct Request {
    pub source: PathBuf,
    pub engine: String,
    pub variant: Option<String>,
    pub method: Option<String>,
    pub unwind: u32,
    pub bits: u32,
    pub entry: String,
    pub harness: Option<PathBuf>,
}
#[derive(Deserialize, Serialize)]
#[serde(deny_unknown_fields)]
struct Analyzer {
    engine: String,
    method: String,
    entry: String,
    bits: u32,
    unwind: Option<u32>,
}
#[derive(Deserialize, Serialize)]
#[serde(deny_unknown_fields)]
struct Harness {
    version: u32,
    source: PathBuf,
    target: String,
    property: String,
    input_assumptions: Vec<String>,
    environment_model: String,
    build_configuration: String,
    requirements: Vec<String>,
    analyzer: Analyzer,
}
pub fn verify(root: &Path, c: &Config, mut request: Request) -> Result<Run> {
    let method = request
        .method
        .get_or_insert_with(|| {
            if request.engine == "frama-c" {
                "eva"
            } else {
                "bmc"
            }
            .into()
        })
        .clone();
    anyhow::ensure!(
        !request.entry.is_empty()
            && request
                .entry
                .chars()
                .all(|c| c.is_ascii_alphanumeric() || c == '_'),
        "invalid proof entry point"
    );
    let harness = if let Some(path) = &request.harness {
        relative(path)?;
        let file = inside(root, path)?;
        let h: Harness = serde_yaml::from_slice(&crate::config::read_bounded(&file, 1024 * 1024)?)?;
        anyhow::ensure!(
            h.version == 1
                && !h.target.is_empty()
                && !h.property.is_empty()
                && !h.environment_model.is_empty()
                && !h.build_configuration.is_empty(),
            "incomplete harness metadata"
        );
        anyhow::ensure!(
            inside(
                root,
                &file.parent().context("harness directory")?.join(&h.source)
            )? == inside(root, &request.source)?,
            "harness source mismatch"
        );
        anyhow::ensure!(
            h.analyzer.engine == request.engine
                && h.analyzer.method == method
                && h.analyzer.entry == request.entry
                && h.analyzer.bits == request.bits
                && (request.engine != "esbmc" || h.analyzer.unwind == Some(request.unwind)),
            "harness analyzer configuration does not match requested proof scope"
        );
        Some(h)
    } else {
        None
    };
    let value = serde_json::json!({"request":request,"harness":harness,"human_approval":"not_granted_by_tool"});
    with_context(value, || {
        Ok(match request.engine.as_str() {
            "frama-c" => adapters::frama_c::prove(
                root,
                c,
                &request.source,
                request.variant.as_deref(),
                &method,
                &request.entry,
                request.bits,
            )?,
            "esbmc" => adapters::esbmc::prove(
                root,
                c,
                &request.source,
                request.variant.as_deref(),
                adapters::esbmc::Options {
                    unwind: request.unwind,
                    bits: request.bits,
                    method: &method,
                    entry: &request.entry,
                },
            )?,
            _ => anyhow::bail!("unsupported verification engine"),
        })
    })
}
