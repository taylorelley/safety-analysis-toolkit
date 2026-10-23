use serde::{Deserialize, Serialize};
use std::collections::BTreeMap;

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq)]
#[serde(rename_all = "snake_case")]
pub enum Status {
    Passed,
    Findings,
    Incomplete,
}
#[derive(Debug, Clone, Default, Serialize, Deserialize)]
pub struct Completeness {
    pub total: usize,
    pub analyzed: usize,
    pub failed: usize,
    pub excluded: usize,
    pub unsupported: usize,
    pub build_variants: Vec<String>,
}
impl Completeness {
    pub fn complete(&self) -> bool {
        self.total > 0
            && self.total == self.analyzed
            && self.failed == 0
            && self.unsupported == 0
            && self.excluded == 0
    }
}
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Finding {
    pub id: String,
    pub tool: String,
    pub rule_id: String,
    pub file: String,
    pub line: usize,
    pub column: usize,
    pub severity: String,
    pub message: String,
    pub build_variant: String,
    pub outcome: String,
    pub native_references: Vec<String>,
}
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Execution {
    pub executable_sha256: String,
    pub restrictions: String,
    pub executable: String,
    pub arguments: Vec<String>,
    pub cwd: String,
    pub environment: BTreeMap<String, String>,
    pub exit_code: Option<i32>,
    pub failure: Option<String>,
    pub duration_ms: u128,
    pub stdout: String,
    pub stderr: String,
    pub timeout_seconds: u64,
}
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Run {
    pub schema_version: u32,
    #[serde(default)]
    pub configuration: serde_json::Value,
    #[serde(default)]
    pub toolkit: serde_json::Value,
    pub id: String,
    pub operation: String,
    pub project: String,
    pub status: Status,
    pub complete: bool,
    pub completeness: Completeness,
    pub started_unix_ms: u128,
    pub completed_unix_ms: u128,
    pub inputs: BTreeMap<String, String>,
    pub executions: Vec<Execution>,
    pub findings: Vec<Finding>,
    #[serde(default)]
    pub properties: Vec<Property>,
    pub limitations: Vec<String>,
    pub git: Option<serde_json::Value>,
}
impl Run {
    pub fn exit_code(&self) -> i32 {
        match self.status {
            Status::Passed => 0,
            Status::Findings => 1,
            Status::Incomplete => 3,
        }
    }
}

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq)]
#[serde(rename_all = "SCREAMING_SNAKE_CASE")]
pub enum ProofOutcome {
    Proven,
    Violated,
    Unknown,
    NotChecked,
    Failed,
}
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Property {
    pub id: String,
    pub description: String,
    pub outcome: ProofOutcome,
    pub scope: String,
    pub assumptions: Vec<String>,
    pub unwind_bound: Option<u32>,
    pub completeness_established: bool,
    pub native_references: Vec<String>,
}

impl ProofOutcome {
    pub fn as_str(&self) -> &'static str {
        match self {
            Self::Proven => "PROVEN",
            Self::Violated => "VIOLATED",
            Self::Unknown => "UNKNOWN",
            Self::NotChecked => "NOT_CHECKED",
            Self::Failed => "FAILED",
        }
    }
}
