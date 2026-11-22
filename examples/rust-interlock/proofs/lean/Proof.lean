import Interlock.Extraction.Funs
set_option warningAsError true

theorem interlock_refines (inhibited fault reset : Bool) :
    interlock.next_inhibit inhibited fault reset = .ok (fault || (inhibited && !reset)) := by
  cases inhibited <;> cases fault <;> cases reset <;> rfl
