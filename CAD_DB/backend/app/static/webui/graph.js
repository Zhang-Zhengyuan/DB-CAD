// ============== 组件实体级投影图（P6/ADR-3） ==============

async function openEntityGraph(hashSat, label) {
  const short = String(hashSat || '').slice(0, 12);
  const body = openModal(`实体投影 · ${short}…`);
  body.innerHTML = '<div class="muted center" style="padding: 30px;">加载投影图…</div>';
  try {
    const url = `${API}/components/${encodeURIComponent(hashSat)}/entity-graph?limit=300` +
      (label ? `&label=${encodeURIComponent(label)}` : '');
    const data = await getJSON(url);
    renderEntityGraphModal(body, data);
  } catch (e) {
    body.innerHTML = `<div class="empty-state" style="color: var(--red);">失败：${escapeHTML(e.message)}</div>`;
  }
}

function renderEntityGraphModal(body, data) {
  const graph = {
    nodes: (data.nodes || []).map(n => ({
      id: n.id,
      labels: n.labels && n.labels.length ? n.labels : ['entity'],
      props: { entity_type: n.entity_type, geometry_type: n.geometry_type, top_level: n.top_level },
    })),
    rels: data.rels || [],
  };
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>hash_sat</th><td class="mono">${escapeHTML(data.hash_sat || '')}</td></tr>
      <tr><th>组件名</th><td>${escapeHTML(data.name || 'component')}</td></tr>
      <tr><th>实体总数</th><td>${(data.total_entities || 0).toLocaleString()}（展示 ${data.returned || 0}${data.truncated ? '，已截断' : ''}）</td></tr>
      <tr><th>拓扑关系</th><td>${(data.rels || []).length.toLocaleString()} 条 TOPOLOGY 边</td></tr>
    </table>
    ${renderNeo4jGraph(graph)}
    <p class="muted mt-16" style="font-size: var(--fz-sm);">
      【解读】投影挂在不可变组件之下（HAS_ENTITY）；Body→Lump→Shell→Face→Loop→Coedge/Edge→Vertex
      为 ACIS 拓扑层级，边为库内实际保存的 TOPOLOGY 关系。点击节点查看属性与相邻关系。
    </p>
  `;
  const graphRoot = body.querySelector('[data-neo4j-graph]');
  if (graphRoot) bindNeo4jGraph(graphRoot, graph);
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
