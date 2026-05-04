const STORAGE_KEY = "quest_json_workshop_draft_v1";
const BASE_FILE_NAME = "Silver_77_Quests.json";
const SAVE_API_URL = "/api/save";
const CONFIG_API_URL = "/api/config";
const CURRENT_JSON_API_URL = "/api/current-json";
const DRAFT_API_URL = "/api/draft";
const STACK_RULES_API_URL = "/api/stack-rules";
const STACK_RULES_VERSION = 1;

const DEFAULT_DATA = {
  version: 3,
  quests: [],
  triggers: []
};

const state = {
  data: normalizeData(DEFAULT_DATA),
  selected: null,
  fileName: "quests.json",
  dirty: false,
  filter: "",
  sidebarQuestTriggerId: "",
  npcFlowMode: "active",
  npcFlowSelectedTriggerId: "",
  npcFlowDraftTriggerIdsByQuest: {},
  issues: [],
  banner: null,
  lastImportedAt: "",
  config: {
    savePath: "",
    backupPath: ""
  },
  stackRules: [],
  stackRulesDirty: false,
  stackRulesStatus: {
    kind: "neutral",
    text: "Не загружено"
  }
};

const elements = {};
let draftSaveTimer = null;
let stackRulesSaveTimer = null;

document.addEventListener("DOMContentLoaded", () => {
  void init();
});

async function init() {
  cacheElements();
  bindEvents();

  const restored = await restoreDraft();
  if (restored) {
    const restoredFromText = restored.source === "file"
      ? "Черновик из файла проекта восстановлен."
      : "Черновик из локального хранилища восстановлен.";

    loadData(restored.data, restored.fileName || "quests.json", { dirty: false });

    const blockingIssues = getBlockingIssues(state.issues);
    updateBanner(
      blockingIssues.length
        ? {
          kind: "warning",
          text: `${restoredFromText} ${formatBlockingIssuesBannerText(blockingIssues, "Сейчас экспорт заблокирован.")}`
        }
        : {
          kind: "success",
          text: restoredFromText
        }
    );
  } else {
    loadData(DEFAULT_DATA, "quests.json", { dirty: false });
  }

  renderAll();
  void loadConfig(!restored);
  void loadStackRules();
}

function cacheElements() {
  elements.fileNameLabel = document.getElementById("fileNameLabel");
  elements.dirtyLabel = document.getElementById("dirtyLabel");
  elements.issueCountLabel = document.getElementById("issueCountLabel");
  elements.validationSummary = document.getElementById("validationSummary");
  elements.banner = document.getElementById("banner");
  elements.importInput = document.getElementById("importInput");
  elements.questList = document.getElementById("questList");
  elements.triggerList = document.getElementById("triggerList");
  elements.editorPane = document.getElementById("editorPane");
  elements.validationList = document.getElementById("validationList");
  elements.jsonPreview = document.getElementById("jsonPreview");
  elements.searchInput = document.getElementById("searchInput");
  elements.questTriggerFilter = document.getElementById("questTriggerFilter");
  elements.questTriggerFilterHint = document.getElementById("questTriggerFilterHint");
  elements.savePathInput = document.getElementById("savePathInput");
  elements.backupPathInput = document.getElementById("backupPathInput");
  elements.saveConfigButton = document.getElementById("saveConfigButton");
  elements.configStatusLabel = document.getElementById("configStatusLabel");
  elements.stackRuleList = document.getElementById("stackRuleList");
  elements.addStackRuleButton = document.getElementById("addStackRuleButton");
  elements.saveStackRulesButton = document.getElementById("saveStackRulesButton");
  elements.stackRulesStatusLabel = document.getElementById("stackRulesStatusLabel");
}

function bindEvents() {
  document.getElementById("newProjectButton").addEventListener("click", handleNewProject);
  document.getElementById("loadBaseButton").addEventListener("click", () => {
    void handleLoadBaseFile();
  });
  document.getElementById("importButton").addEventListener("click", () => elements.importInput.click());
  document.getElementById("validateButton").addEventListener("click", () => {
    state.issues = validateData(state.data);
    updateBanner({
      kind: state.issues.some((issue) => issue.level === "error") ? "warning" : "success",
      text: state.issues.length
        ? "Проверка завершена. Ниже показаны найденные замечания."
        : "Проверка завершена. Критичных замечаний не найдено."
    });
    renderValidation();
    renderHeader();
  });
  document.getElementById("copyButton").addEventListener("click", handleCopyJson);
  document.getElementById("exportButton").addEventListener("click", handleExport);
  document.getElementById("addQuestButton").addEventListener("click", addQuest);
  document.getElementById("addTriggerButton").addEventListener("click", addTrigger);
  elements.addStackRuleButton.addEventListener("click", addStackRule);
  elements.saveConfigButton.addEventListener("click", () => {
    void handleSaveConfig();
  });
  elements.saveStackRulesButton.addEventListener("click", () => {
    void handleSaveStackRules();
  });

  elements.searchInput.addEventListener("input", (event) => {
    state.filter = String(event.target.value || "").trim().toLowerCase();
    renderSidebar();
    renderStackRules();
  });

  elements.questTriggerFilter.addEventListener("change", handleQuestTriggerFilterChange);

  elements.importInput.addEventListener("change", handleImportFile);

  elements.questList.addEventListener("click", handleSidebarClick);
  elements.triggerList.addEventListener("click", handleSidebarClick);
  elements.stackRuleList.addEventListener("input", handleStackRuleInput);
  elements.stackRuleList.addEventListener("click", handleStackRuleClick);

  elements.editorPane.addEventListener("input", handleEditorInput);
  elements.editorPane.addEventListener("change", handleEditorInput);
  elements.editorPane.addEventListener("click", handleEditorClick);
  window.addEventListener("beforeunload", handleBeforeUnload);
}

function handleBeforeUnload() {
  if (!state.dirty && !state.stackRulesDirty) {
    return;
  }

  saveDraftNow();
}

function handleSidebarClick(event) {
  const button = event.target.closest("[data-select-kind]");
  if (!button) {
    return;
  }

  const kind = button.dataset.selectKind;
  const index = Number(button.dataset.selectIndex);
  state.selected = {
    kind,
    index
  };

  if (kind === "trigger") {
    const trigger = state.data.triggers[index];
    state.sidebarQuestTriggerId = trigger?.id || "";
    state.npcFlowSelectedTriggerId = trigger?.id || state.npcFlowSelectedTriggerId;
  }

  renderSidebar();
  renderEditor();
}

function handleQuestTriggerFilterChange(event) {
  state.sidebarQuestTriggerId = String(event.target.value || "").trim();
  if (state.sidebarQuestTriggerId) {
    state.npcFlowSelectedTriggerId = state.sidebarQuestTriggerId;
    state.npcFlowMode = "selected";
  } else if (
    normalizeNpcFlowMode(state.npcFlowMode) === "selected" &&
    !normalizeNpcFlowSelectedTriggerId(state.npcFlowSelectedTriggerId)
  ) {
    state.npcFlowMode = "active";
  }
  renderSidebar();
  renderEditor();
}

function handleStackRuleInput(event) {
  const target = event.target;
  if (!target.dataset.stackRuleField) {
    return;
  }

  const index = Number(target.dataset.stackRuleIndex);
  const rule = state.stackRules[index];
  if (!rule) {
    return;
  }

  if (target.dataset.stackRuleField === "stackSize") {
    rule.stackSize = Math.max(1, parseNumber(target.value, 1));
  } else {
    rule.className = String(target.value || "");
  }

  touchStackRules();
  renderStackRules();
}

function handleStackRuleClick(event) {
  const button = event.target.closest("[data-stack-rule-action]");
  if (!button) {
    return;
  }

  if (button.dataset.stackRuleAction === "remove") {
    removeStackRule(Number(button.dataset.stackRuleIndex));
  }
}

function handleEditorInput(event) {
  const target = event.target;
  if (target.dataset.role === "npc-flow-trigger-picker") {
    const selectedTriggerId = String(target.value || "").trim();
    state.npcFlowSelectedTriggerId = selectedTriggerId;

    if (selectedTriggerId && state.selected?.kind === "quest") {
      const quest = state.data.quests[state.selected.index];
      addQuestFlowDraftTriggerId(quest, state.selected.index, selectedTriggerId);
      state.npcFlowMode = "active";
    } else {
      state.npcFlowMode = selectedTriggerId ? "selected" : "active";
    }

    renderEditor();
    return;
  }

  if (target.dataset.role === "quest-flow-card-trigger-picker") {
    handleQuestFlowCardTriggerChange(
      Number(target.dataset.questIndex),
      target.dataset.currentTriggerId,
      target.value
    );
    return;
  }

  if (target.dataset.role === "quest-trigger-toggle") {
    handleQuestTriggerToggleChange(
      Number(target.dataset.questIndex),
      target.dataset.triggerRole,
      target.dataset.triggerId,
      target.checked
    );
    return;
  }

  if (!target.dataset.path) {
    return;
  }

  const path = target.dataset.path;
  const previousValue = getValueByPath(state.data, path);
  const nextValue = readInputValue(target);
  setValueByPath(state.data, path, nextValue);

  const questIdMatch = path.match(/^quests\.(\d+)\.id$/);
  if (questIdMatch) {
    syncQuestIdReferences(previousValue, nextValue);
  }
  
  const triggerIdMatch = path.match(/^triggers\.(\d+)\.id$/);
  if (triggerIdMatch) {
    syncTriggerIdReferences(previousValue, nextValue);
  }

  touchState();

  if (path.endsWith(".id") || path.endsWith(".name") || path.endsWith(".hintText")) {
    renderSidebar();
  }

  renderHeader();
  renderValidation();
  renderPreview();
}

function handleEditorClick(event) {
  const button = event.target.closest("[data-action]");
  if (!button) {
    return;
  }

  const action = button.dataset.action;

  if (action === "add-array-item") {
    addArrayItem(button.dataset.path, button.dataset.itemType);
    return;
  }

  if (action === "remove-array-item") {
    removeArrayItem(button.dataset.path, Number(button.dataset.index));
    return;
  }

  if (action === "remove-entity") {
    removeSelectedEntity(button.dataset.entityKind, Number(button.dataset.index));
    return;
  }

  if (action === "duplicate-entity") {
    duplicateEntity(button.dataset.entityKind, Number(button.dataset.index));
    return;
  }

  if (action === "create-quest") {
    addQuest();
    return;
  }

  if (action === "create-trigger") {
    addTrigger();
    return;
  }

  if (action === "load-base-file") {
    void handleLoadBaseFile();
    return;
  }

  if (action === "set-npc-flow-mode") {
    state.npcFlowMode = normalizeNpcFlowMode(button.dataset.mode);
    renderEditor();
  }
}

function handleNewProject() {
  const confirmed = window.confirm("Создать новый проект и сбросить текущий черновик?");
  if (!confirmed) {
    return;
  }

  loadData(DEFAULT_DATA, "quests.json", {
    dirty: false,
    banner: {
      kind: "success",
      text: "Создан пустой проект. Можно добавлять новые квесты и триггеры."
    }
  });

  void clearDraftStorage();
}

async function handleImportFile(event) {
  const [file] = event.target.files || [];
  if (!file) {
    return;
  }

  const text = await file.text();
  const parsed = tryParseJsonText(text);

  if (!parsed.ok) {
    updateBanner({
      kind: "error",
      text: `Импорт не удался: ${parsed.error}`
    });
    event.target.value = "";
    return;
  }

  loadData(parsed.data, file.name, {
    dirty: false,
    banner: {
      kind: parsed.repaired ? "warning" : "success",
      text: parsed.repaired
        ? "Файл загружен с автоматической починкой пустых числовых полей и висячих запятых."
        : "Файл успешно импортирован."
    }
  });

  state.lastImportedAt = new Date().toLocaleString("ru-RU");
  event.target.value = "";
}

async function handleLoadBaseFile() {
  try {
    const response = await fetch(`./${BASE_FILE_NAME}`, { cache: "no-store" });
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const text = await response.text();
    const parsed = tryParseJsonText(text);
    if (!parsed.ok) {
      throw new Error(parsed.error);
    }

    loadData(parsed.data, BASE_FILE_NAME, {
      dirty: false,
      banner: {
        kind: parsed.repaired ? "warning" : "success",
        text: parsed.repaired
          ? `Базовый файл ${BASE_FILE_NAME} загружен с автоисправлением пустых числовых полей.`
          : `Базовый файл ${BASE_FILE_NAME} загружен.`
      }
    });
  } catch (error) {
    updateBanner({
      kind: "warning",
      text: `Автозагрузка ${BASE_FILE_NAME} не сработала. Если редактор открыт как локальный файл, просто нажми "Импорт JSON" и выбери его вручную.`
    });
  }
}

async function loadConfig(autoLoadConfiguredFile = false) {
  if (!window.location.protocol.startsWith("http")) {
    state.config = {
      savePath: "",
      backupPath: ""
    };
    renderConfig();
    return;
  }

  try {
    const response = await fetch(CONFIG_API_URL, { cache: "no-store" });
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const config = await response.json();
    state.config = {
      savePath: String(config.savePath || ""),
      backupPath: String(config.backupPath || "")
    };
    renderConfig();
    renderHeader();
    if (autoLoadConfiguredFile) {
      await loadConfiguredSaveFile();
    }
  } catch (error) {
    state.config = {
      savePath: "",
      backupPath: ""
    };
    renderConfig("warning", "Пути не загружены");
  }
}

async function loadStackRules() {
  if (!window.location.protocol.startsWith("http")) {
    state.stackRules = [];
    state.stackRulesDirty = false;
    renderStackRulesStatus("warning", "Справочник только через сервер");
    renderStackRules();
    return;
  }

  renderStackRulesStatus("neutral", "Загрузка...");

  try {
    const response = await fetch(STACK_RULES_API_URL, { cache: "no-store" });
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const payload = await response.json();
    state.stackRules = normalizeStackRules(payload.rules);
    state.stackRulesDirty = false;
    renderStackRulesStatus(
      state.stackRules.length ? "good" : "neutral",
      state.stackRules.length ? `Правил: ${state.stackRules.length}` : "Справочник пуст"
    );
  } catch (error) {
    state.stackRules = [];
    state.stackRulesDirty = false;
    renderStackRulesStatus("bad", "Ошибка загрузки");
  }

  renderStackRules();
}

async function loadConfiguredSaveFile() {
  try {
    const response = await fetch(CURRENT_JSON_API_URL, { cache: "no-store" });
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const text = await response.text();
    const parsed = tryParseJsonText(text);
    if (!parsed.ok) {
      throw new Error(parsed.error);
    }

    const sourceFileName = response.headers.get("X-File-Name") || getBaseNameFromPath(state.config.savePath || BASE_FILE_NAME);
    loadData(parsed.data, sourceFileName, {
      dirty: false,
      banner: {
        kind: parsed.repaired ? "warning" : "success",
        text: parsed.repaired
          ? `Рабочий файл ${sourceFileName} загружен с автоисправлением пустых числовых полей.`
          : `Рабочий файл ${sourceFileName} загружен по пути сохранения.`
      }
    });
  } catch (error) {
    updateBanner({
      kind: "warning",
      text: "Не удалось автоматически загрузить файл по пути сохранения. Проверь Save Path или импортируй JSON вручную."
    });
  }
}

async function handleSaveConfig() {
  const nextConfig = {
    savePath: String(elements.savePathInput.value || "").trim(),
    backupPath: String(elements.backupPathInput.value || "").trim()
  };

  if (!nextConfig.savePath || !nextConfig.backupPath) {
    updateBanner({
      kind: "error",
      text: "И путь сохранения, и путь бекапа должны быть заполнены."
    });
    renderConfig("warning", "Нужно заполнить оба пути");
    return;
  }

  if (!window.location.protocol.startsWith("http")) {
    updateBanner({
      kind: "warning",
      text: "Настройки путей сохраняются только при запуске через start-editor.cmd."
    });
    return;
  }

  try {
    const response = await fetch(CONFIG_API_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json;charset=utf-8"
      },
      body: JSON.stringify(nextConfig)
    });

    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const config = await response.json();
    state.config = {
      savePath: String(config.savePath || ""),
      backupPath: String(config.backupPath || "")
    };
    renderConfig("good", "Пути сохранены");
    renderHeader();
    updateBanner({
      kind: "success",
      text: "Локальные пути сохранены. Следующий запуск создаст бекап по новому пути."
    });
  } catch (error) {
    renderConfig("bad", "Ошибка сохранения путей");
    updateBanner({
      kind: "error",
      text: "Не удалось сохранить пути на локальном компьютере."
    });
  }
}

