use anyhow::{Context, Result, bail};
use serde::{Deserialize, Serialize};
use std::{
    fs,
    path::{Path, PathBuf},
};
#[derive(Debug, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Config {
    pub version: u32,
    #[serde(default)]
    pub trace: Trace,
    pub project: Project,
    #[serde(default)]
    pub assurance: Option<Assurance>,
    #[serde(default)]
    pub coding: Coding,
    #[serde(default)]
    pub build: Build,
    #[serde(default)]
    pub tools: Tools,
    #[serde(default)]
    pub evidence: Evidence,
}
#[derive(Debug, Serialize, Deserialize)]
#[serde(default, deny_unknown_fields)]
pub struct Trace {
    pub manifest: PathBuf,
}
impl Default for Trace {
    fn default() -> Self {
        Self {
            manifest: "trace.yaml".into(),
        }
    }
}
#[derive(Debug, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Project {
    pub name: String,
}
#[derive(Debug, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Assurance {
    pub standard: String,
    pub level: String,
}
#[derive(Debug, Default, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Coding {
    #[serde(default)]
    pub registries: Vec<PathBuf>,
    #[serde(default)]
    pub deviations: Option<PathBuf>,
    #[serde(default)]
    pub recategorization: Option<PathBuf>,
    pub c: Option<String>,
    pub cpp: Option<String>,
}
#[derive(Debug, Serialize, Deserialize)]
#[serde(default, deny_unknown_fields)]
pub struct Build {
    pub variants: std::collections::BTreeMap<String, Vec<String>>,
    pub compile_commands: PathBuf,
}
impl Default for Build {
    fn default() -> Self {
        Self {
            compile_commands: "build/compile_commands.json".into(),
            variants: Default::default(),
        }
    }
}
#[derive(Debug, Serialize, Deserialize)]
#[serde(default, deny_unknown_fields)]
pub struct Tools {
    pub clang_module: Option<PathBuf>,
    pub require_filesystem_isolation: bool,
    pub clang: bool,
    pub frama_c: bool,
    pub esbmc: bool,
    pub timeout_seconds: u64,
    pub memory_mb: u64,
    /// Translation units analysed concurrently; each analyzer keeps its own limits.
    pub jobs: usize,
    pub allowlist: Vec<PathBuf>,
}
impl Default for Tools {
    fn default() -> Self {
        Self {
            clang_module: None,
            require_filesystem_isolation: false,
            clang: true,
            frama_c: false,
            esbmc: false,
            timeout_seconds: 60,
            memory_mb: 2048,
            jobs: 1,
            allowlist: Vec::new(),
        }
    }
}
#[derive(Debug, Serialize, Deserialize)]
#[serde(default, deny_unknown_fields)]
pub struct Evidence {
    pub reviews: Option<PathBuf>,
    pub directory: PathBuf,
}
impl Default for Evidence {
    fn default() -> Self {
        Self {
            directory: ".safety/runs".into(),
            reviews: None,
        }
    }
}
impl Config {
    pub fn new(name: String) -> Self {
        Self {
            version: 1,
            trace: Trace::default(),
            project: Project { name },
            assurance: None,
            coding: Coding::default(),
            build: Build::default(),
            tools: Tools::default(),
            evidence: Evidence::default(),
        }
    }
    pub fn load(root: &Path) -> Result<Self> {
        let config: Self = serde_yaml::from_slice(
            &read_bounded(&inside(root, Path::new("safety.yaml"))?, 1024 * 1024)
                .context("read safety.yaml")?,
        )?;
        config.validate()?;
        Ok(config)
    }
    pub fn validate(&self) -> Result<()> {
        if self.version != 1 {
            bail!("unsupported config version")
        }
        if self.project.name.trim().is_empty() {
            bail!("project name is empty")
        }
        if let Some(a) = &self.assurance {
            let valid = match a.standard.as_str() {
                "DO-178C" => ["A", "B", "C", "D", "E"].contains(&a.level.as_str()),
                "DO-278A" => ["AL1", "AL2", "AL3", "AL4", "AL5", "AL6"].contains(&a.level.as_str()),
                _ => false,
            };
            if !valid {
                bail!("invalid assurance standard/level")
            }
        }
        if self
            .coding
            .c
            .as_deref()
            .is_some_and(|v| v != "MISRA-C:2012")
            || self
                .coding
                .cpp
                .as_deref()
                .is_some_and(|v| v != "MISRA-CPP:2023")
        {
            bail!("unsupported coding edition")
        }
        for path in [
            &self.build.compile_commands,
            &self.evidence.directory,
            &self.trace.manifest,
        ] {
            relative(path)?;
        }
        if self.tools.timeout_seconds == 0
            || self.tools.timeout_seconds > 3600
            || !(128..=65536).contains(&self.tools.memory_mb)
            || !(1..=64).contains(&self.tools.jobs)
        {
            bail!("invalid execution limits")
        }
        if let Some(module) = &self.tools.clang_module
            && (!module.is_absolute() || !self.tools.allowlist.contains(module))
        {
            bail!("clang_module must be an absolute path explicitly included in tools.allowlist")
        }
        if let Some(path) = &self.evidence.reviews {
            relative(path)?;
        }
        for path in self
            .coding
            .registries
            .iter()
            .chain(self.coding.deviations.iter())
            .chain(self.coding.recategorization.iter())
        {
            relative(path)?;
        }
        for (name, definitions) in &self.build.variants {
            anyhow::ensure!(
                !name.is_empty()
                    && name.len() <= 128
                    && !definitions.is_empty()
                    && definitions.iter().all(|d| !d.is_empty()),
                "invalid build variant grouping"
            );
        }
        for p in &self.tools.allowlist {
            if !p.is_absolute() {
                bail!("allowlist paths must be absolute")
            }
        }
        Ok(())
    }
}
pub fn relative(path: &Path) -> Result<()> {
    if path.as_os_str().is_empty()
        || path
            .components()
            .any(|c| !matches!(c, std::path::Component::Normal(_)))
    {
        bail!(
            "path must be a nonempty relative path without traversal: {}",
            path.display()
        )
    }
    Ok(())
}
pub fn inside(root: &Path, path: &Path) -> Result<PathBuf> {
    let p = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root.join(path)
    };
    let resolved = p
        .canonicalize()
        .with_context(|| format!("resolve {}", p.display()))?;
    if !resolved.starts_with(root) {
        bail!("path escapes workspace: {}", p.display())
    }
    Ok(resolved)
}
pub fn output_dir(root: &Path, path: &Path) -> Result<PathBuf> {
    relative(path)?;
    let mut p = root.to_path_buf();
    for part in path.components() {
        p.push(part);
        match fs::create_dir(&p) {
            Ok(()) => {}
            Err(e) if e.kind() == std::io::ErrorKind::AlreadyExists => {}
            Err(e) => return Err(e.into()),
        }
        let meta = fs::symlink_metadata(&p)?;
        if meta.file_type().is_symlink() || !meta.is_dir() {
            bail!("output path contains symlink or non-directory")
        }
    }
    Ok(p)
}
/// Bound untrusted structured input before parsing; reject device and pipe inputs.
pub fn read_bounded(path: &Path, limit: u64) -> Result<Vec<u8>> {
    use std::io::Read;
    let file = fs::File::open(path)?;
    anyhow::ensure!(file.metadata()?.is_file(), "input must be a regular file");
    let mut bytes = Vec::new();
    file.take(limit + 1).read_to_end(&mut bytes)?;
    anyhow::ensure!(
        bytes.len() as u64 <= limit,
        "input exceeds configured parser limit"
    );
    Ok(bytes)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn validation() {
        let mut c = Config::new("x".into());
        assert!(c.validate().is_ok());
        c.coding.c = Some("MISRA-C:2023".into());
        assert!(c.validate().is_err());
        c.coding.c = None;
        c.tools.jobs = 0;
        assert!(c.validate().is_err());
        c.tools.jobs = 1;
        c.assurance = Some(Assurance {
            standard: "DO-278A".into(),
            level: "C".into(),
        });
        assert!(c.validate().is_err());
    }
    #[test]
    fn traversal() {
        assert!(relative(Path::new("../out")).is_err());
        assert!(relative(Path::new("/tmp")).is_err());
    }
    #[test]
    fn unknown_fields() {
        assert!(
            serde_yaml::from_str::<Config>("version: 1\nproject: {name: x}\ntools: {fix: true}")
                .is_err()
        );
    }
}
