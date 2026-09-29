# FACTS
Authoritative answers from the interviewer. Fill as you learn. If it is not
here, it is not decided - ask, do not guess. Number each fact (Q1, Q2, ...)
so code, CLI help and tests can cite it.

## Goal
- Q1: What the tool does, in one sentence.
- Q2: Who uses the output and for what.

## Inputs
- Q3: Each source: name, location/path, format, encoding, delimiter.
- Q4: Exact fields/structure per source, with types and units.
- Q5: ID for each item (see CLAUDE.md Logging). Unique? Composite?
- Q6: Date/time formats and timezone.
- Q7: What missing / blank / zero / duplicate means per field.

## Processing rules
- Q8: Core formula or transform, written out in full.
- Q9: How sources relate (join/match rules, what happens on no match).
- Q10: Dedup rule, sort order, rounding, tolerances.
- Q11: Invalid item handling: repair, skip, flag, or abort - per case.

## Outputs
- Q12: Output destination and format.
- Q13: Exact fields/columns, order, units, sort order.
- Q14: Summary line contents.

## Parameters (become CLI flags)
- Q15: Name - default - allowed range - source.

## Oracle cases (external source of truth)
| Input | Expected output | Source |
|-------|-----------------|--------|
|       |                 |        |

## Assumptions (also shown in output + logged at WARNING)
- A1:

## Open questions
- [ ] (anything assumed and not yet confirmed)
