(() => {
  "use strict";

  const WORLD_SIZE = 15360;
  const MIN_VIEW_SIZE = WORLD_SIZE / 32;
  const EXPECTED_MEDICAL_ATTENTION_COUNT = 78;
  const MEDICAL_ATTENTION_API = "/api/map/medical-attention";
  const MEDICAL_ATTENTION_SOURCE = "DayZ_Server_Reference/Server_Custom_Config/Danger_Zones/MedicalAttention/config.txt";
  const SVG_NS = "http://www.w3.org/2000/svg";

  const mapState = {
    initialized: false,
    loading: false,
    medicalFeatures: [],
    migrationProjection: { available: false, points: [] },
    selected: null,
    view: { x: 0, y: 0, width: WORLD_SIZE, height: WORLD_SIZE },
    panDrag: null,
    migrationDrag: null
  };

  const mapElements = {};

  document.addEventListener("s77:migration-projection", handleMigrationProjection);
  document.addEventListener("DOMContentLoaded", () => {
    bindWorkshopTabs();
    cacheMapElements();
    bindMapControls();
    applyViewBox();
    renderMigrationProjection();
    document.dispatchEvent(new CustomEvent("s77:migration-projection-request"));
  });

  function bindWorkshopTabs() {
    const tabs = Array.from(document.querySelectorAll("[data-workshop-tab]"));
    const modules = Array.from(document.querySelectorAll("[data-workshop-module]"));

    tabs.forEach((tab) => {
      tab.addEventListener("click", () => {
        const target = tab.dataset.workshopTab;
        tabs.forEach((entry) => {
          const active = entry.dataset.workshopTab === target;
          entry.classList.toggle("active", active);
          entry.setAttribute("aria-selected", active ? "true" : "false");
        });
        modules.forEach((module) => {
          module.hidden = module.dataset.workshopModule !== target;
        });

        if (target === "map" && !mapState.initialized) {
          void initializeMap();
        }
      });
    });
  }

  function cacheMapElements() {
    mapElements.status = document.getElementById("mapStatus");
    mapElements.count = document.getElementById("mapFeatureCount");
    mapElements.coordinates = document.getElementById("mapCursorCoordinates");
    mapElements.viewport = document.getElementById("mapViewport");
    mapElements.svg = document.getElementById("mapSvg");
    mapElements.medicalLayer = document.getElementById("mapFeatureLayer");
    mapElements.migrationLayer = document.getElementById("mapMigrationLayer");
    mapElements.infoCard = document.getElementById("mapInfoCard");
    mapElements.fitButton = document.getElementById("mapFitButton");
    mapElements.zoomInButton = document.getElementById("mapZoomInButton");
    mapElements.zoomOutButton = document.getElementById("mapZoomOutButton");
    mapElements.medicalToggle = document.getElementById("mapLayerToggle");
    mapElements.migrationToggle = document.getElementById("mapMigrationToggle");
    mapElements.radiiToggle = document.getElementById("mapRadiiToggle");
    mapElements.labelsToggle = document.getElementById("mapLabelsToggle");
  }

  function bindMapControls() {
    if (!mapElements.svg) {
      return;
    }

    mapElements.fitButton.addEventListener("click", fitMap);
    mapElements.zoomInButton.addEventListener("click", () => zoomAtCenter(1 / 1.35));
    mapElements.zoomOutButton.addEventListener("click", () => zoomAtCenter(1.35));
    mapElements.medicalToggle.addEventListener("change", updateLayerVisibility);
    mapElements.migrationToggle.addEventListener("change", updateLayerVisibility);
    mapElements.radiiToggle.addEventListener("change", updateLayerVisibility);
    mapElements.labelsToggle.addEventListener("change", updateLayerVisibility);
    mapElements.svg.addEventListener("wheel", handleWheel, { passive: false });
    mapElements.svg.addEventListener("pointerdown", handlePointerDown);
    mapElements.svg.addEventListener("pointermove", handlePointerMove);
    mapElements.svg.addEventListener("pointerup", handlePointerUp);
    mapElements.svg.addEventListener("pointercancel", handlePointerCancel);
    mapElements.svg.addEventListener("pointerleave", handlePointerLeave);
    document.addEventListener("keydown", handleKeyDown);
  }

  async function initializeMap() {
    if (mapState.loading || mapState.initialized) {
      return;
    }

    mapState.loading = true;
    setStatus("Загрузка snapshot…", "");

    try {
      const response = await fetch(MEDICAL_ATTENTION_API, { cache: "no-store" });
      if (!response.ok) {
        throw new Error("HTTP " + response.status + " " + response.statusText);
      }

      const text = await response.text();
      const result = parseMedicalAttention(text);
      mapState.medicalFeatures = result.features;
      renderMedicalFeatures();

      if (result.features.length === EXPECTED_MEDICAL_ATTENTION_COUNT && result.invalidLines.length === 0) {
        setStatus("Snapshot: OK · 78 active DeclareZone", "");
      } else {
        let warning = "Snapshot: " + result.features.length + " active";
        if (result.invalidLines.length > 0) {
          warning += " · пропущено " + result.invalidLines.length;
        }
        warning += " · ожидалось " + EXPECTED_MEDICAL_ATTENTION_COUNT;
        setStatus(warning, "warning");
      }
      verifyCalibration();
    } catch (error) {
      setStatus("Ошибка MedicalAttention: " + error.message, "error");
      mapState.medicalFeatures = [];
      renderMedicalFeatures();
    } finally {
      mapState.initialized = true;
      mapState.loading = false;
      updateFeatureCount();
    }
  }

  function parseMedicalAttention(text) {
    const features = [];
    const invalidLines = [];
    const lines = String(text || "").split(/\r?\n/);
    const declarationPattern = /^\s*DeclareZone\s*=\s*([^=]+?)\s*=\s*(-?\d+(?:\.\d+)?)\s*=\s*(-?\d+(?:\.\d+)?)\s*=\s*(-?\d+(?:\.\d+)?)\s*(?:\/\/\s*(.*))?$/;

    lines.forEach((line, lineIndex) => {
      if (/^\s*\/\/\s*DeclareZone\s*=/.test(line) || !/^\s*DeclareZone\s*=/.test(line)) {
        return;
      }

      const match = line.match(declarationPattern);
      if (!match) {
        invalidLines.push({ lineNumber: lineIndex + 1, line: line });
        return;
      }

      const zoneType = match[1].trim();
      const x = Number(match[2]);
      const z = Number(match[3]);
      const radius = Number(match[4]);
      const name = (match[5] || "").trim();
      const validNumbers = Number.isFinite(x) && Number.isFinite(z) && Number.isFinite(radius);
      const validBounds = x >= 0 && x <= WORLD_SIZE && z >= 0 && z <= WORLD_SIZE && radius > 0;

      if (!validNumbers || !validBounds) {
        invalidLines.push({ lineNumber: lineIndex + 1, line: line });
        return;
      }

      const number = features.length + 1;
      features.push({
        id: "medical-attention-" + number,
        number: number,
        name: name,
        category: "Hazard",
        source: {
          path: MEDICAL_ATTENTION_SOURCE,
          status: "repository snapshot",
          lineNumber: lineIndex + 1
        },
        center: { x: x, z: z },
        geometry: { type: "circle", radii: [{ role: "Hazard", radius: radius }] },
        zoneType: zoneType,
        rawValues: { x: match[2], z: match[3], radius: match[4] }
      });
    });

    return { features: features, invalidLines: invalidLines };
  }

  function renderMedicalFeatures() {
    if (!mapElements.medicalLayer) {
      return;
    }
    mapElements.medicalLayer.replaceChildren();

    mapState.medicalFeatures.forEach((feature) => {
      const group = createSvgElement("g");
      group.classList.add("map-feature");
      group.dataset.featureId = feature.id;
      group.setAttribute("role", "button");
      group.setAttribute("tabindex", "0");
      group.setAttribute("aria-label", getMedicalFeatureTitle(feature));

      const mapY = WORLD_SIZE - feature.center.z;
      feature.geometry.radii.forEach((radiusDefinition) => {
        const circle = createSvgElement("circle");
        circle.classList.add("map-feature-radius");
        circle.setAttribute("cx", String(feature.center.x));
        circle.setAttribute("cy", String(mapY));
        circle.setAttribute("r", String(radiusDefinition.radius));
        circle.dataset.radiusRole = radiusDefinition.role;
        group.appendChild(circle);
      });

      const center = createSvgElement("circle");
      center.classList.add("map-feature-center");
      center.setAttribute("cx", String(feature.center.x));
      center.setAttribute("cy", String(mapY));
      group.appendChild(center);

      const label = createSvgElement("text");
      label.classList.add("map-feature-label");
      label.setAttribute("x", String(feature.center.x));
      label.setAttribute("y", String(mapY));
      label.textContent = String(feature.number);
      group.appendChild(label);

      group.addEventListener("click", (event) => {
        event.stopPropagation();
        selectMedicalFeature(feature.id);
      });
      group.addEventListener("keydown", (event) => {
        if (event.key === "Enter" || event.key === " ") {
          event.preventDefault();
          selectMedicalFeature(feature.id);
        }
      });
      mapElements.medicalLayer.appendChild(group);
    });

    updateSymbolScale();
    updateLayerVisibility();
    updateFeatureCount();
  }

  function handleMigrationProjection(event) {
    mapState.migrationProjection = event.detail || { available: false, points: [] };
    if (mapState.selected && mapState.selected.module === "Migration") {
      const selectedStillExists = Array.isArray(mapState.migrationProjection.points) &&
        mapState.migrationProjection.points.some((point) => point.id === mapState.selected.id);
      if (!selectedStillExists) {
        mapState.selected = null;
      }
    }
    renderMigrationProjection();
  }

  function renderMigrationProjection() {
    if (!mapElements.migrationLayer) {
      return;
    }
    mapElements.migrationLayer.replaceChildren();
    const projection = mapState.migrationProjection;
    if (!projection || !projection.available || !projection.group || !Array.isArray(projection.points)) {
      updateFeatureCount();
      updateLayerVisibility();
      return;
    }

    if (projection.points.length > 1) {
      const route = createSvgElement("polyline");
      route.classList.add("map-migration-route");
      route.setAttribute("points", projection.points.map((point) => point.x + "," + (WORLD_SIZE - point.z)).join(" "));
      mapElements.migrationLayer.appendChild(route);
    }

    projection.points.forEach((point) => {
      const mapY = WORLD_SIZE - point.z;
      point.radii.forEach((radiusDefinition) => {
        const circle = createSvgElement("circle");
        circle.classList.add("map-migration-radius", radiusDefinition.style || "reference");
        circle.classList.toggle("disabled", !radiusDefinition.enabled);
        circle.setAttribute("cx", String(point.x));
        circle.setAttribute("cy", String(mapY));
        circle.setAttribute("r", String(radiusDefinition.radius));
        circle.setAttribute("aria-label", radiusDefinition.role + " " + radiusDefinition.radius + " m " + (radiusDefinition.enabled ? "enabled" : "disabled"));
        mapElements.migrationLayer.appendChild(circle);
      });
    });

    projection.points.forEach((point) => {
      const group = createSvgElement("g");
      group.classList.add("map-migration-point", point.ref.kind);
      group.classList.toggle("selected", Boolean(mapState.selected && mapState.selected.module === "Migration" && mapState.selected.id === point.id));
      group.dataset.featureId = point.id;
      group.dataset.groupId = point.ref.groupId;
      group.dataset.kind = point.ref.kind;
      group.dataset.index = point.ref.index === null ? "" : String(point.ref.index);
      group.setAttribute("role", "button");
      group.setAttribute("tabindex", "0");
      group.setAttribute("aria-label", projection.group.groupId + " " + point.title);

      const mapY = WORLD_SIZE - point.z;
      const shape = createSvgElement("circle");
      shape.classList.add("map-migration-point-shape");
      shape.setAttribute("cx", String(point.x));
      shape.setAttribute("cy", String(mapY));
      group.appendChild(shape);

      const label = createSvgElement("text");
      label.classList.add("map-migration-point-label");
      label.setAttribute("x", String(point.x));
      label.setAttribute("y", String(mapY));
      label.textContent = point.label;
      group.appendChild(label);

      group.addEventListener("click", (event) => {
        event.stopPropagation();
        selectMigrationPoint(point.id);
      });
      group.addEventListener("keydown", (event) => {
        if (event.key === "Enter" || event.key === " ") {
          event.preventDefault();
          selectMigrationPoint(point.id);
        }
      });
      mapElements.migrationLayer.appendChild(group);
    });

    updateSymbolScale();
    updateLayerVisibility();
    updateFeatureCount();
    refreshSelectedInfo();
  }

  function selectMedicalFeature(featureId) {
    mapState.selected = { module: "MedicalAttention", id: featureId };
    updateSelectionClasses();
    renderMedicalInfo(featureId);
  }

  function selectMigrationPoint(featureId) {
    mapState.selected = { module: "Migration", id: featureId };
    updateSelectionClasses();
    renderMigrationInfo(featureId);
    const point = getMigrationPoint(featureId);
    if (point) {
      document.dispatchEvent(new CustomEvent("s77:migration-select-point", { detail: { ref: point.ref } }));
    }
  }

  function updateSelectionClasses() {
    Array.from(mapElements.medicalLayer.querySelectorAll(".map-feature")).forEach((element) => {
      element.classList.toggle("selected", Boolean(mapState.selected && mapState.selected.module === "MedicalAttention" && mapState.selected.id === element.dataset.featureId));
    });
    Array.from(mapElements.migrationLayer.querySelectorAll(".map-migration-point")).forEach((element) => {
      element.classList.toggle("selected", Boolean(mapState.selected && mapState.selected.module === "Migration" && mapState.selected.id === element.dataset.featureId));
    });
  }

  function refreshSelectedInfo() {
    if (!mapState.selected) {
      return;
    }
    if (mapState.selected.module === "Migration") {
      renderMigrationInfo(mapState.selected.id);
    }
  }

  function renderMedicalInfo(featureId) {
    const feature = mapState.medicalFeatures.find((entry) => entry.id === featureId);
    if (!feature) {
      return;
    }
    const radiusText = feature.geometry.radii
      .map((entry) => entry.role + ": " + formatCoordinate(entry.radius) + " m")
      .join("; ");
    const title = document.createElement("h2");
    title.textContent = getMedicalFeatureTitle(feature);
    const list = document.createElement("dl");
    addInfoRow(list, "Модуль", "MedicalAttention");
    addInfoRow(list, "ID", feature.id);
    addInfoRow(list, "Комментарий", feature.name || "нет");
    addInfoRow(list, "Категория", feature.category);
    addInfoRow(list, "Координаты", "X " + feature.rawValues.x + " / Z " + feature.rawValues.z);
    addInfoRow(list, "Радиусы", radiusText);
    addInfoRow(list, "Тип", feature.zoneType);
    addInfoRow(list, "Источник", feature.source.path);
    addInfoRow(list, "Статус", feature.source.status + ", строка " + feature.source.lineNumber + " · read-only");
    mapElements.infoCard.replaceChildren(title, list);
  }

  function renderMigrationInfo(featureId) {
    const projection = mapState.migrationProjection;
    const point = getMigrationPoint(featureId);
    if (!projection || !projection.group || !point) {
      return;
    }
    const title = document.createElement("h2");
    title.textContent = projection.group.groupId + " — " + point.title;
    const list = document.createElement("dl");
    addInfoRow(list, "Модуль", "Migration");
    addInfoRow(list, "GROUP", projection.group.groupId);
    addInfoRow(list, "Название", projection.group.name || "нет");
    addInfoRow(list, "Точка", point.title);
    if (point.ref.kind === "route") {
      addInfoRow(list, "Route index", String(point.ref.index));
    }
    addInfoRow(list, "X / Y / Z", formatPrecise(point.x) + " / " + formatPrecise(point.y) + " / " + formatPrecise(point.z));
    addInfoRow(list, "Y status", point.yConfirmed ? "подтверждён" : "требует подтверждения");
    addInfoRow(list, "Радиусы", formatMigrationRadii(point.radii));
    if (point.ref.kind === "target" && Number.isFinite(projection.group.finalHoldReturnRadius)) {
      addInfoRow(list, "Hold return", formatCoordinate(projection.group.finalHoldReturnRadius) + " m · только текст, не общий круг");
    }
    addInfoRow(list, "Источник", projection.sourceId);
    addInfoRow(list, "Статус", point.dirty ? "in-memory edit" : "loaded baseline");
    mapElements.infoCard.replaceChildren(title, list);
  }

  function getMigrationPoint(featureId) {
    const points = mapState.migrationProjection && Array.isArray(mapState.migrationProjection.points)
      ? mapState.migrationProjection.points
      : [];
    return points.find((entry) => entry.id === featureId) || null;
  }

  function addInfoRow(list, label, value) {
    const term = document.createElement("dt");
    term.textContent = label;
    const description = document.createElement("dd");
    description.textContent = value;
    list.append(term, description);
  }

  function formatMigrationRadii(radii) {
    if (!Array.isArray(radii) || radii.length === 0) {
      return "нет общей radius geometry";
    }
    return radii.map((entry) => entry.role + ": " + formatCoordinate(entry.radius) + " m (" + (entry.enabled ? "enabled" : "disabled") + ")").join("; ");
  }

  function getMedicalFeatureTitle(feature) {
    const suffix = feature.name ? " — " + feature.name : "";
    return "MedicalAttention #" + feature.number + suffix;
  }

  function createSvgElement(tagName) {
    return document.createElementNS(SVG_NS, tagName);
  }

  function setStatus(text, kind) {
    mapElements.status.textContent = text;
    mapElements.status.classList.toggle("warning", kind === "warning");
    mapElements.status.classList.toggle("error", kind === "error");
  }

  function updateFeatureCount() {
    if (!mapElements.count) {
      return;
    }
    const migrationCount = mapState.migrationProjection && Array.isArray(mapState.migrationProjection.points)
      ? mapState.migrationProjection.points.length
      : 0;
    mapElements.count.textContent = mapState.medicalFeatures.length + " зон · " + migrationCount + " Migration points";
  }

  function updateLayerVisibility() {
    if (!mapElements.medicalLayer || !mapElements.migrationLayer) {
      return;
    }
    mapElements.medicalLayer.style.display = mapElements.medicalToggle.checked ? "" : "none";
    mapElements.migrationLayer.style.display = mapElements.migrationToggle.checked ? "" : "none";
    mapElements.medicalLayer.classList.toggle("hide-radii", !mapElements.radiiToggle.checked);
    mapElements.medicalLayer.classList.toggle("hide-labels", !mapElements.labelsToggle.checked);
    mapElements.migrationLayer.classList.toggle("hide-radii", !mapElements.radiiToggle.checked);
    mapElements.migrationLayer.classList.toggle("hide-labels", !mapElements.labelsToggle.checked);
  }

  function fitMap() {
    mapState.view = { x: 0, y: 0, width: WORLD_SIZE, height: WORLD_SIZE };
    applyViewBox();
  }

  function zoomAtCenter(factor) {
    const centerX = mapState.view.x + mapState.view.width / 2;
    const centerY = mapState.view.y + mapState.view.height / 2;
    zoomAroundMapPoint(centerX, centerY, factor);
  }

  function handleWheel(event) {
    event.preventDefault();
    const point = screenToMap(event.clientX, event.clientY);
    const factor = event.deltaY > 0 ? 1.18 : 1 / 1.18;
    zoomAroundMapPoint(point.x, point.y, factor);
  }

  function zoomAroundMapPoint(anchorX, anchorY, factor) {
    const oldView = mapState.view;
    const newWidth = clamp(oldView.width * factor, MIN_VIEW_SIZE, WORLD_SIZE);
    const ratio = newWidth / oldView.width;
    mapState.view = clampView({
      x: anchorX - (anchorX - oldView.x) * ratio,
      y: anchorY - (anchorY - oldView.y) * ratio,
      width: newWidth,
      height: newWidth
    });
    applyViewBox();
  }

  function handlePointerDown(event) {
    if (event.button !== 0) {
      return;
    }

    const migrationPoint = event.target.closest && event.target.closest(".map-migration-point");
    if (migrationPoint) {
      event.preventDefault();
      const ref = migrationRefFromElement(migrationPoint);
      mapState.migrationDrag = { pointerId: event.pointerId, ref: ref };
      selectMigrationPoint(migrationPoint.dataset.featureId);
      document.dispatchEvent(new CustomEvent("s77:migration-drag-start", { detail: { ref: ref } }));
      mapElements.svg.setPointerCapture(event.pointerId);
      mapElements.viewport.classList.add("migration-dragging");
      return;
    }

    if (event.target.closest && event.target.closest(".map-feature")) {
      return;
    }
    mapState.panDrag = {
      pointerId: event.pointerId,
      clientX: event.clientX,
      clientY: event.clientY,
      startViewX: mapState.view.x,
      startViewY: mapState.view.y
    };
    mapElements.svg.setPointerCapture(event.pointerId);
    mapElements.viewport.classList.add("dragging");
  }

  function handlePointerMove(event) {
    updateCursorCoordinates(event.clientX, event.clientY);
    if (mapState.migrationDrag && mapState.migrationDrag.pointerId === event.pointerId) {
      const point = screenToMap(event.clientX, event.clientY);
      document.dispatchEvent(new CustomEvent("s77:migration-drag-preview", {
        detail: {
          ref: mapState.migrationDrag.ref,
          x: clamp(point.x, 0, WORLD_SIZE),
          z: clamp(WORLD_SIZE - point.y, 0, WORLD_SIZE)
        }
      }));
      return;
    }
    if (!mapState.panDrag || mapState.panDrag.pointerId !== event.pointerId) {
      return;
    }

    const rect = mapElements.svg.getBoundingClientRect();
    const dx = (event.clientX - mapState.panDrag.clientX) * mapState.view.width / rect.width;
    const dy = (event.clientY - mapState.panDrag.clientY) * mapState.view.height / rect.height;
    mapState.view = clampView({
      x: mapState.panDrag.startViewX - dx,
      y: mapState.panDrag.startViewY - dy,
      width: mapState.view.width,
      height: mapState.view.height
    });
    applyViewBox();
  }

  function handlePointerUp(event) {
    if (mapState.migrationDrag && mapState.migrationDrag.pointerId === event.pointerId) {
      const drag = mapState.migrationDrag;
      finishMigrationDrag(event.pointerId);
      document.dispatchEvent(new CustomEvent("s77:migration-drag-commit", { detail: { ref: drag.ref } }));
      return;
    }
    finishPanDrag(event.pointerId);
  }

  function handlePointerCancel(event) {
    if (mapState.migrationDrag && mapState.migrationDrag.pointerId === event.pointerId) {
      cancelMigrationDrag();
      return;
    }
    finishPanDrag(event.pointerId);
  }

  function handleKeyDown(event) {
    if (event.key === "Escape" && mapState.migrationDrag) {
      event.preventDefault();
      cancelMigrationDrag();
    }
  }

  function cancelMigrationDrag() {
    const drag = mapState.migrationDrag;
    if (!drag) {
      return;
    }
    finishMigrationDrag(drag.pointerId);
    document.dispatchEvent(new CustomEvent("s77:migration-drag-cancel", { detail: { ref: drag.ref } }));
  }

  function finishMigrationDrag(pointerId) {
    mapState.migrationDrag = null;
    mapElements.viewport.classList.remove("migration-dragging");
    if (mapElements.svg.hasPointerCapture(pointerId)) {
      mapElements.svg.releasePointerCapture(pointerId);
    }
  }

  function finishPanDrag(pointerId) {
    if (!mapState.panDrag || mapState.panDrag.pointerId !== pointerId) {
      return;
    }
    mapState.panDrag = null;
    mapElements.viewport.classList.remove("dragging");
    if (mapElements.svg.hasPointerCapture(pointerId)) {
      mapElements.svg.releasePointerCapture(pointerId);
    }
  }

  function handlePointerLeave(event) {
    if (!mapState.panDrag && !mapState.migrationDrag) {
      mapElements.coordinates.textContent = "X — / Z —";
      return;
    }
    updateCursorCoordinates(event.clientX, event.clientY);
  }

  function migrationRefFromElement(element) {
    return {
      groupId: element.dataset.groupId,
      kind: element.dataset.kind,
      index: element.dataset.kind === "route" ? Number(element.dataset.index) : null
    };
  }

  function updateCursorCoordinates(clientX, clientY) {
    const point = screenToMap(clientX, clientY);
    const worldX = clamp(point.x, 0, WORLD_SIZE);
    const worldZ = clamp(WORLD_SIZE - point.y, 0, WORLD_SIZE);
    mapElements.coordinates.textContent = "X " + formatCoordinate(worldX) + " / Z " + formatCoordinate(worldZ);
  }

  function screenToMap(clientX, clientY) {
    const point = mapElements.svg.createSVGPoint();
    point.x = clientX;
    point.y = clientY;
    const matrix = mapElements.svg.getScreenCTM();
    return matrix ? point.matrixTransform(matrix.inverse()) : { x: 0, y: 0 };
  }

  function clampView(view) {
    const width = clamp(view.width, MIN_VIEW_SIZE, WORLD_SIZE);
    return {
      x: clamp(view.x, 0, WORLD_SIZE - width),
      y: clamp(view.y, 0, WORLD_SIZE - width),
      width: width,
      height: width
    };
  }

  function applyViewBox() {
    if (!mapElements.svg) {
      return;
    }
    const view = mapState.view;
    mapElements.svg.setAttribute("viewBox", view.x + " " + view.y + " " + view.width + " " + view.height);
    updateSymbolScale();
  }

  function updateSymbolScale() {
    if (!mapElements.medicalLayer || !mapElements.migrationLayer) {
      return;
    }
    const markerRadius = Math.max(10, mapState.view.width * 0.0036);
    const fontSize = Math.max(24, mapState.view.width * 0.0085);
    const radiusStroke = Math.max(1.2, Math.min(3.5, 1.2 + (WORLD_SIZE / mapState.view.width) * 0.08));
    Array.from(mapElements.svg.querySelectorAll(".map-feature-center, .map-migration-point-shape")).forEach((element) => {
      element.setAttribute("r", String(markerRadius));
    });
    Array.from(mapElements.svg.querySelectorAll(".map-feature-label, .map-migration-point-label")).forEach((element) => {
      element.setAttribute("font-size", String(fontSize));
    });
    Array.from(mapElements.svg.querySelectorAll(".map-feature-radius, .map-migration-radius")).forEach((element) => {
      element.setAttribute("stroke-width", String(radiusStroke));
    });
  }

  function verifyCalibration() {
    const checks = [
      { name: "Tisy", x: 1695, z: 14042, region: "north-west" },
      { name: "Berezino", x: 12924, z: 9964, region: "north-east" },
      { name: "Solnechny", x: 13390, z: 6620, region: "south-east" },
      { name: "Chernogorsk", x: 6718, z: 2950, region: "south-west" }
    ];
    const valid = checks.every((entry) => {
      const mapY = WORLD_SIZE - entry.z;
      const west = entry.x < WORLD_SIZE / 2;
      const north = entry.z > WORLD_SIZE / 2;
      return mapY >= 0 && mapY <= WORLD_SIZE && west === entry.region.endsWith("west") && north === entry.region.startsWith("north");
    });
    if (!valid) {
      setStatus("Ошибка калибровки X/Z", "error");
    }
  }

  function formatCoordinate(value) {
    return Number(value).toFixed(1);
  }

  function formatPrecise(value) {
    return Number(value).toFixed(6).replace(/0+$/, "").replace(/\.$/, "");
  }

  function clamp(value, min, max) {
    return Math.min(max, Math.max(min, value));
  }
})();
