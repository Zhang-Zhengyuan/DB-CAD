// V1 版本详情（组件清单 + 实体投影分布）与全局交互绑定。
async function openVersionDetail(project, version) {
  const body = openModal(`v${version} · ${project}`);
  body.innerHTML = '<div class="muted center" style="padding: 16px;">加载版本详情…</div>';
  try {
    const d = await getJSON(`${API}/projects/${encodeURIComponent(project)}/versions/${version}`);
    renderVersionModal(d, body);
  } catch (e) {
    body.innerHTML = `<div class="empty-state" style="color: var(--red);">加载失败：${escapeHTML(e.message)}</div>`;
  }
}

function renderVersionModal(d, body) {
  const entities = d.entity_counts || [];
  const entMax = entities[0]?.count || 1;
  body.innerHTML = `
    <div class="muted" style="margin-bottom: 12px; font-size: var(--fz-sm);">
      ${escapeHTML(d.message || '—')} · ${escapeHTML(d.author || '')} · ${fmtTime(d.created_at)}
      · 父版本 <span class="mono">${d.base_version != null ? 'v' + d.base_version : 'root'}</span>
    </div>
    <div class="section-title">组件清单（${d.component_count}）</div>
    <div class="table-wrap"><table><thead><tr>
      <th>#</th><th>名称</th><th>hash_sat</th><th>三角</th><th>实体投影</th><th>覆盖</th><th>操作</th>
    </tr></thead><tbody>
      ${(d.components || []).map((c, i) => `
        <tr>
          <td class="muted">${i + 1}</td>
          <td>${escapeHTML(c.name || 'component')}</td>
          <td class="mono muted" style="font-size: var(--fz-xs);">${(c.hash_sat || '').slice(0, 12)}…</td>
          <td>${(c.triangles || 0).toLocaleString()}</td>
          <td>${c.entity_count}</td>
          <td>${c.fallback === 'fine_grained'
            ? '<span class="badge mode1">细粒度</span>'
            : c.fallback === 'sat_asset'
              ? '<span class="badge mode0">SAT 兜底</span>'
              : '<span class="badge zero">无投影</span>'}</td>
          <td><button class="btn ghost" data-hash="${escapeAttr(c.hash_sat)}">投影图</button></td>
        </tr>`).join('')}
    </tbody></table></div>
    <div class="section-title mt-16">实体标签分布</div>
    <div class="bar-chart">
      ${entities.map(row => `
        <div class="bar-row">
          <div class="bar-label">${escapeHTML(topologyLabelZh(row.label))}</div>
          <div class="bar-track"><div class="bar-fill" style="width: ${(row.count / entMax * 100).toFixed(1)}%"></div></div>
          <div class="bar-val">${row.count.toLocaleString()}</div>
        </div>`).join('') || '<div class="muted">该版本组件暂无实体投影（可用 backend/scripts/rebuild_entity_projections.py 补齐）</div>'}
    </div>
  `;
  body.querySelectorAll('button[data-hash]').forEach(btn => {
    btn.addEventListener('click', () => openEntityGraph(btn.dataset.hash));
  });
}

function openModal(title) {
  $('#modal-title').innerHTML = title;
  $('#modal').classList.add('show');
  return $('#modal-body');
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
    .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;').replace(/'/g, '&#39;');
}
function escapeAttr(s) { return escapeHTML(s); }
function cssEscape(s) { return String(s).replace(/"/g, '\\"'); }

// 启动
loadOverview();