async function handleSaveStackRules() {
  cancelScheduledStackRulesSave();
  await saveStackRulesNow(true);
}

async function saveStackRulesNow(showBanner = false) {
  if (!window.location.protocol.startsWith("http")) {
    renderStackRulesStatus("warning", "Справочник только через сервер");
    if (showBanner) {
      updateBanner({
        kind: "warning",
        text: "Справочник стеков сохраняется только при запуске через start-editor.cmd."
      });
    }
    return;
  }

  renderStackRulesStatus("neutral", "Сохранение...");

  try {
    const response = await fetch(STACK_RULES_API_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json;charset=utf-8"
      },
      body: JSON.stringify(buildStackRulesPayload())
    });

    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    await response.json();
    state.stackRulesDirty = false;
    renderStackRulesStatus(
      state.stackRules.length ? "good" : "neutral",
      state.stackRules.length ? `Сохранено: ${state.stackRules.length}` : "Справочник пуст"
    );

    if (showBanner) {
      updateBanner({
        kind: "success",
        text: "Справочник размеров стеков сохранен."
      });
    }
  } catch (error) {
    renderStackRulesStatus("bad", "Ошибка сохранения");

    if (showBanner) {
      updateBanner({
        kind: "error",
        text: "Не удалось сохранить справочник размеров стеков."
      });
    }
  }
}

async function handleCopyJson() {
  const payload = JSON.stringify(state.data, null, 2);
  try {
    await navigator.clipboard.writeText(payload);
    updateBanner({
      kind: "success",
      text: "JSON скопирован в буфер обмена."
    });
  } catch (error) {
    updateBanner({
      kind: "error",
      text: "Не удалось скопировать JSON. Возможно, браузер заблокировал доступ к буферу обмена."
    });
  }
}

function handleExport() {
  state.issues = validateData(state.data);
  const blockingIssues = getBlockingIssues(state.issues);
  if (blockingIssues.length) {
    renderHeader();
    renderValidation();
    scrollValidationIntoView();
    updateBanner({
      kind: "error",
      text: formatBlockingIssuesBannerText(blockingIssues, "Экспорт остановлен.")
    });
    return;
  }

  if (window.location.protocol.startsWith("http")) {
    void handleDirectSave();
    return;
  }

  const content = JSON.stringify(state.data, null, 2);
  const blob = new Blob([content], { type: "application/json;charset=utf-8" });
  const url = URL.createObjectURL(blob);
  const anchor = document.createElement("a");
  anchor.href = url;
  anchor.download = ensureJsonExtension(state.fileName || "quests.json");
  anchor.click();
  URL.revokeObjectURL(url);

  state.dirty = false;
  saveDraftNow();
  renderHeader();

  updateBanner({
    kind: "success",
    text: `Экспорт готов: ${anchor.download}`
  });
}

async function handleDirectSave() {
  const content = JSON.stringify(state.data, null, 2);

  try {
    const response = await fetch(SAVE_API_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json;charset=utf-8"
      },
      body: JSON.stringify({
        fileName: ensureJsonExtension(state.fileName || BASE_FILE_NAME),
        json: content
      })
    });

    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const result = await response.json();

    state.dirty = false;
    await clearDraftStorage();
    renderHeader();

    updateBanner({
      kind: "success",
      text: `Файл сохранен поверх ${result.savedTo || ensureJsonExtension(state.fileName || BASE_FILE_NAME)}. Резервная копия создается при запуске редактора.`
    });
  } catch (error) {
    fallbackDownloadExport(content);
    updateBanner({
      kind: "warning",
      text: "Прямое сохранение не сработало, поэтому редактор выгрузил обычный JSON-файл для ручного сохранения."
    });
  }
}

function fallbackDownloadExport(content) {
  const blob = new Blob([content], { type: "application/json;charset=utf-8" });
  const url = URL.createObjectURL(blob);
  const anchor = document.createElement("a");
  anchor.href = url;
  anchor.download = ensureJsonExtension(state.fileName || "quests.json");
  anchor.click();
  URL.revokeObjectURL(url);

  state.dirty = false;
  saveDraftNow();
  renderHeader();
}

function addQuest() {
  const triggerId = pickDefaultTriggerIdForNewQuest();
  if (!triggerId) {
    updateBanner({
      kind: "error",
      text: "Сначала создай хотя бы один trigger / NPC. Поле привязки квеста к trigger обязательно."
    });
    return;
  }

  const quest = createQuest(triggerId);
  state.data.quests.push(quest);
  assignQuestToTrigger(quest.id, triggerId);
  syncQuestTriggerActions(quest);
  state.selected = {
    kind: "quest",
    index: state.data.quests.length - 1
  };
  touchState({
    banner: {
      kind: "success",
      text: `Добавлен квест ${quest.id} и привязан к trigger ${triggerId}.`
    }
  });
  renderAll();
}

function addTrigger() {
  const trigger = createTrigger();
  state.data.triggers.push(trigger);
  state.selected = {
    kind: "trigger",
    index: state.data.triggers.length - 1
  };
  touchState({
    banner: {
      kind: "success",
      text: `Добавлен триггер ${trigger.id}.`
    }
  });
  renderAll();
}

function addStackRule() {
  state.stackRules.push(createStackRule());
  touchStackRules();
  renderStackRules();
}

function removeStackRule(index) {
  if (!state.stackRules[index]) {
    return;
  }

  state.stackRules.splice(index, 1);
  touchStackRules();
  renderStackRules();
}

function handleQuestTriggerToggleChange(questIndex, triggerRole, triggerId, checked) {
  const quest = state.data.quests[questIndex];
  if (!quest) {
    return;
  }

  const normalizedTriggerId = String(triggerId || "").trim();
  if (!normalizedTriggerId) {
    return;
  }

  const fieldName = getQuestRoleFieldName(triggerRole);
  if (!fieldName) {
    return;
  }

  if (checked && !canAssignRoleToTrigger(quest, triggerRole, normalizedTriggerId)) {
    const uniquenessText = isQuestRoleUnique(triggerRole)
      ? `Роль ${triggerRole} должна быть уникальной в квесте.`
      : `Роль ${triggerRole} уже назначена этому NPC-блоку.`;
    touchState({
      banner: {
        kind: "warning",
        text: `${uniquenessText} Сними старую галку и создай отдельный блок в нужном месте цепочки.`
      }
    });
    renderHeader();
    renderEditor();
    return;
  }

  quest[fieldName] = isQuestRoleUnique(triggerRole)
    ? updateSingleTriggerRoleMembership(quest[fieldName], normalizedTriggerId, checked)
    : updateStringArrayMembership(quest[fieldName], normalizedTriggerId, checked);

  const hasAnyRoleOnTrigger = questUsesTriggerInAnyRole(quest, normalizedTriggerId);
  if (hasAnyRoleOnTrigger) {
    removeQuestFlowDraftTriggerId(quest, questIndex, normalizedTriggerId);
  }

  state.npcFlowSelectedTriggerId = normalizedTriggerId;
  state.npcFlowMode = "active";

  setQuestVisibleInTrigger(quest.id, normalizedTriggerId, checked || hasAnyRoleOnTrigger);
  syncQuestTriggerActions(quest);

  touchState({
    banner: {
      kind: "success",
      text: `Для квеста ${quest.id || "без id"} обновлена роль trigger ${normalizedTriggerId}.`
    }
  });
  renderAll();
}

function handleQuestFlowCardTriggerChange(questIndex, currentTriggerId, nextTriggerId) {
  const quest = state.data.quests[questIndex];
  if (!quest) {
    return;
  }

  const sourceTriggerId = String(currentTriggerId || "").trim();
  const targetTriggerId = String(nextTriggerId || "").trim();
  const hasDraftBlockOnSource = sourceTriggerId
    ? getQuestFlowDraftTriggerIds(quest, questIndex).some((block) => block.triggerId === sourceTriggerId)
    : false;

  if (!targetTriggerId) {
    if (sourceTriggerId && hasDraftBlockOnSource) {
      removeQuestFlowDraftTriggerId(quest, questIndex, sourceTriggerId);
    }

    state.npcFlowSelectedTriggerId = sourceTriggerId;
    state.npcFlowMode = "active";
    renderEditor();
    return;
  }

  if (!findTriggerById(targetTriggerId)) {
    return;
  }

  if (!sourceTriggerId) {
    addQuestFlowDraftTriggerId(quest, questIndex, targetTriggerId);
    state.npcFlowSelectedTriggerId = targetTriggerId;
    state.npcFlowMode = "active";
    renderEditor();
    return;
  }

  if (sourceTriggerId === targetTriggerId) {
    state.npcFlowSelectedTriggerId = targetTriggerId;
    state.npcFlowMode = "active";
    renderEditor();
    return;
  }

  if (hasDraftBlockOnSource) {
    replaceQuestFlowDraftTriggerId(quest, questIndex, sourceTriggerId, targetTriggerId);
    state.npcFlowSelectedTriggerId = targetTriggerId;
    state.npcFlowMode = "active";
    renderEditor();
    return;
  }

  if (!findTriggerById(sourceTriggerId)) {
    addQuestFlowDraftTriggerId(quest, questIndex, targetTriggerId);
    state.npcFlowSelectedTriggerId = targetTriggerId;
    state.npcFlowMode = "active";
    renderEditor();
    return;
  }

  if (questUsesTriggerInAnyRole(quest, sourceTriggerId)) {
    state.npcFlowSelectedTriggerId = sourceTriggerId;
    state.npcFlowMode = "active";
    updateBanner({
      kind: "warning",
      text: `У блока ${sourceTriggerId} уже выбраны роли. Чтобы добавить другого NPC для Completion или Reward, используй отдельный блок цепочки.`
    });
    renderEditor();
    renderHeader();
    return;
  }

  const rolesToMove = ["offer", "completion", "reward"].filter((role) =>
    getQuestRoleTriggerIds(quest, role).includes(sourceTriggerId)
  );

  if (!rolesToMove.length) {
    replaceQuestFlowDraftTriggerId(quest, questIndex, sourceTriggerId, targetTriggerId);
    state.npcFlowSelectedTriggerId = targetTriggerId;
    state.npcFlowMode = "active";
    renderEditor();
    return;
  }

  rolesToMove.forEach((role) => {
    const fieldName = getQuestRoleFieldName(role);
    quest[fieldName] = updateStringArrayMembership(quest[fieldName], sourceTriggerId, false);
    quest[fieldName] = updateStringArrayMembership(quest[fieldName], targetTriggerId, true);
  });

  moveQuestTriggerAction(quest, "offer", sourceTriggerId, targetTriggerId);
  moveQuestTriggerAction(quest, "completion", sourceTriggerId, targetTriggerId);
  moveQuestTriggerAction(quest, "reward", sourceTriggerId, targetTriggerId);

  setQuestVisibleInTrigger(quest.id, sourceTriggerId, questUsesTriggerInAnyRole(quest, sourceTriggerId));
  setQuestVisibleInTrigger(quest.id, targetTriggerId, true);
  removeQuestFlowDraftTriggerId(quest, questIndex, sourceTriggerId);
  removeQuestFlowDraftTriggerId(quest, questIndex, targetTriggerId);
  syncQuestTriggerActions(quest);

  state.npcFlowSelectedTriggerId = targetTriggerId;
  state.npcFlowMode = "active";

  touchState({
    banner: {
      kind: "success",
      text: `Блок NPC перенесён с ${sourceTriggerId} на ${targetTriggerId}.`
    }
  });
  renderAll();
}

function pickDefaultTriggerIdForNewQuest() {
  if (state.sidebarQuestTriggerId) {
    return state.sidebarQuestTriggerId;
  }

  if (state.selected?.kind === "trigger") {
    return state.data.triggers[state.selected.index]?.id || "";
  }

  if (state.selected?.kind === "quest") {
    const selectedQuest = state.data.quests[state.selected.index];
    const assignedTriggerId = selectedQuest ? getAssignedTriggerId(selectedQuest.id) : "";
    if (assignedTriggerId) {
      return assignedTriggerId;
    }
  }

  return state.data.triggers[0]?.id || "";
}

function findTriggerById(triggerId) {
  return state.data.triggers.find((trigger) => trigger.id === triggerId) || null;
}

function normalizeSidebarQuestTriggerId(triggerId = state.sidebarQuestTriggerId) {
  const normalizedTriggerId = String(triggerId || "").trim();
  if (!normalizedTriggerId) {
    return "";
  }

  return findTriggerById(normalizedTriggerId) ? normalizedTriggerId : "";
}

function normalizeNpcFlowSelectedTriggerId(triggerId = state.npcFlowSelectedTriggerId) {
  const normalizedTriggerId = String(triggerId || "").trim();
  if (normalizedTriggerId && findTriggerById(normalizedTriggerId)) {
    return normalizedTriggerId;
  }

  return "";
}

function normalizeNpcFlowMode(mode = state.npcFlowMode) {
  const normalizedMode = String(mode || "").trim();
  if (normalizedMode === "selected" || normalizedMode === "all") {
    return normalizedMode;
  }

  return "active";
}

function getQuestFlowDraftKey(quest, questIndex = -1) {
  const questId = String(quest?.id || "").trim();
  return questId || `index:${questIndex}`;
}

function normalizeNpcFlowDraftBlock(raw) {
  const block = raw && typeof raw === "object" ? raw : {};
  return {
    id: String(block.id || `draft_${Date.now()}_${Math.random().toString(36).slice(2, 8)}`).trim(),
    triggerId: String(block.triggerId || "").trim()
  };
}

function createNpcFlowDraftBlock(triggerId = "") {
  return normalizeNpcFlowDraftBlock({ triggerId });
}

function setQuestFlowDraftTriggerIds(quest, questIndex, triggerIds) {
  const key = getQuestFlowDraftKey(quest, questIndex);
  const seenTriggerIds = new Set();
  state.npcFlowDraftTriggerIdsByQuest[key] = normalizeArray(triggerIds)
    .map((entry) => typeof entry === "string" ? createNpcFlowDraftBlock(entry) : normalizeNpcFlowDraftBlock(entry))
    .filter((block) => block.triggerId && findTriggerById(block.triggerId))
    .filter((block) => {
      if (seenTriggerIds.has(block.triggerId)) {
        return false;
      }
      seenTriggerIds.add(block.triggerId);
      return true;
    });
}

function getQuestFlowDraftTriggerIds(quest, questIndex) {
  const key = getQuestFlowDraftKey(quest, questIndex);
  const draftTriggerIds = normalizeArray(state.npcFlowDraftTriggerIdsByQuest[key])
    .map((entry) => typeof entry === "string" ? createNpcFlowDraftBlock(entry) : normalizeNpcFlowDraftBlock(entry))
    .filter((block) => block.triggerId && findTriggerById(block.triggerId));

  setQuestFlowDraftTriggerIds(quest, questIndex, draftTriggerIds);
  return state.npcFlowDraftTriggerIdsByQuest[key];
}

function addQuestFlowDraftTriggerId(quest, questIndex, triggerId) {
  const normalizedTriggerId = String(triggerId || "").trim();
  if (!quest || !findTriggerById(normalizedTriggerId)) {
    return;
  }

  const currentDraftBlocks = getQuestFlowDraftTriggerIds(quest, questIndex);
  if (currentDraftBlocks.some((block) => block.triggerId === normalizedTriggerId)) {
    return;
  }

  setQuestFlowDraftTriggerIds(quest, questIndex, [
    ...currentDraftBlocks,
    createNpcFlowDraftBlock(normalizedTriggerId)
  ]);
}

