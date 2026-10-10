function renderModeCol(cls, title, sub, items, total, mode) {
  const body = items.length === 0
    ? `<div class="empty-state">该模式还没有版本</div>`
    : items.slice().reverse().map(v => {
        const vnum = v.version;
        return `
          <div class="version-item" data-mode="${mode}" data-version="${vnum}">
            <div class="version-num">#${vnum}</div>
            <div class="version-meta">
              <div style="color: var(--text); font-size: var(--fz-sm);">
                ${escapeHTML(v.author || 'anon')}
                ${v.part_name ? `<span class="mono muted" style="margin-left: 6px;">${escapeHTML(v.part_name)}</span>` : ''}
              </div>
              <div class="muted" style="font-size: var(--fz-xs); margin-top: 2px;">
                ${v.graph_bytes ? `${(v.graph_bytes/1024).toFixed(1)} KB graph` : ''}
                ${v.manifest_chars ? `${v.manifest_chars.toLocaleString()} 字符版本清单` : ''}
                ${v.delta_uuid_count !== undefined && v.delta_uuid_count >= 0
                  ? ` · +${v.delta_uuid_count} bodies · -${v.removed_uuid_count || 0}`
                  : ''}
              </div>
            </div>
            <div class="version-time">${fmtTime(v.created_at)}</div>
          </div>
        `;
      }).join('');
  return `
    <div class="mode-col ${cls}">
      <div class="mode-col-head">
        <div class="mode-col-title">
          ${title} <small>${sub} · ${total}</small>
        </div>
      </div>
      <div class="mode-col-body">${body}</div>
    </div>
  `;
}

async function openVersionDetail(project, mode, version) {
  const url =
    mode === 'mode0' ? `${API}/projects/${encodeURIComponent(project)}/mode0/${version}`
    : mode === 'mode1' ? `${API}/projects/${encodeURIComponent(project)}/json/${version}`
    : `${API}/projects/${encodeURIComponent(project)}/eg/${version}`;

  openModal(`版本详情 · ${mode.toUpperCase()} v${version}`);
  const body = $('#modal-body');
  body.innerHTML = '<div class="muted center" style="padding: 20px;">加载中…</div>';
  try {
    const d = await getJSON(url);
    if (mode === 'mode0') renderMode0Modal(d, body, project, url);
    else if (mode === 'mode1') renderJsonMode1Modal(d, body, project, url);
    else renderEGModal(d, body, project);
  } catch (e) {
    body.innerHTML = `<div class="empty-state" style="color: var(--red);">失败：${escapeHTML(e.message)}</div>`;
  }
}

async function revealRaw(url, targetId, button) {
  button.disabled = true; button.textContent = '读取中…';
  try {
    const d = await getJSON(url + (url.includes('?') ? '&' : '?') + 'include_raw=true');
    const target = document.getElementById(targetId);
    target.textContent = d.raw_push_json || d.raw_sat || '历史版本没有保存原始 Push 文本';
    target.parentElement.hidden = false; button.hidden = true;
  } catch (e) { button.disabled = false; button.textContent = '查看原始 Push'; toast(e.message, 'error'); }
}
function auditRows(d, mode) {
  const timingScope = mode === 'mode1'
    ? 'Neo4j 图版本事务；含原始请求归档，不含随后计时值写入'
    : '等待项目写锁及 Mode0 版本写入；不含随后审计写入';
  return '<tr><th>Push 格式</th><td>' + escapeHTML(d.payload_format || '历史版本') + '</td></tr>'
    + '<tr><th>是否含 SAT</th><td>' + (d.contains_sat ? '是' : '否') + '</td></tr>'
    + '<tr><th>请求消息字节</th><td>' + (d.request_bytes ? Number(d.request_bytes).toLocaleString() + ' B（UTF-8；不含传输协议开销）' : '历史版本无审计数据') + '</td></tr>'
    + '<tr><th>SAT 快照字节</th><td>' + Number(d.sat_bytes || 0).toLocaleString() + '</td></tr>'
    + '<tr><th>服务端写入计时</th><td>' + (d.db_write_ms == null ? '历史版本无审计数据' : Number(d.db_write_ms).toFixed(3) + ' ms；' + timingScope + '。不含网络与客户端处理；两模式口径不同') + '</td></tr>';
}

