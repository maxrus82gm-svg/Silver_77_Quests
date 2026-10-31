"""Read-only DayZ documentation graph and historical-link checks."""
from pathlib import Path
import json
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
DOC = ROOT / "Documentation_DayZ_Qwest"
BEFORE = DOC / "Task_Reports/TASK_184/tests/navigation_edges.json"


class DocumentationGraphTests(unittest.TestCase):
    def test_graph_filter_and_color_groups(self):
        graph = json.loads((ROOT / ".obsidian/graph.json").read_text(encoding="utf-8"))
        query = graph["search"]
        self.assertIn('path:"Documentation_DayZ_Qwest/"', query)
        for token in ("Task_Reports/", "Ultra/", "90_Legacy_Archive/",
                      "22_Legacy_Check/", "11_Задача_агенту.md", "12_Старт_агента.md"):
            self.assertIn('-path:"Documentation_DayZ_Qwest/' + token + '"', query)
        self.assertIn("11_Задачи_Codex.md", graph["colorGroups"][0]["query"])
        self.assertIn("12_Старт.md", graph["colorGroups"][1]["query"])
        self.assertTrue(graph["showOrphans"])

    def test_historical_wiki_links_not_lost(self):
        before = json.loads(BEFORE.read_text(encoding="utf-8"))["before"]
        missing = []
        for source, links in before.items():
            path = ROOT / source
            if not path.is_file():
                missing.append((source, "source not found"))
                continue
            content = path.read_text(encoding="utf-8")
            for raw, _resolved in links:
                if source == "Documentation_DayZ_Qwest/00_Главная.md" and raw.startswith(
                        ("11_Задача_агенту", "12_Старт_агента", "Ultra/00_Главная")):
                    continue  # New canonical destinations, intentionally archived Ultra.
                if "[[" + raw + "]]" not in content:
                    missing.append((source, raw))
        self.assertFalse(missing, f"Previously linked sources changed: {missing[:15]}")

    def test_new_wiki_links_only_for_visual_navigation(self):
        hub = (DOC / "00_Главная.md").read_text(encoding="utf-8")
        for target in ("000_Задачи для агента", "11_Задачи_Codex",
                       "12_Старт", "001_История_задач_Codex",
                       "05_Очередь_задач", "42_TACTICAL_CONTEXT"):
            self.assertIn("[[" + target, hub)
        self.assertNotIn("[[Ultra/00_Главная", hub)
        for relative in ("42_TACTICAL_CONTEXT.md", "11_Задача_агенту.md",
                         "12_Старт_агента.md", "001_История_задач_Codex.md",
                         "000_Задачи для агента.md"):
            self.assertNotIn("[[", (DOC / relative).read_text(encoding="utf-8"), relative)
        index = (DOC / "21_Project_Docs/00_INDEX.md").read_text(encoding="utf-8")
        self.assertNotIn("## Организационный вход", index)
        card = (DOC / "11_Задачи_Codex.md").read_text(encoding="utf-8")
        self.assertEqual(re.findall(r"\[\[([^\]]+)\]\]", card),
                         ["001_История_задач_Codex"])

    def test_snapshot_documentation_still_exists(self):
        archive = DOC / "Task_Reports/TASK_184/snapshot/files/Documentation_DayZ_Qwest"
        self.assertTrue((archive / "00_Главная.md").is_file())
        self.assertTrue((archive / "04_Архитектура.md").is_file())


if __name__ == "__main__":
    unittest.main(verbosity=2)
