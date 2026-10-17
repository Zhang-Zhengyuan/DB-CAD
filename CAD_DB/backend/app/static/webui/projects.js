async function loadProjects() {
  const tb = $('#project-tbody');
  tb.innerHTML = '<tr><td colspan="7" class="empty-state">正在加载项目列表…</td></tr>';
  try {
    const projects = await getJSON(`${API}/projects`);
    ALL_PROJECTS = projects;
    renderProjects();

    const totalMode0 = projects.reduce((s,p) => s + p.mode0_version_count, 0);
    const totalMode1 = projects.reduce((s,p) => s + p.mode1_json_count, 0);
    const totalEG    = projects.reduce((s,p) => s + p.entity_graph_version_count, 0);

    $('#project-stats').innerHTML = `
      <div class="stat-card project">
        <div class="label">项目总数</div>
        <div class="value">${projects.length.toLocaleString()}</div>
        <div class="sub">Project / BridgeProject / DBCADProject</div>
      </div>
      <div class="stat-card mode0">
        <div class="label">旧 Mode0 历史</div>
        <div class="value">${totalMode0.toLocaleString()}</div>
        <div class="sub">累计完整 SAT 版本</div>
      </div>
      <div class="stat-card mode1">
        <div class="label">Mode1 JSON 版本</div>
        <div class="value">${totalMode1.toLocaleString()}</div>
        <div class="sub">累计 JSON 图版本</div>
      </div>
      <div class="stat-card eg">
        <div class="label">Entity Graph 总版本</div>
        <div class="value">${totalEG.toLocaleString()}</div>
        <div class="sub">累计 Entity Graph 版本</div>
      </div>
    `;

    $('#project-count').textContent = `共 ${projects.length} 个项目`;
  } catch (e) {
    tb.innerHTML = `<tr><td colspan="7" class="empty-state">加载失败：${e.message}</td></tr>`;
    toast('加载项目失败：' + e.message, 'error');
  }
}

function renderProjects() {
  const kw = $('#project-search').value.trim().toLowerCase();
  const list = kw ? ALL_PROJECTS.filter(p => (p.name||'').toLowerCase().includes(kw)) : ALL_PROJECTS;
  const tb = $('#project-tbody');
  if (list.length === 0) {
    tb.innerHTML = '<tr><td colspan="7" class="empty-state">无匹配项目</td></tr>';
    return;
  }
  tb.innerHTML = list.map(p => projectRowHTML(p)).join('');
  // 绑定 row 事件
  $$('#project-tbody tr.project-row').forEach(tr => {
    const name = tr.dataset.name;
    tr.querySelector('.cell-name').addEventListener('click', () => toggleProject(name, tr));
    tr.querySelector('.expand-caret').addEventListener('click', () => toggleProject(name, tr));
  });
}

function badge(mode, n) {
  if (!n) return `<span class="badge zero">0</span>`;
  const cls = (mode === 'mode0') ? 'mode0' : (mode === 'mode1' ? 'mode1' : 'eg');
  const label = (mode === 'mode0') ? 'M0' : (mode === 'mode1' ? 'M1' : 'EG');
  return `<span class="badge ${cls}">${label} × ${n}</span>`;
}

function fmtTime(s) {
  if (!s) return '<span class="muted">—</span>';
  try {
    const d = new Date(s);
    if (isNaN(d.getTime())) return s;
    const pad = (n) => String(n).padStart(2,'0');
    return `${d.getFullYear()}-${pad(d.getMonth()+1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}`;
  } catch (_) { return s; }
}

function projectRowHTML(p) {
  const latest = []
    .concat(p.mode0_latest ? ['M0#'+p.mode0_latest] : [])
    .concat(p.mode1_latest ? ['M1#'+p.mode1_latest] : [])
    .concat(p.entity_graph_latest ? ['EG#'+p.entity_graph_latest] : []);
  return `
    <tr class="project-row" data-name="${escapeAttr(p.name)}">
      <td class="center"><span class="expand-caret">▶</span></td>
      <td>
        <span class="cell-name">
          <span>${escapeHTML(p.name)}</span>
        </span>
        <span class="pid">${(p.project_id||'').slice(0,8)}</span>
      </td>
      <td class="center">${badge('mode0', p.mode0_version_count)}</td>
      <td class="center">${badge('mode1', p.mode1_json_count)}</td>
      <td class="center">${badge('eg', p.entity_graph_version_count)}</td>
      <td class="center"><span class="mono muted" style="font-size: var(--fz-sm);">${latest.length ? latest.join(' / ') : '—'}</span></td>
      <td class="right muted" style="font-size: var(--fz-sm);">${fmtTime(p.updated_at)}</td>
    </tr>
    <tr class="detail-row" data-name="${escapeAttr(p.name)}" style="display: none;">
      <td colspan="7">
        <div class="detail-panel" id="detail-${escapeAttr(p.name)}">
          <div class="muted center" style="padding: 16px;">加载中…</div>
        </div>
      </td>
    </tr>
  `;
}

const expanded = new Set();

