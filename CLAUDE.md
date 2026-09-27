# Project
Interview build. Time budget 3 hr. Working MVP beats complete design.

## FIRST GOAL: walking skeleton (do THIS before the working loop)
Before any planning, tracing, or edge cases, get ONE runnable path:
- A `main()` / `python -m src.<name>` that runs end to end on the real input.
- Every stage STUBBED: load returns 2-3 hardcoded rows, transform is
  identity, output just prints. It must EXECUTE and produce something.
- No classes, no abstractions, no logging setup yet. One file is fine.
- Target: runnable in the first 20 min. Commit it as "walking skeleton".
Do NOT plan-mode this. Just tell Claude the one small thing to write.
Rule: from here on there is always a green, runnable main.
Never break it for >10 min.

## Working loop (ONLY after the skeleton runs)
Apply per FEATURE, not up front. One feature = one function, <40 lines, one test.
1. PLAN     - restate the ONE next step, name the file touched. No code yet.
2. REVIEW   - cite the requirement in FACTS/README that justifies it.
              Cannot trace = do not build it. If the plan is longer than
              the code, the step is too big - split it.
3. REPLAN   - drop untraceable steps, split anything >1 file or >40 lines.
4. IMPLEMENT- one step. Test first. Run pytest. Thicken one stub at a time.
5. VALIDATE - re-read diff vs plan: does it do what was planned, nothing
              more, and keep main runnable? If no -> back to step 1.
Ask me before IMPLEMENT on anything touching >1 stage. After each green
step, append one line to LOG.md.

## Scope ceilings (hard)
- No stage built beyond a stub until the skeleton runs end-to-end.
- One feature per prompt. If my prompt implies >1 file, stop and ask me to split.
- No new abstraction (base classes, config systems, plugins) unless I ask.
- Prefer a dumb working version now; note "could improve" in LOG.md, move on.

## Tradeoffs & gaps (keep in README)
README has a `## Tradeoffs & gaps` section, updated in the same change as
the code that causes it. Never leave it for the end.
- Tradeoff: `<choice made> - <why> - <cost> - <better option given more time>`.
- Gap: `<what is missing or not handled> - <impact> - <how to close it>`.
- Include: shortcuts, stubs still in place, assumptions not yet confirmed
  (link to OPEN QUESTIONS), inputs not handled, perf/scale limits.
- "could improve" notes in LOG.md feed into this section.

## Rules
- README contains all important info for anyone to understand the application,
  with a mermaid diagram of the workflow.
- Python 3.11, pytest, type hints, functions under ~30 lines.
- No silent except. Fail loud with the offending row/value in the message.
- Use the stdlib `logging` module, never bare print (except in the skeleton stub).
- No new dependency without asking me first.
- Small diffs: one behaviour per change. Do one module and test before commit.
- If a requirement is missing from FACTS, stop and ask. Do not invent it.
- State assumptions explicitly in your reply, do not bury them in code.
- Allow for user review of the diff before committing code.

## Verification
- Before building, ask the interviewer for one oracle row: a known input +
  expected output to verify the core formula or transform independently.
  Never verify your own logic against your own formula — use an external source
  (textbook, online calculator, or interviewer-supplied value).
- After building, hand-verify at least 3 cases:
  - One standard happy path.
  - One inverted or negated case (short, reversed, negative qty).
  - One boundary case (near-zero, near-limit, missing optional field).
- Record each verification in LOG.md with: input values, expected output,
  actual output, and source of truth.

## Assumptions — make visible in outputs
- Every assumption must appear in two places: FACTS.md and the output itself
  (column, footnote, anomaly row, or printed summary line).
- If an assumption materially affects any output value, log it at WARNING level
  so it cannot be missed in the log.
- Example pattern: "flat X assumed — using single value for all Y and Z".

## External data validation
- Validate and type each data source on load, in its own function, before
  joining to any other source.
- Each source gets its own anomaly channel: use a `source` field in
  anomalies.csv (e.g. source="positions", source="market").
- Unused rows from a secondary source (e.g. lookup rows with no matching
  primary row) are logged at INFO, not flagged as anomalies.

## CLI hardening
- Every model parameter or constant that a user might need to change
  (rate, multiplier, date, threshold) must be a CLI flag with a sensible
  default. Never hardcode a number that lives outside the code.
- Pattern: `--param VALUE  # default: X (FACTS Qn)` in argparse help text.
- If a parameter changes the output materially, include it in the printed
  summary line so the run is self-documenting.

## Test coverage requirements
- Every happy-path case in FACTS has a test.
- Every skip/flag anomaly case in FACTS has a test that checks the anomaly
  row fields (row_id, field, reason), not just that it didn't crash.
- At least one inverted/negated case (short, reversed, negative).
- At least one boundary case per numeric input (zero, near-zero, very large).
- Tests must not read from `data/` — use inline fixtures or tmp_path.

## Logging
- Configure once in `src/logging_setup.py`; every module uses
  `logger = logging.getLogger(__name__)`.
- Goal: from the log alone, reconstruct what happened to every piece of data.
  Log ALL data changes, whatever the data (rows, records, objects, files,
  state, API responses). Terms below are generic - adapt to the task.
- "ID" = whatever identifies the item in this task (key, index, id, path,
  line number, composite of fields). Pick it once, note it in FACTS, use it
  consistently in every log line.
- DEBUG = every data change, one line each, greppable prefix:
  - `ADD    <id> <value or summary> reason=<why>`
  - `UPDATE <id> <what changed> old=<before> new=<after> reason=<why>`
  - `REMOVE <id> <value or summary> reason=<why>`
  "What changed" = field, attribute, element, or whole item - whatever fits.
  Large values: log a short summary (type, length, first N chars), not a dump.
- DEBUG = function entry/exit: name + arg shapes (types, lengths, counts),
  never full data. Use one small decorator, not hand-written lines.
- INFO  = each stage start (`START <stage> in=<count>`) and end
  (`END <stage> out=<count> added=<n> updated=<n> removed=<n>`).
- WARNING = item repaired or skipped, with the ID and the reason.
  Also: any assumption that materially affects an output value.
- ERROR = aborts, with input path and ID/position.
- `logs/run.log` always captures DEBUG (appends each run). stderr also shows
  DEBUG. The user wants as much detail as possible
  (`setup_logging(console_level=...)` changes it).
- Never log secrets or full file dumps.

## Commands
- Setup: `python3.11 -m venv .venv && .venv/bin/pip install -r requirements.txt`
- Run: `python -m src.<name> [--input PATH] [--out DIR]`
- Single test: `pytest -q tests/test_<module>.py::test_<case>`
- Test: `pytest -q`

## FACTS (clarified with interviewer - authoritative, fill as you learn)
Put in far more detail than feels necessary. Every ambiguity you leave out,
Claude fills in silently - and its guess is not your design. Spell out the
boring stuff: exact headers, delimiters, join keys, dedup keys, tolerances,
date formats, what missing/blank means, output columns and sort order.
Test: could a stranger rebuild the tool from this section alone? If no, add more.

@FACTS.md
