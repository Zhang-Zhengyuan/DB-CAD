// ============== Neo4j 当前图状态 ==============

async function loadNeo4jState(projectId) {
  const target = document.getElementById(`neo4j-state-${cssEscape(projectId)}`);
  if (!target) return;
  target.innerHTML = `<div class="neo4j-state-strip"><div class="neo4j-state-empty">加载 Neo4j 图状态…</div></div>`;
  try {
    const r = await adminFetch(`${API}/projects/${encodeURIComponent(ALL_PROJECTS.find(p => p.project_id === projectId)?.name || '')}/neo4j-state`);
    if (!r.ok) throw new Error(await r.text());
    const data = await r.json();
    renderNeo4jStateStrip(target, data);
  } catch (e) {
    target.innerHTML = `<div class="neo4j-state-strip"><div class="neo4j-state-empty">Neo4j 当前图状态加载失败：${escapeHTML(e.message)}</div></div>`;
  }
}

function renderNeo4jStateStrip(el, data) {
  if (!data || !data.has_part) {
    el.innerHTML = `
      <div class="neo4j-state-strip">
        <div class="neo4j-state-header">
          <div class="neo4j-state-title"><span class="dot"></span> Neo4j 当前图状态</div>
          <div class="neo4j-state-meta">该项目尚无可查询的实体图</div>
        </div>
      </div>
    `;
    return;
  }
  const buckets = data.buckets || [];
  const max = buckets.reduce((m, b) => Math.max(m, b.count), 0) || 1;
  el.innerHTML = `
    <div class="neo4j-state-strip">
      <div class="neo4j-state-header">
        <div class="neo4j-state-title"><span class="dot"></span> Neo4j 当前图状态 <small class="muted" style="font-size: var(--fz-sm); font-weight: 400;">— ${escapeHTML(data.storage_label || '')}</small></div>
        <div class="neo4j-state-meta">
          part: <b>${escapeHTML(data.part_name)}</b>
          · version: <b>${data.part_version}</b>
          · 总节点 <b>${data.total_nodes}</b>
        </div>
      </div>
      <div class="neo4j-state-grid">
        ${buckets.map(b => `
          <div class="neo4j-stat">
            <div class="label">${escapeHTML(b.label_zh || b.label)}</div>
            <div class="value">${b.count.toLocaleString()}</div>
          </div>
        `).join('')}
      </div>
    </div>
  `;
}

async function openNeo4jStateModal(projectName, projectId) {
  openModal(`Neo4j 当前图状态 · ${escapeHTML(projectName)}`);
  const body = $('#modal-body');
  body.innerHTML = '<div class="muted center" style="padding: 30px;">加载中…</div>';
  try {
    const r = await adminFetch(`${API}/projects/${encodeURIComponent(projectName)}/neo4j-state`);
    const data = await r.json();
    if (!r.ok) throw new Error(data.detail || '加载失败');
    renderNeo4jStateModal(body, data, projectName);
  } catch (e) {
    body.innerHTML = `<div class="empty-state" style="color: var(--red);">失败：${escapeHTML(e.message)}</div>`;
  }
}

