use vstd::prelude::*;

verus! {
fn reset_transition(fault: bool, reset: bool, previous_inhibit: bool) -> (inhibit: bool)
    ensures fault ==> inhibit,
            reset && !fault ==> !inhibit,
            !fault && !reset ==> (inhibit == previous_inhibit),
{
    fault || (!reset && previous_inhibit)
}

fn repeat_active_fault(count: u8) -> (inhibit: bool)
    ensures count > 0 ==> inhibit,
{
    let mut i: u8 = 0;
    let mut latched = false;
    while i < count
        invariant i <= count, i > 0 ==> latched,
        decreases (count as int) - (i as int),
    {
        latched = reset_transition(true, true, latched);
        i += 1;
    }
    latched
}
}
