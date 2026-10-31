"""Behavioral tests: fixture-only writes, source preservation and adversarial inputs."""
import argparse
import contextlib
import importlib.util
import io
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

PROJECT = Path(__file__).absolute().parents[4]
SPEC = importlib.util.spec_from_file_location('task_slot_rotate', PROJECT/'Tools/task_slot_rotate.py')
S = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(S)
HERE = Path(__file__).absolute().parent
RECORD = '--record' in sys.argv


class RotationTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='fixture-', dir=HERE)
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)/'проект с пробелами'
        self.root.mkdir()
        self.package = self.root/'пакет'
        self.package.mkdir()
        self.card = self.root/'карточка.md'
        self.body = '**TASK ID:** `TASK-TEST`\n\nТочная постановка: ёж, кавычки `x`, пробелы.  \n'.encode()
        self.nl = b'\n'
        self.make_card()
        self.snapshot = self.package/'task.md'
        self.snapshot.write_bytes(self.body)
        self.report = self.package/'report.md'
        self.report.write_text('TASK ID: TASK-TEST\nSTATUS: RESULT READY\nCHECKS: fixture evidence\n', encoding='utf-8')
        self.manifest = self.package/'manifest.json'
        self.args = argparse.Namespace(root=str(self.root), card=str(self.card), task_id='TASK-TEST',
            snapshot=str(self.snapshot), report=str(self.report), manifest=str(self.manifest),
            output_dir=str(self.package/'handoff'), previous_archive=None, previous_sha256=None,
            amendment=[], apply=True)
        self.refresh()

    def make_card(self, previous=None):
        nl = self.nl
        previous = previous if previous is not None else S.EMPTY.encode()+nl
        self.card.write_bytes(b'# Fixture rules'+nl+S.B1.encode()+nl+
            S.MARKERS[0].encode()+nl+self.body+S.MARKERS[1].encode()+nl+
            S.B11.encode()+nl+S.MARKERS[2].encode()+nl+previous+S.MARKERS[3].encode()+nl+
            '# БЛОК 2 — factual\nDo not change\n# БЛОК 3 — OPEN\n# БЛОК 4 — verifier\n'.encode().replace(b'\n',nl))

    def refresh(self, status='RESULT READY', final=True, extras=()):
        paths=[self.snapshot, self.report, *extras]
        obj={'schema_version':1,'task_id':'TASK-TEST','final':final,'status':status,
             'artifacts':[{'path':p.relative_to(self.package).as_posix(),
                           'size':len(p.read_bytes()),'sha256':S.digest(p.read_bytes())} for p in paths]}
        self.manifest.write_text(json.dumps(obj, ensure_ascii=False),encoding='utf-8')

    def inventory(self):
        # No symlink recursion: record lstat targets as links, not external bytes.
        result={}
        for folder, dirs, files in os.walk(self.root, followlinks=False):
            for n in dirs+files:
                p=Path(folder)/n
                if p.is_symlink() or getattr(p.lstat(),'st_file_attributes',0)&1024:
                    result[str(p.relative_to(self.root))]='LINK'
                elif p.is_file():
                    result[str(p.relative_to(self.root))]=p.read_bytes()
        return result

    def refused(self):
        before=self.inventory()
        with self.assertRaises((S.Refusal,OSError,ValueError,TypeError)):
            S.rotate(self.args)
        self.assertEqual(before,self.inventory())

    def test_success_lf_exact_and_receipt(self):
        before=self.card.read_bytes(); spans,_=S.layout(before)
        result=S.rotate(self.args)
        after=self.card.read_bytes();newspans,_=S.layout(after)
        self.assertEqual(result['result'],'ROTATED')
        self.assertEqual(S.outside(before,spans),S.outside(after,newspans))
        self.assertEqual(after[slice(*newspans[0])],S.WAITING.encode()+b'\n')
        self.assertEqual((Path(self.args.output_dir)/'handoff_task.md').read_bytes(),self.body)
        self.assertEqual(result['task_sha256'],S.digest(self.body))
        self.assertIn(b'SUBMITTED / REVIEW PENDING',after)
        self.assertEqual((Path(self.args.output_dir)/'card_before.md').read_bytes(),before)
        self.assertEqual(result['independent_verdict'],'NOT PROVIDED BY ROTATION')
        # Stable, inspectable test evidence, never the live task.
        if RECORD:
            (HERE/'handoff_task.md').write_bytes(self.body)
            (HERE/'fixture_rotation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')

    def test_success_crlf(self):
        self.nl=b'\r\n';self.body=self.body.replace(b'\n',self.nl)
        self.make_card();self.snapshot.write_bytes(self.body);self.refresh()
        result=S.rotate(self.args)
        self.assertEqual(result['result'],'ROTATED')
        self.assertEqual((Path(self.args.output_dir)/'handoff_task.md').read_bytes(),self.body)
        self.assertEqual(S.text_bytes(self.card.read_bytes())[1],self.nl)

    def test_dry_run_zero_writes(self):
        self.args.apply=False;before=self.inventory()
        self.assertEqual(S.rotate(self.args)['result'],'DRY_RUN')
        self.assertEqual(before,self.inventory())

    def test_check_cli(self):
        capture=io.StringIO()
        with contextlib.redirect_stdout(capture):
            code=S.main(['check','--root',str(self.root),'--card',str(self.card),'--task-id','TASK-TEST'])
        self.assertEqual(code,0);self.assertEqual(json.loads(capture.getvalue())['result'],'CHECK_OK')

    def test_cli_refusal_exit_json(self):
        capture=io.StringIO();before=self.inventory()
        with contextlib.redirect_stdout(capture):
            code=S.main(['rotate','--root',str(self.root),'--card',str(self.card)])
        self.assertEqual(code,3);self.assertEqual(json.loads(capture.getvalue())['result'],'NO ROTATE')
        self.assertEqual(before,self.inventory())

    def test_wrong_task_id(self):
        self.args.task_id='TASK-WRONG';self.refused()

    def test_empty_task(self):
        self.body=b'\n';self.make_card();self.refused()

    def test_two_task_ids(self):
        self.body+=b'TASK ID: TASK-OTHER\n';self.make_card();self.refused()

    def test_missing_marker(self):
        self.card.write_bytes(self.card.read_bytes().replace(S.MARKERS[1].encode(),b''));self.refused()

    def test_duplicate_marker(self):
        self.card.write_bytes(self.card.read_bytes()+S.MARKERS[1].encode()+b'\n');self.refused()

    def test_duplicate_heading(self):
        self.card.write_bytes(self.card.read_bytes()+S.B1.encode()+b'\n');self.refused()

    def test_reordered_heading(self):
        raw=self.card.read_bytes();self.card.write_bytes(raw.replace(S.B1.encode(),b'X').replace(S.B11.encode(),S.B1.encode()).replace(b'X',S.B11.encode(),1));self.refused()

    def test_marker_not_own_line(self):
        self.card.write_bytes(self.card.read_bytes().replace(S.MARKERS[0].encode(),b'prefix'+S.MARKERS[0].encode()));self.refused()

    def test_unknown_marker(self):
        self.card.write_bytes(self.card.read_bytes()+b'<!-- TASK-SLOT:BROKEN -->\n');self.refused()

    def test_bom(self):
        self.card.write_bytes(b'\xef\xbb\xbf'+self.card.read_bytes());self.refused()

    def test_invalid_utf8(self):
        self.card.write_bytes(self.card.read_bytes()+b'\xff');self.refused()

    def test_mixed_eol(self):
        self.card.write_bytes(self.card.read_bytes().replace(b'\n',b'\r\n',1));self.refused()

    def test_bare_cr(self):
        self.card.write_bytes(self.card.read_bytes()+b'\r');self.refused()

    def test_nul(self):
        self.card.write_bytes(self.card.read_bytes()+b'\0');self.refused()

    def test_snapshot_mismatch(self):
        self.snapshot.write_bytes(self.body+b'changed');self.refresh();self.refused()

    def test_report_missing(self):
        self.report.unlink();self.refused()

    def test_manifest_missing(self):
        self.manifest.unlink();self.refused()

    def test_manifest_not_final(self):
        self.refresh(final=False);self.refused()

    def test_status_draft(self):
        self.refresh(status='DRAFT');self.refused()

    def test_status_blocked(self):
        self.refresh(status='BLOCKED');self.refused()

    def test_final_partial(self):
        self.report.write_text('TASK ID: TASK-TEST\nSTATUS: PARTIAL\nOPEN: followup required\n',encoding='utf-8');self.refresh(status='PARTIAL')
        self.assertEqual(S.rotate(self.args)['result'],'ROTATED')

    def test_final_revise(self):
        self.report.write_text('TASK ID: TASK-TEST\nSTATUS: REVISE\nOPEN: not accepted\n',encoding='utf-8');self.refresh(status='REVISE')
        self.assertEqual(S.rotate(self.args)['result'],'ROTATED')

    def test_report_status_mismatch(self):
        self.refresh(status='PARTIAL');self.refused()

    def test_report_task_mismatch(self):
        self.report.write_text('TASK ID: WRONG\nSTATUS: RESULT READY\n',encoding='utf-8');self.refresh();self.refused()

    def test_artifact_tampered(self):
        self.report.write_bytes(self.report.read_bytes()+b'changed');self.refused()

    def source_pin(self):
        p=self.root/'source.txt';p.write_bytes(b'published candidate')
        obj=json.loads(self.manifest.read_text())
        obj['source_files']=[{'path':'source.txt','size':p.stat().st_size,'sha256':S.digest(p.read_bytes())}]
        self.manifest.write_text(json.dumps(obj));return p

    def test_source_pin_success(self):
        self.source_pin();self.assertEqual(S.rotate(self.args)['result'],'ROTATED')

    def test_source_pin_changed(self):
        self.source_pin().write_bytes(b'external change');self.refused()

    def test_source_pin_traversal(self):
        self.source_pin();obj=json.loads(self.manifest.read_text());obj['source_files'][0]['path']='../foreign'
        self.manifest.write_text(json.dumps(obj));self.refused()

    def test_empty_manifest_artifacts(self):
        obj=json.loads(self.manifest.read_text());obj['artifacts']=[];self.manifest.write_text(json.dumps(obj));self.refused()

    def test_duplicate_artifact(self):
        obj=json.loads(self.manifest.read_text());obj['artifacts'].append(obj['artifacts'][0]);self.manifest.write_text(json.dumps(obj));self.refused()

    def test_duplicate_json_key(self):
        self.manifest.write_text('{"task_id":"TASK-TEST","task_id":"TASK-TEST"}');self.refused()

    def test_artifact_traversal(self):
        obj=json.loads(self.manifest.read_text());obj['artifacts'][0]['path']='../карточка.md';self.manifest.write_text(json.dumps(obj));self.refused()

    def test_previous_unarchived(self):
        self.make_card(b'previous exact handoff\n');self.refused()

    def test_previous_wrong_hash(self):
        self.make_card(b'previous\n');p=self.package/'previous.md';p.write_bytes(b'previous\n')
        self.args.previous_archive=str(p);self.args.previous_sha256='0'*64;self.refused()

    def test_previous_archived_success(self):
        previous=b'previous task and accepted amendments\n';self.make_card(previous)
        p=self.package/'previous.md';p.write_bytes(previous)
        self.args.previous_archive=str(p);self.args.previous_sha256=S.digest(previous)
        self.assertEqual(S.rotate(self.args)['result'],'ROTATED');self.assertEqual(p.read_bytes(),previous)

    def test_repeat(self):
        S.rotate(self.args);self.refused()

    def test_existing_output(self):
        Path(self.args.output_dir).mkdir();(Path(self.args.output_dir)/'keep.txt').write_bytes(b'keep');self.refused()

    def test_outside_output(self):
        self.args.output_dir=str(Path(self.temp.name)/'outside');self.refused()

    def test_outside_snapshot(self):
        p=Path(self.temp.name)/'outside.md';p.write_bytes(self.body);self.args.snapshot=str(p);self.refused()

    def test_parent_traversal(self):
        self.args.output_dir=str(self.package/'..'/'escape');self.refused()

    def test_output_inside_root_but_outside_package(self):
        self.args.output_dir=str(self.root/'unrelated');self.refused()

    def test_atomic_replace_failure(self):
        before=self.inventory()
        with patch.object(S.os,'replace',side_effect=OSError('injected replace failure')):
            with self.assertRaises(OSError):S.rotate(self.args)
        self.assertEqual(before,self.inventory())

    def test_post_commit_external_edit_preserves_recovery(self):
        before=self.card.read_bytes();original_replace=S.os.replace
        external=b'external edit after commit\n'
        def replace_then_edit(source,target):
            original_replace(source,target)
            Path(target).write_bytes(external)
        with patch.object(S.os,'replace',side_effect=replace_then_edit):
            with self.assertRaises(S.CommitUncertain):S.rotate(self.args)
        self.assertEqual(self.card.read_bytes(),external)
        self.assertEqual((Path(self.args.output_dir)/'card_before.md').read_bytes(),before)
        self.assertFalse(Path(str(self.card)+'.rotation.lock').exists())

    def test_reserved_task_delimiter(self):
        self.body+=b'<!-- HANDOFF:TASK:BEGIN -->\n';self.make_card();self.snapshot.write_bytes(self.body);self.refresh();self.refused()

    def test_ads_path(self):
        self.args.output_dir=str(self.package/'x:stream');self.refused()

    def test_device_path(self):
        self.args.output_dir=str(self.package/'CON');self.refused()

    def test_input_hardlink(self):
        p=self.package/'hard.md'
        try:os.link(self.snapshot,p)
        except OSError as e:self.skipTest(str(e))
        self.refused()

    def amendment(self,obj):
        p=self.package/'amendment.json';p.write_bytes((json.dumps(obj,ensure_ascii=False)+'\n').encode('utf-8'))
        self.args.amendment=[str(p)];self.refresh(extras=[p]);return p

    def test_amendment_exact(self):
        p=self.amendment({'task_id':'TASK-TEST','amendment_id':'A1','text':'Уточнение ё','source':'USER 2026-10-31'})
        raw=p.read_bytes();result=S.rotate(self.args)
        self.assertEqual((Path(self.args.output_dir)/'amendment_A1.json').read_bytes(),raw)
        self.assertIn(raw,self.card.read_bytes());self.assertEqual(result['amendments'][0]['sha256'],S.digest(raw))

    def test_amendment_wrong_task(self):
        self.amendment({'task_id':'OTHER','amendment_id':'A1','text':'x','source':'user'});self.refused()

    def test_amendment_no_source(self):
        self.amendment({'task_id':'TASK-TEST','amendment_id':'A1','text':'x'});self.refused()

    def test_amendment_invalid_json(self):
        p=self.amendment({'task_id':'TASK-TEST'});p.write_bytes(b'{bad');self.refresh(extras=[p]);self.refused()

    def test_duplicate_amendment(self):
        p=self.amendment({'task_id':'TASK-TEST','amendment_id':'A1','text':'x','source':'user'})
        self.args.amendment.append(str(p));self.refused()

    def test_amendment_missing_from_manifest(self):
        self.amendment({'task_id':'TASK-TEST','amendment_id':'A1','text':'x','source':'user'});self.refresh();self.refused()

    def test_concurrent_card_edit(self):
        changed=self.card.read_bytes()+b'external edit\n'
        with self.assertRaises(S.Refusal):S.rotate(self.args,before_commit=lambda:self.card.write_bytes(changed))
        self.assertEqual(self.card.read_bytes(),changed)
        self.assertFalse(Path(self.args.output_dir).exists())
        self.assertFalse(Path(str(self.card)+'.rotation.lock').exists())

    def test_concurrent_report_edit(self):
        before=self.card.read_bytes();changed=self.report.read_bytes()+b'external\n'
        with self.assertRaises(S.Refusal):S.rotate(self.args,before_commit=lambda:self.report.write_bytes(changed))
        self.assertEqual(self.card.read_bytes(),before);self.assertEqual(self.report.read_bytes(),changed)
        self.assertFalse(Path(self.args.output_dir).exists())

    def test_existing_lock(self):
        lock=Path(str(self.card)+'.rotation.lock');lock.write_bytes(b'other process');self.refused()
        self.assertEqual(lock.read_bytes(),b'other process')

    def test_windows_case_paths(self):
        if os.name!='nt':self.skipTest('Windows only')
        self.args.card=str(self.card).swapcase()
        self.assertEqual(S.rotate(self.args)['result'],'ROTATED')

    def test_windows_junction_output(self):
        if os.name!='nt':self.skipTest('Windows only')
        external=Path(self.temp.name)/'external';external.mkdir();(external/'keep').write_bytes(b'safe')
        link=self.root/'junction'
        result=subprocess.run(['cmd','/c','mklink','/J',str(link),str(external)],capture_output=True)
        if result.returncode:self.skipTest('junction unavailable: '+result.stderr.decode(errors='replace'))
        self.addCleanup(lambda:os.rmdir(link) if os.path.lexists(link) else None)
        self.args.output_dir=str(link/'handoff');self.refused()
        self.assertEqual((external/'keep').read_bytes(),b'safe');self.assertFalse((external/'handoff').exists())

    def test_windows_junction_input(self):
        if os.name!='nt':self.skipTest('Windows only')
        external=Path(self.temp.name)/'external';external.mkdir();(external/'task.md').write_bytes(self.body)
        link=self.root/'junction'
        result=subprocess.run(['cmd','/c','mklink','/J',str(link),str(external)],capture_output=True)
        if result.returncode:self.skipTest('junction unavailable')
        self.addCleanup(lambda:os.rmdir(link) if os.path.lexists(link) else None)
        self.args.snapshot=str(link/'task.md');self.refused()
        self.assertEqual((external/'task.md').read_bytes(),self.body)


if __name__=='__main__':
    stream=io.StringIO()
    suite=unittest.defaultTestLoader.loadTestsFromTestCase(RotationTests)
    result=unittest.TextTestRunner(stream=stream,verbosity=2).run(suite)
    if RECORD:
        (HERE/'test_results.txt').write_text(stream.getvalue(),encoding='utf-8')
    positive_names={'test_success_lf_exact_and_receipt','test_success_crlf','test_dry_run_zero_writes',
                    'test_check_cli','test_final_partial','test_final_revise','test_previous_archived_success',
                    'test_amendment_exact','test_windows_case_paths','test_source_pin_success'}
    total_names={n for n in dir(RotationTests) if n.startswith('test_')}
    summary={'tests':result.testsRun,'failures':len(result.failures),'errors':len(result.errors),
             'skipped':len(result.skipped),'success':result.wasSuccessful(),
             'positive_scenarios':len(positive_names),'negative_scenarios':len(total_names-positive_names)}
    if RECORD:
        (HERE/'test_summary.json').write_text(json.dumps(summary,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(summary))
    if not result.wasSuccessful():print(stream.getvalue())
    raise SystemExit(0 if result.wasSuccessful() else 1)
