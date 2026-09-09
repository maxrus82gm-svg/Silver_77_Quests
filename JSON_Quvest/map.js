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
    features: [],
    selectedId: "",
    view: { x: 0, y: 0, width: WORLD_SIZE, height: WORLD_SIZE },
    drag: null
  };

  const mapElements = {};

  document.addEventListener("DOMContentLoaded", () => {
    bindWorkshopTabs();
    cacheMapElements();
    bindMapControls();
    applyViewBox();
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
    mapElements.featureLayer = document.getElementById("mapFeatureLayer");
    mapElements.infoCard = document.getElementById("mapInfoCard");
    mapElements.fitButton = document.getElementById("mapFitButton");
    mapElements.zoomInButton = document.getElementById("mapZoomInButton");
    mapElements.zoomOutButton = document.getElementById("mapZoomOutButton");
    mapElements.layerToggle = document.getElementById("mapLayerToggle");
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
    mapElements.layerToggle.addEventListener("change", updateLayerVisibility);
    mapElements.radiiToggle.addEventListener("change", updateLayerVisibility);
    mapElements.labelsToggle.addEventListener("change", updateLayerVisibility);
    mapElements.svg.addEventListener("wheel", handleWheel, { passive: false });
    mapElements.svg.addEventListener("pointerdown", handlePointerDown);
    mapElements.svg.addEventListener("pointermove", handlePointerMove);
    mapElements.svg.addEventListener("pointerup", handlePointerUp);
    mapElements.svg.addEventListener("pointercancel", handlePointerUp);
    mapElements.svg.addEventListener("pointerleave", handlePointerLeave);
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
      mapState.features = result.features;
      renderFeatures();
      mapElements.count.textContent = result.features.length + " зон";

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

      mapState.initialized = true;
      verifyCalibration();
    } catch (error) {
      setStatus("Ошибка загрузки: " + error.message, "error");
      mapElements.count.textContent = "0 зон";
    } finally {
      mapState.loading = false;
    }
  }

  function parseMedicalAttention(text) {
    const features = [];
    const invalidLines = [];
    const lines = String(text || "").split(/\r?\n/);
    const declarationPattern = /^\s*DeclareZone\s*=\s*([^=]+?)\s*=\s*(-?\d+(?:\.\d+)?)\s*=\s*(-?\d+(?:\.\d+)?)\s*=\s*(-?\d+(?:\.\d+)?)\s*(?:\/\/\s*(.*))?$/;

    lines.forEach((line, lineIndex) => {
      if (/^\s*\/\/\s*DeclareZone\s*=/.test(line)) {
        return;
      }

      if (!/^\s*DeclareZone\s*=/.test(line)) {
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
        geometry: {
          type: "circle",
          radii: [
            { role: "Hazard", radius: radius }
          ]
        },
        zoneType: zoneType,
        rawValues: {
          x: match[2],
          z: match[3],
          radius: match[4]
        }
      });
    });

    return { features: features, invalidLines: invalidLines };
  }

  function renderFeatures() {
    mapElements.featureLayer.replaceChildren();

    mapState.features.forEach((feature) => {
      const group = createSvgElement("g");
      group.classList.add("map-feature");
      group.dataset.featureId = feature.id;
      group.setAttribute("role", "button");
      group.setAttribute("tabindex", "0");
      group.setAttribute("aria-label", getFeatureTitle(feature));

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
        selectFeature(feature.id);
      });
      group.addEventListener("keydown", (event) => {
        if (event.key === "Enter" || event.key === " ") {
          event.preventDefault();
          selectFeature(feature.id);
        }
      });
      mapElements.featureLayer.appendChild(group);
    });

    updateSymbolScale();
    updateLayerVisibility();
  }

  function selectFeature(featureId) {
    mapState.selectedId = featureId;
    Array.from(mapElements.featureLayer.querySelectorAll(".map-feature")).forEach((element) => {
      element.classList.toggle("selected", element.dataset.featureId === featureId);
    });

    const feature = mapState.features.find((entry) => entry.id === featureId);
    if (!feature) {
      return;
    }

    const radiusText = feature.geometry.radii
      .map((entry) => entry.role + ": " + formatCoordinate(entry.radius) + " m")
      .join("; ");
    const title = document.createElement("h2");
    title.textContent = getFeatureTitle(feature);
    const list = document.createElement("dl");
    addInfoRow(list, "ID", feature.id);
    addInfoRow(list, "Комментарий", feature.name || "нет");
    addInfoRow(list, "Категория", feature.category);
    addInfoRow(list, "Координаты", "X " + feature.rawValues.x + " / Z " + feature.rawValues.z);
    addInfoRow(list, "Радиусы", radiusText);
    addInfoRow(list, "Тип", feature.zoneType);
    addInfoRow(list, "Геометрия", feature.geometry.type);
    addInfoRow(list, "Источник", feature.source.path);
    addInfoRow(list, "Статус", feature.source.status + ", строка " + feature.source.lineNumber);
    mapElements.infoCard.replaceChildren(title, list);
  }

  function addInfoRow(list, label, value) {
    const term = document.createElement("dt");
    term.textContent = label;
    const description = document.createElement("dd");
    description.textContent = value;
    list.append(term, description);
  }

  function getFeatureTitle(feature) {
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

  function updateLayerVisibility() {
    mapElements.featureLayer.style.display = mapElements.layerToggle.checked ? "" : "none";
    mapElements.featureLayer.classList.toggle("hide-radii", !mapElements.radiiToggle.checked);
    mapElements.featureLayer.classList.toggle("hide-labels", !mapElements.labelsToggle.checked);
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
    const newView = {
      x: anchorX - (anchorX - oldView.x) * ratio,
      y: anchorY - (anchorY - oldView.y) * ratio,
      width: newWidth,
      height: newWidth
    };
    mapState.view = clampView(newView);
    applyViewBox();
  }

  function handlePointerDown(event) {
    if (event.button !== 0) {
      return;
    }
    if (event.target.closest && event.target.closest(".map-feature")) {
      return;
    }
    mapState.drag = {
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
    if (!mapState.drag || mapState.drag.pointerId !== event.pointerId) {
      return;
    }

    const rect = mapElements.svg.getBoundingClientRect();
    const dx = (event.clientX - mapState.drag.clientX) * mapState.view.width / rect.width;
    const dy = (event.clientY - mapState.drag.clientY) * mapState.view.height / rect.height;
    mapState.view = clampView({
      x: mapState.drag.startViewX - dx,
      y: mapState.drag.startViewY - dy,
      width: mapState.view.width,
      height: mapState.view.height
    });
    applyViewBox();
  }

  function handlePointerUp(event) {
    if (!mapState.drag || mapState.drag.pointerId !== event.pointerId) {
      return;
    }
    mapState.drag = null;
    mapElements.viewport.classList.remove("dragging");
    if (mapElements.svg.hasPointerCapture(event.pointerId)) {
      mapElements.svg.releasePointerCapture(event.pointerId);
    }
  }

  function handlePointerLeave(event) {
    if (!mapState.drag) {
      mapElements.coordinates.textContent = "X — / Z —";
      return;
    }
    updateCursorCoordinates(event.clientX, event.clientY);
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
    if (!matrix) {
      return { x: 0, y: 0 };
    }
    return point.matrixTransform(matrix.inverse());
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
    if (!mapElements.featureLayer) {
      return;
    }
    const markerRadius = Math.max(10, mapState.view.width * 0.0036);
    const fontSize = Math.max(24, mapState.view.width * 0.0085);
    const radiusStroke = Math.max(1.2, Math.min(3.5, 1.2 + (WORLD_SIZE / mapState.view.width) * 0.08));
    Array.from(mapElements.featureLayer.querySelectorAll(".map-feature-center")).forEach((element) => {
      element.setAttribute("r", String(markerRadius));
    });
    Array.from(mapElements.featureLayer.querySelectorAll(".map-feature-label")).forEach((element) => {
      element.setAttribute("font-size", String(fontSize));
    });
    Array.from(mapElements.featureLayer.querySelectorAll(".map-feature-radius")).forEach((element) => {
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
      const expectedWest = entry.region.endsWith("west");
      const expectedNorth = entry.region.startsWith("north");
      return mapY >= 0 && mapY <= WORLD_SIZE && west === expectedWest && north === expectedNorth;
    });
    if (!valid) {
      setStatus("Ошибка калибровки X/Z", "error");
    }
  }

  function formatCoordinate(value) {
    return Number(value).toFixed(1);
  }

  function clamp(value, min, max) {
    return Math.min(max, Math.max(min, value));
  }
})();
