"""Guarded DayZ task handoff. No Git, network, automatic review or acceptance.

Coordinator invocation is documented in TASK_LIFECYCLE_RULES.md. All paths must
remain inside --root, including test fixtures. Dry runs perform no writes.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys
import tempfile

B1 = "# БЛОК 1 — ТЕКУЩАЯ ЗАДАЧА"
B11 = "# БЛОК 1.1 — ПОСЛЕДНЯЯ СДАННАЯ ПОСТАНОВКА"
MARKERS = tuple(f"<!-- TASK-SLOT:{slot}:{edge} -->" for slot in
                ("BLOCK-1", "BLOCK-1.1") for edge in ("BEGIN", "END"))
EMPTY = "**НЕ ЗАПОЛНЕН — ПЕРВЫЙ ПЕРЕНОС ЕЩЁ НЕ ВЫПОЛНЕН.**"
WAITING = "**ОЖИДАНИЕ НОВОЙ ЗАДАЧИ.**"
FINAL = {"RESULT READY", "PARTIAL", "REVISE"}


class Refusal(Exception):
    pass


class CommitUncertain(Exception):
    """An error after atomic replacement must not be reported as NO ROTATE."""


def require(condition, reason):
    if not condition:
        raise Refusal(reason)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def safe(root, value, exists=False):
    """Lexical containment first; never follow reparse points to check a path."""
    raw = os.fspath(value)
    require(not any(p == ".." for p in raw.replace("\\", "/").split("/")), "parent traversal")
    p = Path(os.path.abspath(raw))
    root = Path(os.path.abspath(root))
    require(os.path.normcase(os.path.commonpath([root, p])) == os.path.normcase(str(root)),
            "path outside allowed root")
    for q in [p, *p.parents]:
        if os.path.lexists(q):
            info = q.lstat()
            require(not stat.S_ISLNK(info.st_mode) and
                    not getattr(info, "st_file_attributes", 0) & 1024, "unsafe link/reparse path")
    # Reject NTFS alternate streams and device-like components, including for
    # fixture runs on other platforms.
    for part in p.parts[1:]:
        require(":" not in part and not part.endswith((" ", ".")), "unsafe path component")
        require(part.split(".")[0].upper() not in
                {"CON", "PRN", "AUX", "NUL", *[f"COM{i}" for i in range(1, 10)],
                 *[f"LPT{i}" for i in range(1, 10)]}, "device path")
    if exists:
        require(p.is_file(), "required regular file missing")
    return p


def read(root, path):
    p = safe(root, path, True)
    require(p.stat().st_nlink == 1, "hard-linked input")
    return p.read_bytes()


def text_bytes(data):
    require(not data.startswith(b"\xef\xbb\xbf") and b"\0" not in data, "BOM/NUL forbidden")
    try:
        text = data.decode("utf-8", errors="strict")
    except UnicodeError as error:
        raise Refusal("invalid UTF-8") from error
    require("\ufffd" not in text, "replacement character")
    stripped = data.replace(b"\r\n", b"")
    require(b"\r" not in stripped, "bare CR")
    require(not (b"\r\n" in data and b"\n" in stripped), "mixed EOL")
    return text, b"\r\n" if b"\r\n" in data else b"\n"


def task_identity(body):
    text, _ = text_bytes(body)
    found = re.findall(r"(?m)^\s*(?:\*\*)?TASK ID:(?:\*\*)?\s*`?([A-Za-z0-9_-]+)`?", text)
    require(len(found) == 1, "exactly one TASK ID field required")
    require(found[0] != "UNKNOWN", "unknown TASK ID")
    return found[0]


def layout(raw):
    text, nl = text_bytes(raw)
    lines = text.splitlines()
    for heading in (B1, B11):
        require(lines.count(heading) == 1, "missing/duplicate exact slot heading")
    spans = []
    for i in (0, 2):
        begin, end = (MARKERS[i].encode(), MARKERS[i + 1].encode())
        for marker in (begin, end):
            require(raw.count(marker) == 1, "missing/duplicate marker")
            require(re.search(rb"(?m)^" + re.escape(marker) + rb"\r?$", raw), "marker must occupy line")
        start = raw.index(begin) + len(begin)
        require(raw[start:start + len(nl)] == nl, "marker EOL")
        start += len(nl)
        stop = raw.index(end)
        require(start < stop and raw[stop - len(nl):stop] == nl, "empty/invalid slot boundary")
        spans.append((start, stop))
    require(raw.index(B1.encode()) < raw.index(MARKERS[0].encode()) < spans[0][1] <
            raw.index(B11.encode()) < raw.index(MARKERS[2].encode()) < spans[1][1], "slot order")
    require(raw.count(b"<!-- TASK-SLOT:") == 4, "unknown/damaged slot marker")
    return spans, nl


def outside(raw, spans):
    (a, b), (c, d) = spans
    return raw[:a], raw[b:c], raw[d:]


def json_object(data):
    def unique(pairs):
        result = {}
        for key, value in pairs:
            require(key not in result, "duplicate JSON key")
            result[key] = value
        return result
    try:
        result = json.loads(data.decode("utf-8"), object_pairs_hook=unique)
    except (ValueError, UnicodeError) as error:
        raise Refusal("invalid JSON") from error
    require(isinstance(result, dict), "JSON object required")
    return result


def fingerprint(path):
    s = path.stat()
    return s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns


def prepare(args):
    root = Path(os.path.abspath(args.root))
    card = safe(root, args.card, True)
    raw = read(root, card)
    spans, nl = layout(raw)
    body = raw[slice(*spans[0])]
    require(body.strip() and body.strip() != WAITING.encode(), "no active task")
    require(b"<!-- HANDOFF:TASK:" not in body, "reserved handoff delimiter in task")
    task_id = task_identity(body)
    require(task_id == args.task_id, "TASK ID mismatch")
    require(re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_-]{0,100}", task_id), "unsafe TASK ID")
    snap = safe(root, args.snapshot, True)
    require(read(root, snap) == body, "task snapshot not byte-exact")
    manifest_path = safe(root, args.manifest, True)
    manifest_raw = read(root, manifest_path)
    manifest = json_object(manifest_raw)
    require(manifest.get("schema_version") == 1 and manifest.get("task_id") == task_id,
            "manifest schema/task mismatch")
    require(manifest.get("final") is True and manifest.get("status") in FINAL,
            "package is DRAFT/BLOCKED/not final")
    artifacts = manifest.get("artifacts")
    require(isinstance(artifacts, list) and artifacts, "artifact manifest missing")
    evidence = {}
    guards = {card: raw, manifest_path: manifest_raw}
    source_files = manifest.get("source_files", [])
    require(isinstance(source_files, list), "source_files must be a list")
    for source in source_files:
        require(isinstance(source, dict) and isinstance(source.get("path"), str), "invalid source pin")
        rel = Path(source["path"])
        require(not rel.is_absolute() and not rel.drive, "source pin must be root-relative")
        p = safe(root, root / rel, True)
        data = read(root, p)
        require(len(data) == source.get("size") and digest(data) == source.get("sha256"),
                "source version differs from final report package")
        guards[p] = data
    for artifact in artifacts:
        require(isinstance(artifact, dict) and isinstance(artifact.get("path"), str), "invalid artifact")
        rel = Path(artifact["path"])
        require(not rel.is_absolute() and not rel.drive, "artifact path must be relative")
        p = safe(root, manifest_path.parent / rel, True)
        key = os.path.normcase(str(p))
        require(key not in evidence and p != card and p != manifest_path, "duplicate/self artifact")
        data = read(root, p)
        require(len(data) == artifact.get("size") and digest(data) == artifact.get("sha256"),
                "artifact integrity mismatch")
        evidence[key] = data
        guards[p] = data
    report = safe(root, args.report, True)
    require(os.path.normcase(str(report)) in evidence and os.path.normcase(str(snap)) in evidence,
            "report/snapshot absent from manifest")
    report_text, _ = text_bytes(read(root, report))
    require(task_identity(read(root, report)) == task_id, "report TASK mismatch")
    require(re.search(r"(?m)^STATUS: " + re.escape(manifest["status"]) + r"\s*$", report_text),
            "report status mismatch")
    previous = raw[slice(*spans[1])]
    if previous != EMPTY.encode() + nl:
        require(args.previous_archive and args.previous_sha256, "previous 1.1 archive not confirmed")
        archive = safe(root, args.previous_archive, True)
        archived = read(root, archive)
        require(archive != card and archived == previous and digest(archived) == args.previous_sha256,
                "previous archive mismatch")
        guards[archive] = archived
    amendments = []
    amendment_ids = set()
    for path in args.amendment:
        p = safe(root, path, True)
        data = read(root, p)
        obj = json_object(data)
        aid = obj.get("amendment_id", "")
        require(obj.get("task_id") == task_id and re.fullmatch(r"[A-Za-z0-9_-]+", aid)
                and aid not in amendment_ids, "amendment identity")
        require(isinstance(obj.get("text"), str) and obj["text"].strip() and
                isinstance(obj.get("source"), str) and obj["source"].strip(), "amendment source/text missing")
        require(os.path.normcase(str(p)) in evidence, "amendment absent from manifest")
        _, amendment_nl = text_bytes(data)
        require(amendment_nl == nl, "amendment EOL differs from card")
        amendment_ids.add(aid)
        amendments.append((aid, data))
    output = safe(root, args.output_dir)
    require(not os.path.lexists(output), "output already exists; repeat refused")
    require(output.is_relative_to(manifest_path.parent), "output must stay inside report package")
    require(output != root and card not in output.parents and output not in card.parents,
            "output/card overlap")
    new_slot = ("SUBMITTED / REVIEW PENDING".encode() + nl +
                f"Submitted task: {task_id}".encode() + nl +
                f"Task SHA-256: {digest(body)}".encode() + nl +
                b"<!-- HANDOFF:TASK:BEGIN -->" + nl + body + b"<!-- HANDOFF:TASK:END -->" + nl)
    for aid, data in amendments:
        new_slot += f"<!-- AMENDMENT:{aid}:BEGIN -->".encode() + nl + data
        if not data.endswith(nl):
            new_slot += nl
        new_slot += f"<!-- AMENDMENT:{aid}:END -->".encode() + nl
    (a, b), (c, d) = spans
    new = raw[:a] + WAITING.encode() + nl + raw[b:c] + new_slot + raw[d:]
    new_spans, _ = layout(new)
    require(outside(raw, spans) == outside(new, new_spans), "outside slots changed")
    receipt = {"schema_version": 1, "task_id": task_id, "status": "SUBMITTED / REVIEW PENDING",
               "card_before_sha256": digest(raw), "card_after_sha256": digest(new),
               "task_sha256": digest(body), "previous_slot_sha256": digest(previous),
               "copied_exact": True, "outside_slots_unchanged": True,
               "amendments": [{"id": aid, "sha256": digest(data)} for aid, data in amendments],
               "independent_verdict": "NOT PROVIDED BY ROTATION"}
    return root, card, raw, new, body, previous, output, amendments, receipt, guards


def rotate(args, before_commit=None):
    root, card, raw, new, body, previous, output, amendments, receipt, guards = prepare(args)
    if not args.apply:
        return {"result": "DRY_RUN", "writes": 0, **receipt}
    lock = safe(root, card.with_name(card.name + ".rotation.lock"))
    created = []
    temp_path = None
    output_created = False
    lock_fd = None
    committed = False
    try:
        lock_fd = os.open(lock, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
        states = {p: fingerprint(p) for p in guards}
        for p, data in guards.items():
            require(read(root, p) == data, "input modified after validation")
        safe(root, output)
        require(output.parent.is_dir(), "output parent must already exist")
        output.mkdir()  # exclusive; never reuse an old handoff directory
        output_created = True
        payloads = {"handoff_task.md": body, "previous_slot.md": previous,
                    "card_before.md": raw,
                    "rotation.json": (json.dumps(receipt, ensure_ascii=False, indent=2) + "\n").encode()}
        payloads.update({f"amendment_{aid}.json": data for aid, data in amendments})
        for name, data in payloads.items():
            p = safe(root, output / name)
            with p.open("xb") as f:
                created.append(p)
                f.write(data)
                f.flush()
                os.fsync(f.fileno())
            require(read(root, p) == data, "handoff readback failed")
        fd, name = tempfile.mkstemp(prefix=".task-slot-", suffix=".tmp", dir=card.parent)
        temp_path = Path(name)
        with os.fdopen(fd, "wb") as f:
            f.write(new)
            f.flush()
            os.fsync(f.fileno())
        if before_commit:
            before_commit()  # test seam for a real intervening external edit
        for p, data in guards.items():
            require(read(root, p) == data and fingerprint(p) == states[p], "concurrent input modification")
        safe(root, card, True)
        safe(root, output)
        require(read(root, temp_path) == new, "temporary card integrity")
        os.replace(temp_path, card)  # card is the final commit point
        committed = True
        temp_path = None
        if read(root, card) != new:
            raise CommitUncertain("card changed after commit; preserve card_before.md and reconcile")
        return {"result": "ROTATED", **receipt, "output_dir": str(output)}
    except Exception as error:
        # If the commit happened, preserve recovery evidence. Never overwrite a
        # subsequent editor change during automatic error handling.
        if not committed:
            for p in reversed(created):
                safe(root, p, True).unlink()
            if output_created:
                safe(root, output).rmdir()
        if committed and not isinstance(error, CommitUncertain):
            raise CommitUncertain("error after commit; retain recovery evidence: " + str(error)) from error
        raise
    finally:
        if temp_path and temp_path.exists():
            safe(root, temp_path, True).unlink()
        if lock_fd is not None:
            os.close(lock_fd)
            safe(root, lock, True).unlink()


def parser():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("command", choices=["check", "rotate"])
    p.add_argument("--root", default=str(Path(__file__).absolute().parent.parent))
    p.add_argument("--card", required=True)
    p.add_argument("--task-id")
    p.add_argument("--snapshot")
    p.add_argument("--report")
    p.add_argument("--manifest")
    p.add_argument("--output-dir")
    p.add_argument("--previous-archive")
    p.add_argument("--previous-sha256")
    p.add_argument("--amendment", action="append", default=[])
    p.add_argument("--apply", action="store_true")
    return p


def main(argv=None):
    args = parser().parse_args(argv)
    try:
        if args.command == "check":
            require(not args.apply, "check cannot apply")
            raw = read(Path(args.root), args.card)
            spans, _ = layout(raw)
            body = raw[slice(*spans[0])]
            task = None if body.strip() == WAITING.encode() else task_identity(body)
            require(not args.task_id or task == args.task_id, "TASK ID mismatch")
            result = {"result": "CHECK_OK", "task_id": task, "sha256": digest(raw), "writes": 0}
        else:
            require(all([args.task_id, args.snapshot, args.report, args.manifest, args.output_dir]),
                    "rotate requires task-id/snapshot/report/manifest/output-dir")
            result = rotate(args)
        print(json.dumps(result, ensure_ascii=False))
        return 0
    except CommitUncertain as error:
        print(json.dumps({"result": "COMMIT_UNCERTAIN", "reason": str(error)}, ensure_ascii=False))
        return 4
    except (Refusal, OSError, ValueError, TypeError) as error:
        print(json.dumps({"result": "NO ROTATE", "reason": str(error)}, ensure_ascii=False))
        return 3


if __name__ == "__main__":
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")
    raise SystemExit(main())
