async function loadProjects() {
  const tb = $('#project-tbody');
  tb.innerHTML = '<tr><td colspan="6" class="empty-state">正在加载项目列表…</td></tr>';
  try {
    const projects = await getJSON(`${API}/projects`);
    ALL_PROJECTS = projects;
    renderProjects();

    const totalVersions = projects.reduce((s, p) => s + p.version_count, 0);
    const totalComponents = projects.reduce((s, p) => s + p.head_component_count, 0);

    $('#project-stats').innerHTML = `
      <div class="stat-card project">
        <div class="label">项目总数</div>
        <div class="value">${projects.length.toLocaleString()}</div>
        <div class="sub">V1Project</div>
      </div>
      <div class="stat-card mode0">
        <div class="label">版本总数</div>
        <div class="value">${totalVersions.toLocaleString()}</div>
        <div class="sub">V1Version（命令全序=版本链）</div>
      </div>
      <div class="stat-card mode1">
        <div class="label">head 组件合计</div>
        <div class="value">${totalComponents.toLocaleString()}</div>
        <div class="sub">不可变组件（内容寻址，跨版本复用）</div>
      </div>
    `;

    $('#project-count').textContent = `共 ${projects.length} 个项目`;
  } catch (e) {
    tb.innerHTML = `<tr><td colspan="6" class="empty-state">加载失败：${e.message}</td></tr>`;
    toast('加载项目失败：' + e.message, 'error');
  }
}

function renderProjects() {
  const kw = $('#project-search').value.trim().toLowerCase();
  const list = kw ? ALL_PROJECTS.filter(p => (p.name || '').toLowerCase().includes(kw)) : ALL_PROJECTS;
  const tb = $('#project-tbody');
  if (list.length === 0) {
    tb.innerHTML = '<tr><td colspan="6" class="empty-state">无匹配项目</td></tr>';
    return;
  }
  tb.innerHTML = list.map(p => projectRowHTML(p)).join('');
  $$('#project-tbody tr.project-row').forEach(tr => {
    const name = tr.dataset.name;
    tr.querySelector('.cell-name').addEventListener('click', () => toggleProject(name, tr));
    tr.querySelector('.expand-caret').addEventListener('click', () => toggleProject(name, tr));
  });
}

function fmtTime(s) {
  if (!s) return '<span class="muted">—</span>';
  try {
    const d = new Date(s);
    if (isNaN(d.getTime())) return s;
    const pad = (n) => String(n).padStart(2, '0');
    return `${d.getFullYear()}-${pad(d.getMonth() + 1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}`;
  } catch (_) { return s; }
}

function projectRowHTML(p) {
  return `
    <tr class="project-row" data-name="${escapeAttr(p.name)}">
      <td class="center"><span class="expand-caret">▶</span></td>
      <td>
        <span class="cell-name"><span>${escapeHTML(p.name)}</span></span>
        <span class="pid">${(p.project_id || '').slice(0, 8)}</span>
      </td>
      <td class="center"><span class="badge mode1">v × ${p.version_count}</span></td>
      <td class="center"><span class="badge eg">comp × ${p.head_component_count}</span></td>
      <td class="center"><span class="mono muted" style="font-size: var(--fz-sm);">${p.latest_version != null ? 'v' + p.latest_version : '—'}</span></td>
      <td class="right muted" style="font-size: var(--fz-sm);">${fmtTime(p.updated_at)}</td>
    </tr>
    <tr class="detail-row" data-name="${escapeAttr(p.name)}" style="display: none;">
      <td colspan="6">
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
  const versions = d.versions || [];
  el.innerHTML = `
    <div class="row" style="gap: 16px; margin-bottom: 18px; align-items: flex-start;">
      <div style="flex: 1;">
        <div style="font-size: var(--fz-xl); font-weight: 600; color: var(--text-bright); letter-spacing: -0.3px;">${escapeHTML(d.project.name)}</div>
        <div class="muted" style="font-size: var(--fz-sm); margin-top: 6px;">
          ID <span class="mono">${escapeHTML(d.project.id)}</span>
          · 创建 ${fmtTime(d.project.created_at)}
          · 更新 ${fmtTime(d.project.updated_at)}
          · head <span class="mono">${d.head_version != null ? 'v' + d.head_version : '—'}</span>
        </div>
      </div>
    </div>
    <div class="mt-8">
      <div class="section-title">版本链（命令全序）</div>
      <div class="table-wrap"><table><thead><tr>
        <th>版本</th><th>命令</th><th>提交人</th><th>父版本</th><th>组件</th><th>实体投影</th><th>时间</th>
      </tr></thead><tbody>
        ${versions.slice().reverse().map(v => `
          <tr class="version-item" data-version="${v.version}" style="cursor: pointer;">
            <td class="mono">v${v.version}</td>
            <td><span class="mono" style="font-size: var(--fz-sm);">${escapeHTML(v.message || '—')}</span></td>
            <td>${escapeHTML(v.author || '—')}</td>
            <td class="mono muted">${v.base_version != null ? 'v' + v.base_version : 'root'}</td>
            <td>${v.component_count}
              <div class="muted" style="font-size: var(--fz-xs);">变更 ${v.changed_component_count} · 复用 ${v.reused_component_count}</div>
            </td>
            <td>${v.entity_count}</td>
            <td class="muted">${fmtTime(v.created_at)}</td>
          </tr>`).join('')}
      </tbody></table></div>
      <div class="muted mt-8" style="font-size: var(--fz-xs);">
        点击版本行查看组件清单与实体投影分布；组件不可变（内容寻址），同几何跨版本自动复用。
      </div>
    </div>
  `;
  el.querySelectorAll('tr.version-item').forEach(row => {
    row.addEventListener('click', () => openVersionDetail(d.project.name, row.dataset.version));
  });
}
