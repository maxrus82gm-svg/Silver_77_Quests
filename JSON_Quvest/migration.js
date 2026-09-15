(() => {
  "use strict";

  const WORLD_SIZE = 15360;
  const MIGRATION_API = "/api/migration/config";
  const EXPECTED_SOURCE = "Silver_77_Migrate_server/config/MigrationConfig.json";

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
    elements.groupSelect = document.getElementById("migrationGroupSelect");
    elements.sourceId = document.getElementById("migrationSourceId");
    elements.revision = document.getElementById("migrationRevision");
    elements.undoButton = document.getElementById("migrationUndoButton");
    elements.redoButton = document.getElementById("migrationRedoButton");
    elements.resetButton = document.getElementById("migrationResetButton");
    elements.groupHeading = document.getElementById("migrationGroupHeading");
    elements.points = document.getElementById("migrationPoints");
    elements.radiiInfo = document.getElementById("migrationRadiiInfo");
    elements.radiiContent = document.getElementById("migrationRadiiContent");
  }

  function bindControls() {
    elements.groupSelect.addEventListener("change", () => {
      cancelActiveEdits();
      state.selectedGroupId = elements.groupSelect.value;
      renderEditor();
      emitProjection();
    });
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

      renderGroupSelector();
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
      renderGroupSelector();
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
    });
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

  function renderGroupSelector() {
    elements.groupSelect.replaceChildren();
    if (!state.loaded || !state.config) {
      const option = document.createElement("option");
      option.textContent = "GROUP недоступны";
      option.value = "";
      elements.groupSelect.appendChild(option);
      elements.groupSelect.disabled = true;
      return;
    }

    state.config.groups.forEach((group) => {
      const option = document.createElement("option");
      option.value = group.groupId;
      option.textContent = group.name ? group.groupId + " — " + group.name : group.groupId;
      option.selected = group.groupId === state.selectedGroupId;
      elements.groupSelect.appendChild(option);
    });
    elements.groupSelect.disabled = state.config.groups.length === 0;
  }

  function renderEditor() {
    updateMetaAndActions();
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

    listPointRefs(group).forEach((ref) => {
      elements.points.appendChild(createPointCard(ref));
    });
    renderRadiiInfo(group);
  }

  function createPointCard(ref) {
    const point = getPoint(ref);
    const card = document.createElement("section");
    card.className = "migration-point-card";
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

    card.append(title, grid, yStatus);
    return card;
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

  function undo() {
    cancelActiveEdits();
    const operation = state.undoStack.pop();
    if (!operation) {
      return;
    }
    applyPointSnapshot(operation.ref, operation.before);
    state.redoStack.push(operation);
    recomputeDirty();
    renderEditor();
    emitProjection();
  }

  function redo() {
    cancelActiveEdits();
    const operation = state.redoStack.pop();
    if (!operation) {
      return;
    }
    applyPointSnapshot(operation.ref, operation.after);
    state.undoStack.push(operation);
    recomputeDirty();
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
    renderGroupSelector();
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
    recomputeDirty();
  }

  function cancelFormEditWithoutCommit() {
    if (!state.formEdit) {
      return;
    }
    applyPointSnapshot(state.formEdit.ref, state.formEdit.before);
    state.formEdit = null;
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
