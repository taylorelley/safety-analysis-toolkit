#![no_std]
#![allow(unexpected_cfgs)]

/// Computes the next inhibit latch state from the sampled fault and reset.
pub fn next_inhibit(inhibited: bool, fault: bool, reset: bool) -> bool {
    fault || (inhibited && !reset)
}

#[cfg(kani)]
mod proofs {
    use super::next_inhibit;

    #[kani::proof]
    fn reset_transition() {
        let fault: bool = kani::any();
        kani::cover!(fault, "active fault is reachable");
        kani::cover!(!fault, "cleared fault is reachable");
        let next = next_inhibit(true, fault, true);
        assert!(!fault || next, "REQ-KEEP");
        assert!(fault || !next, "REQ-CLEAR");
    }
}
