mod adapters;
mod config;
mod discovery;
mod evidence;
mod findings;
mod mcp;
mod model;
mod report;
mod runner;
use anyhow::{Context, Result};
use clap::{Parser, Subcommand, ValueEnum};
use config::Config;

use std::{fs, path::PathBuf};
#[derive(Parser)]
#[command(version, about = "Local C/C++ analysis with reproducible evidence")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}
#[derive(Clone, Copy, ValueEnum)]
enum Format {
    Json,
    Markdown,
    Sarif,
}
#[derive(Subcommand)]
enum Commands {
    Mcp {
        #[arg(long, default_value = ".")]
        path: PathBuf,
    },
    Init {
        #[arg(long, default_value = ".")]
        path: PathBuf,
    },
    Doctor {
        #[arg(default_value = ".")]
        path: PathBuf,
        #[arg(long, value_enum, default_value = "json")]
        format: Format,
    },
    Scan {
        #[arg(default_value = ".")]
        path: PathBuf,
        #[arg(long)]
        profile: Option<String>,
        #[arg(long, value_enum, default_value = "json")]
        format: Format,
    },
    Report {
        #[arg(default_value = ".")]
        path: PathBuf,
        #[arg(long)]
        run: String,
        #[arg(long, value_enum, default_value = "markdown")]
        format: Format,
    },
    Runs {
        #[arg(default_value = ".")]
        path: PathBuf,
        #[arg(long)]
        run: Option<String>,
    },
}
fn root(path: PathBuf) -> Result<PathBuf> {
    let p = path.canonicalize().context("workspace does not exist")?;
    anyhow::ensure!(p.is_dir(), "workspace must be a directory");
    Ok(p)
}
fn emit(run: &model::Run, format: Format) -> Result<()> {
    match format {
        Format::Json => println!("{}", serde_json::to_string_pretty(run)?),
        Format::Markdown => print!("{}", report::markdown(run)),
        Format::Sarif => println!("{}", serde_json::to_string_pretty(&report::sarif(run))?),
    }
    Ok(())
}
fn main() {
    let cli = Cli::parse();
    let is_mcp = matches!(cli.command, Commands::Mcp { .. });
    let result = run(cli);
    let exit = match result {
        Ok(code) => code,
        Err(e) => {
            let error = serde_json::json!({"schema_version":1,"status":"invalid","complete":false,"error":format!("{e:#}")});
            if is_mcp {
                eprintln!("{error}")
            } else {
                println!("{error}")
            };
            2
        }
    };
    std::process::exit(exit)
}
fn run(cli: Cli) -> Result<i32> {
    match cli.command {
        Commands::Mcp { path } => {
            let root = root(path)?;
            tokio::runtime::Runtime::new()?.block_on(mcp::serve(root))?;
            Ok(0)
        }
        Commands::Init { path } => {
            let root = root(path)?;
            let c = Config::new(
                root.file_name()
                    .unwrap_or_default()
                    .to_string_lossy()
                    .into_owned(),
            );
            evidence::write_new(
                &root.join("safety.yaml"),
                serde_yaml::to_string(&c)?.as_bytes(),
            )?;
            println!(
                "{}",
                serde_json::json!({"status":"passed","complete":true,"config":root.join("safety.yaml"),"assurance":"unconfigured; advisory analysis only"})
            );
            Ok(0)
        }
        Commands::Doctor { path, format } => {
            let root = root(path)?;
            let c = Config::load(&root)?;
            let r = discovery::doctor(&root, &c)?;
            emit(&r, format)?;
            Ok(r.exit_code())
        }
        Commands::Scan {
            path,
            profile,
            format,
        } => {
            adapters::clang::profile_check(profile.as_deref())?;
            let root = root(path)?;
            let c = Config::load(&root)?;
            let r = adapters::clang::scan(&root, &c)?;
            emit(&r, format)?;
            Ok(r.exit_code())
        }
        Commands::Report { path, run, format } => {
            let root = root(path)?;
            let c = Config::load(&root)?;
            let r = evidence::load(&root, &c, &run)?;
            emit(&r, format)?;
            Ok(r.exit_code())
        }
        Commands::Runs { path, run } => {
            let root = root(path)?;
            let c = Config::load(&root)?;
            if let Some(id) = run {
                let r = evidence::load(&root, &c, &id)?;
                emit(&r, Format::Json)?;
                return Ok(r.exit_code());
            }
            let mut runs = Vec::new();
            let base = root.join(&c.evidence.directory);
            if base.exists() {
                let base = config::inside(&root, &base)?;
                for entry in fs::read_dir(base)? {
                    let entry = entry?;
                    let id = entry.file_name().to_string_lossy().into_owned();
                    match evidence::load(&root,&c,&id){Ok(r)=>runs.push(serde_json::json!({"id":id,"status":r.status,"complete":r.complete,"operation":r.operation})),Err(e)=>runs.push(serde_json::json!({"id":id,"status":"incomplete","complete":false,"error":e.to_string()}))}
                }
            }
            runs.sort_by_key(|r| r["id"].as_str().unwrap_or("").to_string());
            let complete = runs.iter().all(|r| r.get("error").is_none());
            println!(
                "{}",
                serde_json::json!({"status":if complete{"passed"}else{"incomplete"},"complete":complete,"runs":runs})
            );
            Ok(if complete { 0 } else { 3 })
        }
    }
}