function removeQuestFlowDraftTriggerId(quest, questIndex, triggerId) {
  const normalizedTriggerId = String(triggerId || "").trim();
  if (!quest || !normalizedTriggerId) {
    return;
  }

  setQuestFlowDraftTriggerIds(
    quest,
    questIndex,
    getQuestFlowDraftTriggerIds(quest, questIndex).filter((block) => block.triggerId !== normalizedTriggerId)
  );
}

function replaceQuestFlowDraftTriggerId(quest, questIndex, sourceTriggerId, targetTriggerId) {
  const fromId = String(sourceTriggerId || "").trim();
  const toId = String(targetTriggerId || "").trim();
  if (!quest || !fromId || !findTriggerById(toId)) {
    return;
  }

  const currentDraftIds = getQuestFlowDraftTriggerIds(quest, questIndex).filter((block) => block.triggerId !== fromId);
  currentDraftIds.push(createNpcFlowDraftBlock(toId));

  setQuestFlowDraftTriggerIds(quest, questIndex, currentDraftIds);
}

function getAssignedTriggerIds(questId) {
  const normalizedQuestId = String(questId || "").trim();
  if (!normalizedQuestId) {
    return [];
  }

  return state.data.triggers
    .filter((trigger) => normalizeArray(trigger.questIds).includes(normalizedQuestId))
    .map((trigger) => trigger.id);
}

function getAssignedTriggerId(questId) {
  return getAssignedTriggerIds(questId)[0] || "";
}

function updateStringArrayMembership(list, value, checked) {
  const normalizedValue = String(value || "").trim();
  const next = Array.from(new Set(normalizeArray(list).map((item) => String(item || "").trim()).filter(Boolean)));

  if (!normalizedValue) {
    return next;
  }

  if (checked) {
    if (!next.includes(normalizedValue)) {
      next.push(normalizedValue);
    }
    return next;
  }

  return next.filter((item) => item !== normalizedValue);
}

function normalizeSingleTriggerRoleIds(list) {
  const normalized = normalizeQuestIdArray(list);
  return normalized.length ? [normalized[0]] : [];
}

function updateSingleTriggerRoleMembership(list, value, checked) {
  const normalizedValue = String(value || "").trim();
  if (!normalizedValue) {
    return normalizeSingleTriggerRoleIds(list);
  }

  if (checked) {
    return [normalizedValue];
  }

  return normalizeSingleTriggerRoleIds(list).filter((item) => item !== normalizedValue);
}

function getQuestRoleFieldName(role) {
  if (role === "offer") {
    return "offerTriggerIds";
  }

  if (role === "completion") {
    return "completionTriggerIds";
  }

  if (role === "reward") {
    return "rewardTriggerIds";
  }

  return "";
}

function getQuestRoleTriggerIds(quest, role) {
  if (!quest) {
    return [];
  }

  if (role === "offer") {
    return normalizeSingleTriggerRoleIds(quest.offerTriggerIds);
  }

  if (role === "completion") {
    return normalizeQuestIdArray(quest.completionTriggerIds);
  }

  if (role === "reward") {
    return normalizeSingleTriggerRoleIds(quest.rewardTriggerIds);
  }

  return [];
}

function questUsesTriggerInAnyRole(quest, triggerId) {
  if (!quest || !triggerId) {
    return false;
  }

  return ["offer", "completion", "reward"].some((role) => getQuestRoleTriggerIds(quest, role).includes(triggerId));
}

function createQuestTriggerAction(actionType = "", triggerId = "") {
  return {
    triggerId: String(triggerId || "").trim(),
    actionType: String(actionType || "").trim(),
    dialogText: "",
    rewards: []
  };
}

function normalizeQuestTriggerAction(raw) {
  const action = raw && typeof raw === "object" ? raw : {};
  return {
    triggerId: toText(action.triggerId).trim(),
    actionType: toText(action.actionType).trim(),
    dialogText: toText(action.dialogText),
    rewards: normalizeArray(action.rewards).map(normalizeRewardItem)
  };
}

function syncQuestTriggerActions(quest) {
  if (!quest) {
    return;
  }

  const existingActions = normalizeArray(quest.triggerActions).map(normalizeQuestTriggerAction);
  const nextActions = [];

  ["offer", "completion", "reward"].forEach((actionType) => {
    getQuestRoleTriggerIds(quest, actionType).forEach((triggerId) => {
      const existingAction = existingActions.find((entry) => entry.actionType === actionType && entry.triggerId === triggerId);
      const nextAction = existingAction || createQuestTriggerAction(actionType, triggerId);
      nextActions.push(nextAction);
    });
  });

  quest.triggerActions = nextActions;
}

function moveQuestTriggerAction(quest, actionType, sourceTriggerId, targetTriggerId) {
  const normalizedActionType = String(actionType || "").trim();
  const fromId = String(sourceTriggerId || "").trim();
  const toId = String(targetTriggerId || "").trim();
  if (!quest || !normalizedActionType || !fromId || !toId || fromId === toId) {
    return;
  }

  const actions = normalizeArray(quest.triggerActions).map(normalizeQuestTriggerAction);
  const sourceAction = actions.find((action) => action.actionType === normalizedActionType && action.triggerId === fromId);
  if (!sourceAction) {
    return;
  }

  const targetAction = actions.find((action) => action.actionType === normalizedActionType && action.triggerId === toId);
  if (targetAction) {
    if (!toText(targetAction.dialogText).trim()) {
      targetAction.dialogText = sourceAction.dialogText;
    }

    if (!normalizeArray(targetAction.rewards).length) {
      targetAction.rewards = normalizeArray(sourceAction.rewards).map(normalizeRewardItem);
    }
  } else {
    actions.push({
      triggerId: toId,
      actionType: normalizedActionType,
      dialogText: sourceAction.dialogText,
      rewards: normalizeArray(sourceAction.rewards).map(normalizeRewardItem)
    });
  }

  quest.triggerActions = actions.filter((action) =>
    !(action.actionType === normalizedActionType && action.triggerId === fromId)
  );
}

function syncAllQuestTriggerActions(data = state.data) {
  normalizeArray(data?.quests).forEach((quest) => {
    syncQuestTriggerActions(quest);
  });
}

function removeTriggerIdFromQuestRoles(triggerId) {
  const normalizedTriggerId = String(triggerId || "").trim();
  if (!normalizedTriggerId) {
    return;
  }

  state.data.quests.forEach((quest) => {
    quest.offerTriggerIds = updateSingleTriggerRoleMembership(quest.offerTriggerIds, normalizedTriggerId, false);
    quest.completionTriggerIds = updateStringArrayMembership(quest.completionTriggerIds, normalizedTriggerId, false);
    quest.rewardTriggerIds = updateSingleTriggerRoleMembership(quest.rewardTriggerIds, normalizedTriggerId, false);
    syncQuestTriggerActions(quest);
  });
}

function setQuestVisibleInTrigger(questId, triggerId, visible) {
  const normalizedQuestId = String(questId || "").trim();
  const normalizedTriggerId = String(triggerId || "").trim();
  if (!normalizedQuestId || !normalizedTriggerId) {
    return;
  }

  const trigger = findTriggerById(normalizedTriggerId);
  if (!trigger) {
    return;
  }

  const nextQuestIds = Array.from(new Set(normalizeArray(trigger.questIds).map((itemId) => String(itemId || "").trim()).filter(Boolean)));
  const hasQuest = nextQuestIds.includes(normalizedQuestId);

  if (visible && !hasQuest) {
    nextQuestIds.push(normalizedQuestId);
  }

  trigger.questIds = visible
    ? nextQuestIds
    : nextQuestIds.filter((itemId) => itemId !== normalizedQuestId);

  if (!visible) {
    const quest = state.data.quests.find((entry) => entry.id === normalizedQuestId);
    if (quest) {
      quest.offerTriggerIds = updateSingleTriggerRoleMembership(quest.offerTriggerIds, normalizedTriggerId, false);
      quest.completionTriggerIds = updateStringArrayMembership(quest.completionTriggerIds, normalizedTriggerId, false);
      quest.rewardTriggerIds = updateSingleTriggerRoleMembership(quest.rewardTriggerIds, normalizedTriggerId, false);
      syncQuestTriggerActions(quest);
    }
  }
}

function removeQuestFromAllTriggers(questId) {
  const normalizedQuestId = String(questId || "").trim();
  if (!normalizedQuestId) {
    return;
  }

  state.data.triggers.forEach((trigger) => {
    trigger.questIds = normalizeArray(trigger.questIds).filter((itemId) => String(itemId || "").trim() !== normalizedQuestId);
  });
}

function assignQuestToTrigger(questId, triggerId) {
  const normalizedQuestId = String(questId || "").trim();
  const normalizedTriggerId = String(triggerId || "").trim();

  if (!normalizedQuestId) {
    return;
  }

  removeQuestFromAllTriggers(normalizedQuestId);

  if (!normalizedTriggerId) {
    return;
  }

  setQuestVisibleInTrigger(normalizedQuestId, normalizedTriggerId, true);
}

function syncQuestIdReferences(previousQuestId, nextQuestId) {
  const oldId = String(previousQuestId || "").trim();
  const newId = String(nextQuestId || "").trim();

  if (!oldId || oldId === newId) {
    return;
  }

  state.data.triggers.forEach((trigger) => {
    const replacedIds = normalizeArray(trigger.questIds)
      .map((questId) => (questId === oldId ? newId : questId))
      .filter(Boolean);

    trigger.questIds = Array.from(new Set(replacedIds));
  });

  state.data.quests.forEach((quest) => {
    if (quest.requiresPrevious === oldId) {
      quest.requiresPrevious = newId;
    }

    quest.requiredQuestIds = Array.from(
      new Set(
        normalizeArray(quest.requiredQuestIds)
          .map((questId) => (questId === oldId ? newId : questId))
          .filter(Boolean)
      )
    );
  });
}

function syncTriggerIdReferences(previousTriggerId, nextTriggerId) {
  const oldId = String(previousTriggerId || "").trim();
  const newId = String(nextTriggerId || "").trim();

  if (!oldId || oldId === newId) {
    return;
  }

  state.data.quests.forEach((quest) => {
    quest.offerTriggerIds = normalizeSingleTriggerRoleIds(
      normalizeArray(quest.offerTriggerIds)
        .map((triggerId) => (triggerId === oldId ? newId : triggerId))
        .filter(Boolean)
    );

    quest.completionTriggerIds = Array.from(new Set(
      normalizeArray(quest.completionTriggerIds)
        .map((triggerId) => (triggerId === oldId ? newId : triggerId))
        .filter(Boolean)
    ));

    quest.rewardTriggerIds = normalizeSingleTriggerRoleIds(
      normalizeArray(quest.rewardTriggerIds)
        .map((triggerId) => (triggerId === oldId ? newId : triggerId))
        .filter(Boolean)
    );

    quest.triggerActions = normalizeArray(quest.triggerActions).map((action) => ({
      ...normalizeQuestTriggerAction(action),
      triggerId: action?.triggerId === oldId ? newId : toText(action?.triggerId).trim()
    }));
    syncQuestTriggerActions(quest);
  });

  if (state.sidebarQuestTriggerId === oldId) {
    state.sidebarQuestTriggerId = newId;
  }
  if (state.npcFlowSelectedTriggerId === oldId) {
    state.npcFlowSelectedTriggerId = newId;
  }
}

function addArrayItem(path, itemType) {
  const list = getValueByPath(state.data, path);
  if (!Array.isArray(list)) {
    return;
  }

  let nextItem = "";
  if (itemType === "reward-item") {
    nextItem = createRewardItem();
  } else if (itemType === "objective-item") {
    nextItem = createObjective();
  }

  list.push(nextItem);
  touchState();
  renderAll();
}

function removeArrayItem(path, index) {
  const list = getValueByPath(state.data, path);
  if (!Array.isArray(list)) {
    return;
  }

  list.splice(index, 1);
  touchState();
  renderAll();
}

function removeSelectedEntity(kind, index) {
  const collection = kind === "quest" ? state.data.quests : state.data.triggers;
  const item = collection[index];
  if (!item) {
    return;
  }

  const label = kind === "quest" ? item.name || item.id || "квест" : item.id || "триггер";
  const affectedQuestCount = kind === "trigger" ? normalizeArray(item.questIds).filter(Boolean).length : 0;
  const confirmText = kind === "trigger" && affectedQuestCount
    ? `Удалить триггер "${label}"? ${affectedQuestCount} квест(а/ов) останутся без привязки к NPC.`
    : `Удалить ${kind === "quest" ? "квест" : "триггер"} "${label}"?`;
  const confirmed = window.confirm(confirmText);
  if (!confirmed) {
    return;
  }

  if (kind === "quest") {
    removeQuestFromAllTriggers(item.id);
    item.offerTriggerIds = [];
    item.completionTriggerIds = [];
    item.rewardTriggerIds = [];
  } else {
    removeTriggerIdFromQuestRoles(item.id);
    if (state.sidebarQuestTriggerId === item.id) {
      state.sidebarQuestTriggerId = "";
    }
    if (state.npcFlowSelectedTriggerId === item.id) {
      state.npcFlowSelectedTriggerId = "";
    }
  }

  collection.splice(index, 1);
  fixSelectionAfterEntityChange(kind, index);
  touchState({
    banner: {
      kind: "warning",
      text: `${kind === "quest" ? "Квест" : "Триггер"} удален.`
    }
  });
  renderAll();
}

function duplicateEntity(kind, index) {
  if (kind === "quest") {
    const source = state.data.quests[index];
    if (!source) {
      return;
    }

    const sourceTriggerIds = getAssignedTriggerIds(source.id);
    const copy = structuredClone(source);
    copy.id = makeUniqueId(state.data.quests.map((quest) => quest.id), `${source.id || "quest"}_copy`);
    copy.name = source.name ? `${source.name} (копия)` : "Новый квест (копия)";
    state.data.quests.splice(index + 1, 0, copy);
    sourceTriggerIds.forEach((triggerId) => setQuestVisibleInTrigger(copy.id, triggerId, true));
    state.selected = { kind: "quest", index: index + 1 };
  } else {
    const source = state.data.triggers[index];
    if (!source) {
      return;
    }

    const copy = structuredClone(source);
    copy.id = makeUniqueId(state.data.triggers.map((trigger) => trigger.id), `${source.id || "trigger"}_copy`);
    copy.questIds = [];
    state.data.triggers.splice(index + 1, 0, copy);
    state.selected = { kind: "trigger", index: index + 1 };
  }

  touchState({
    banner: {
      kind: "success",
      text: kind === "trigger"
        ? "Создана копия триггера без привязанных квестов."
        : "Сделана копия выбранного квеста."
    }
  });
  renderAll();
}

function fixSelectionAfterEntityChange(kind, removedIndex) {
  const collection = kind === "quest" ? state.data.quests : state.data.triggers;
  if (!collection.length) {
    const fallbackKind = kind === "quest" ? "trigger" : "quest";
    const fallbackCollection = fallbackKind === "quest" ? state.data.quests : state.data.triggers;

    state.selected = fallbackCollection.length
      ? { kind: fallbackKind, index: 0 }
      : null;
    return;
  }

  const nextIndex = Math.min(removedIndex, collection.length - 1);
  state.selected = { kind, index: nextIndex };
}

function loadData(rawData, fileName, options = {}) {
  state.data = normalizeData(rawData);
  state.fileName = fileName || "quests.json";
  state.dirty = Boolean(options.dirty);
  state.sidebarQuestTriggerId = normalizeSidebarQuestTriggerId(state.sidebarQuestTriggerId);
  state.npcFlowSelectedTriggerId = normalizeNpcFlowSelectedTriggerId(state.npcFlowSelectedTriggerId);
  state.issues = validateData(state.data);
  state.banner = options.banner || null;
  state.selected = pickInitialSelection(state.data, state.selected);
  if (state.dirty) {
    saveDraftNow();
  }
  renderAll();
}

