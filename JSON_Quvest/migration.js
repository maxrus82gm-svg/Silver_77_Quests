(() => {
  "use strict";

  const WORLD_SIZE = 15360;
  const MIGRATION_API = "/api/migration/config";
  const EXPECTED_SOURCE = "Silver_77_Migrate_server/config/MigrationConfig.json";
  const GROUP_FIELD_RULES = {
    infectedCount: { min: 1, integer: true },
    spawnFormationSpacing: { min: 0 },
    spawnFormationJitter: { min: 0 },
    routePointReachRadius: { min: 0, exclusiveMin: true },
    routeActivationTriggerPercent: { min: 0, max: 100, exclusiveMin: true },
    routeActivationRadius: { min: 0, exclusiveMin: true },
    routeActivationRadii: { min: 0, exclusiveMin: true },
    targetFormationSpacing: { min: 0 },
    targetFormationJitter: { min: 0 },
    finalActivationTriggerPercent: { min: 0, max: 100, exclusiveMin: true },
    finalActivationDistance: { min: 0, exclusiveMin: true },
    finalHoldRadius: { min: 0, exclusiveMin: true },
    finalHoldReturnRadius: { min: 0, exclusiveMin: true }
  };

  const state = {
    loaded: false,
    sourceId: "",
    baseRevision: "",
    baseConfig: null,
    config: null,
    selectedGroupId: "",
    dirty: false,
    undoStack: [],
    redoStack: [],
    yConfirmed: new Map(),
    formEdit: null,
    groupFieldEdit: null,
    mapDrag: null
  };

  const elements = {};

  document.addEventListener("s77:migration-drag-start", handleMapDragStart);
  document.addEventListener("s77:migration-drag-preview", handleMapDragPreview);
  document.addEventListener("s77:migration-drag-commit", handleMapDragCommit);
  document.addEventListener("s77:migration-drag-cancel", handleMapDragCancel);
  document.addEventListener("s77:migration-projection-request", emitProjection);
  document.addEventListener("s77:migration-select-point", handleMapPointFocus);

  document.addEventListener("DOMContentLoaded", () => {
    cacheElements();
    bindControls();
    void loadMigrationConfig();
  });

  function cacheElements() {
    elements.loadStatus = document.getElementById("migrationLoadStatus");
    elements.dirtyStatus = document.getElementById("migrationDirtyStatus");
    elements.error = document.getElementById("migrationError");
    elements.groupList = document.getElementById("migrationGroupList");
    elements.sourceId = document.getElementById("migrationSourceId");
    elements.revision = document.getElementById("migrationRevision");
    elements.undoButton = document.getElementById("migrationUndoButton");
    elements.redoButton = document.getElementById("migrationRedoButton");
    elements.resetButton = document.getElementById("migrationResetButton");
    elements.groupHeading = document.getElementById("migrationGroupHeading");
    elements.infected = document.getElementById("migrationInfected");
    elements.points = document.getElementById("migrationPoints");
    elements.radiiInfo = document.getElementById("migrationRadiiInfo");
    elements.radiiContent = document.getElementById("migrationRadiiContent");
  }

  function bindControls() {
    elements.undoButton.addEventListener("click", undo);
    elements.redoButton.addEventListener("click", redo);
    elements.resetButton.addEventListener("click", resetChanges);
  }

  async function loadMigrationConfig() {
    setLoadStatus("Загрузка DEV-конфига…", "");
    clearError();

    try {
      const response = await fetch(MIGRATION_API, { cache: "no-store" });
      if (!response.ok) {
        throw new Error("HTTP " + response.status + " " + response.statusText);
      }

      const sourceId = response.headers.get("X-S77-Source-Id") || "";
      const revision = response.headers.get("X-S77-Revision-SHA256") || "";
      if (sourceId !== EXPECTED_SOURCE) {
        throw new Error("Неожиданный Migration source identity: " + (sourceId || "отсутствует"));
      }
      if (!/^[a-f0-9]{64}$/i.test(revision)) {
        throw new Error("Migration source revision отсутствует или имеет неверный формат.");
      }

      const bytes = await response.arrayBuffer();
      const text = new TextDecoder("utf-8", { fatal: true }).decode(bytes);
      let config;
      try {
        config = JSON.parse(text);
      } catch (error) {
        throw new Error("MigrationConfig содержит malformed JSON: " + error.message);
      }

      validateConfig(config);

      state.sourceId = sourceId;
      state.baseRevision = revision.toLowerCase();
      state.baseConfig = cloneJson(config);
      state.config = cloneJson(config);
      state.selectedGroupId = config.groups.length > 0 ? config.groups[0].groupId : "";
      state.undoStack = [];
      state.redoStack = [];
      state.yConfirmed = createBaselineConfirmation(config);
      state.loaded = true;
      recomputeDirty();

      renderGroupList();
      renderEditor();
      setLoadStatus("DEV-конфиг загружен · " + config.groups.length + " GROUP", "");
      emitProjection();
    } catch (error) {
      state.loaded = false;
      state.baseConfig = null;
      state.config = null;
      state.selectedGroupId = "";
      state.undoStack = [];
      state.redoStack = [];
      state.yConfirmed = new Map();
      showError(error.message);
      setLoadStatus("Migration недоступна", "error");
      renderGroupList();
      renderEditor();
      emitProjection();
    }
  }

  function validateConfig(config) {
    if (!config || typeof config !== "object" || Array.isArray(config)) {
      throw new Error("MigrationConfig должен быть JSON-объектом.");
    }
    if (!Array.isArray(config.groups)) {
      throw new Error("MigrationConfig не содержит обязательный массив groups[].");
    }

    const ids = new Set();
    config.groups.forEach((group, groupIndex) => {
      if (!group || typeof group !== "object" || Array.isArray(group)) {
        throw new Error("groups[" + groupIndex + "] должен быть объектом.");
      }
      const groupId = typeof group.groupId === "string" ? group.groupId.trim() : "";
      if (!groupId) {
        throw new Error("groups[" + groupIndex + "].groupId отсутствует.");
      }
      if (ids.has(groupId)) {
        throw new Error("Обнаружен duplicate groupId: " + groupId);
      }
      ids.add(groupId);

      validateVector(group.spawnPosition, groupId + ".spawnPosition");
      validateVector(group.targetPosition, groupId + ".targetPosition");
      if (!Array.isArray(group.routePoints)) {
        throw new Error(groupId + ".routePoints должен быть массивом.");
      }
      group.routePoints.forEach((point, routeIndex) => {
        validateVector(point, groupId + ".routePoints[" + routeIndex + "]");
      });
      if (!Number.isInteger(group.infectedCount) || group.infectedCount < 1) {
        throw new Error(groupId + ".infectedCount должен быть целым числом не меньше 1.");
      }
      if (!Array.isArray(group.infectedTypes) || group.infectedTypes.length === 0) {
        throw new Error(groupId + ".infectedTypes должен содержать хотя бы один classname.");
      }
      group.infectedTypes.forEach((className, infectedIndex) => {
        if (!isClassName(className)) {
          throw new Error(groupId + ".infectedTypes[" + infectedIndex + "] содержит неверный classname.");
        }
      });
      validateGroupNumber(group, "spawnFormationSpacing", 0, null, groupId);
      validateGroupNumber(group, "spawnFormationJitter", 0, null, groupId);
      validateGroupNumber(group, "routePointReachRadius", 0, null, groupId, false);
      validateGroupNumber(group, "routeActivationTriggerPercent", 0, 100, groupId, false);
      validateGroupNumber(group, "routeActivationRadius", 0, null, groupId, false);
      if (!Array.isArray(group.routeActivationRadii) || group.routeActivationRadii.length < group.routePoints.length) {
        throw new Error(groupId + ".routeActivationRadii должен содержать radius для каждой route point.");
      }
      group.routePoints.forEach((unused, routeIndex) => {
        validateFiniteNumber(group.routeActivationRadii[routeIndex], groupId + ".routeActivationRadii[" + routeIndex + "]", 0, null, false);
      });
      validateGroupNumber(group, "targetFormationSpacing", 0, null, groupId);
      validateGroupNumber(group, "targetFormationJitter", 0, null, groupId);
      validateGroupNumber(group, "finalActivationTriggerPercent", 0, 100, groupId, false);
      validateGroupNumber(group, "finalActivationDistance", 0, null, groupId, false);
      validateGroupNumber(group, "finalHoldRadius", 0, null, groupId, false);
      validateGroupNumber(group, "finalHoldReturnRadius", 0, null, groupId, false);
      validateToggle(group.routeActivationEnabled, groupId + ".routeActivationEnabled");
      validateToggle(group.finalActivationEnabled, groupId + ".finalActivationEnabled");
      validateToggle(group.finalHoldEnabled, groupId + ".finalHoldEnabled");
    });
  }

  function validateGroupNumber(group, field, min, max, groupId, inclusiveMin = true) {
    validateFiniteNumber(group[field], groupId + "." + field, min, max, inclusiveMin);
  }

  function validateFiniteNumber(value, path, min, max, inclusiveMin) {
    const minValid = inclusiveMin ? value >= min : value > min;
    const maxValid = max === null || value <= max;
    if (typeof value !== "number" || !Number.isFinite(value) || !minValid || !maxValid) {
      throw new Error(path + " содержит недопустимое число.");
    }
  }

  function validateToggle(value, path) {
    if (value !== 0 && value !== 1 && value !== false && value !== true) {
      throw new Error(path + " должен быть 0/1 или boolean.");
    }
  }

  function validateVector(vector, path) {
    if (!Array.isArray(vector) || vector.length !== 3) {
      throw new Error(path + " должен быть вектором [X, Y, Z].");
    }
    vector.forEach((value, axisIndex) => {
      if (typeof value !== "number" || !Number.isFinite(value)) {
        throw new Error(path + "[" + axisIndex + "] должен быть finite number.");
      }
    });
    if (!isWorldCoordinate(vector[0]) || !isWorldCoordinate(vector[2])) {
      throw new Error(path + " выходит за границы ChernarusPlus 0.." + WORLD_SIZE + ".");
    }
  }

  function createBaselineConfirmation(config) {
    const result = new Map();
    config.groups.forEach((group) => {
      listPointRefs(group).forEach((ref) => result.set(pointKey(ref), true));
    });
    return result;
  }

  function renderGroupList() {
    elements.groupList.replaceChildren();
    if (!state.loaded || !state.config) {
      elements.groupList.appendChild(createTextElement("p", "GROUP недоступны", "migration-group-empty"));
      elements.groupList.setAttribute("aria-disabled", "true");
      return;
    }

    state.config.groups.forEach((group) => {
      const option = document.createElement("button");
      const selected = group.groupId === state.selectedGroupId;
      option.type = "button";
      option.className = "migration-group-option";
      option.dataset.groupId = group.groupId;
      option.setAttribute("role", "option");
      option.setAttribute("aria-selected", selected ? "true" : "false");
      option.classList.toggle("selected", selected);
      option.title = group.name ? group.groupId + " — " + group.name : group.groupId;
      option.appendChild(createTextElement("strong", group.groupId));
      if (group.name) {
        option.appendChild(createTextElement("span", group.name));
      }
      option.addEventListener("click", () => selectGroup(group.groupId));
      elements.groupList.appendChild(option);
    });
    elements.groupList.setAttribute("aria-disabled", state.config.groups.length === 0 ? "true" : "false");
  }

  function selectGroup(groupId) {
    if (!getGroup(groupId) || groupId === state.selectedGroupId) {
      return;
    }
    cancelActiveEdits();
    state.selectedGroupId = groupId;
    renderGroupList();
    renderEditor();
    emitProjection();
  }

  function renderEditor() {
    updateMetaAndActions();
    elements.infected.replaceChildren();
    elements.points.replaceChildren();
    elements.radiiInfo.hidden = true;
    elements.radiiContent.replaceChildren();

    const group = getSelectedGroup();
    if (!group) {
      elements.groupHeading.replaceChildren(
        createTextElement("h2", state.loaded ? "GROUP не выбрана" : "Migration недоступна"),
        createTextElement("p", state.loaded ? "В конфиге нет доступных GROUP." : "Проверьте сообщение об ошибке загрузки.")
      );
      return;
    }

    elements.groupHeading.replaceChildren(
      createTextElement("h2", group.name || group.groupId),
      createTextElement("p", group.groupId + " · редактируется одна существующая GROUP")
    );

    renderInfectedEditor(group);
    listPointRefs(group).forEach((ref) => {
      elements.points.appendChild(createPointCard(ref));
    });
    renderRadiiInfo(group);
  }

  function renderInfectedEditor(group) {
    const section = document.createElement("section");
    section.className = "migration-infected-card";

    const heading = document.createElement("div");
    heading.className = "migration-section-heading";
    heading.append(
      createTextElement("h3", "Заражённые"),
      createTextElement("span", "infectedCount / infectedTypes[]", "migration-section-key")
    );

    const countRow = document.createElement("div");
    countRow.className = "migration-infected-count";
    countRow.appendChild(createGroupNumberField(group, "infectedCount", "Количество", { step: "1" }));

    const types = document.createElement("div");
    types.className = "migration-infected-types";
    group.infectedTypes.forEach((className, index) => {
      const row = document.createElement("div");
      row.className = "migration-infected-type-row";

      const label = document.createElement("label");
      label.appendChild(createTextElement("span", "Classname " + (index + 1)));
      const input = createGroupFieldInput(group, "infectedTypes", {
        inputKind: "classname",
        index: index,
        value: className
      });
      input.placeholder = "Zmb... classname";
      label.appendChild(input);

      const remove = document.createElement("button");
      remove.type = "button";
      remove.className = "button ghost small migration-remove-infected";
      remove.textContent = "Удалить";
      remove.disabled = group.infectedTypes.length <= 1;
      remove.addEventListener("click", () => removeInfectedType(group.groupId, index));
      row.append(label, remove);
      types.appendChild(row);
    });

    const add = document.createElement("button");
    add.type = "button";
    add.className = "button ghost small migration-add-infected";
    add.textContent = "+ Добавить classname";
    add.addEventListener("click", () => addInfectedType(group.groupId));

    section.append(heading, countRow, types, add);
    elements.infected.appendChild(section);
  }

  function createPointCard(ref) {
    const point = getPoint(ref);
    const group = getGroup(ref.groupId);
    const card = document.createElement("section");
    const routeTone = ref.kind === "route" ? (ref.index % 2 === 0 ? " route-odd" : " route-even") : "";
    card.className = "migration-point-card " + ref.kind + routeTone;
    card.dataset.pointKey = pointKey(ref);

    const title = document.createElement("div");
    title.className = "migration-point-title";
    title.append(
      createTextElement("h3", pointTitle(ref)),
      createTextElement("span", ref.groupId + " / " + pointAddress(ref), "migration-point-address")
    );

    const grid = document.createElement("div");
    grid.className = "migration-coordinate-grid";
    ["x", "y", "z"].forEach((axis, axisIndex) => {
      const label = document.createElement("label");
      label.className = "migration-coordinate";
      label.appendChild(document.createTextNode(axis.toUpperCase()));
      const input = document.createElement("input");
      input.type = "number";
      input.step = "any";
      input.value = String(point[axisIndex]);
      input.dataset.axis = axis;
      input.dataset.groupId = ref.groupId;
      input.dataset.kind = ref.kind;
      input.dataset.index = ref.index === null ? "" : String(ref.index);
      input.addEventListener("focus", beginFormEdit);
      input.addEventListener("input", previewFormEdit);
      input.addEventListener("change", commitFormEdit);
      input.addEventListener("blur", finishFormEdit);
      label.appendChild(input);
      grid.appendChild(label);
    });

    const yStatus = document.createElement("div");
    yStatus.className = "migration-y-status";
    updateYStatus(yStatus, ref);

    card.append(title, grid, yStatus, createSpatialParameters(group, ref));
    return card;
  }

  function createSpatialParameters(group, ref) {
    const wrapper = document.createElement("div");
    wrapper.className = "migration-point-parameters";

    if (ref.kind === "spawn") {
      wrapper.append(
        createGroupNumberField(group, "spawnFormationSpacing", "Шаг построения", { step: "any" }),
        createGroupNumberField(group, "spawnFormationJitter", "Разброс построения", { step: "any" })
      );
    }

    if (ref.kind === "route") {
      if (ref.index === 0) {
        const shared = document.createElement("div");
        shared.className = "migration-route-shared";
        shared.append(
          createTextElement("strong", "Общие настройки маршрута"),
          createGroupNumberField(group, "routePointReachRadius", "Радиус достижения точки", { step: "any" }),
          createGroupToggleField(group, "routeActivationEnabled", "Активация у route points"),
          createGroupNumberField(group, "routeActivationTriggerPercent", "Шанс активации, %", { step: "any" }),
          createGroupNumberField(group, "routeActivationRadius", "Fallback-радиус активации", { step: "any" })
        );
        wrapper.appendChild(shared);
      }
      wrapper.appendChild(createGroupNumberField(
        group,
        "routeActivationRadii",
        "Радиус активации Route " + (ref.index + 1),
        { step: "any", index: ref.index }
      ));
    }

    if (ref.kind === "target") {
      wrapper.append(
        createGroupNumberField(group, "targetFormationSpacing", "Шаг построения у цели", { step: "any" }),
        createGroupNumberField(group, "targetFormationJitter", "Разброс построения у цели", { step: "any" }),
        createGroupToggleField(group, "finalActivationEnabled", "Финальная активация"),
        createGroupNumberField(group, "finalActivationTriggerPercent", "Шанс финальной активации, %", { step: "any" }),
        createGroupNumberField(group, "finalActivationDistance", "Дистанция финальной активации", { step: "any" }),
        createGroupToggleField(group, "finalHoldEnabled", "Удержание у цели"),
        createGroupNumberField(group, "finalHoldRadius", "Радиус удержания", { step: "any" }),
        createGroupNumberField(group, "finalHoldReturnRadius", "Радиус возврата", { step: "any" })
      );
    }

    return wrapper;
  }

  function createGroupNumberField(group, field, labelText, options) {
    const label = document.createElement("label");
    label.className = "migration-parameter-field";
    const caption = document.createElement("span");
    caption.append(
      document.createTextNode(labelText),
      createTextElement("code", field + (Number.isInteger(options.index) ? "[" + options.index + "]" : ""))
    );
    label.append(
      caption,
      createGroupFieldInput(group, field, {
        inputKind: "number",
        index: Number.isInteger(options.index) ? options.index : null,
        step: options.step,
        value: getGroupFieldValue(group, field, options.index)
      })
    );
    return label;
  }

  function createGroupToggleField(group, field, labelText) {
    const label = document.createElement("label");
    label.className = "migration-parameter-toggle";
    const input = document.createElement("input");
    input.type = "checkbox";
    input.checked = isEnabled(group[field]);
    input.addEventListener("change", () => commitGroupToggle(group.groupId, field, input.checked));
    label.append(
      input,
      createTextElement("span", labelText),
      createTextElement("code", field)
    );
    return label;
  }

  function createGroupFieldInput(group, field, options) {
    const input = document.createElement("input");
    input.type = options.inputKind === "number" ? "number" : "text";
    if (options.step) {
      input.step = options.step;
    }
    input.value = String(options.value);
    input.dataset.groupId = group.groupId;
    input.dataset.editorField = field;
    input.dataset.inputKind = options.inputKind;
    input.dataset.index = Number.isInteger(options.index) ? String(options.index) : "";
    input.addEventListener("focus", beginGroupFieldEdit);
    input.addEventListener("input", previewGroupFieldEdit);
    input.addEventListener("change", commitGroupFieldEdit);
    input.addEventListener("blur", finishGroupFieldEdit);
    return input;
  }

  function beginGroupFieldEdit(event) {
    const input = event.currentTarget;
    const groupId = input.dataset.groupId;
    const group = getGroup(groupId);
    if (!group) {
      return;
    }
    state.groupFieldEdit = {
      input: input,
      groupId: groupId,
      field: input.dataset.editorField,
      inputKind: input.dataset.inputKind,
      index: input.dataset.index === "" ? null : Number(input.dataset.index),
      beforeGroup: cloneJson(group),
      invalid: false
    };
  }

  function previewGroupFieldEdit(event) {
    const input = event.currentTarget;
    if (!state.groupFieldEdit || state.groupFieldEdit.input !== input) {
      beginGroupFieldEdit(event);
    }
    const edit = state.groupFieldEdit;
    if (!edit) {
      return;
    }

    let value;
    let valid;
    if (edit.inputKind === "classname") {
      value = input.value.trim();
      valid = isClassName(value);
    } else {
      value = Number(input.value);
      valid = input.value.trim() !== "" && isValidGroupFieldValue(edit.field, value);
    }

    input.classList.toggle("invalid", !valid);
    input.setAttribute("aria-invalid", valid ? "false" : "true");
    input.title = valid ? "" : groupFieldValidationMessage(edit.field, edit.inputKind);
    edit.invalid = !valid;
    if (!valid) {
      return;
    }

    setGroupFieldValue(getGroup(edit.groupId), edit.field, edit.index, value);
    recomputeDirty();
    updateMetaAndActions();
    emitProjection();
  }

  function commitGroupFieldEdit(event) {
    finishGroupFieldEdit(event);
  }

  function finishGroupFieldEdit(event) {
    if (!state.groupFieldEdit || state.groupFieldEdit.input !== event.currentTarget) {
      return;
    }
    const edit = state.groupFieldEdit;
    if (edit.invalid) {
      replaceGroupSnapshot(edit.groupId, edit.beforeGroup);
      state.groupFieldEdit = null;
      recomputeDirty();
      renderEditor();
      emitProjection();
      return;
    } else {
      const group = getGroup(edit.groupId);
      if (edit.inputKind === "classname") {
        setGroupFieldValue(group, edit.field, edit.index, event.currentTarget.value.trim());
      }
      finalizeGroupHistoryOperation(edit.groupId, edit.beforeGroup, cloneJson(group));
    }
    state.groupFieldEdit = null;
    recomputeDirty();
    elements.radiiContent.replaceChildren();
    renderRadiiInfo(getSelectedGroup());
    emitProjection();
  }

  function commitGroupToggle(groupId, field, checked) {
    cancelActiveEdits();
    const group = getGroup(groupId);
    if (!group) {
      return;
    }
    const beforeGroup = cloneJson(group);
    group[field] = checked ? 1 : 0;
    finalizeGroupHistoryOperation(groupId, beforeGroup, cloneJson(group));
    renderEditor();
    emitProjection();
  }

  function addInfectedType(groupId) {
    cancelActiveEdits();
    const group = getGroup(groupId);
    if (!group || !Array.isArray(group.infectedTypes) || group.infectedTypes.length === 0) {
      return;
    }
    const beforeGroup = cloneJson(group);
    group.infectedTypes.push(group.infectedTypes[group.infectedTypes.length - 1]);
    finalizeGroupHistoryOperation(groupId, beforeGroup, cloneJson(group));
    renderEditor();
    emitProjection();
    const inputs = elements.infected.querySelectorAll('[data-editor-field="infectedTypes"]');
    const input = inputs[inputs.length - 1];
    if (input) {
      input.focus();
      input.select();
    }
  }

  function removeInfectedType(groupId, index) {
    cancelActiveEdits();
    const group = getGroup(groupId);
    if (!group || !Array.isArray(group.infectedTypes) || group.infectedTypes.length <= 1 || !Number.isInteger(index)) {
      return;
    }
    const beforeGroup = cloneJson(group);
    group.infectedTypes.splice(index, 1);
    finalizeGroupHistoryOperation(groupId, beforeGroup, cloneJson(group));
    renderEditor();
    emitProjection();
  }

  function getGroupFieldValue(group, field, index) {
    return Number.isInteger(index) ? group[field][index] : group[field];
  }

  function setGroupFieldValue(group, field, index, value) {
    if (!group) {
      return;
    }
    if (Number.isInteger(index)) {
      group[field][index] = value;
    } else {
      group[field] = value;
    }
  }

  function isValidGroupFieldValue(field, value) {
    const rule = GROUP_FIELD_RULES[field];
    if (!rule || !Number.isFinite(value) || (rule.integer && !Number.isInteger(value))) {
      return false;
    }
    if (rule.exclusiveMin ? value <= rule.min : value < rule.min) {
      return false;
    }
    return rule.max === undefined || value <= rule.max;
  }

  function groupFieldValidationMessage(field, inputKind) {
    if (inputKind === "classname") {
      return "Classname должен быть непустой строкой из латинских букв, цифр и подчёркиваний.";
    }
    const rule = GROUP_FIELD_RULES[field];
    if (!rule) {
      return "Введите допустимое число.";
    }
    const lower = rule.exclusiveMin ? "больше " + rule.min : "не меньше " + rule.min;
    const upper = rule.max === undefined ? "" : " и не больше " + rule.max;
    return "Значение должно быть " + lower + upper + (rule.integer ? ", целым." : ".");
  }

  function updateYStatus(container, ref) {
    const confirmed = isYConfirmed(ref);
    container.classList.toggle("confirmed", confirmed);
    container.replaceChildren();
    container.appendChild(createTextElement(
      "span",
      confirmed ? "Y подтверждён" : "Y требует подтверждения после перемещения X/Z"
    ));
    const button = document.createElement("button");
    button.type = "button";
    button.className = "button ghost small";
    button.textContent = "Подтвердить Y";
    button.disabled = confirmed;
    button.addEventListener("click", () => confirmY(ref));
    container.appendChild(button);
  }

  function renderRadiiInfo(group) {
    const list = document.createElement("ul");
    list.className = "migration-radii-list";
    addListItem(list, "Route point reach: " + formatRadius(group.routePointReachRadius));
    addListItem(
      list,
      "Route activation: " + enabledLabel(group.routeActivationEnabled) + "; per-point radii[] с fallback " + formatRadius(group.routeActivationRadius)
    );
    addListItem(
      list,
      "Final activation: " + enabledLabel(group.finalActivationEnabled) + "; distance " + formatRadius(group.finalActivationDistance)
    );
    addListItem(
      list,
      "Final hold: " + enabledLabel(group.finalHoldEnabled) + "; radius " + formatRadius(group.finalHoldRadius)
    );
    addListItem(
      list,
      "Final hold return: " + formatRadius(group.finalHoldReturnRadius) + " — только текст; не рисуется общим кругом"
    );
    addListItem(list, "Spawn formation spacing не является радиусом и на карте не рисуется.");
    elements.radiiContent.appendChild(list);
    elements.radiiInfo.hidden = false;
  }

  function beginFormEdit(event) {
    const ref = refFromDataset(event.currentTarget.dataset);
    state.formEdit = {
      input: event.currentTarget,
      ref: ref,
      axis: event.currentTarget.dataset.axis,
      before: getPointSnapshot(ref),
      invalid: false
    };
  }

  function previewFormEdit(event) {
    const input = event.currentTarget;
    if (!state.formEdit || state.formEdit.input !== input) {
      beginFormEdit(event);
    }
    const axis = input.dataset.axis;
    const value = Number(input.value);
    const valid = input.value.trim() !== "" && Number.isFinite(value) && (axis === "y" || isWorldCoordinate(value));
    input.classList.toggle("invalid", !valid);
    input.setAttribute("aria-invalid", valid ? "false" : "true");
    state.formEdit.invalid = !valid;
    if (!valid) {
      input.title = axis === "y" ? "Y должен быть finite number" : "X/Z должны быть finite number в пределах 0..15360";
      return;
    }

    input.title = "";
    const ref = state.formEdit.ref;
    const point = getPoint(ref);
    const axisIndex = axisToIndex(axis);
    if (Object.is(point[axisIndex], value)) {
      return;
    }
    point[axisIndex] = value;
    if (axis === "x" || axis === "z") {
      state.yConfirmed.set(pointKey(ref), false);
      refreshPointYStatus(ref);
    }
    recomputeDirty();
    updateMetaAndActions();
    emitProjection();
  }

  function commitFormEdit(event) {
    if (!state.formEdit || state.formEdit.input !== event.currentTarget) {
      return;
    }
    if (state.formEdit.invalid) {
      restoreInvalidInput(state.formEdit);
      return;
    }
    finalizeHistoryOperation(state.formEdit.ref, state.formEdit.before, getPointSnapshot(state.formEdit.ref));
    state.formEdit = null;
    renderEditor();
    emitProjection();
  }

  function finishFormEdit(event) {
    if (!state.formEdit || state.formEdit.input !== event.currentTarget) {
      return;
    }
    if (state.formEdit.invalid) {
      restoreInvalidInput(state.formEdit);
      return;
    }
    finalizeHistoryOperation(state.formEdit.ref, state.formEdit.before, getPointSnapshot(state.formEdit.ref));
    state.formEdit = null;
    renderEditor();
    emitProjection();
  }

  function restoreInvalidInput(edit) {
    applyPointSnapshot(edit.ref, edit.before);
    state.formEdit = null;
    recomputeDirty();
    renderEditor();
    emitProjection();
  }

  function confirmY(ref) {
    cancelActiveEdits();
    const before = getPointSnapshot(ref);
    state.yConfirmed.set(pointKey(ref), true);
    const after = getPointSnapshot(ref);
    finalizeHistoryOperation(ref, before, after);
    renderEditor();
    emitProjection();
  }

  function handleMapDragStart(event) {
    if (!state.loaded || state.mapDrag) {
      return;
    }
    const ref = normalizeEventRef(event.detail && event.detail.ref);
    if (!ref || !getPoint(ref)) {
      return;
    }
    cancelFormEditWithoutCommit();
    cancelGroupFieldEditWithoutCommit();
    state.mapDrag = { ref: ref, before: getPointSnapshot(ref) };
  }

  function handleMapDragPreview(event) {
    if (!state.mapDrag) {
      return;
    }
    const ref = normalizeEventRef(event.detail && event.detail.ref);
    const x = Number(event.detail && event.detail.x);
    const z = Number(event.detail && event.detail.z);
    if (!sameRef(ref, state.mapDrag.ref) || !isWorldCoordinate(x) || !isWorldCoordinate(z)) {
      return;
    }
    const point = getPoint(ref);
    point[0] = x;
    point[2] = z;
    state.yConfirmed.set(pointKey(ref), false);
    recomputeDirty();
    renderEditor();
    emitProjection();
  }

  function handleMapDragCommit(event) {
    if (!state.mapDrag) {
      return;
    }
    const ref = normalizeEventRef(event.detail && event.detail.ref);
    if (!sameRef(ref, state.mapDrag.ref)) {
      return;
    }
    const drag = state.mapDrag;
    state.mapDrag = null;
    finalizeHistoryOperation(drag.ref, drag.before, getPointSnapshot(drag.ref));
    renderEditor();
    emitProjection();
  }

  function handleMapDragCancel(event) {
    if (!state.mapDrag) {
      return;
    }
    const ref = normalizeEventRef(event.detail && event.detail.ref);
    if (ref && !sameRef(ref, state.mapDrag.ref)) {
      return;
    }
    const drag = state.mapDrag;
    state.mapDrag = null;
    applyPointSnapshot(drag.ref, drag.before);
    recomputeDirty();
    renderEditor();
    emitProjection();
  }

  function handleMapPointFocus(event) {
    const ref = normalizeEventRef(event.detail && event.detail.ref);
    if (!ref || ref.groupId !== state.selectedGroupId) {
      return;
    }
    const card = Array.from(elements.points.querySelectorAll(".migration-point-card"))
      .find((entry) => entry.dataset.pointKey === pointKey(ref));
    if (card) {
      card.scrollIntoView({ block: "nearest", behavior: "smooth" });
    }
  }

  function finalizeHistoryOperation(ref, before, after) {
    if (snapshotsEqual(before, after)) {
      recomputeDirty();
      updateMetaAndActions();
      return;
    }
    state.undoStack.push({ ref: cloneRef(ref), before: before, after: after });
    state.redoStack = [];
    recomputeDirty();
    updateMetaAndActions();
  }

  function finalizeGroupHistoryOperation(groupId, beforeGroup, afterGroup) {
    if (JSON.stringify(beforeGroup) === JSON.stringify(afterGroup)) {
      recomputeDirty();
      updateMetaAndActions();
      return;
    }
    state.undoStack.push({ groupId: groupId, beforeGroup: beforeGroup, afterGroup: afterGroup });
    state.redoStack = [];
    recomputeDirty();
    updateMetaAndActions();
  }

  function undo() {
    cancelActiveEdits();
    const operation = state.undoStack.pop();
    if (!operation) {
      return;
    }
    applyHistoryOperation(operation, "before");
    state.redoStack.push(operation);
    state.selectedGroupId = historyGroupId(operation);
    recomputeDirty();
    renderGroupList();
    renderEditor();
    emitProjection();
  }

  function redo() {
    cancelActiveEdits();
    const operation = state.redoStack.pop();
    if (!operation) {
      return;
    }
    applyHistoryOperation(operation, "after");
    state.undoStack.push(operation);
    state.selectedGroupId = historyGroupId(operation);
    recomputeDirty();
    renderGroupList();
    renderEditor();
    emitProjection();
  }

  function resetChanges() {
    cancelActiveEdits();
    if (!state.loaded || !state.baseConfig) {
      return;
    }
    state.config = cloneJson(state.baseConfig);
    state.yConfirmed = createBaselineConfirmation(state.config);
    state.undoStack = [];
    state.redoStack = [];
    if (!state.config.groups.some((group) => group.groupId === state.selectedGroupId)) {
      state.selectedGroupId = state.config.groups.length > 0 ? state.config.groups[0].groupId : "";
    }
    recomputeDirty();
    renderGroupList();
    renderEditor();
    emitProjection();
  }

  function cancelActiveEdits() {
    if (state.mapDrag) {
      const drag = state.mapDrag;
      state.mapDrag = null;
      applyPointSnapshot(drag.ref, drag.before);
    }
    cancelFormEditWithoutCommit();
    cancelGroupFieldEditWithoutCommit();
    recomputeDirty();
  }

  function cancelFormEditWithoutCommit() {
    if (!state.formEdit) {
      return;
    }
    applyPointSnapshot(state.formEdit.ref, state.formEdit.before);
    state.formEdit = null;
  }

  function cancelGroupFieldEditWithoutCommit() {
    if (!state.groupFieldEdit) {
      return;
    }
    replaceGroupSnapshot(state.groupFieldEdit.groupId, state.groupFieldEdit.beforeGroup);
    state.groupFieldEdit = null;
  }

  function applyHistoryOperation(operation, side) {
    if (operation.ref) {
      applyPointSnapshot(operation.ref, operation[side]);
      return;
    }
    replaceGroupSnapshot(operation.groupId, operation[side + "Group"]);
  }

  function historyGroupId(operation) {
    return operation.ref ? operation.ref.groupId : operation.groupId;
  }

  function emitProjection() {
    document.dispatchEvent(new CustomEvent("s77:migration-projection", {
      detail: buildProjection()
    }));
  }

  function buildProjection() {
    if (!state.loaded || !state.config) {
      return {
        available: false,
        sourceId: state.sourceId || EXPECTED_SOURCE,
        error: elements.error && !elements.error.hidden ? elements.error.textContent : "Migration не загружена"
      };
    }

    const group = getSelectedGroup();
    if (!group) {
      return {
        available: true,
        sourceId: state.sourceId,
        revision: state.baseRevision,
        dirty: state.dirty,
        group: null,
        points: []
      };
    }

    const points = listPointRefs(group).map((ref) => {
      const vector = getPoint(ref);
      return {
        id: featureId(ref),
        ref: cloneRef(ref),
        address: pointAddress(ref),
        label: pointMarkerLabel(ref),
        title: pointTitle(ref),
        x: vector[0],
        y: vector[1],
        z: vector[2],
        yConfirmed: isYConfirmed(ref),
        dirty: pointDiffersFromBaseline(ref),
        radii: getPointRadii(group, ref)
      };
    });

    return {
      available: true,
      sourceId: state.sourceId,
      revision: state.baseRevision,
      dirty: state.dirty,
      group: {
        groupId: group.groupId,
        name: group.name || "",
        finalHoldReturnRadius: finitePositive(group.finalHoldReturnRadius) ? group.finalHoldReturnRadius : null
      },
      points: points
    };
  }

  function getPointRadii(group, ref) {
    const radii = [];
    if (ref.kind === "route") {
      pushRadius(radii, "Route reach", group.routePointReachRadius, true, "reach");
      const configured = Array.isArray(group.routeActivationRadii) ? group.routeActivationRadii[ref.index] : null;
      const activationRadius = finitePositive(configured) ? configured : group.routeActivationRadius;
      pushRadius(radii, "Route activation", activationRadius, isEnabled(group.routeActivationEnabled), "activation");
    }
    if (ref.kind === "target") {
      pushRadius(radii, "Final activation", group.finalActivationDistance, isEnabled(group.finalActivationEnabled), "activation");
      pushRadius(radii, "Final hold", group.finalHoldRadius, isEnabled(group.finalHoldEnabled), "hold");
    }
    return radii;
  }

  function pushRadius(target, role, value, enabled, style) {
    if (!finitePositive(value)) {
      return;
    }
    target.push({ role: role, radius: Number(value), enabled: enabled, style: style });
  }

  function updateMetaAndActions() {
    elements.sourceId.textContent = state.sourceId || "—";
    elements.revision.textContent = state.baseRevision || "—";
    elements.dirtyStatus.textContent = state.dirty
      ? "Есть несохранённые изменения — только в памяти"
      : "Без изменений · загруженный baseline";
    elements.undoButton.disabled = state.undoStack.length === 0;
    elements.redoButton.disabled = state.redoStack.length === 0;
    elements.resetButton.disabled = !state.loaded || !state.dirty;
  }

  function recomputeDirty() {
    syncBaselineConfirmations();
    state.dirty = Boolean(state.baseConfig && state.config && JSON.stringify(state.config) !== JSON.stringify(state.baseConfig));
  }

  function syncBaselineConfirmations() {
    if (!state.config || !state.baseConfig) {
      return;
    }
    state.config.groups.forEach((group) => {
      listPointRefs(group).forEach((ref) => {
        if (!pointDiffersFromBaseline(ref)) {
          state.yConfirmed.set(pointKey(ref), true);
        }
      });
    });
  }

  function refreshPointYStatus(ref) {
    const card = Array.from(elements.points.querySelectorAll(".migration-point-card"))
      .find((entry) => entry.dataset.pointKey === pointKey(ref));
    const status = card && card.querySelector(".migration-y-status");
    if (status) {
      updateYStatus(status, ref);
    }
  }

  function getSelectedGroup() {
    if (!state.config) {
      return null;
    }
    return state.config.groups.find((group) => group.groupId === state.selectedGroupId) || null;
  }

  function getGroup(groupId, config = state.config) {
    return config && Array.isArray(config.groups)
      ? config.groups.find((group) => group.groupId === groupId) || null
      : null;
  }

  function getPoint(ref, config = state.config) {
    const group = getGroup(ref.groupId, config);
    if (!group) {
      return null;
    }
    if (ref.kind === "spawn") {
      return group.spawnPosition;
    }
    if (ref.kind === "target") {
      return group.targetPosition;
    }
    if (ref.kind === "route" && Number.isInteger(ref.index)) {
      return group.routePoints[ref.index] || null;
    }
    return null;
  }

  function listPointRefs(group) {
    const refs = [{ groupId: group.groupId, kind: "spawn", index: null }];
    group.routePoints.forEach((unused, index) => {
      refs.push({ groupId: group.groupId, kind: "route", index: index });
    });
    refs.push({ groupId: group.groupId, kind: "target", index: null });
    return refs;
  }

  function getPointSnapshot(ref) {
    const point = getPoint(ref);
    return {
      coordinates: point.slice(0, 3),
      yConfirmed: isYConfirmed(ref)
    };
  }

  function applyPointSnapshot(ref, snapshot) {
    const point = getPoint(ref);
    if (!point || !snapshot) {
      return;
    }
    point[0] = snapshot.coordinates[0];
    point[1] = snapshot.coordinates[1];
    point[2] = snapshot.coordinates[2];
    state.yConfirmed.set(pointKey(ref), Boolean(snapshot.yConfirmed));
  }

  function replaceGroupSnapshot(groupId, snapshot) {
    if (!state.config || !snapshot) {
      return;
    }
    const index = state.config.groups.findIndex((group) => group.groupId === groupId);
    if (index >= 0) {
      state.config.groups[index] = cloneJson(snapshot);
    }
  }

  function pointDiffersFromBaseline(ref) {
    const current = getPoint(ref, state.config);
    const baseline = getPoint(ref, state.baseConfig);
    return Boolean(current && baseline && current.some((value, index) => !Object.is(value, baseline[index])));
  }

  function isYConfirmed(ref) {
    return state.yConfirmed.get(pointKey(ref)) !== false;
  }

  function refFromDataset(dataset) {
    return {
      groupId: dataset.groupId,
      kind: dataset.kind,
      index: dataset.kind === "route" ? Number(dataset.index) : null
    };
  }

  function normalizeEventRef(ref) {
    if (!ref || typeof ref.groupId !== "string" || !["spawn", "route", "target"].includes(ref.kind)) {
      return null;
    }
    const index = ref.kind === "route" ? Number(ref.index) : null;
    if (ref.kind === "route" && !Number.isInteger(index)) {
      return null;
    }
    return { groupId: ref.groupId, kind: ref.kind, index: index };
  }

  function sameRef(left, right) {
    return Boolean(left && right && left.groupId === right.groupId && left.kind === right.kind && left.index === right.index);
  }

  function cloneRef(ref) {
    return { groupId: ref.groupId, kind: ref.kind, index: ref.index };
  }

  function pointKey(ref) {
    return ref.groupId + "|" + ref.kind + "|" + (ref.index === null ? "" : ref.index);
  }

  function featureId(ref) {
    return "migration:" + ref.groupId + ":" + (ref.kind === "route" ? "route:" + ref.index : ref.kind);
  }

  function pointAddress(ref) {
    return ref.kind === "route" ? "route:" + ref.index : ref.kind;
  }

  function pointTitle(ref) {
    if (ref.kind === "spawn") {
      return "Spawn";
    }
    if (ref.kind === "target") {
      return "Target";
    }
    return "Route " + (ref.index + 1);
  }

  function pointMarkerLabel(ref) {
    if (ref.kind === "spawn") {
      return "S";
    }
    if (ref.kind === "target") {
      return "T";
    }
    return String(ref.index + 1);
  }

  function axisToIndex(axis) {
    return axis === "x" ? 0 : axis === "y" ? 1 : 2;
  }

  function snapshotsEqual(left, right) {
    return left.yConfirmed === right.yConfirmed && left.coordinates.every((value, index) => Object.is(value, right.coordinates[index]));
  }

  function isWorldCoordinate(value) {
    return Number.isFinite(value) && value >= 0 && value <= WORLD_SIZE;
  }

  function isClassName(value) {
    return typeof value === "string" && /^[A-Za-z0-9_]+$/.test(value.trim());
  }

  function isEnabled(value) {
    return value === true || value === 1;
  }

  function finitePositive(value) {
    return typeof value === "number" && Number.isFinite(value) && value > 0;
  }

  function formatRadius(value) {
    return finitePositive(value) ? Number(value).toFixed(1) + " m" : "не задан";
  }

  function enabledLabel(value) {
    return isEnabled(value) ? "enabled" : "disabled";
  }

  function addListItem(list, text) {
    list.appendChild(createTextElement("li", text));
  }

  function createTextElement(tagName, text, className = "") {
    const element = document.createElement(tagName);
    element.textContent = text;
    if (className) {
      element.className = className;
    }
    return element;
  }

  function cloneJson(value) {
    return JSON.parse(JSON.stringify(value));
  }

  function setLoadStatus(text, kind) {
    elements.loadStatus.textContent = text;
    elements.loadStatus.classList.toggle("error", kind === "error");
  }

  function showError(message) {
    elements.error.hidden = false;
    elements.error.textContent = message;
  }

  function clearError() {
    elements.error.hidden = true;
    elements.error.textContent = "";
  }
})();
