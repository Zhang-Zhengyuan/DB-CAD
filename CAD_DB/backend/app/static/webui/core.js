const API = '/api/neo4j';
let adminPassword = sessionStorage.getItem('dbcad_admin_password') || '';
async function adminFetch(url, options={}) {
  const headers = {...(options.headers || {})};
  if (adminPassword) headers['X-API-Password'] = adminPassword;
  let response = await fetch(url, {...options, headers});
  if (response.status === 401) {
    const entered = window.prompt('请输入管理员 API 密码');
    if (!entered) return response;
    adminPassword = entered;
    sessionStorage.setItem('dbcad_admin_password', entered);
    headers['X-API-Password'] = entered;
    response = await fetch(url, {...options, headers});
  }
  return response;
}

const $ = (s, p=document) => p.querySelector(s);
const $$ = (s, p=document) => [...p.querySelectorAll(s)];

function toast(msg, kind='') {
  const box = $('#toast');
  const el = document.createElement('div');
  el.className = 'toast-msg ' + kind;
  el.textContent = msg;
  box.appendChild(el);
  setTimeout(() => el.remove(), 3500);
}

async function getJSON(url) {
  const r = await adminFetch(url);
  if (!r.ok) {
    let detail = r.statusText;
    try { const j = await r.json(); detail = j.detail || JSON.stringify(j); } catch(_) {}
    throw new Error(`${r.status} ${detail}`);
  }
  return r.json();
}

function setCypher(el) {
  $('#cypher-input').value = el.textContent;
}

// ============== Tab 切换 ==============
$$('.nav-tab').forEach(t => {
  t.addEventListener('click', () => {
    $$('.nav-tab').forEach(x => x.classList.remove('active'));
    $$('.tab-panel').forEach(x => x.classList.remove('active'));
    t.classList.add('active');
    $('#tab-' + t.dataset.tab).classList.add('active');
    if (t.dataset.tab === 'projects') loadProjects();
    if (t.dataset.tab === 'overview') loadOverview();
  });
});

// ============== 总览 ==============
async function loadOverview() {
  try {
    const data = await getJSON(`${API}/overview`);
    $('#topbar-projects').textContent = data.total_projects.toLocaleString();
    $('#topbar-mode0').textContent = data.total_mode0_versions.toLocaleString();
    $('#topbar-mode1').textContent = data.total_mode1_json_versions.toLocaleString();
    $('#topbar-eg').textContent = data.total_entity_graph_versions.toLocaleString();

    $('#overview-sub').textContent = `共 ${data.total_nodes.toLocaleString()} 节点 · ${data.total_relationships.toLocaleString()} 关系`;

    const topStats = $('#top-stats');
    topStats.innerHTML = `
      <div class="stat-card project">
        <div class="label">项目总数</div>
        <div class="value">${data.total_projects.toLocaleString()}</div>
        <div class="sub">Project / BridgeProject / DBCADProject</div>
      </div>
      <div class="stat-card mode0">
        <div class="label">旧 Mode0 历史</div>
        <div class="value">${data.total_mode0_versions.toLocaleString()}</div>
        <div class="sub">BridgeVersion（完整 SAT）</div>
      </div>
      <div class="stat-card mode1">
        <div class="label">Mode1 JSON 版本</div>
        <div class="value">${data.total_mode1_json_versions.toLocaleString()}</div>
        <div class="sub">JsonGraphVersion（完整快照）</div>
      </div>
      <div class="stat-card eg">
        <div class="label">Entity Graph 版本</div>
        <div class="value">${data.total_entity_graph_versions.toLocaleString()}</div>
        <div class="sub">BridgeEntityGraphVersion</div>
      </div>
      <div class="stat-card accent">
        <div class="label">总节点</div>
        <div class="value">${data.total_nodes.toLocaleString()}</div>
        <div class="sub">${data.label_counts.length} 种 label</div>
      </div>
      <div class="stat-card">
        <div class="label">总关系</div>
        <div class="value">${data.total_relationships.toLocaleString()}</div>
        <div class="sub">${data.relationship_counts.length} 种关系类型</div>
      </div>
    `;

    // Label chart
    const labels = data.label_counts.slice(0, 22);
    const labelMax = labels[0]?.count || 1;
    $('#label-total').textContent = `${data.label_counts.length} 种 label · 累计 ${data.total_nodes.toLocaleString()}`;
    $('#label-chart').innerHTML = labels.map(r => `
      <div class="bar-row">
        <div class="bar-label" title="${r.label}">${r.label}</div>
        <div class="bar-track"><div class="bar-fill" style="width: ${(r.count/labelMax*100).toFixed(1)}%"></div></div>
        <div class="bar-val">${r.count.toLocaleString()}</div>
      </div>
    `).join('');

    // Topology buckets
    const buckets = data.topology_buckets;
    const bucketMax = buckets[0]?.count || 1;
    $('#bucket-chart').innerHTML = buckets.map(r => `
      <div class="bar-row">
        <div class="bar-label">${r.bucket}</div>
        <div class="bar-track"><div class="bar-fill" style="width: ${(r.count/bucketMax*100).toFixed(1)}%"></div></div>
        <div class="bar-val">${r.count.toLocaleString()}</div>
      </div>
    `).join('');

    // Rel chart
    const rels = data.relationship_counts.slice(0, 18);
    const relMax = rels[0]?.count || 1;
    $('#rel-total').textContent = `${data.relationship_counts.length} 种关系`;
    $('#rel-chart').innerHTML = rels.map(r => `
      <div class="bar-row">
        <div class="bar-label">${r.rel_type}</div>
        <div class="bar-track"><div class="bar-fill" style="width: ${(r.count/relMax*100).toFixed(1)}%"></div></div>
        <div class="bar-val">${r.count.toLocaleString()}</div>
      </div>
    `).join('');
  } catch (e) {
    toast('加载概览失败：' + e.message, 'error');
  }
}

// ============== 项目 ==============
let ALL_PROJECTS = [];