async function toggleProject(name, tr) {
  const detailTr = document.querySelector(`tr.detail-row[data-name="${cssEscape(name)}"]`);
  if (!detailTr) return;
  if (expanded.has(name)) {
    expanded.delete(name);
    detailTr.style.display = 'none';
    tr.classList.remove('expanded');
    return;
  }
  expanded.add(name);
  detailTr.style.display = '';
  tr.classList.add('expanded');

  const detailEl = detailTr.querySelector('.detail-panel');
  try {
    const d = await getJSON(`${API}/projects/${encodeURIComponent(name)}`);
    renderDetail(d, detailEl);
  } catch (e) {
    detailEl.innerHTML = `<div class="empty-state" style="color: var(--red);">加载失败：${escapeHTML(e.message)}</div>`;
  }
}

function renderDetail(d, el) {
  const totalM0 = d.mode0_versions.length;
  const totalM1 = d.mode1_json_versions.length;
  const totalEG = d.entity_graph_versions.length;

  el.innerHTML = `
    <div class="row" style="gap: 16px; margin-bottom: 18px; align-items: flex-start;">
      <div style="flex: 1;">
        <div style="font-size: var(--fz-xl); font-weight: 600; color: var(--text-bright); letter-spacing: -0.3px;">${escapeHTML(d.project.name)}</div>
        <div class="muted" style="font-size: var(--fz-sm); margin-top: 6px;">
          ID <span class="mono">${escapeHTML(d.project.id)}</span>
          · 创建 ${fmtTime(d.project.created_at)}
          · 更新 ${fmtTime(d.project.updated_at)}
        </div>
      </div>
      <button class="btn ghost" data-action="open-graph">查看 Neo4j 当前图状态 →</button>
    </div>
    <div id="neo4j-state-${cssEscape(d.project.id)}"></div>
    <div class="detail-grid">
      ${renderModeCol('mode0', '旧 Mode 0', '只读历史记录', d.mode0_versions, totalM0, 'mode0')}
      ${renderModeCol('mode1', 'Mode 1', 'JSON 图版本', d.mode1_json_versions, totalM1, 'mode1')}
      ${renderModeCol('eg', 'Entity Graph', '图版本', d.entity_graph_versions, totalEG, 'eg')}
    </div>
    ${renderMode1History(d.mode1_history_chain || [])}
  `;

  // 加载 Neo4j 当前图状态
  loadNeo4jState(d.project.id);

  // 绑定版本点击
  el.querySelectorAll('.version-item').forEach(v => {
    v.addEventListener('click', () => {
      const project = d.project.name;
      const mode = v.dataset.mode;
      const version = v.dataset.version;
      openVersionDetail(project, mode, version);
    });
  });

  // 绑定"查看 Neo4j 当前图状态"按钮
  const openGraphBtn = el.querySelector('[data-action="open-graph"]');
  if (openGraphBtn) {
    openGraphBtn.addEventListener('click', () => {
      openNeo4jStateModal(d.project.name, d.project.id);
    });
  }
}

function renderMode1History(history) {
  if (!history.length) return '';
  return `<div class="mt-16">
    <div class="section-title">Mode1 增量存储历史链</div>
    <div class="table-wrap"><table><thead><tr>
      <th>版本</th><th>提交人</th><th>本次变更</th><th>组件数</th><th>跨组件关系</th><th>存储模式</th><th>时间</th>
    </tr></thead><tbody>${history.slice().reverse().map(v => `<tr>
      <td class="mono">v${v.version}</td><td>${escapeHTML(v.author)}</td>
      <td>${renderChangeSummary(v)}</td><td>${v.component_count}<div class="muted" style="font-size: var(--fz-xs);">本次新增/替换 ${v.changed_component_count ?? '—'} · 复用 ${v.reused_component_count ?? '—'}</div></td><td>${v.relation_count}</td>
      <td><span class="badge mode1">${escapeHTML(v.storage_mode)}</span></td>
      <td class="muted">${fmtTime(v.created_at)}</td>
    </tr>`).join('')}</tbody></table></div>
    <div class="muted mt-8" style="font-size: var(--fz-xs);">组件数是该版本引用的 Body 拓扑组件数；跨组件关系是组件之间的设计依赖关系。Coedge、Edge 等是 Body 内部拓扑节点。</div>
  </div>`;
}

function renderChangeSummary(v) {
  const changes = v.changes || [];
  if (!changes.length) return '<span class="muted">无显式变更记录</span>';
  const groups = {};
  for (const c of changes) {
    const key = c.change_type || 'CHANGE';
    groups[key] = (groups[key] || 0) + 1;
  }
  const parts = Object.entries(groups).map(([key, count]) => {
    const label = key === 'ADD' ? '新增' : key === 'REMOVE' ? '删除' : key === 'MODIFY' ? '修改' : key;
    return `${label} ${count}`;
  });
  const types = (v.new_entity_types || []).map(t => escapeHTML(topologyLabelZh(t))).join('、');
  return `${parts.join(' · ')}${types ? `<div class="muted" style="font-size: var(--fz-xs); margin-top: 3px;">类型：${types}</div>` : ''}`;
}