function pickInitialSelection(data, currentSelection) {
  if (currentSelection) {
    if (currentSelection.kind === "quest" && data.quests[currentSelection.index]) {
      return currentSelection;
    }
    if (currentSelection.kind === "trigger" && data.triggers[currentSelection.index]) {
      return currentSelection;
    }
  }

  if (data.quests.length) {
    return { kind: "quest", index: 0 };
  }

  if (data.triggers.length) {
    return { kind: "trigger", index: 0 };
  }

  return null;
}

function touchState(options = {}) {
  state.dirty = true;
  syncAllQuestTriggerActions();
  state.issues = validateData(state.data);

  if (options.banner) {
    state.banner = options.banner;
  }

  scheduleDraftSave();
}

function touchStackRules() {
  state.stackRulesDirty = true;
  renderStackRulesStatus("warn", "Есть изменения");
  scheduleStackRulesSave();
}

function renderAll() {
  syncAllQuestTriggerActions();
  state.npcFlowMode = normalizeNpcFlowMode(state.npcFlowMode);
  state.npcFlowSelectedTriggerId = normalizeNpcFlowSelectedTriggerId(state.npcFlowSelectedTriggerId);
  renderHeader();
  renderBanner();
  renderSidebar();
  renderStackRules();
  renderEditor();
  renderConfig();
  renderValidation();
  renderPreview();
}

function renderHeader() {
  const headerFileName = window.location.protocol.startsWith("http") && state.config.savePath
    ? getBaseNameFromPath(state.config.savePath)
    : state.fileName || "quests.json";
  elements.fileNameLabel.textContent = headerFileName;
  elements.dirtyLabel.textContent = state.dirty ? "Есть изменения" : "Сохранено";
  elements.issueCountLabel.textContent = String(state.issues.filter((issue) => issue.level === "error").length);
}

function renderBanner() {
  if (!state.banner) {
    elements.banner.className = "banner hidden";
    elements.banner.textContent = "";
    return;
  }

  elements.banner.className = `banner ${state.banner.kind}`;
  elements.banner.textContent = state.banner.text;
}

function renderConfig(status = "neutral", text = "") {
  elements.savePathInput.value = state.config.savePath || "";
  elements.backupPathInput.value = state.config.backupPath || "";

  const labelText = text || (state.config.savePath && state.config.backupPath ? "Пути заданы" : "Пути не заданы");
  elements.configStatusLabel.textContent = labelText;
  elements.configStatusLabel.className =
    status === "good" ? "badge good" :
    status === "warning" ? "badge warn" :
    status === "bad" ? "badge bad" :
    "badge neutral";
}

function renderSidebar() {
  const query = state.filter;
  state.sidebarQuestTriggerId = normalizeSidebarQuestTriggerId(state.sidebarQuestTriggerId);
  const activeTriggerId = state.sidebarQuestTriggerId;
  const quests = getSidebarQuestEntries(query, activeTriggerId);
  const triggers = state.data.triggers.filter((trigger) => matchesFilter(query, [trigger.id, trigger.hintText, trigger.npcClassName]));

  renderQuestTriggerFilter(activeTriggerId);

  elements.questList.innerHTML = quests.length
    ? quests
        .map((entry) => {
          const { quest, index, roleText } = entry;
          return `
            <button
              type="button"
              class="entity-item ${state.selected && state.selected.kind === "quest" && state.selected.index === index ? "active" : ""}"
              data-select-kind="quest"
              data-select-index="${index}">
              <span class="entity-title">${escapeHtml(quest.name || "Без названия")}</span>
              <span class="entity-subtitle">${escapeHtml(quest.id || "id не задан")}</span>
              ${roleText ? `<span class="entity-meta">${escapeHtml(roleText)}</span>` : ""}
            </button>
          `;
        })
        .join("")
    : `<div class="entity-item"><span class="entity-title">Ничего не найдено</span><span class="entity-subtitle">${escapeHtml(getQuestListEmptyMessage(activeTriggerId))}</span></div>`;

  elements.triggerList.innerHTML = triggers.length
    ? triggers
        .map((trigger) => {
          const index = state.data.triggers.indexOf(trigger);
          return `
            <button
              type="button"
              class="entity-item ${state.selected && state.selected.kind === "trigger" && state.selected.index === index ? "active" : ""}"
              data-select-kind="trigger"
              data-select-index="${index}">
              <span class="entity-title">${escapeHtml(trigger.id || "Без id")}</span>
              <span class="entity-subtitle">${escapeHtml(trigger.hintText || "Подсказка не задана")}</span>
            </button>
          `;
        })
        .join("")
    : `<div class="entity-item"><span class="entity-title">Ничего не найдено</span><span class="entity-subtitle">Добавь новый триггер или смени фильтр.</span></div>`;
}

function renderQuestTriggerFilter(activeTriggerId) {
  const options = [
    `<option value="">Все квесты</option>`,
    ...state.data.triggers.map((trigger) => {
      const selected = trigger.id === activeTriggerId ? "selected" : "";
      const label = trigger.hintText
        ? `${trigger.id} - ${trigger.hintText}`
        : trigger.id;

      return `<option value="${escapeAttribute(trigger.id)}" ${selected}>${escapeHtml(label)}</option>`;
    })
  ];

  elements.questTriggerFilter.innerHTML = options.join("");
  elements.questTriggerFilterHint.textContent = activeTriggerId
    ? `Показываются квесты, которые берутся у trigger ${activeTriggerId}, в порядке questIds.`
    : "Можно выбрать конкретный trigger, чтобы видеть только квесты, которые берутся у него, в его порядке.";
}

function getSidebarQuestEntries(query, activeTriggerId) {
  if (!activeTriggerId) {
    return state.data.quests
      .map((quest, index) => ({ quest, index }))
      .filter(({ quest }) => matchesFilter(query, [quest.id, quest.name, quest.description]))
      .map(({ quest, index }) => ({
        quest,
        index,
        roleText: buildQuestTriggerCountText(quest.id)
      }));
  }

  const trigger = findTriggerById(activeTriggerId);
  if (!trigger) {
    return [];
  }

  return normalizeArray(trigger.questIds)
    .map((questId) => String(questId || "").trim())
    .filter(Boolean)
    .map((questId) => {
      const index = state.data.quests.findIndex((quest) => quest.id === questId);
      if (index === -1) {
        return null;
      }

      const quest = state.data.quests[index];
      if (!isQuestInTriggerRole(quest, activeTriggerId, "offer")) {
        return null;
      }

      if (!matchesFilter(query, [quest.id, quest.name, quest.description])) {
        return null;
      }

      return {
        quest,
        index,
        roleText: buildQuestRoleTextForTrigger(quest, activeTriggerId)
      };
    })
    .filter(Boolean);
}

function buildQuestTriggerCountText(questId) {
  const triggerCount = getAssignedTriggerIds(questId).length;
  if (!triggerCount) {
    return "Без trigger";
  }

  if (triggerCount === 1) {
    return "1 trigger";
  }

  return `${triggerCount} trigger`;
}

function buildQuestRoleTextForTriggerListLegacy(quest, triggerId) {
  if (!quest || !triggerId) {
    return "";
  }

  const roleLabels = [];
  if (isQuestInTriggerRole(quest, triggerId, "offer")) {
    roleLabels.push("взять");
  }
  if (isQuestInTriggerRole(quest, triggerId, "completion")) {
    roleLabels.push("сдать");
  }
  if (isQuestInTriggerRole(quest, triggerId, "reward")) {
    roleLabels.push("награда");
  }

  if (!roleLabels.length) {
    return "Только видимость";
  }

  return `Роли: ${roleLabels.join(" · ")}`;
}

function isQuestInTriggerRole(quest, triggerId, role) {
  if (!quest || !triggerId) {
    return false;
  }

  const visibleTriggerIds = getAssignedTriggerIds(quest.id);
  if (!visibleTriggerIds.includes(triggerId)) {
    return false;
  }

  if (role === "offer") {
    const offerTriggerIds = normalizeQuestIdArray(quest.offerTriggerIds);
    return offerTriggerIds.includes(triggerId);
  }

  if (role === "completion") {
    const completionTriggerIds = normalizeQuestIdArray(quest.completionTriggerIds);
    return completionTriggerIds.includes(triggerId);
  }

  if (role === "reward") {
    const rewardTriggerIds = normalizeQuestIdArray(quest.rewardTriggerIds);
    return rewardTriggerIds.includes(triggerId);
  }

  return false;
}

function getQuestListEmptyMessage(activeTriggerId) {
  if (activeTriggerId) {
    const trigger = findTriggerById(activeTriggerId);
    if (trigger && normalizeArray(trigger.questIds).length === 0) {
      return `У trigger ${activeTriggerId} пока нет квестов.`;
    }

    if (trigger) {
      const hasOfferQuest = normalizeArray(trigger.questIds)
        .map((questId) => String(questId || "").trim())
        .filter(Boolean)
        .some((questId) => {
          const quest = state.data.quests.find((entry) => entry.id === questId);
          return quest ? isQuestInTriggerRole(quest, activeTriggerId, "offer") : false;
        });

      if (!hasOfferQuest) {
        return `У trigger ${activeTriggerId} пока нет квестов, которые берутся у него.`;
      }
    }

    return `По текущему фильтру для trigger ${activeTriggerId} ничего не найдено.`;
  }

  return "Добавь новый квест или смени фильтр.";
}

function renderStackRules() {
  const query = state.filter;
  const rules = state.stackRules.filter((rule) => matchesFilter(query, [rule.className, rule.stackSize]));

  elements.stackRuleList.innerHTML = rules.length
    ? rules
        .map((rule) => {
          const index = state.stackRules.indexOf(rule);
          return `
            <div class="stack-rule-row">
              <input
                type="text"
                value="${escapeAttribute(rule.className)}"
                placeholder="ClassName"
                data-stack-rule-index="${index}"
                data-stack-rule-field="className">
              <input
                type="number"
                class="stack-rule-size"
                min="1"
                step="1"
                value="${escapeAttribute(rule.stackSize)}"
                data-stack-rule-index="${index}"
                data-stack-rule-field="stackSize">
              <button
                type="button"
                class="mini-button danger stack-rule-remove"
                data-stack-rule-action="remove"
                data-stack-rule-index="${index}">
                ×
              </button>
            </div>
          `;
        })
        .join("")
    : `<div class="stack-rule-empty">${state.stackRules.length ? "Ничего не найдено по текущему фильтру." : "Справочник пока пуст. Добавь первое правило для нужного класса."}</div>`;
}

function renderStackRulesStatus(kind = state.stackRulesStatus.kind, text = state.stackRulesStatus.text) {
  state.stackRulesStatus = { kind, text };
  elements.stackRulesStatusLabel.textContent = text;
  elements.stackRulesStatusLabel.className =
    kind === "good" ? "badge good" :
    kind === "warn" || kind === "warning" ? "badge warn" :
    kind === "bad" ? "badge bad" :
    "badge neutral";
}

function renderEditor() {
  if (!state.selected) {
    elements.editorPane.innerHTML = renderWelcomeState();
    return;
  }

  if (state.selected.kind === "quest") {
    const quest = state.data.quests[state.selected.index];
    if (!quest) {
      elements.editorPane.innerHTML = renderWelcomeState();
      return;
    }

    elements.editorPane.innerHTML = renderQuestEditor(quest, state.selected.index);
    return;
  }

  const trigger = state.data.triggers[state.selected.index];
  if (!trigger) {
    elements.editorPane.innerHTML = renderWelcomeState();
    return;
  }

  elements.editorPane.innerHTML = renderTriggerEditor(trigger, state.selected.index);
}

function renderValidation() {
  const issues = state.issues;
  const errorCount = issues.filter((issue) => issue.level === "error").length;
  const warningCount = issues.filter((issue) => issue.level === "warning").length;

  if (!issues.length) {
    elements.validationSummary.className = "badge good";
    elements.validationSummary.textContent = "Структура выглядит чисто";
    elements.validationList.innerHTML = `
      <div class="validation-item success">
        <strong>Все хорошо</strong>
        <span>Сейчас явных проблем по структуре не видно. Можно экспортировать файл.</span>
      </div>
    `;
    return;
  }

  elements.validationSummary.className = errorCount ? "badge bad" : "badge warn";
  elements.validationSummary.textContent = errorCount
    ? `Ошибок: ${errorCount}, предупреждений: ${warningCount}`
    : `Предупреждений: ${warningCount}`;

  elements.validationList.innerHTML = issues
    .map(
      (issue) => `
        <div class="validation-item ${issue.level}">
          <strong>${escapeHtml(issue.title)}</strong>
          <span>${escapeHtml(issue.message)}</span>
        </div>
      `
    )
    .join("");
}

function renderPreview() {
  elements.jsonPreview.textContent = JSON.stringify(state.data, null, 2);
}

function renderWelcomeState() {
  return `
    <section class="welcome-card">
      <p class="eyebrow">Начало работы</p>
      <h2>Здесь будет удобный редактор твоего JSON</h2>
      <p>
        Импортируй существующий файл или начинай с чистого листа. В первой версии уже можно
        разделять квесты и триггеры по блокам, править вложенные списки, проверять ошибки и
        экспортировать результат обратно в JSON.
      </p>
      <p>
        Базовым ориентиром сейчас считается файл <strong>${BASE_FILE_NAME}</strong>, который лежит
        рядом с редактором.
      </p>
      <div class="welcome-actions">
        <button type="button" class="button ghost" data-action="load-base-file">Загрузить ${BASE_FILE_NAME}</button>
        <button type="button" class="button" data-action="create-quest">Создать первый квест</button>
        <button type="button" class="button ghost" data-action="create-trigger">Создать первый триггер</button>
      </div>
    </section>
  `;
}

function renderTriggerEditor(trigger, index) {
  const base = `triggers.${index}`;

  return `
    <div class="editor-grid">
      <section class="editor-card">
        <div class="panel-head">
          <div>
            <p class="eyebrow">Trigger (триггер)</p>
            <h2>${escapeHtml(trigger.id || "Без id")}</h2>
            <p class="muted">${escapeHtml(trigger.hintText || "Подсказка не задана")}</p>
          </div>
          <div class="toolbar-actions">
            <button type="button" class="button ghost small" data-action="duplicate-entity" data-entity-kind="trigger" data-index="${index}">Дублировать</button>
            <button type="button" class="button danger small" data-action="remove-entity" data-entity-kind="trigger" data-index="${index}">Удалить</button>
          </div>
        </div>
      </section>

      <div class="section-grid">
        ${sectionCard(
          "Main (основное)",
          `
            <div class="field-grid single">
              ${textField("ID (идентификатор триггера)", `${base}.id`, trigger.id, "Уникальный служебный код триггера.", "id")}
              ${textField("Hint Text (подсказка для игрока)", `${base}.hintText`, trigger.hintText, "Текст, который игрок увидит возле NPC или точки взаимодействия.", "hintText")}
              ${numberField("Radius (радиус срабатывания)", `${base}.radius`, trigger.radius, "Насколько близко нужно подойти для активации.", 0.1, "radius")}
              ${numberField("Focus Height (высота фокуса)", `${base}.focusHeight`, trigger.focusHeight, "Высота области, в которой ловится фокус на объекте.", 0.1, "focusHeight")}
              ${numberField("Focus Radius (радиус фокуса)", `${base}.focusRadius`, trigger.focusRadius, "Ширина зоны наведения на триггер.", 0.1, "focusRadius")}
              ${toggleField("Spawn NPC (спавнить NPC)", `${base}.spawnNpc`, trigger.spawnNpc, "Если включено, вместе с триггером будет создан NPC.", "spawnNpc")}
            </div>
          `
        )}

        ${sectionCard(
          "Quest IDs (связанные квесты)",
          `
            <div class="stack">
              ${stringArrayEditor(`${base}.questIds`, trigger.questIds, "quest id (id квеста)")}
            </div>
          `
        )}
      </div>

      ${sectionCard("Trigger Position (позиция триггера)", vectorField(`${base}.position`, trigger.position))}

      <div class="section-grid">
        ${sectionCard(
          "NPC (персонаж)",
          `
            <div class="field-grid single">
              ${textField("NPC Class Name (класс NPC)", `${base}.npcClassName`, trigger.npcClassName, "Внутреннее имя типа персонажа, который должен появиться.", "npcClassName")}
              ${textField("Hands Item (предмет в руках NPC)", `${base}.npcHandsItem`, trigger.npcHandsItem, "Что NPC будет держать в руках после спавна.", "npcHandsItem")}
            </div>
          `
        )}

        ${sectionCard(
          "NPC Position & Orientation (позиция и направление NPC)",
          `
            <div class="stack">
              <div>
                <p class="eyebrow">NPC Position (позиция NPC)</p>
                ${vectorField(`${base}.npcPosition`, trigger.npcPosition)}
              </div>
              <div>
                <p class="eyebrow">NPC Orientation (ориентация NPC)</p>
                ${vectorField(`${base}.npcOrientation`, trigger.npcOrientation)}
              </div>
            </div>
          `
        )}
      </div>

      ${sectionCard(
        "NPC Loadout (одежда и экипировка NPC)",
        `
          <div class="stack">
            ${stringArrayEditor(`${base}.npcLoadout`, trigger.npcLoadout, "loadout item (элемент экипировки)")}
          </div>
        `
      )}

      ${sectionCard(
        "NPC Back Items (предметы за спиной NPC)",
        `
          <div class="stack">
            ${stringArrayEditor(`${base}.npcBackItems`, trigger.npcBackItems, "back item (предмет за спиной)")}
          </div>
        `
      )}
    </div>
  `;
}

