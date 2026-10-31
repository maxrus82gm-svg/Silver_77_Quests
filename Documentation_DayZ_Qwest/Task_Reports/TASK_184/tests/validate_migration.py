"""Read-only validation, except --record writes evidence in TASK184 report area."""
from pathlib import Path
import argparse
import collections
import hashlib
import importlib.util
import json
import os
import re
import sys
import tempfile
from urllib.parse import unquote

ROOT=Path(__file__).absolute().parents[4]
D=ROOT/'Documentation_DayZ_Qwest'
OUT=Path(__file__).absolute().parent.parent
BASE=json.loads((OUT/'snapshot/baseline.json').read_text(encoding='utf-8'))


def sha(b):return hashlib.sha256(b).hexdigest()


def inventory():
    result={}
    for folder,dirs,files in os.walk(ROOT,followlinks=False):
        dirs[:]=[d for d in dirs if d not in ['.git','node_modules','.venv','__pycache__']]
        for name in files:
            p=Path(folder)/name
            rel=p.relative_to(ROOT).as_posix()
            data=p.read_bytes()
            result[rel]={'size':len(data),'sha256':sha(data)}
    return result


def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m


def validate(record=False):
    results=[]
    def check(label,condition):
        results.append({'check':label,'passed':bool(condition)})
        if not condition:raise AssertionError(label)
    current=inventory()
    changed=[p for p,b in BASE['files'].items() if current.get(p)!=b]
    external=[p for p in changed if p in ['.obsidian/graph.json','.obsidian/workspace.json']]
    changed=[p for p in changed if p not in external]
    created=[p for p in current if p not in BASE['files']]
    writes=json.loads((OUT/'tests/migration_writes.json').read_text(encoding='utf-8'))
    approved_new={'Tools/task_slot_rotate.py',*[p for p in writes if p not in BASE['files']]}
    check('only approved pre-existing files changed',set(changed)<=set(BASE['mutable']))
    check('new files limited to declared documents, utility and report package',
          all(p in approved_new or p.startswith('Documentation_DayZ_Qwest/Task_Reports/TASK_184/') for p in created))
    for rel in BASE['mutable']:
        check('snapshot byte-exact: '+rel,sha((OUT/'snapshot/files'/rel).read_bytes())==BASE['files'][rel]['sha256'])
    protected=[p for p in BASE['files'] if p not in BASE['mutable'] and p not in external]
    check('all protected source/config/history files unchanged; observed external settings reported separately',all(current.get(p)==BASE['files'][p] for p in protected))
    check('legacy backlog retained byte-exact', (D/'05_Очередь_задач.md').read_bytes().endswith(
          (OUT/'snapshot/files/Documentation_DayZ_Qwest/05_Очередь_задач.md').read_bytes()))
    S=module('rotation_validation',ROOT/'Tools/task_slot_rotate.py')
    card=(D/'11_Задачи_Codex.md').read_bytes();spans,_=S.layout(card)
    body=card[slice(*spans[0])]
    check('real TASK184 exact and active',body==(OUT/'snapshot/block_1_task.md').read_bytes() and S.task_identity(body)=='TASK-184')
    check('real B1.1 remains uninitialized',card[slice(*spans[1])]==S.EMPTY.encode()+b'\n')
    source=(OUT/'snapshot/files/Documentation_DayZ_Qwest/11_Задача_агенту.md').read_bytes()
    b2=source.index('# 🟩 БЛОК 2 — ПОСЛЕДНИЙ ОТЧЁТ АГЕНТА'.encode())
    b3=source.index('# 🟨 БЛОК 3 — REVIEW / STATUS'.encode())
    check('previous full report181 exact',source[b2:b3]==(OUT/'snapshot/block_2_legacy.md').read_bytes())
    check('previous full review101 exact',source[b3:]==(OUT/'snapshot/block_3_legacy.md').read_bytes())
    check('independent slot remains unfilled', 'независимого verdict TASK-184 нет' in card.decode())
    check('no self-closeout in permanent history',current['Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md']==BASE['files']['Documentation_DayZ_Qwest/20_SplitDoc/TASK_HISTORY.md'])
    check('no invented accepted archive entries','Пока не заполнено координатором' in (D/'001_История_задач_Codex.md').read_text(encoding='utf-8'))
    check('old entrypoints are noncompeting locators',all('LEGACY LOCATOR' in (D/n).read_text(encoding='utf-8') for n in ['11_Задача_агенту.md','12_Старт_агента.md']))
    for rel in writes:
        data=(ROOT/rel).read_bytes()
        text=data.decode('utf-8',errors='strict')
        check('UTF8/no BOM/NUL: '+rel,not data.startswith(b'\xef\xbb\xbf') and b'\0' not in data)
        stripped=data.replace(b'\r\n',b'')
        consistent=b'\r' not in stripped and not (b'\r\n' in data and b'\n' in stripped)
        if rel=='Documentation_DayZ_Qwest/05_Очередь_задач.md':
            # The historic suffix already has mixed EOL; preserving its exact
            # bytes takes precedence over normalizing somebody else's history.
            suffix=(OUT/'snapshot/files'/rel).read_bytes()
            prefix=data[:-len(suffix)]
            consistent=data.endswith(suffix) and b'\r' not in prefix.replace(b'\r\n',b'')
        elif not consistent and (OUT/'snapshot/files'/rel).exists():
            original=(OUT/'snapshot/files'/rel).read_bytes()
            consistent=re.findall(rb'\r\n|(?<!\r)\n',data)==re.findall(rb'\r\n|(?<!\r)\n',original)
        check('consistent new EOL / byte-exact legacy suffix: '+rel,consistent)
        prior=OUT/'snapshot/files'/rel
        if rel=='Documentation_DayZ_Qwest/12_Старт.md':
            prior=OUT/'snapshot/files/Documentation_DayZ_Qwest/12_Старт_агента.md'
        allowed=prior.read_text(encoding='utf-8').count('\ufffd') if prior.exists() else 0
        check('no new replacement characters (literal encoding examples retained): '+rel,text.count('\ufffd')<=allowed)
    registry=(D/'05_Очередь_задач.md').read_text(encoding='utf-8')
    active=registry.split('## Исторический backlog')[0]
    check('no old Ultra task imported into active registry',not re.search(r'TASK[ -]183',active))
    check('107/108 disagreements remain OPEN',all(x in active for x in ['FU-DOC-107-STATUS','FU-DOC-108-STATUS','STATUS CONFLICT']))
    routing=(D/'20_SplitDoc/CONTEXT_ROUTING.md').read_text(encoding='utf-8')
    check('no active Ultra bridge dispatch',not any(x in routing for x in ['gigachat_file_task','task_file:','report_file:','EXECUTOR: ULTRA']))
    for rel in ['Tools/task_slot_rotate.py',*['Documentation_DayZ_Qwest/Task_Reports/TASK_184/tests/'+p for p in ['test_task_slot_rotate.py','rollback_migration.py','validate_migration.py']]]:
        compile((ROOT/rel).read_bytes(),rel,'exec')
    check('Python source syntax',True)

    # Conservative static navigation resolver. Not an Obsidian metadata-cache export.
    mdpaths={p for p in current if p.endswith(('.md','.canvas')) and '/Task_Reports/' not in p}
    allpaths={p for p in current if '/Task_Reports/' not in p}
    folded={p.casefold():p for p in allpaths}
    check('new active document paths unique ignoring case',all(sum(p.casefold()==q.casefold() for q in mdpaths)==1 for p in approved_new if p.endswith('.md')))
    def resolve(source,target):
        target=unquote(target.split('|')[0].split('#')[0])
        if not target:return source
        candidates=[target] if Path(target).suffix else [target+'.md',target+'.canvas']
        for name in candidates:
            for p in [(Path(source).parent/name).as_posix(),name,'Documentation_DayZ_Qwest/'+name]:
                if p.casefold() in folded:return folded[p.casefold()]
        matches=[p for p in allpaths if any(p.endswith('/'+n) for n in candidates)]
        return matches[0] if len(matches)==1 else None
    def edges(source,text):
        # Links in code examples are not graph edges.
        text=re.sub(r'(?ms)^(`{3,}|~{3,})[^\n]*\n.*?^\1[^\n]*$', '', text)
        text=re.sub(r'`[^`\n]*`','',text)
        return [(target,resolve(source,target)) for target in re.findall(r'!?\[\[([^\]\n]+)\]\]',text)]
    before_edges={};after_edges={};new_broken=[]
    for p in mdpaths:
        if not p.endswith('.md'):continue
        now=(ROOT/p).read_text(encoding='utf-8-sig')
        after_edges[p]=edges(p,now)
        if p in BASE['files']:
            prior=OUT/'snapshot/files'/p
            before=prior.read_text(encoding='utf-8-sig') if prior.exists() else now
            before_edges[p]=edges(p,before)
        old=collections.Counter(t for t,_ in before_edges.get(p,[]))
        for target,resolved in after_edges[p]:
            if not resolved and not old[target]:new_broken.append({'source':p,'target':target})
    check('no new unresolved wiki edges in active documents',not new_broken)
    check('all old 11/12 path references still resolve through compatibility nodes',
          (D/'11_Задача_агенту.md').is_file() and (D/'12_Старт_агента.md').is_file())
    # Pre-existing ambiguities/broken links are reported, not repaired outside scope.
    baseline_unresolved=[{'source':p,'target':t} for p,es in before_edges.items() for t,r in es if not r]
    anchors=[]
    for p,es in after_edges.items():
        for target,resolved in es:
            if '#' in target and resolved:
                anchor=target.split('|')[0].split('#',1)[1]
                anchors.append({'source':p,'target':resolved,'anchor':anchor,'review':'STATIC CANDIDATE; Obsidian GUI not exported'})

    # Per-occurrence matrix: frozen TASK184 and history remain original text.
    rows=[];updated=0
    for ref in BASE['references']:
        source=ref['source'];action=ref['action'];target='11_Задачи_Codex.md' if '11_Задача_агенту' in ref['text'] else '12_Старт.md'
        if source=='Documentation_DayZ_Qwest/11_Задача_агенту.md':action='BYTE_EXACT_SNAPSHOT_AND_FROZEN_TASK'
        elif action=='UPDATE_ACTIVE':
            source_path=ROOT/source
            if source=='Documentation_DayZ_Qwest/12_Старт_агента.md':
                source_path=D/'12_Старт.md';action='MOVED_ACTIVE_TO_NEW_START'
            text=source_path.read_text(encoding='utf-8-sig')
            new_token='11_Задачи_Codex' if '11_Задача_агенту' in ref['text'] else '12_Старт'
            check('active reference updated: '+source+':'+str(ref['line']),new_token in text)
            updated+=1
        rows.append({**ref,'action':action,'new_target':target,'target_exists':(D/target).is_file(),
                     'owner':'USER SETTINGS' if source.startswith('.obsidian/') else 'HISTORY' if action.startswith('PRESERVE') else 'DOCUMENTATION'})

    R=module('rollback_validation',OUT/'tests/rollback_migration.py')
    with tempfile.TemporaryDirectory(prefix='rollback-fixture-',dir=OUT/'tests') as tmp:
        r=Path(tmp);(r/'snapshot').mkdir();(r/'snapshot/old.md').write_bytes(b'old exact\r\n')
        (r/'old.md').write_bytes(b'candidate\n');(r/'new.md').write_bytes(b'new unique\n')
        state={'rollback_saved':'saved','files':[
            {'path':'old.md','after_sha256':sha(b'candidate\n'),'before_sha256':sha(b'old exact\r\n'),'snapshot':'snapshot/old.md'},
            {'path':'new.md','after_sha256':sha(b'new unique\n'),'before_sha256':None,'snapshot':None}]}
        check('rollback dry-run zero writes',R.rollback(r,state)['writes']==0 and not (r/'saved').exists())
        (r/'old.md').write_bytes(b'user edit')
        try:R.rollback(r,state,True);raise AssertionError('rollback accepted external edit')
        except ValueError:pass
        check('rollback rejects user changes without modifying new file',(r/'old.md').read_bytes()==b'user edit' and (r/'new.md').read_bytes()==b'new unique\n' and not (r/'saved').exists())
        (r/'old.md').write_bytes(b'candidate\n');R.rollback(r,state,True)
        check('rollback restores exact source/removes only saved candidate',(r/'old.md').read_bytes()==b'old exact\r\n' and not (r/'new.md').exists() and (r/'saved/new.md').read_bytes()==b'new unique\n')

    state={'schema_version':1,'rollback_saved':'Documentation_DayZ_Qwest/Task_Reports/TASK_184/rollback_saved','files':[]}
    for p in changed+[p for p in created if p in approved_new]:
        state['files'].append({'path':p,'before_sha256':BASE['files'].get(p,{}).get('sha256'),
            'after_sha256':current[p]['sha256'],
            'snapshot':('Documentation_DayZ_Qwest/Task_Reports/TASK_184/snapshot/files/'+p) if p in BASE['files'] else None})
    summary={'passed':True,'checks':len(results),'changed_existing':changed,
             'new_functional_files':[p for p in created if p in approved_new],
             'protected_files_unchanged':len(protected),'reference_occurrences':len(rows),
             'updated_reference_occurrences':updated,'wiki_edges_checked':sum(map(len,after_edges.values())),
             'preexisting_unresolved':baseline_unresolved,'anchor_candidates':anchors,
             'external_settings_changes':[{'path':p,'before':BASE['files'][p],'observed':current[p]} for p in external],
             'live_graph_export':'NOT RUN','real_rotation':'NOT RUN / prohibited in TASK184',
             'rollback_fixture_checks':3,'results':results}
    if record:
        (OUT/'tests/validation_results.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
        if external:
            evidence=OUT/'tests/observed_settings';evidence.mkdir(exist_ok=True)
            for p in external:
                (evidence/Path(p).name).write_bytes((ROOT/p).read_bytes())
        (OUT/'migration_state.json').write_text(json.dumps(state,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
        (OUT/'tests/navigation_edges.json').write_text(json.dumps({'before':before_edges,'after':after_edges},ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
        header='''# TASK184 — матрица путей, владельцев и сохранности

Источник: локальный byte-exact baseline; Git/GitHub не использовались.
Все относительные пути здесь отсчитываются от P:/Silver_77_Quests.
Старые11/12 — только compatibility locators, новый активный вход один.
Пользовательские Obsidian-фильтры не редактировались. Исторические цитаты и
замороженная TASK184 не переписывались. Source line — координата до миграции.

## OLD PATH/HEADING → NEW PATH/HEADING → OWNER → ACTION → CHECK

| OLD PATH / heading | NEW PATH / heading | OWNER | ACTION | CHECK |
| --- | --- | --- | --- | --- |
| 11_Задача_агенту / БЛОК1 | 11_Задачи_Codex / БЛОК1 + 4 markers | USER/Codex | точные байты текущей184, без ротации | snapshot/block_1_task.md SHA/byte equality |
| прежний БЛОК2 TASK181 | snapshot/block_2_legacy.md | HISTORY | архив без импорта как результата184 | byte-exact readback |
| прежний БЛОК3 TASK101 | snapshot/block_3_legacy.md | verifier HISTORY | сохранён старый verdict, новый БЛОК4 пуст | byte-exact readback |
| 12_Старт_агента | 12_Старт | executor core | действующее содержание перенесено; старое имя locator | UTF8 / routes / retained headings |
| прежний05 целиком | 05 / Исторический backlog | HISTORY | полный неизменённый suffix | byte equality |
| TASK_HISTORY | тот же файл | HISTORY | без новых self-closeout записей | SHA unchanged |
| №30/38/39/40/41, Ultra/*, .obsidian | те же файлы | protected owners | без записей | full inventory SHA |

## Каждое исходное вхождение старых путей

| SOURCE:line | Исходная строка | NEW TARGET | OWNER / ACTION | CHECK |
| --- | --- | --- | --- | --- |
'''
        for row in rows:
            escaped=row['text'].replace('|','\\|').replace('\n',' ')
            header+=f"| {row['source']}:{row['line']} | {escaped} | {row['new_target']} | {row['owner']} / {row['action']} | target exists={row['target_exists']}; snapshot/context retained |\n"
        header+='\n## Фактический набор изменений\n\n'
        header+='\n'.join('- '+p for p in changed+[p for p in created if p in approved_new])+'\n'
        header+='\nГраф: статический inventory в tests/navigation_edges.json; не live metadata-cache export Obsidian. Архивные snapshots имеют отдельные path-узлы; визуальная приёмка NOT RUN.\n'
        (OUT/'migration_matrix.md').write_text(header,encoding='utf-8')
    print(json.dumps({k:summary[k] for k in ['passed','checks','protected_files_unchanged','reference_occurrences','updated_reference_occurrences','wiki_edges_checked','preexisting_unresolved','external_settings_changes']},ensure_ascii=False))
    return summary


if __name__=='__main__':
    sys.stdout.reconfigure(encoding='utf-8')
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--record',action='store_true')
    args=parser.parse_args();validate(args.record)
