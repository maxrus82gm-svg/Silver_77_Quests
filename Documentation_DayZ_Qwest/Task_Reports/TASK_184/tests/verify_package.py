"""Verify sealed report/source hashes without rotating or writing any files."""
from pathlib import Path
import hashlib
import json
import re
import sys

ROOT=Path(__file__).absolute().parents[4]
OUT=Path(__file__).absolute().parent.parent


def verify():
    manifest=json.loads((OUT/'manifest.json').read_text(encoding='utf-8'))
    assert manifest['task_id']=='TASK-184' and manifest['final'] is True
    for row in manifest['artifacts']:
        p=(OUT/row['path'])
        assert p.absolute().is_relative_to(OUT) and '..' not in Path(row['path']).parts
        data=p.read_bytes()
        assert len(data)==row['size'] and hashlib.sha256(data).hexdigest()==row['sha256'],str(p)
    for row in manifest['source_files']:
        p=ROOT/row['path'];assert p.absolute().is_relative_to(ROOT)
        data=p.read_bytes()
        assert len(data)==row['size'] and hashlib.sha256(data).hexdigest()==row['sha256'],str(p)
    for row in manifest['changes']:
        p=ROOT/row['path'];assert p.absolute().is_relative_to(ROOT)
        data=p.read_bytes();pin=row['after']
        assert len(data)==pin['size'] and hashlib.sha256(data).hexdigest()==pin['sha256'],str(p)
    report=(OUT/'report.md').read_text(encoding='utf-8')
    assert re.search(r'^TASK ID: TASK-184$',report,re.M)
    assert re.search(r'^STATUS: RESULT READY$',report,re.M)
    data=(ROOT/'Documentation_DayZ_Qwest/11_Задачи_Codex.md').read_bytes()
    a=data.index(b'<!-- TASK-SLOT:BLOCK-1:BEGIN -->\n')+len(b'<!-- TASK-SLOT:BLOCK-1:BEGIN -->\n')
    b=data.index(b'<!-- TASK-SLOT:BLOCK-1:END -->')
    assert data[a:b]==(OUT/'snapshot/block_1_task.md').read_bytes()
    assert b'**TASK ID:** `TASK-184`' in data[a:b]
    print(json.dumps({'result':'PACKAGE_INTEGRITY_OK','artifacts':len(manifest['artifacts']),
                      'source_files':len(manifest['source_files']),'real_rotation':False,
                      'functional_files':len(manifest['changes']),
                      'independent_acceptance':'PENDING'}))


if __name__=='__main__':
    sys.stdout.reconfigure(encoding='utf-8');verify()