function sectionCard(title, content) {
  return `
    <section class="editor-card tight">
      <div class="card-head">
        <div>
          <p class="eyebrow">Block (блок)</p>
          <h2>${escapeHtml(title)}</h2>
        </div>
      </div>
      ${content}
    </section>
  `;
}

function textField(label, path, value, hint, jsonKey = "") {
  return `
    <div class="field">
      <div class="field-label-row">
        <label>${escapeHtml(label)}</label>
        ${jsonKey ? `<span class="field-key">JSON: ${escapeHtml(jsonKey)}</span>` : ""}
      </div>
      <input type="text" value="${escapeAttribute(value)}" data-path="${escapeAttribute(path)}" data-type="text">
      ${hint ? `<small>${escapeHtml(hint)}</small>` : ""}
    </div>
  `;
}

function textareaField(label, path, value, hint, jsonKey = "") {
  return `
    <div class="field">
      <div class="field-label-row">
        <label>${escapeHtml(label)}</label>
        ${jsonKey ? `<span class="field-key">JSON: ${escapeHtml(jsonKey)}</span>` : ""}
      </div>
      <textarea data-path="${escapeAttribute(path)}" data-type="text">${escapeHtml(value)}</textarea>
      ${hint ? `<small>${escapeHtml(hint)}</small>` : ""}
    </div>
  `;
}

function numberField(label, path, value, hint, step, jsonKey = "") {
  return `
    <div class="field">
      <div class="field-label-row">
        <label>${escapeHtml(label)}</label>
        ${jsonKey ? `<span class="field-key">JSON: ${escapeHtml(jsonKey)}</span>` : ""}
      </div>
      <input
        type="number"
        value="${escapeAttribute(value)}"
        data-path="${escapeAttribute(path)}"
        data-type="number"
        step="${escapeAttribute(step || 1)}">
      ${hint ? `<small>${escapeHtml(hint)}</small>` : ""}
    </div>
  `;
}

function toggleField(label, path, value, hint, jsonKey = "") {
  return `
    <div class="toggle-field">
      <div>
        <div class="field-label-row">
          <label>${escapeHtml(label)}</label>
          ${jsonKey ? `<span class="field-key">JSON: ${escapeHtml(jsonKey)}</span>` : ""}
        </div>
        ${hint ? `<div class="muted">${escapeHtml(hint)}</div>` : ""}
      </div>
      <input type="checkbox" ${value ? "checked" : ""} data-path="${escapeAttribute(path)}" data-type="flag">
    </div>
  `;
}

function questTriggerVisibilityField(questIndex, questId) {
  const assignedTriggerIds = getAssignedTriggerIds(questId);
  const triggers = state.data.triggers;

  if (!triggers.length) {
    return `
      <div class="field">
        <div class="field-label-row">
          <label>Visible Trigger IDs (где квест показывается)</label>
          <span class="field-key">JSON: triggers[].questIds</span>
        </div>
        <div class="empty-note">Сначала создай trigger / NPC. Без видимого trigger квест нельзя оформить правильно.</div>
      </div>
    `;
  }

  return `
    <div class="field">
      <div class="field-label-row">
        <label>Visible Trigger IDs (где квест показывается)</label>
        <span class="field-key">JSON: triggers[].questIds</span>
      </div>
      <div class="stack">
        ${triggers
          .map((trigger) => {
            const checked = assignedTriggerIds.includes(trigger.id) ? "checked" : "";
            const optionLabel = trigger.hintText
              ? `${trigger.id} - ${trigger.hintText}`
              : trigger.id;

            return `
              <label class="toggle-field">
                <div>
                  <strong>${escapeHtml(optionLabel)}</strong>
                  <div class="muted">Если включено, квест будет показываться в меню этого trigger / NPC.</div>
                </div>
                <input
                  type="checkbox"
                  ${checked}
                  data-role="quest-trigger-toggle"
                  data-trigger-role="visible"
                  data-quest-index="${questIndex}"
                  data-trigger-id="${escapeAttribute(trigger.id)}">
              </label>
            `;
          })
          .join("")}
      </div>
      <small>Можно выбрать несколько trigger / NPC. Это только видимость квеста, а не его роль в цепочке.</small>
    </div>
  `;
}

function vectorField(basePath, values) {
  const vector = normalizeVector(values);
  return `
    <div class="field-grid triple">
      ${numberField("X", `${basePath}.0`, vector[0], "", 0.01)}
      ${numberField("Y", `${basePath}.1`, vector[1], "", 0.01)}
      ${numberField("Z", `${basePath}.2`, vector[2], "", 0.01)}
    </div>
  `;
}

function stringArrayEditor(path, items, placeholder) {
  const list = Array.isArray(items) ? items : [];

  return `
    <div class="stack">
      ${list.length
        ? list
            .map(
              (item, index) => `
                <div class="inline-row">
                  <input
                    type="text"
                    value="${escapeAttribute(item)}"
                    placeholder="${escapeAttribute(placeholder)}"
                    data-path="${escapeAttribute(`${path}.${index}`)}"
                    data-type="text">
                  <button type="button" class="mini-button danger" data-action="remove-array-item" data-path="${escapeAttribute(path)}" data-index="${index}">Удалить</button>
                </div>
              `
            )
            .join("")
        : `<div class="empty-note">Список пуст. Нажми кнопку ниже, чтобы добавить новый элемент.</div>`}
      <button type="button" class="mini-button" data-action="add-array-item" data-path="${escapeAttribute(path)}" data-item-type="string-item">+ Добавить</button>
    </div>
  `;
}

function objectArrayEditor(path, items, itemType, renderer) {
  const list = Array.isArray(items) ? items : [];
  return `
    <div class="stack">
      ${list.length
        ? list
            .map(
              (item, index) => `
                <section class="sub-card">
                  <div class="sub-card-head">
                    <div>
                      <p class="eyebrow">Элемент ${index + 1}</p>
                      <h3 class="sub-card-title">${escapeHtml(item.className || item.type || "Новый блок")}</h3>
                    </div>
                    <button type="button" class="mini-button danger" data-action="remove-array-item" data-path="${escapeAttribute(path)}" data-index="${index}">Удалить</button>
                  </div>
                  ${renderer(`${path}.${index}`, item)}
                </section>
              `
            )
            .join("")
        : `<div class="empty-note">Пока пусто. Добавь первый элемент этого блока.</div>`}
      <button type="button" class="mini-button" data-action="add-array-item" data-path="${escapeAttribute(path)}" data-item-type="${escapeAttribute(itemType)}">+ Добавить элемент</button>
    </div>
  `;
}

function renderRewardItemFields(basePath, item) {
  return `
    <div class="field-grid">
      ${textField("Class Name (код предмета)", `${basePath}.className`, item.className, "Внутреннее имя предмета из игры. Например: SteakKnife.", "className")}
      ${numberField("Quantity (количество)", `${basePath}.quantity`, item.quantity, "Сколько предметов выдать или наградить.", 1, "quantity")}
      ${toggleField("Spawn On Ground (положить на землю)", `${basePath}.spawnOnGround`, item.spawnOnGround, "Если включено, предмет появится рядом на земле, а не сразу у игрока.", "spawnOnGround")}
    </div>
  `;
}

function renderObjectiveFields(basePath, item) {
  return `
    <div class="field-grid">
      ${textField("Type (тип цели)", `${basePath}.type`, item.type, "Обычно здесь остается значение item.", "type")}
      ${textField("Class Name (код предмета)", `${basePath}.className`, item.className, "Внутреннее имя нужного предмета из игры.", "className")}
      ${numberField("Quantity (сколько нужно принести)", `${basePath}.quantity`, item.quantity, "Общее требуемое количество. Если режим ниже выключен, считаются целые предметы / стаки. Если включен, считается внутреннее quantity предмета.", 1, "quantity")}
      ${toggleField("Remove On Complete (забирать после сдачи)", `${basePath}.removeOnComplete`, item.removeOnComplete, "Если включено, предметы исчезнут у игрока после выполнения цели.", "removeOnComplete")}
      ${toggleField("Use Item Quantity (считать внутреннее quantity, а не стаки)", `${basePath}.useItemQuantity`, item.useItemQuantity, "Включай для количественных предметов: деньги в стаках, патроны, дрова, палки, вода, топливо. Если выключено, каждый отдельный предмет / стак считается как 1.", "useItemQuantity")}
      ${toggleField("Allow Partial Turn In (разрешить сдачу по частям)", `${basePath}.allowPartialTurnIn`, item.allowPartialTurnIn, "Игрок сможет приносить нужное количество не за один раз, а постепенно.", "allowPartialTurnIn")}
    </div>
  `;
}

function readInputValue(target) {
  if (target.dataset.type === "number") {
    const raw = String(target.value || "").trim();
    return raw === "" ? 0 : Number(raw);
  }

  if (target.dataset.type === "flag") {
    return target.checked ? 1 : 0;
  }

  return target.value;
}

function getValueByPath(object, path) {
  return path.split(".").reduce((accumulator, token) => {
    if (accumulator == null) {
      return undefined;
    }
    return accumulator[token];
  }, object);
}

function setValueByPath(object, path, value) {
  const tokens = path.split(".");
  const lastToken = tokens.pop();
  let cursor = object;

  for (const token of tokens) {
    cursor = cursor[token];
  }

  cursor[lastToken] = value;
}

function getAssignedTriggerIdsFromData(data, questId) {
  const normalizedQuestId = String(questId || "").trim();
  if (!normalizedQuestId) {
    return [];
  }

  return normalizeArray(data.triggers)
    .filter((trigger) => normalizeArray(trigger.questIds).includes(normalizedQuestId))
    .map((trigger) => trigger.id);
}

function validateInventoryBlock(issues, questId, blockName, item, itemIndex) {
  if (!String(item.className || "").trim()) {
    issues.push(issue("error", `Пустой className в ${blockName}`, `Квест "${questId}" содержит элемент #${itemIndex + 1} в ${blockName} без className.`));
  }

  if (!Number.isFinite(Number(item.quantity))) {
    issues.push(issue("error", `Не число в ${blockName}.quantity`, `Квест "${questId}" содержит некорректное количество в ${blockName} #${itemIndex + 1}.`));
  }
}

function issue(level, title, message) {
  return { level, title, message };
}

function getBlockingIssues(issues) {
  return normalizeArray(issues).filter((issue) => issue.level === "error");
}

function formatBlockingIssuesBannerText(blockingIssues, prefix) {
  const issues = getBlockingIssues(blockingIssues);
  if (!issues.length) {
    return prefix;
  }

  const firstIssue = issues[0];
  const tail = issues.length > 1
    ? ` Критичных ошибок: ${issues.length}.`
    : "";

  return `${prefix} ${firstIssue.message}${tail}`;
}

function normalizeQuest(raw) {
  const quest = raw && typeof raw === "object" ? raw : {};
  return {
    id: toText(quest.id),
    name: toText(quest.name),
    description: toText(quest.description),
    repeatable: toFlag(quest.repeatable, 0),
    cooldownSeconds: parseNumber(quest.cooldownSeconds, 0),
    requiresPrevious: toText(quest.requiresPrevious),
    requiredQuestIds: normalizeQuestIdArray(quest.requiredQuestIds),
    hideUntilRequirementsComplete: toFlag(quest.hideUntilRequirementsComplete, 0),
    offerTriggerIds: normalizeSingleTriggerRoleIds(quest.offerTriggerIds),
    completionTriggerIds: normalizeQuestIdArray(quest.completionTriggerIds),
    rewardTriggerIds: normalizeSingleTriggerRoleIds(quest.rewardTriggerIds),
    triggerActions: normalizeArray(quest.triggerActions).map(normalizeQuestTriggerAction),
    giveItems: normalizeArray(quest.giveItems).map(normalizeRewardItem),
    objectives: normalizeArray(quest.objectives).map(normalizeObjective),
    rewards: normalizeArray(quest.rewards).map(normalizeRewardItem)
  };
}

function normalizeTrigger(raw) {
  const trigger = raw && typeof raw === "object" ? raw : {};
  return {
    id: toText(trigger.id),
    position: normalizeVector(trigger.position),
    radius: parseNumber(trigger.radius, 2),
    focusHeight: parseNumber(trigger.focusHeight, 1.2),
    focusRadius: parseNumber(trigger.focusRadius, 1),
    questIds: normalizeQuestIdArray(trigger.questIds),
    hintText: toText(trigger.hintText),
    spawnNpc: toFlag(trigger.spawnNpc, 1),
    npcClassName: toText(trigger.npcClassName),
    npcPosition: normalizeVector(trigger.npcPosition),
    npcOrientation: normalizeVector(trigger.npcOrientation),
    npcLoadout: normalizeArray(trigger.npcLoadout).map(toText),
    npcHandsItem: toText(trigger.npcHandsItem),
    npcBackItems: normalizeArray(trigger.npcBackItems).map(toText)
  };
}

function normalizeRewardItem(raw) {
  const item = raw && typeof raw === "object" ? raw : {};
  return {
    className: toText(item.className),
    quantity: parseNumber(item.quantity, 0),
    spawnOnGround: toFlag(item.spawnOnGround, 0)
  };
}

function normalizeObjective(raw) {
  const item = raw && typeof raw === "object" ? raw : {};
  return {
    type: toText(item.type || "item"),
    className: toText(item.className),
    quantity: parseNumber(item.quantity, 0),
    removeOnComplete: toFlag(item.removeOnComplete, 1),
    useItemQuantity: toFlag(item.useItemQuantity, 0),
    allowPartialTurnIn: toFlag(item.allowPartialTurnIn, 1)
  };
}

function normalizeStackRules(rawRules) {
  return normalizeArray(rawRules).map(normalizeStackRule);
}

function normalizeStackRule(raw) {
  const rule = raw && typeof raw === "object" ? raw : {};
  return {
    className: toText(rule.className).trim(),
    stackSize: Math.max(1, parseNumber(rule.stackSize, 1))
  };
}

function normalizeVector(input) {
  const array = normalizeArray(input);
  return [0, 1, 2].map((index) => parseNumber(array[index], 0));
}

function normalizeArray(value) {
  return Array.isArray(value) ? value : [];
}

function normalizeQuestIdArray(value) {
  return normalizeArray(value)
    .flatMap((item) =>
      String(item == null ? "" : item)
        .split(",")
        .map((part) => part.trim())
        .filter(Boolean)
    );
}