function renderNeo4jStateModal(body, data, projectName) {
  if (!data.has_part) {
    body.innerHTML = `
      <div class="empty-state">该项目尚无可查询的实体图。</div>
    `;
    return;
  }
  const buckets = data.buckets || [];
  const max = buckets.reduce((m, b) => Math.max(m, b.count), 0) || 1;
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>项目名</th><td>${escapeHTML(projectName)}</td></tr>
      <tr><th>项目 ID</th><td>${escapeHTML(data.project_id)}</td></tr>
      <tr><th>图标识</th><td>${escapeHTML(data.part_name)}</td></tr>
      <tr><th>当前版本</th><td>${data.part_version}</td></tr>
      <tr><th>实体节点数</th><td>${data.total_nodes}</td></tr>
      <tr><th>存储模式</th><td>${escapeHTML(data.storage_label || '—')}</td></tr>
    </table>
    <h4 style="margin-top: 22px; color: var(--text-bright); font-size: var(--fz-md);">ACIS 拓扑节点分布</h4>
    <div style="margin-top: 10px;">
      ${buckets.map(b => `
        <div class="bar-row">
          <div class="bar-label">${escapeHTML(b.label_zh || b.label)}</div>
          <div class="bar-track"><div class="bar-fill" style="width: ${(b.count / max * 100).toFixed(1)}%"></div></div>
          <div class="bar-val">${b.count.toLocaleString()}</div>
        </div>
      `).join('')}
    </div>
    <h4 style="margin-top: 24px; color: var(--text-bright); font-size: var(--fz-md);">Body 列表（前 ${Math.min(data.bodies?.length || 0, 30)} 个，共 ${data.body_count}）</h4>
    <div class="topo-strip">
      ${(data.bodies || []).slice(0, 30).map(b => `
        <div class="topo-pill" title="${escapeHTML(b.uuid)}">body <span class="count">${escapeHTML(b.uuid.slice(0, 8))}…</span></div>
      `).join('') || '<div class="muted">无 body</div>'}
    </div>
    ${renderNeo4jGraph(data.graph)}
    <p class="muted mt-16" style="font-size: var(--fz-sm);">
      【解读】图中 Body 是可理解的 CAD 实体根节点；Lump、Shell、Face、Loop、Coedge、Edge、Vertex 是 ACIS 几何拓扑的组成层级。点击节点可查看原始 Neo4j 属性。
    </p>
  `;
  const graphRoot = body.querySelector('[data-neo4j-graph]');
  if (graphRoot) bindNeo4jGraph(graphRoot, data.graph);
}

function renderNeo4jGraph(graph) {
  if (!graph || !graph.nodes || !graph.nodes.length) return '<div class="neo4j-graph-detail">当前版本没有可绘制的实体图。</div>';
  const nodes = graph.nodes.slice(0, 120);
  const rels = (graph.rels || []).filter(r => nodes.some(n => n.id === r.start) && nodes.some(n => n.id === r.end)).slice(0, 240);
  const width = 900, height = 460, cx = width / 2, cy = height / 2;
  const positions = {};
  const bodyNodes = nodes.filter(n => (n.labels || []).map(String).map(x => x.toLowerCase()).includes('body'));
  nodes.forEach((n, i) => {
    const root = (n.labels || []).map(String).map(x => x.toLowerCase()).includes('body');
    const radius = root ? 90 : 170;
    const angle = root ? (bodyNodes.indexOf(n) * Math.PI * 2 / Math.max(1, bodyNodes.length)) : (i * 2.399);
    positions[n.id] = { x: cx + (root ? radius * Math.cos(angle) : radius * Math.cos(angle)), y: cy + (root ? radius * Math.sin(angle) : radius * Math.sin(angle)) };
  });
  const presentTypes = [...new Set(nodes.flatMap(n => (n.labels || []).map(x => String(x).toLowerCase())))];
  const legend = presentTypes.map(key => `${escapeHTML(topologyLabelZh(key))}`).join('　·　');
  return `<div data-neo4j-graph class="neo4j-graph-wrap"><div class="muted" style="font-size: var(--fz-xs);">Neo4j 实际存储图（展示前 ${nodes.length} 个节点 / ${rels.length} 条关系；点击节点查看属性和相邻关系）</div><svg viewBox="0 0 ${width} ${height}" role="img" aria-label="Neo4j CAD 拓扑图">
    ${rels.map(r => { const a=positions[r.start], b=positions[r.end]; return a&&b ? `<line x1="${a.x}" y1="${a.y}" x2="${b.x}" y2="${b.y}" stroke="var(--border)" stroke-width="1" opacity=".7"><title>${escapeHTML(r.type || '关系')}</title></line>` : ''; }).join('')}
    ${nodes.map(n => { const p=positions[n.id], key=String((n.labels||[])[0]||'entity').toLowerCase(), body=key==='body'; return `<g class="neo4j-graph-node" data-node-id="${escapeAttr(n.id)}" transform="translate(${p.x},${p.y})"><circle r="${body?18:8}" fill="${body?'var(--mode1)':'var(--surface)'}" stroke="var(--cyan)" stroke-width="${body?3:1.5}"/><text y="${body?34:20}" text-anchor="middle" fill="var(--text)" font-size="${body?13:10}">${escapeHTML(topologyLabelZh(key))}</text></g>`; }).join('')}
  </svg><div class="neo4j-graph-legend">${legend || '实体节点'}　·　线条表示 Neo4j 中实际保存的拓扑关系</div><div class="neo4j-graph-detail" data-graph-detail>点击节点查看 UUID、属性和相邻关系</div></div>`;
}

function _topologyLabel(key) {
  return ({body:'Body', lump:'Lump', shell:'Shell', face:'Face', loop:'Loop', coedge:'Coedge', edge:'Edge', vertex:'Vertex', transform:'Transform'})[key] || key;
}

function topologyLabelZh(key) {
  const k = String(key || '').toLowerCase();
  return ({body:'Body 实体', lump:'Lump 体块', shell:'Shell 壳体', face:'Face 面', loop:'Loop 环', coedge:'Coedge 半边', edge:'Edge 边', vertex:'Vertex 顶点', transform:'Transform 变换'})[k] || _topologyLabel(k) || '实体节点';
}

function bindNeo4jGraph(root, graph) {
  const detail = root.querySelector('[data-graph-detail]');
  root.querySelectorAll('[data-node-id]').forEach(node => node.addEventListener('click', () => {
    const item = (graph.nodes || []).find(n => n.id === node.dataset.nodeId);
    if (!item || !detail) return;
    const key = String((item.labels || ['entity'])[0]).toLowerCase();
    const adjacent = (graph.rels || []).filter(r => r.start === item.id || r.end === item.id).slice(0, 12).map(r => {
      const otherId = r.start === item.id ? r.end : r.start;
      const other = (graph.nodes || []).find(n => n.id === otherId);
      const otherKey = String((other?.labels || ['entity'])[0]).toLowerCase();
      return `${r.start === item.id ? '→' : '←'}${r.type || '关系'} ${topologyLabelZh(otherKey)}`;
    });
    detail.textContent = `${topologyLabelZh(key)} · ${item.id} · 属性 ${JSON.stringify(item.props || {})}${adjacent.length ? ` · 相邻：${adjacent.join('；')}` : ''}`;
  }));
}