function renderMode0Modal(d, body, project, base) {
  const rows = d.topology_by_label.map(t =>
    `<tr><th>${escapeHTML(topologyLabelZh(t.label))}<div class="muted" style="font-size: var(--fz-xs);">Neo4j label: ${escapeHTML(t.label)}</div></th><td>${t.count.toLocaleString()}</td></tr>`
  ).join('');
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>版本号</th><td>#${d.version}</td></tr>
      <tr><th>作者</th><td>${escapeHTML(d.author)}</td></tr>
      <tr><th>创建时间</th><td>${fmtTime(d.created_at)}</td></tr>
      <tr><th>所属项目</th><td>${escapeHTML(project)}</td></tr>
      <tr><th>Part 名</th><td>${escapeHTML(d.part_name)}</td></tr>
      ${auditRows(d, 'mode0')}
    </table>
    <button class="btn ghost mt-16" onclick="revealRaw('${base}', 'raw-mode0-${d.version}', this)">查看原始 Push / SAT 快照</button>
    <div hidden class="raw-audit"><pre id="raw-mode0-${d.version}"></pre></div>
    <h4 style="margin-top: 18px; color: var(--text-bright); font-size: var(--fz-md);">Mode 0 拓扑分布（基于 part 实体）</h4>
    <table class="kv-table">
      <tr><th>label</th><th>count</th></tr>
      ${rows || '<tr><th colspan="2" class="muted">未找到 part 拓扑节点</th></tr>'}
    </table>
  `;
}

function renderJsonMode1Modal(d, body, project, base) {
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>版本号</th><td>#${d.version}</td></tr>
      <tr><th>作者</th><td>${escapeHTML(d.author)}</td></tr>
      <tr><th>创建时间</th><td>${fmtTime(d.created_at)}</td></tr>
      ${auditRows(d, 'mode1')}
      <tr><th>图哈希</th><td class="mono">${escapeHTML(d.graph_hash || '')}</td></tr>
    </table>
    <button class="btn ghost mt-16" onclick="revealRaw('${base}', 'raw-mode1-${d.version}', this)">查看原始 JSON Push</button>
    <div hidden class="raw-audit"><pre id="raw-mode1-${d.version}"></pre></div>
    <h3>实体类型</h3>
    <table class="kv-table">${(d.entity_counts || []).map(item =>
      `<tr><th>${escapeHTML((item.labels || []).map(topologyLabelZh).join('、'))}<div class="muted" style="font-size: var(--fz-xs);">Neo4j label: ${escapeHTML((item.labels || []).join(', '))}</div></th><td>${item.count}</td></tr>`
    ).join('')}</table>`;
}

function renderEGModal(d, body, project) {
  const lbls = d.label_breakdown.map(t =>
    `<tr><th>${escapeHTML(t.label)}</th><td>${t.count.toLocaleString()}</td></tr>`
  ).join('');
  const rels = d.rel_type_breakdown.map(t =>
    `<tr><th>${escapeHTML(t.type)}</th><td>${t.count.toLocaleString()}</td></tr>`
  ).join('');
  body.innerHTML = `
    <table class="kv-table">
      <tr><th>版本号</th><td>#${d.version}</td></tr>
      <tr><th>作者</th><td>${escapeHTML(d.author)}</td></tr>
      <tr><th>创建时间</th><td>${fmtTime(d.created_at)}</td></tr>
      <tr><th>所属项目</th><td>${escapeHTML(project)}</td></tr>
      <tr><th>图 JSON 长度</th><td>${(d.graph_bytes/1024).toFixed(2)} KB (${d.graph_bytes.toLocaleString()} B)</td></tr>
      <tr><th>节点 / 关系</th><td>${d.node_count.toLocaleString()} nodes · ${d.rel_count.toLocaleString()} rels</td></tr>
      <tr><th>Part 名</th><td>${escapeHTML(d.part_name)}</td></tr>
    </table>
    <div class="row mt-16" style="gap: 18px; align-items: flex-start;">
      <div style="flex: 1;">
        <h4 style="color: var(--text-bright); font-size: var(--fz-md); margin: 0 0 8px;">节点 Label 分布</h4>
        <table class="kv-table">${lbls || '<tr><th colspan="2" class="muted">空</th></tr>'}</table>
      </div>
      <div style="flex: 1;">
        <h4 style="color: var(--text-bright); font-size: var(--fz-md); margin: 0 0 8px;">关系类型分布</h4>
        <table class="kv-table">${rels || '<tr><th colspan="2" class="muted">空</th></tr>'}</table>
      </div>
    </div>
  `;
}

function openModal(title) {
  $('#modal-title').innerHTML = title;
  $('#modal').classList.add('show');
}
function closeModal() { $('#modal').classList.remove('show'); }

// 搜索
$('#project-search').addEventListener('input', () => renderProjects());
$('#btn-refresh').addEventListener('click', loadProjects);

// ============== 数据浏览器 ==============
$('#cypher-run').addEventListener('click', runCypher);
$('#cypher-clear').addEventListener('click', () => {
  $('#cypher-input').value = '';
  $('#cypher-result').innerHTML = '';
});
$('#cypher-input').addEventListener('keydown', e => {
  if (e.key === 'Enter' && (e.ctrlKey || e.metaKey)) runCypher();
});
async function runCypher() {
  const q = $('#cypher-input').value.trim();
  if (!q) { toast('请输入 Cypher 查询', 'error'); return; }
  const out = $('#cypher-result');
  out.innerHTML = '<div class="muted">执行中…</div>';
  try {
    const r = await adminFetch(`${API}/query`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ query: q, params: {} }),
    });
    const data = await r.json();
    if (!r.ok || data.error) {
      out.innerHTML = `<div class="row-result" style="color: var(--red); border-color: var(--red);">${escapeHTML(data.error || data.detail || '查询失败')}</div>`;
      return;
    }
    out.innerHTML = `<div class="muted" style="margin-bottom: 10px;">${data.count} 条结果</div>` +
      data.rows.slice(0, 200).map(r => `<div class="row-result">${escapeHTML(JSON.stringify(r, null, 2))}</div>`).join('');
  } catch (e) {
    out.innerHTML = `<div class="row-result" style="color: var(--red);">${escapeHTML(e.message)}</div>`;
  }
}

// Modal 关闭
$('#modal').addEventListener('click', e => {
  if (e.target.id === 'modal') closeModal();
});

// ============== 工具 ==============
function escapeHTML(s) {
  if (s == null) return '';
  return String(s)
    .replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;')
    .replace(/"/g,'&quot;').replace(/'/g,'&#39;');
}
function escapeAttr(s) { return escapeHTML(s); }
function cssEscape(s) { return String(s).replace(/"/g, '\\"'); }

// 启动
loadOverview();