function toText(value) {
  return value == null ? "" : String(value);
}

function toFlag(value, fallback) {
  if (value === true || value === "true") {
    return 1;
  }
  if (value === false || value === "false") {
    return 0;
  }
  return Number(value) === 1 ? 1 : Number(value) === 0 ? 0 : fallback;
}

function parseNumber(value, fallback) {
  if (value === "" || value == null) {
    return fallback;
  }
  const parsed = Number(value);
  return Number.isFinite(parsed) ? parsed : fallback;
}

function createQuest(defaultTriggerId = "") {
  const normalizedTriggerId = String(defaultTriggerId || "").trim();
  const defaultOfferTriggerIds = normalizedTriggerId ? [normalizedTriggerId] : [];
  const defaultRewardTriggerIds = normalizedTriggerId ? [normalizedTriggerId] : [];
  const defaultTriggerActions = normalizedTriggerId
    ? [
      createQuestTriggerAction("offer", normalizedTriggerId),
      createQuestTriggerAction("reward", normalizedTriggerId)
    ]
    : [];

  return {
    id: makeUniqueId(state.data.quests.map((quest) => quest.id), "quest_new"),
    name: "Новый квест",
    description: "",
    repeatable: 0,
    cooldownSeconds: 0,
    requiresPrevious: "",
    requiredQuestIds: [],
    hideUntilRequirementsComplete: 0,
    offerTriggerIds: defaultOfferTriggerIds,
    completionTriggerIds: [],
    rewardTriggerIds: defaultRewardTriggerIds,
    triggerActions: defaultTriggerActions,
    giveItems: [],
    objectives: [],
    rewards: []
  };
}

function createTrigger() {
  return {
    id: makeUniqueId(state.data.triggers.map((trigger) => trigger.id), "trigger_new"),
    position: [0, 0, 0],
    radius: 2,
    focusHeight: 1.2,
    focusRadius: 1,
    questIds: [],
    hintText: "",
    spawnNpc: 1,
    npcClassName: "",
    npcPosition: [0, 0, 0],
    npcOrientation: [0, 0, 0],
    npcLoadout: [],
    npcHandsItem: "",
    npcBackItems: []
  };
}

function createRewardItem() {
  return {
    className: "",
    quantity: 1,
    spawnOnGround: 0
  };
}

function createObjective() {
  return {
    type: "item",
    className: "",
    quantity: 1,
    removeOnComplete: 1,
    useItemQuantity: 0,
    allowPartialTurnIn: 1
  };
}

function createStackRule() {
  return {
    className: "",
    stackSize: 1
  };
}

function buildStackRulesPayload() {
  return {
    version: STACK_RULES_VERSION,
    rules: state.stackRules.map((rule) => ({
      className: String(rule.className || "").trim(),
      stackSize: Math.max(1, parseNumber(rule.stackSize, 1))
    }))
  };
}

function makeUniqueId(existing, base) {
  const safeBase = String(base || "new_item").trim() || "new_item";
  const seen = new Set(existing);
  if (!seen.has(safeBase)) {
    return safeBase;
  }

  let counter = 2;
  while (seen.has(`${safeBase}_${counter}`)) {
    counter += 1;
  }
  return `${safeBase}_${counter}`;
}

function matchesFilter(query, values) {
  if (!query) {
    return true;
  }
  return values.some((value) => String(value || "").toLowerCase().includes(query));
}

function ensureJsonExtension(fileName) {
  return fileName.toLowerCase().endsWith(".json") ? fileName : `${fileName}.json`;
}

function getBaseNameFromPath(inputPath) {
  const value = String(inputPath || "").trim();
  if (!value) {
    return "quests.json";
  }

  const normalized = value.replaceAll("\\", "/");
  const parts = normalized.split("/");
  return parts[parts.length - 1] || value;
}

function updateBanner(nextBanner) {
  state.banner = nextBanner;
  renderBanner();
}

function scrollValidationIntoView() {
  const validationCard = elements.validationSummary?.closest(".inspector-card");
  if (!validationCard) {
    return;
  }

  validationCard.scrollIntoView({
    behavior: "smooth",
    block: "start"
  });
}

function scheduleDraftSave() {
  cancelScheduledDraftSave();
  draftSaveTimer = window.setTimeout(() => {
    draftSaveTimer = null;
    saveDraftNow();
  }, 180);
}

function cancelScheduledDraftSave() {
  window.clearTimeout(draftSaveTimer);
  draftSaveTimer = null;
}

function scheduleStackRulesSave() {
  cancelScheduledStackRulesSave();
  stackRulesSaveTimer = window.setTimeout(() => {
    stackRulesSaveTimer = null;
    void saveStackRulesNow(false);
  }, 320);
}

function cancelScheduledStackRulesSave() {
  window.clearTimeout(stackRulesSaveTimer);
  stackRulesSaveTimer = null;
}

function saveDraftNow() {
  const payload = {
    fileName: state.fileName,
    data: state.data,
    updatedAt: Date.now()
  };

  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(payload));
  } catch (error) {
    console.warn("Не удалось сохранить локальный черновик", error);
  }

  if (window.location.protocol.startsWith("http")) {
    void saveDraftToProjectFile(payload);
  }
}

async function restoreDraft() {
  let localDraft = null;
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (raw) {
      localDraft = normalizeDraftPayload(JSON.parse(raw));
    }
  } catch (error) {
    console.warn("Не удалось прочитать черновик", error);
  }

  if (window.location.protocol.startsWith("http")) {
    const fileDraft = normalizeDraftPayload(await restoreDraftFromProjectFile());

    if (fileDraft && fileDraft.updatedAt > 0) {
      return {
        ...fileDraft,
        source: "file"
      };
    }

    if (localDraft) {
      try {
        localStorage.removeItem(STORAGE_KEY);
      } catch (error) {
        console.warn("Не удалось очистить локальный черновик в режиме сервера", error);
      }
    }

    return null;
  }

  if (!localDraft || localDraft.updatedAt === 0) {
    if (localDraft) {
      void clearDraftStorage();
    }
    return null;
  }

  return {
    ...localDraft,
    source: "localStorage"
  };
}

async function saveDraftToProjectFile(payload) {
  try {
    await fetch(DRAFT_API_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json;charset=utf-8"
      },
      body: JSON.stringify(payload)
    });
  } catch (error) {
    console.warn("Не удалось сохранить черновик в файл проекта", error);
  }
}

async function restoreDraftFromProjectFile() {
  try {
    const response = await fetch(DRAFT_API_URL, { cache: "no-store" });
    if (response.status === 404) {
      return null;
    }
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }
    return await response.json();
  } catch (error) {
    console.warn("Не удалось прочитать черновик из файла проекта", error);
    return null;
  }
}

async function clearDraftStorage() {
  cancelScheduledDraftSave();

  try {
    localStorage.removeItem(STORAGE_KEY);
  } catch (error) {
    console.warn("Не удалось очистить локальный черновик", error);
  }

  if (!window.location.protocol.startsWith("http")) {
    return;
  }

  try {
    await fetch(DRAFT_API_URL, {
      method: "DELETE"
    });
  } catch (error) {
    console.warn("Не удалось удалить черновик из файла проекта", error);
  }
}

function tryParseJsonText(text) {
  try {
    return {
      ok: true,
      data: JSON.parse(text),
      repaired: false
    };
  } catch (originalError) {
    const repairedText = attemptKnownRepairs(text);
    if (repairedText !== text) {
      try {
        return {
          ok: true,
          data: JSON.parse(repairedText),
          repaired: true
        };
      } catch (repairError) {
        return {
          ok: false,
          error: repairError.message || originalError.message
        };
      }
    }

    return {
      ok: false,
      error: originalError.message || "Неизвестная ошибка парсинга."
    };
  }
}

function attemptKnownRepairs(text) {
  const numericKeys = [
    "quantity",
    "repeatable",
    "cooldownSeconds",
    "hideUntilRequirementsComplete",
    "spawnOnGround",
    "removeOnComplete",
    "useItemQuantity",
    "allowPartialTurnIn",
    "radius",
    "focusHeight",
    "focusRadius",
    "spawnNpc"
  ];

  let next = String(text || "").replace(/^\uFEFF/, "");
  next = next.replace(/,\s*([}\]])/g, "$1");

  const emptyNumericValuePattern = new RegExp(`("(${numericKeys.join("|")})"\\s*:\\s*)(?=,|}|\\])`, "g");
  next = next.replace(emptyNumericValuePattern, (match, prefix) => `${prefix}0`);

  return next;
}

function escapeHtml(value) {
  return String(value ?? "")
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#39;");
}

function escapeAttribute(value) {
  return escapeHtml(value);
}

function normalizeDraftPayload(draft) {
  if (!draft || typeof draft !== "object" || !draft.data || typeof draft.data !== "object") {
    return null;
  }

  const updatedAt = Number(draft.updatedAt || 0);

  return {
    fileName: String(draft.fileName || "quests.json"),
    data: draft.data,
    updatedAt: Number.isFinite(updatedAt) && updatedAt > 0 ? updatedAt : 0
  };
}

function pickPreferredDraft(fileDraft, localDraft) {
  if (!fileDraft && !localDraft) {
    return null;
  }

  if (fileDraft && fileDraft.updatedAt === 0) {
    fileDraft = null;
  }

  if (localDraft && localDraft.updatedAt === 0) {
    localDraft = null;
  }

  if (!fileDraft && !localDraft) {
    return null;
  }

  if (fileDraft && !localDraft) {
    return {
      ...fileDraft,
      source: "file"
    };
  }

  if (localDraft && !fileDraft) {
    return {
      ...localDraft,
      source: "localStorage"
    };
  }

  if (localDraft.updatedAt >= fileDraft.updatedAt) {
    return {
      ...localDraft,
      source: "localStorage"
    };
  }

  return {
    ...fileDraft,
    source: "file"
  };
}

// ============================================================================
// Role-driven quest editor overrides
// ============================================================================

function getTriggerDisplayLabel(triggerId) {
  const normalizedTriggerId = String(triggerId || "").trim();
  if (!normalizedTriggerId) {
    return "Без trigger";
  }

  const trigger = findTriggerById(normalizedTriggerId);
  if (!trigger) {
    return normalizedTriggerId;
  }

  return trigger.hintText
    ? `${normalizedTriggerId} - ${trigger.hintText}`
    : normalizedTriggerId;
}

function getQuestActionCards(quest, actionType) {
  return normalizeArray(quest?.triggerActions)
    .map((action, actionIndex) => ({
      action: normalizeQuestTriggerAction(action),
      actionIndex
    }))
    .filter(({ action }) => action.actionType === actionType);
}

function findQuestActionCard(quest, actionType, triggerId) {
  const normalizedActionType = String(actionType || "").trim();
  const normalizedTriggerId = String(triggerId || "").trim();

  if (!normalizedActionType || !normalizedTriggerId) {
    return null;
  }

  return getQuestActionCards(quest, normalizedActionType).find(({ action }) => action.triggerId === normalizedTriggerId) || null;
}

function getQuestActionSectionMeta(quest, actionType) {
  if (actionType === "offer") {
    return {
      title: "Взятие квеста",
      description: "Отдельная стартовая реплика NPC, который выдаёт этот квест.",
      emptyText: "Пока нет ни одного trigger в роли offer.",
      dialogLabel: "Offer Dialog Text (triggerActions[].dialogText / что скажет NPC при взятии)",
      dialogHint: "Это NPC-реплика из triggerActions[].dialogText, а не отдельное поле dialogue и не quest.description.",
      rewardHint: "Предметы при старте задаются ниже через Give Items."
    };
  }

  if (actionType === "completion") {
    return {
      title: "Сдача / передача",
      description: "Отдельные реплики и, при необходимости, локальная награда у NPC, который принимает предметы или завершает этап.",
      emptyText: "Пока нет ни одного trigger в роли completion.",
      dialogLabel: "Completion Dialog Text (triggerActions[].dialogText / что скажет NPC при сдаче)",
      dialogHint: "Это NPC-реплика из triggerActions[].dialogText. Если этот же NPC должен дать награду, включи ему роль Reward ниже.",
      rewardHint: "Необязательно: здесь можно выдать предметы за этот этап передачи. Финальное закрытие квеста всё равно делает Reward-блок."
    };
  }

  return {
    title: "Выдача награды",
    description: "Эти блоки срабатывают после выполненной передачи или сдачи. Награду можно выдать у любого NPC, которому включена роль Reward.",
    emptyText: "Пока нет ни одного trigger в роли reward.",
    dialogLabel: "Reward Dialog Text (triggerActions[].dialogText / что скажет NPC перед наградой)",
    dialogHint: "Это NPC-реплика из triggerActions[].dialogText, а не отдельное поле dialogue.",
    rewardHint: "Если локальный список наград пуст, сюда подставятся общие Rewards квеста."
  };
}

function buildQuestRoleTextForTrigger(quest, triggerId) {
  if (!quest || !triggerId) {
    return "";
  }

  const roleLabels = [];
  if (isQuestInTriggerRole(quest, triggerId, "offer")) {
    roleLabels.push("взять");
  }
  if (isQuestInTriggerRole(quest, triggerId, "completion")) {
    roleLabels.push("сдать");
  }
  if (isQuestInTriggerRole(quest, triggerId, "reward")) {
    roleLabels.push("награда");
  }

  if (!roleLabels.length) {
    return "Без роли";
  }

  return `Роли: ${roleLabels.join(" · ")}`;
}

function getTriggerQuestOrderIndex(trigger, questId) {
  const normalizedQuestId = String(questId || "").trim();
  if (!trigger || !normalizedQuestId) {
    return -1;
  }

  return normalizeArray(trigger.questIds)
    .map((entry) => String(entry || "").trim())
    .findIndex((entry) => entry === normalizedQuestId);
}

function buildQuestTextPreview(value, emptyText = "Пока пусто.") {
  const normalized = toText(value).replace(/\s+/g, " ").trim();
  if (!normalized) {
    return emptyText;
  }

  if (normalized.length <= 160) {
    return normalized;
  }

  return `${normalized.slice(0, 157)}...`;
}

function buildQuestItemSummary(items, emptyText) {
  const entries = normalizeArray(items)
    .map((item) => {
      const className = toText(item?.className).trim();
      const type = toText(item?.type).trim();
      const label = className || type;
      if (!label) {
        return "";
      }

      const quantity = Number(item?.quantity || 0);
      if (Number.isFinite(quantity) && quantity > 0) {
        return `${label} x${quantity}`;
      }

      return label;
    })
    .filter(Boolean);

  if (!entries.length) {
    return emptyText;
  }

  if (entries.length <= 3) {
    return entries.join(", ");
  }

  return `${entries.slice(0, 3).join(", ")} + ещё ${entries.length - 3}`;
}

function buildQuestTriggerAssignmentHint(trigger, quest) {
  const normalizedQuestId = String(quest?.id || "").trim();
  const orderIndex = getTriggerQuestOrderIndex(trigger, normalizedQuestId);
  const hasAnyRole = questUsesTriggerInAnyRole(quest, trigger?.id);

  if (orderIndex >= 0 && hasAnyRole) {
    return `Квест уже стоит у этого NPC в questIds. Порядок у NPC: #${orderIndex + 1}.`;
  }

  if (orderIndex >= 0) {
    return `Квест уже есть в questIds этого NPC, но для него ещё не выбрана ни одна основная роль.`;
  }

  return "Пока этот NPC не участвует в квесте. Включи нужную роль ниже, и квест автоматически добавится в его questIds.";
}

function isQuestRoleUnique(role) {
  return role === "offer" || role === "reward";
}

function canAssignRoleToTrigger(quest, role, triggerId, currentRole = "") {
  const normalizedRole = String(role || "").trim();
  const normalizedTriggerId = String(triggerId || "").trim();
  const normalizedCurrentRole = String(currentRole || "").trim();
  if (!quest || !normalizedRole || !normalizedTriggerId) {
    return false;
  }

  if (normalizedCurrentRole && normalizedCurrentRole !== normalizedRole) {
    return false;
  }

  const roleTriggerIds = getQuestRoleTriggerIds(quest, normalizedRole);
  if (roleTriggerIds.includes(normalizedTriggerId)) {
    return normalizedCurrentRole === normalizedRole;
  }

  if (isQuestRoleUnique(normalizedRole) && roleTriggerIds.length > 0) {
    return false;
  }

  return true;
}

function renderQuestTriggerRoleToggle(questIndex, triggerId, role, title, hint, checked, options = {}) {
  const disabled = Boolean(options.disabled);
  return `
    <label class="toggle-field trigger-role-toggle ${checked ? "active" : ""} ${disabled ? "disabled" : ""}">
      <div>
        <div class="field-label-row">
          <label>${escapeHtml(title)}</label>
          <span class="field-key">role: ${escapeHtml(role)}</span>
        </div>
        <div class="muted">${escapeHtml(hint)}</div>
      </div>
      <input
        type="checkbox"
        ${checked ? "checked" : ""}
        ${disabled ? "disabled" : ""}
        data-role="quest-trigger-toggle"
        data-trigger-role="${escapeAttribute(role)}"
        data-quest-index="${questIndex}"
        data-trigger-id="${escapeAttribute(triggerId)}">
    </label>
  `;
}

function getQuestFlowActiveTriggerIds(quest) {
  return Array.from(new Set([
    ...normalizeQuestIdArray(quest?.offerTriggerIds),
    ...normalizeQuestIdArray(quest?.completionTriggerIds),
    ...normalizeQuestIdArray(quest?.rewardTriggerIds)
  ]));
}

function getQuestFlowActiveBlocks(quest) {
  return [
    ...normalizeQuestIdArray(quest?.offerTriggerIds).map((triggerId) => ({
      role: "offer",
      triggerId,
      key: `offer:${triggerId}`
    })),
    ...normalizeQuestIdArray(quest?.completionTriggerIds).map((triggerId) => ({
      role: "completion",
      triggerId,
      key: `completion:${triggerId}`
    })),
    ...normalizeQuestIdArray(quest?.rewardTriggerIds).map((triggerId) => ({
      role: "reward",
      triggerId,
      key: `reward:${triggerId}`
    }))
  ];
}

function getQuestFlowVisibleTriggers(quest, questIndex = -1) {
  const mode = normalizeNpcFlowMode(state.npcFlowMode);
  const selectedTriggerId = normalizeNpcFlowSelectedTriggerId();
  const activeBlocks = getQuestFlowActiveBlocks(quest);
  const draftBlocks = getQuestFlowDraftTriggerIds(quest, questIndex).map((block) => ({
    role: "",
    triggerId: block.triggerId,
    key: block.id || `draft:${block.triggerId}`
  }));
  const allBlocks = [...activeBlocks, ...draftBlocks];

  if (mode === "all") {
    return allBlocks;
  }

  if (mode === "selected") {
    return selectedTriggerId
      ? allBlocks.filter((block) => block.triggerId === selectedTriggerId)
      : [];
  }

  return allBlocks;
}

function renderQuestFlowModeButton(mode, label, countText = "") {
  const isActive = normalizeNpcFlowMode(state.npcFlowMode) === mode;
  return `
    <button
      type="button"
      class="mini-button quest-flow-mode-button ${isActive ? "active" : ""}"
      data-action="set-npc-flow-mode"
      data-mode="${escapeAttribute(mode)}">
      <span>${escapeHtml(label)}</span>
      ${countText ? `<span class="quest-flow-mode-count">${escapeHtml(countText)}</span>` : ""}
    </button>
  `;
}

function renderQuestFlowTriggerPicker(selectedTriggerId) {
  const options = [
    `<option value="">Выбери NPC / trigger</option>`,
    ...normalizeArray(state.data.triggers).map((trigger) => {
      const selected = trigger.id === selectedTriggerId ? "selected" : "";
      return `<option value="${escapeAttribute(trigger.id)}" ${selected}>${escapeHtml(getTriggerDisplayLabel(trigger.id))}</option>`;
    })
  ];

  return `
    <label class="quest-flow-picker">
      <span class="quest-flow-picker-label">Открыть / добавить NPC</span>
      <select data-role="npc-flow-trigger-picker">
        ${options.join("")}
      </select>
    </label>
  `;
}

function renderQuestFlowCardTriggerPicker(questIndex, currentTriggerId, options = {}) {
  if (options.locked) {
    return `
      <div class="quest-flow-card-picker quest-flow-card-picker--locked">
        <span class="quest-flow-picker-label">NPC / trigger этого блока</span>
        <div class="quest-flow-card-value">
          <strong>${escapeHtml(getTriggerDisplayLabel(currentTriggerId))}</strong>
          <span class="field-key">блок зафиксирован</span>
        </div>
      </div>
    `;
  }

  const includeEmptyOption = Boolean(options.includeEmptyOption);
  const optionRows = normalizeArray(state.data.triggers).map((trigger) => {
    const selected = trigger.id === currentTriggerId ? "selected" : "";
    return `<option value="${escapeAttribute(trigger.id)}" ${selected}>${escapeHtml(getTriggerDisplayLabel(trigger.id))}</option>`;
  });

  if (includeEmptyOption) {
    optionRows.unshift(`<option value="">${escapeHtml(options.emptyLabel || "Выбери NPC / trigger")}</option>`);
  }

  return `
    <label class="quest-flow-card-picker">
      <span class="quest-flow-picker-label">NPC / trigger этого блока</span>
      <select
        data-role="quest-flow-card-trigger-picker"
        data-quest-index="${questIndex}"
        data-current-trigger-id="${escapeAttribute(currentTriggerId)}">
        ${optionRows.join("")}
      </select>
    </label>
  `;
}

function renderQuestOfferSummary(quest, questIndex, triggerId) {
  const basePath = `quests.${questIndex}`;
  const actionCard = findQuestActionCard(quest, "offer", triggerId);
  const actionBasePath = actionCard ? `quests.${questIndex}.triggerActions.${actionCard.actionIndex}` : "";
  const offerDialog = actionCard ? actionCard.action.dialogText : "";
  const hasCompletionStages = normalizeQuestIdArray(quest.completionTriggerIds).length > 0;
  const nextStepText = hasCompletionStages
    ? "Следующий шаг задаётся блоком с галочкой Completion. Этот Offer-блок только выдаёт старт и задачу."
    : "Если промежуточных этапов нет, квест всё равно закрывается только через Reward-блок.";

  return `
    <div class="trigger-role-detail trigger-role-detail--offer">
      <div class="trigger-role-detail-head">
        <div>
          <p class="eyebrow">Offer / Взятие</p>
          <h3 class="sub-card-title">Стартовые настройки этого NPC</h3>
        </div>
        <span class="trigger-role-chip">offer</span>
      </div>
      <div class="stack">
        <div class="muted">Эти настройки срабатывают у NPC, где игрок берёт квест: стартовый диалог, предметы на выдачу и задача игроку.</div>
        ${actionBasePath
          ? textareaField("Offer Dialog Text (что скажет NPC при взятии)", `${actionBasePath}.dialogText`, offerDialog, "Например: возьми предмет и отнеси его нужному человеку.", "dialogText")
          : `<div class="empty-note">Offer dialog живёт в <code>triggerActions[].dialogText</code>. Здесь нет отдельного поля <code>dialogue</code>, а <code>quest.description</code> редактируется выше как описание квеста для DescriptionPanel.</div>`}
        <div class="muted">DescriptionPanel читает <code>quest.description</code> и сводку по целям/наградам. DialogPanel читает NPC-реплику из <code>triggerActions[].dialogText</code>.</div>
        <div class="trigger-summary-grid">
          <div class="trigger-summary-item">
            <strong>Что выдать при старте</strong>
            <div class="muted">${escapeHtml(buildQuestItemSummary(quest.giveItems, "Ничего не выдаётся."))}</div>
          </div>
          <div class="trigger-summary-item">
            <strong>Задача игроку</strong>
            <div class="muted">${escapeHtml(buildQuestItemSummary(quest.objectives, "Цели пока не заданы."))}</div>
          </div>
          <div class="trigger-summary-item">
            <strong>Куда дальше</strong>
            <div class="muted">${escapeHtml(nextStepText)}</div>
          </div>
        </div>
        <div class="stack">
          <h4 class="inline-section-title">Give Items (предметы при взятии)</h4>
          ${objectArrayEditor(`${basePath}.giveItems`, quest.giveItems, "reward-item", renderRewardItemFields)}
        </div>
        <div class="stack">
          <h4 class="inline-section-title">Objectives (quests[].objectives[] / что нужно передать или принести, не отдельное requiredItems)</h4>
          ${objectArrayEditor(`${basePath}.objectives`, quest.objectives, "objective-item", renderObjectiveFields)}
        </div>
      </div>
    </div>
  `;
}

function renderQuestTriggerActionDetail(quest, questIndex, actionType, triggerId) {
  const actionCard = findQuestActionCard(quest, actionType, triggerId);
  const meta = getQuestActionSectionMeta(quest, actionType);

  if (!actionCard) {
    return `
      <div class="trigger-role-detail trigger-role-detail--${escapeAttribute(actionType)}">
        <div class="empty-note">Блок triggerActions для ${escapeHtml(triggerId)} ещё не создан.</div>
      </div>
    `;
  }

  const { action, actionIndex } = actionCard;
  const basePath = `quests.${questIndex}.triggerActions.${actionIndex}`;
  const showRewardEditor = actionType === "completion" || actionType === "reward";
  const detailTitle = actionType === "completion"
    ? "Сдача / передача у этого NPC"
    : "Выдача награды у этого NPC";

  return `
    <div class="trigger-role-detail trigger-role-detail--${escapeAttribute(actionType)}">
      <div class="trigger-role-detail-head">
        <div>
          <p class="eyebrow">${escapeHtml(meta.title)}</p>
          <h3 class="sub-card-title">${escapeHtml(detailTitle)}</h3>
        </div>
        <span class="trigger-role-chip">${escapeHtml(actionType)}</span>
      </div>
      <div class="stack">
        ${textareaField(meta.dialogLabel, `${basePath}.dialogText`, action.dialogText, meta.dialogHint, "dialogText")}
        <div class="muted">DialogPanel в моде показывает именно это поле: <code>triggerActions[].dialogText</code>.</div>
        ${showRewardEditor
          ? `
            <div class="stack">
              <div class="muted">${escapeHtml(meta.rewardHint)}</div>
              ${objectArrayEditor(`${basePath}.rewards`, action.rewards, "reward-item", renderRewardItemFields)}
            </div>
          `
          : `<div class="muted">${escapeHtml(meta.rewardHint)}</div>`}
      </div>
    </div>
  `;
}

function renderQuestRoleFlowCard(quest, questIndex, block, triggerIndex) {
  const trigger = findTriggerById(block.triggerId);
  if (!trigger) {
    return "";
  }

  const currentRole = String(block.role || "").trim();
  const isDraft = !currentRole;
  const orderIndex = getTriggerQuestOrderIndex(trigger, quest.id);
  const offerEnabled = currentRole === "offer";
  const completionEnabled = currentRole === "completion";
  const rewardEnabled = currentRole === "reward";
  const toneClass = triggerIndex % 2 === 0 ? "trigger-flow-card--clay" : "trigger-flow-card--sage";
  const isFocused = normalizeNpcFlowSelectedTriggerId() === trigger.id;
  const roleNames = {
    offer: "взять",
    completion: "сдать",
    reward: "награда"
  };
  const stageTitles = {
    offer: "СТАРТ КВЕСТА",
    completion: "ПРОМЕЖУТОЧНОЕ ЗАДАНИЕ",
    reward: "ЗАВЕРШАЮЩИЙ"
  };
  const siblingRoles = ["offer", "completion", "reward"]
    .filter((role) => role !== currentRole && getQuestRoleTriggerIds(quest, role).includes(trigger.id));
  const siblingRoleLabels = siblingRoles.map((role) => roleNames[role] || role);
  const roleLabel = offerEnabled
    ? roleNames.offer
    : completionEnabled
      ? roleNames.completion
      : rewardEnabled
        ? roleNames.reward
        : "";

  return `
    <section class="trigger-flow-card ${toneClass} ${isFocused ? "trigger-flow-card--focused" : ""}">
      <div class="trigger-flow-head">
        <div>
          ${!isDraft && stageTitles[currentRole] ? `<div class="quest-flow-stage-label">${escapeHtml(stageTitles[currentRole])}</div>` : ""}
          <p class="eyebrow">Trigger / NPC</p>
          ${renderQuestFlowCardTriggerPicker(questIndex, trigger.id, { locked: !isDraft })}
          <div class="muted">${escapeHtml(roleLabel ? `Роль блока: ${roleLabel}` : "Роль этого блока пока не выбрана.")}</div>
        </div>
        <div class="trigger-flow-meta">
          <span class="trigger-role-chip">${isDraft ? "новый role-блок" : orderIndex >= 0 ? `questIds #${orderIndex + 1}` : "вне questIds"}</span>
        </div>
      </div>

      <div class="stack">
        <div class="muted">${escapeHtml(buildQuestTriggerAssignmentHint(trigger, quest))}</div>
        <div class="muted">
          ${isDraft
            ? "Выбери для этого блока только одну роль. Если этот же NPC нужен ещё раз, добавь для него отдельный блок."
            : `Один блок хранит только одну роль.${siblingRoleLabels.length ? ` У этого же NPC уже есть другие блоки: ${siblingRoleLabels.join(", ")}.` : ""}`}
        </div>
        <div class="trigger-role-toggle-list">
          ${renderQuestTriggerRoleToggle(questIndex, trigger.id, "offer", "Offer: здесь берут квест", "Только старт квеста: этот блок выдаёт задачу и сам по себе не закрывает квест.", offerEnabled, { disabled: !canAssignRoleToTrigger(quest, "offer", trigger.id, currentRole) })}
          ${renderQuestTriggerRoleToggle(questIndex, trigger.id, "completion", "Completion: промежуточная передача / этап", "Опциональный этап цепочки. Reward не закроет квест, пока все Completion-этапы не выполнены.", completionEnabled, { disabled: !canAssignRoleToTrigger(quest, "completion", trigger.id, currentRole) })}
          ${renderQuestTriggerRoleToggle(questIndex, trigger.id, "reward", "Reward: закрыть квест и выдать награду", "Обязательная финальная точка. Даже если это тот же NPC, что и Offer, именно Reward проверяет цели и все Completion, а потом закрывает квест.", rewardEnabled, { disabled: !canAssignRoleToTrigger(quest, "reward", trigger.id, currentRole) })}
        </div>

        ${offerEnabled ? renderQuestOfferSummary(quest, questIndex, trigger.id) : ""}
        ${completionEnabled ? renderQuestTriggerActionDetail(quest, questIndex, "completion", trigger.id) : ""}
        ${rewardEnabled ? renderQuestTriggerActionDetail(quest, questIndex, "reward", trigger.id) : ""}
        ${isDraft ? `<div class="empty-note">Пока это только заготовка role-блока. Включи одну роль выше, и настройки этого этапа откроются здесь же.</div>` : ""}
      </div>
    </section>
  `;
}

function renderQuestTriggerFlowEmptyCard(questIndex, triggerIndex) {
  const toneClass = triggerIndex % 2 === 0 ? "trigger-flow-card--clay" : "trigger-flow-card--sage";

  return `
    <section class="trigger-flow-card trigger-flow-card--empty ${toneClass}">
      <div class="trigger-flow-head">
        <div>
          <p class="eyebrow">Новый блок цепочки</p>
          ${renderQuestFlowCardTriggerPicker(questIndex, "", {
            includeEmptyOption: true,
            emptyLabel: "Выбери NPC / trigger"
          })}
          <div class="muted">Выбери NPC / trigger, затем в появившейся карточке включи нужные роли.</div>
        </div>
        <div class="trigger-flow-meta">
          <span class="trigger-role-chip">новый блок</span>
        </div>
      </div>

      <div class="empty-note">
        Этот блок пока не попадёт в JSON. Он станет частью цепочки только после галочки
        <strong>взять</strong>, <strong>сдать</strong> или <strong>награда</strong>.
      </div>
    </section>
  `;
}

function renderQuestTriggerFlowSection(quest, questIndex) {
  const allTriggers = normalizeArray(state.data.triggers);
  if (!allTriggers.length) {
    return `<div class="empty-note">Сначала создай хотя бы один trigger / NPC.</div>`;
  }

  const mode = normalizeNpcFlowMode(state.npcFlowMode);
  const selectedTriggerId = normalizeNpcFlowSelectedTriggerId();
  const selectedTriggerLabel = selectedTriggerId ? getTriggerDisplayLabel(selectedTriggerId) : "";
  const activeBlocks = getQuestFlowActiveBlocks(quest);
  const flowBlocks = getQuestFlowVisibleTriggers(quest, questIndex);
  const sidebarTriggerId = normalizeSidebarQuestTriggerId();

  let body = "";

  if (mode === "selected" && !selectedTriggerId) {
    body = `
      <div class="empty-note">
        Выбери нужный NPC в поле «Открыть / добавить NPC», и здесь откроется его универсальный блок.
      </div>
    `;
  } else if (!flowBlocks.length && mode !== "active") {
    body = `<div class="empty-note">По текущему режиму показа здесь пока нечего показывать.</div>`;
  } else {
    body = `
      <div class="trigger-flow-grid">
        ${flowBlocks.map((block, triggerIndex) => renderQuestRoleFlowCard(quest, questIndex, block, triggerIndex)).join("")}
        ${mode === "active" ? renderQuestTriggerFlowEmptyCard(questIndex, flowBlocks.length) : ""}
      </div>
    `;
  }

  return `
    <div class="stack">
      <div class="quest-flow-toolbar">
        <div class="quest-flow-control-row">
          <div class="quest-flow-mode-group">
            ${renderQuestFlowModeButton("active", "Активные блоки", String(activeBlocks.length))}
            ${renderQuestFlowModeButton("selected", "Выбранный NPC", selectedTriggerId ? "1" : "0")}
            ${renderQuestFlowModeButton("all", "Все NPC", String(allTriggers.length))}
          </div>
          ${renderQuestFlowTriggerPicker(selectedTriggerId)}
        </div>
      <div class="quest-flow-toolbar-note">
        ${selectedTriggerId
            ? `Сейчас выбран NPC: <strong>${escapeHtml(selectedTriggerLabel)}</strong>. В активном режиме ниже остаются видны все текущие блоки и пустой блок для добавления следующего NPC в цепочку.`
            : "В активном режиме снизу всегда есть пустой блок: выбери в нём NPC и включи нужные роли."}
      </div>
      </div>
      <div class="muted">
        Здесь используются универсальные блоки с ограничениями: один блок хранит только одну роль. Если один и тот же NPC нужен повторно, для него создаётся отдельный блок.
        <strong>Offer</strong> только выдаёт квест и не закрывает его. <strong>Reward</strong> обязателен всегда, даже если это тот же NPC, что и Offer. Если у квеста есть хотя бы один <strong>Completion</strong>, Reward не закроет квест, пока все Completion не выполнены.
        Список и порядок активных квестов у NPC по-прежнему берутся из <code>trigger.questIds</code>, а цепочка ролей уходит в <code>offerTriggerIds</code>, <code>completionTriggerIds</code>, <code>rewardTriggerIds</code> и <code>triggerActions</code>.
        ${sidebarTriggerId ? ` Левый фильтр квестов сейчас стоит на <strong>${escapeHtml(getTriggerDisplayLabel(sidebarTriggerId))}</strong>, но блоки NPC можно настраивать и независимо от него.` : ""}
      </div>
      ${body}
      ${mode !== "active" ? `<div class="muted">Если хочешь добавлять следующий NPC в цепочку через пустой блок, переключись обратно в режим <strong>Активные блоки</strong>.</div>` : ""}
    </div>
  `;
}

function renderQuestEditor(quest, index) {
  const base = `quests.${index}`;

  return `
    <div class="editor-grid">
      <section class="editor-card">
        <div class="panel-head">
          <div>
            <p class="eyebrow">Quest (квест)</p>
            <h2>${escapeHtml(quest.name || "Без названия")}</h2>
            <p class="muted">${escapeHtml(quest.id || "id не задан")}</p>
          </div>
          <div class="toolbar-actions">
            <button type="button" class="button ghost small" data-action="duplicate-entity" data-entity-kind="quest" data-index="${index}">Дублировать</button>
            <button type="button" class="button danger small" data-action="remove-entity" data-entity-kind="quest" data-index="${index}">Удалить</button>
          </div>
        </div>
      </section>

      <div class="section-grid">
        ${sectionCard(
          "Main (основное)",
          `
            <div class="field-grid single">
              ${textField("ID (идентификатор квеста)", `${base}.id`, quest.id, "Уникальный служебный код квеста. Не должен повторяться.", "id")}
              ${textField("Name (название для игрока)", `${base}.name`, quest.name, "Человеческое название, которое проще читать в редакторе и логике.", "name")}
              ${textareaField("Quest Description (quest.description / текст для DescriptionPanel)", `${base}.description`, quest.description, "Это описание квеста для DescriptionPanel: что это за квест, что делать, что получит игрок. Это не NPC dialog.", "description")}
              <div class="muted">Стартовый диалог, предметы при взятии, цели и награды теперь редактируются ниже в <strong>NPC Flow</strong>. Какая настройка откроется, определяет галочка роли внутри блока NPC.</div>
              <div class="muted">DescriptionPanel = <code>quest.description</code> + status/progress/objectives/rewards. DialogPanel = <code>triggerActions[].dialogText</code>. Отдельных JSON-полей <code>requiredItems</code> и <code>dialogue</code> в контракте нет.</div>
              <div class="muted">Список и порядок квестов у NPC задаются в <code>trigger.questIds</code>. Видимость в игре считается автоматически: offer до взятия, completion для активных промежуточных этапов, reward когда все Completion выполнены или Completion вообще нет.</div>
            </div>
          `
        )}

        ${sectionCard(
          "Progress & Logic (прогресс и логика)",
          `
            <div class="field-grid single">
              ${toggleField("Repeatable (можно брать повторно)", `${base}.repeatable`, quest.repeatable, "Если включено, квест можно снова получить после отката.", "repeatable")}
              ${numberField("Cooldown Seconds (откат в секундах)", `${base}.cooldownSeconds`, quest.cooldownSeconds, "Через сколько секунд квест снова станет доступен.", 1, "cooldownSeconds")}
              ${textField("Requires Previous (предыдущий обязательный квест)", `${base}.requiresPrevious`, quest.requiresPrevious, "Укажи ID квеста, который должен быть завершён раньше. Можно оставить пустым.", "requiresPrevious")}
              ${toggleField("Hide Until Requirements Complete (скрывать до выполнения требований)", `${base}.hideUntilRequirementsComplete`, quest.hideUntilRequirementsComplete, "Если включено, offer не будет показываться игроку, пока не выполнены requiresPrevious / requiredQuestIds.", "hideUntilRequirementsComplete")}
            </div>
          `
        )}
      </div>

      ${sectionCard(
        "Required Quest IDs (обязательные квесты)",
        `
          <div class="stack">
            ${stringArrayEditor(`${base}.requiredQuestIds`, quest.requiredQuestIds, "quest id (id квеста)")}
          </div>
        `
      )}

      ${sectionCard(
        "NPC Flow (роли и настройки по trigger / NPC)",
        renderQuestTriggerFlowSection(quest, index)
      )}
    </div>
  `;
}

function seedQuestRolesFromAssignments(data) {
  normalizeArray(data?.quests).forEach((quest) => {
    if (!quest) {
      return;
    }

    const assignedTriggerIds = getAssignedTriggerIdsFromData(data, quest.id);
    if (!normalizeQuestIdArray(quest.offerTriggerIds).length) {
      quest.offerTriggerIds = assignedTriggerIds.length ? [assignedTriggerIds[0]] : [];
    }

    quest.completionTriggerIds = normalizeQuestIdArray(quest.completionTriggerIds);

    if (!normalizeQuestIdArray(quest.rewardTriggerIds).length) {
      quest.rewardTriggerIds = assignedTriggerIds.length ? [assignedTriggerIds[0]] : [];
    }

    quest.offerTriggerIds = normalizeSingleTriggerRoleIds(quest.offerTriggerIds);
    quest.rewardTriggerIds = normalizeSingleTriggerRoleIds(quest.rewardTriggerIds);
  });
}

function questUsesTriggerInAnyRoleFromData(quest, triggerId) {
  const normalizedTriggerId = String(triggerId || "").trim();
  if (!quest || !normalizedTriggerId) {
    return false;
  }

  return ["offerTriggerIds", "completionTriggerIds", "rewardTriggerIds"].some((fieldName) =>
    normalizeQuestIdArray(quest[fieldName]).includes(normalizedTriggerId)
  );
}

function validateData(data) {
  const issues = [];
  const questIds = new Set();
  const triggerIds = new Set();
  const knownQuestIds = data.quests.map((quest) => quest.id).filter(Boolean);

  data.quests.forEach((quest, index) => {
    if (!quest.id.trim()) {
      issues.push(issue("error", "Квест без id", `Квест #${index + 1} должен иметь уникальный id.`));
    } else if (questIds.has(quest.id)) {
      issues.push(issue("error", "Повторяющийся id квеста", `id "${quest.id}" встречается более одного раза.`));
    } else {
      questIds.add(quest.id);
    }

    if (!quest.name.trim()) {
      issues.push(issue("warning", "Пустое название квеста", `Квест "${quest.id || `#${index + 1}`}" лучше подписать, чтобы не потеряться в списке.`));
    }

    if (quest.id.trim()) {
      const assignedTriggerIds = getAssignedTriggerIdsFromData(data, quest.id);
      if (!assignedTriggerIds.length) {
        issues.push(issue("error", "Квест без NPC в списке", `Квест "${quest.id}" должен быть добавлен хотя бы в один trigger.questIds.`));
      }
    }

    if (!normalizeQuestIdArray(quest.offerTriggerIds).length) {
      issues.push(issue("error", "Не задан offer trigger", `Квест "${quest.id || `#${index + 1}`}" должен иметь хотя бы одного NPC, где его можно взять.`));
    }
    if (normalizeQuestIdArray(quest.offerTriggerIds).length > 1) {
      issues.push(issue("error", "Слишком много offer trigger", `Квест "${quest.id || `#${index + 1}`}" должен иметь только один Offer-блок.`));
    }

    if (!normalizeQuestIdArray(quest.rewardTriggerIds).length) {
      issues.push(issue("error", "Не задан reward trigger", `Квест "${quest.id || `#${index + 1}`}" должен иметь NPC, который финально закрывает квест и выдаёт награду.`));
    }
    if (normalizeQuestIdArray(quest.rewardTriggerIds).length > 1) {
      issues.push(issue("error", "Слишком много reward trigger", `Квест "${quest.id || `#${index + 1}`}" должен иметь только один Reward-блок.`));
    }

    if (quest.requiresPrevious && !knownQuestIds.includes(quest.requiresPrevious)) {
      issues.push(issue("warning", "Ссылка на несуществующий requiresPrevious", `Квест "${quest.id}" ссылается на "${quest.requiresPrevious}", но такого квеста нет в массиве quests.`));
    }

    quest.requiredQuestIds.forEach((requiredId) => {
      if (requiredId && !knownQuestIds.includes(requiredId)) {
        issues.push(issue("warning", "Неизвестный requiredQuestId", `Квест "${quest.id}" ссылается на "${requiredId}", которого нет в quests.`));
      }
    });

    validateTriggerRoleArray(issues, data, quest, "offerTriggerIds", "Offer Trigger IDs");
    validateTriggerRoleArray(issues, data, quest, "completionTriggerIds", "Completion Trigger IDs");
    validateTriggerRoleArray(issues, data, quest, "rewardTriggerIds", "Reward Trigger IDs");

    quest.giveItems.forEach((item, itemIndex) => validateInventoryBlock(issues, quest.id, "giveItems", item, itemIndex));
    quest.rewards.forEach((item, itemIndex) => validateInventoryBlock(issues, quest.id, "rewards", item, itemIndex));

    quest.objectives.forEach((objective, objectiveIndex) => {
      if (!objective.type.trim()) {
        issues.push(issue("error", "Objective без type", `Квест "${quest.id}" содержит objective #${objectiveIndex + 1} без поля type.`));
      }
      validateInventoryBlock(issues, quest.id, "objectives", objective, objectiveIndex);
    });
  });

  data.triggers.forEach((trigger, index) => {
    if (!trigger.id.trim()) {
      issues.push(issue("error", "Триггер без id", `Триггер #${index + 1} должен иметь уникальный id.`));
    } else if (triggerIds.has(trigger.id)) {
      issues.push(issue("error", "Повторяющийся id триггера", `id "${trigger.id}" встречается более одного раза.`));
    } else {
      triggerIds.add(trigger.id);
    }

    trigger.questIds.forEach((questId) => {
      if (questId.includes(",")) {
        issues.push(issue("warning", "В questIds похоже попала строка со списком", `Триггер "${trigger.id}" содержит "${questId}". Скорее всего тут должны быть отдельные элементы массива.`));
      }

      if (questId && !knownQuestIds.includes(questId)) {
        issues.push(issue("warning", "Триггер ссылается на несуществующий квест", `Триггер "${trigger.id}" содержит questId "${questId}", которого нет в quests.`));
        return;
      }

      const quest = data.quests.find((entry) => entry.id === questId);
      if (quest && !questUsesTriggerInAnyRoleFromData(quest, trigger.id)) {
        issues.push(issue("warning", "Квест у NPC без роли", `Trigger "${trigger.id}" содержит questId "${questId}" в списке, но у этого NPC не отмечена ни выдача, ни сдача, ни награда.`));
      }
    });

    if (trigger.spawnNpc && !trigger.npcClassName.trim()) {
      issues.push(issue("warning", "NPC включён, но класс пустой", `Триггер "${trigger.id}" спавнит NPC, но поле npcClassName пустое.`));
    }
  });

  if (data.version !== 3) {
    issues.push(issue("warning", "Неожиданная версия", `Сейчас файл выставлен на version = ${data.version}. Текущий редактор ожидает version = 3.`));
  }

  return issues;
}

function validateTriggerRoleArray(issues, data, quest, fieldName, label) {
  const assignedTriggerIds = getAssignedTriggerIdsFromData(data, quest.id);
  normalizeArray(quest[fieldName]).forEach((triggerId) => {
    if (!triggerId) {
      return;
    }

    const trigger = normalizeArray(data.triggers).find((entry) => entry.id === triggerId);
    if (!trigger) {
      issues.push(issue("warning", `Неизвестный trigger в ${label}`, `Квест "${quest.id}" ссылается на trigger "${triggerId}" в ${label}, но такого trigger нет.`));
      return;
    }

    if (!assignedTriggerIds.includes(triggerId)) {
      issues.push(issue("error", `Trigger из ${label} нет в questIds`, `Квест "${quest.id}" использует trigger "${triggerId}" в ${label}, но этого квеста нет в trigger.questIds этого NPC.`));
    }
  });
}

function normalizeData(raw) {
  const data = raw && typeof raw === "object" ? raw : {};
  const sourceVersion = parseNumber(data.version, 0);
  const normalized = {
    version: sourceVersion || 3,
    quests: normalizeArray(data.quests).map(normalizeQuest),
    triggers: normalizeArray(data.triggers).map(normalizeTrigger)
  };

  seedQuestRolesFromAssignments(normalized);
  if (sourceVersion > 0 && sourceVersion < 3) {
    normalized.version = 3;
  }

  syncAllQuestTriggerActions(normalized);
  return normalized;
}
