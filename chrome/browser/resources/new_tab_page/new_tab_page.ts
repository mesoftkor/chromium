// Copyright 2026 MESOFT. All rights reserved.
// @ts-nocheck

// Preserve Chromium's test-only export contract even though MEWEB replaces the
// visual new-tab entry point below.
export {GlifAnimationState} from '//resources/cr_components/composebox/common.js';
export {CustomizeButtonsElement} from 'chrome://new-tab-page/shared/customize_buttons/customize_buttons.js';
export {PlaceholderTextCycler} from 'chrome://resources/cr_components/searchbox/placeholder_text_cycler.js';
export {SearchboxBrowserProxy} from 'chrome://resources/cr_components/searchbox/searchbox_browser_proxy.js';
export {SearchboxIconElement} from 'chrome://resources/cr_components/searchbox/searchbox_icon.js';
export {SearchboxMatchElement} from 'chrome://resources/cr_components/searchbox/searchbox_match.js';
export type {CrA11yAnnouncerMessagesSentEvent} from 'chrome://resources/cr_elements/cr_a11y_announcer/cr_a11y_announcer.js';
export {CrAutoImgElement} from 'chrome://resources/cr_elements/cr_auto_img/cr_auto_img.js';
export type {CrIconElement} from 'chrome://resources/cr_elements/cr_icon/cr_icon.js';
export {BrowserCommandProxy} from 'chrome://resources/js/browser_command/browser_command_proxy.js';
export {BrowserProxyImpl} from 'chrome://resources/js/metrics_reporter/browser_proxy.js';
export {MetricsReporterImpl} from 'chrome://resources/js/metrics_reporter/metrics_reporter.js';
export {getTrustedHTML} from 'chrome://resources/js/static_types.js';
export {SuggestInventory} from 'chrome://resources/mojo/components/omnibox/browser/searchbox.mojom-webui.js';
export {AppElement, CONTEXTUAL_ENTRYPOINT_ELEMENT_ID, CUSTOMIZE_CHROME_BUTTON_ELEMENT_ID, NtpCustomizeChromeEntryPoint, NtpElement} from './app.js';
export {BackgroundManager} from './background_manager.js';
export {CustomizeButtonsProxy} from './customize_buttons_proxy.js';
export {CustomizeDialogPage} from './customize_dialog_types.js';
export {DoodleShareDialogElement} from './doodle_share_dialog.js';
export {IframeElement} from './iframe.js';
export {processFile} from './image_processor.js';
export type {ProcessedFile} from './image_processor.js';
export {LogoElement} from './logo.js';
export {recordBoolean, recordDuration, recordEnumeration, recordLinearValue, recordLoadDuration, recordLogValue, recordOccurrence, recordPerdecage, recordSmallCount, recordSparseValueWithPersistentHash} from './metrics_utils.js';
export {NewTabPageProxy} from './new_tab_page_proxy.js';
export {NtpSearchboxElement} from './ntp_searchbox.js';
export {checkTransparency, isBMP, isPNG, isWebP} from './transparency.js';
export {$$} from './utils.js';
export {Action as VoiceAction, Error as VoiceError} from './voice_search_overlay.js';
export {WindowProxy} from './window_proxy.js';
export * from './new_tab_page.mojom-webui.js';
export type {ActionChip, PageRemote as ActionChipsPageRemote, TabInfo} from './action_chips.mojom-webui.js';
export {ActionChipsHandlerRemote, IconType, PageCallbackRouter as ActionChipsPageCallbackRouter} from './action_chips.mojom-webui.js';
export type {FuseboxAction} from './fusebox_action.mojom-webui.js';
export {InputSource} from './fusebox_action.mojom-webui.js';
export type {CalendarEvent, Attachment} from './calendar_data.mojom-webui.js';
export type {Promo as NtpPromo, NtpPromoClientRemote, NtpPromoHandlerInterface} from './ntp_promo.mojom-webui.js';
export {NtpPromoClientCallbackRouter} from './ntp_promo.mojom-webui.js';
export type {CustomizeChromePageRemote} from './customize_chrome.mojom-webui.js';
export {CustomizeChromeSection} from './customize_chrome.mojom-webui.js';
export type {CustomizeButtonsDocumentRemote} from './customize_buttons.mojom-webui.js';
export {CustomizeButtonsDocumentCallbackRouter, CustomizeButtonsHandlerRemote, SidePanelOpenTrigger} from './customize_buttons.mojom-webui.js';
export {DriveSuggestionHandlerRemote} from './drive_suggestion.mojom-webui.js';
export type {File} from './file_suggestion.mojom-webui.js';
export {RecommendationType} from './file_suggestion.mojom-webui.js';
export {QueryActionOverride, SearchboxOverride} from './fusebox_action.mojom-webui.js';
export {GoogleCalendarPageHandlerRemote} from './google_calendar.mojom-webui.js';
export {MicrosoftAuthPageHandlerRemote} from './microsoft_auth.mojom-webui.js';
export {MicrosoftFilesPageHandlerRemote} from './microsoft_files.mojom-webui.js';
export {PageHandlerRemote as TabResumptionPageHandlerRemote, ScoredURLUserAction} from './most_relevant_tab_resumption.mojom-webui.js';
export {AuthType, MicrosoftAuthUntrustedDocumentRemote} from './ntp_microsoft_auth_shared_ui.mojom-webui.js';
export {OutlookCalendarPageHandlerRemote} from './outlook_calendar.mojom-webui.js';
export type {TabGroup} from './tab_groups.mojom-webui.js';
export {PageHandlerRemote as TabGroupsPageHandlerRemote} from './tab_groups.mojom-webui.js';
export {Color} from './tab_group_types.mojom-webui.js';
export type {URLVisit} from './url_visit_types.mojom-webui.js';
export {DecorationType, FormFactor, VisitSource} from './url_visit_types.mojom-webui.js';
// <if expr="not is_official_build">
export {FooHandlerRemote} from './foo.mojom-webui.js';
// </if>

(() => {
  'use strict';

  const STORAGE_KEY = 'meweb.agent.workspace.v1';
  // Dynamic values are escaped before they reach this policy. Keeping the
  // policy local to the MEWEB NTP lets Chromium's Trusted Types enforcement
  // remain enabled for every other script sink.
  const htmlPolicy = window.trustedTypes?.createPolicy(
      'meweb-agent-workspace', {createHTML: value => value});
  const $ = selector => document.querySelector(selector);
  const $$ = selector => Array.from(document.querySelectorAll(selector));
  const setHtml = (element, value) => {
    element.innerHTML = htmlPolicy ? htmlPolicy.createHTML(value) : value;
  };

  const AVATARS = {
    ribbon: '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M4 19V7l8 7 8-7v12"/></svg>',
    orbit: '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="4"/><ellipse cx="12" cy="12" rx="10" ry="4.6" transform="rotate(-28 12 12)"/></svg>',
    spark: '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2l2.4 7.6L22 12l-7.6 2.4L12 22l-2.4-7.6L2 12l7.6-2.4z"/></svg>',
    grid: '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="7.5" height="7.5" rx="1.6"/><rect x="13.5" y="3" width="7.5" height="7.5" rx="1.6"/><rect x="3" y="13.5" width="7.5" height="7.5" rx="1.6"/><rect x="13.5" y="13.5" width="7.5" height="7.5" rx="1.6"/></svg>',
  };
  const ACCENTS = [
    ['mint', '#7ee0c0', '민트'], ['cyan', '#4fd1e0', '시안'],
    ['indigo', '#8b9bf9', '인디고'], ['amber', '#e8ab3f', '앰버'],
    ['violet', '#b18cf5', '바이올렛'], ['rose', '#f2857e', '로즈'],
  ];
  const ENGINES = [
    {id: 'google', name: 'Google', host: 'google.com', color: '#4a8cf7', query: 'https://www.google.com/search?q='},
    {id: 'naver', name: '네이버', host: 'search.naver.com', color: '#3ec46d', query: 'https://search.naver.com/search.naver?query='},
    {id: 'daum', name: '다음', host: 'search.daum.net', color: '#f2857e', query: 'https://search.daum.net/search?q='},
    {id: 'bing', name: 'Bing', host: 'bing.com', color: '#4fd1e0', query: 'https://www.bing.com/search?q='},
    {id: 'ddg', name: 'DuckDuckGo', host: 'duckduckgo.com', color: '#e8ab3f', query: 'https://duckduckgo.com/?q='},
  ];
  const DOMAINS = {
    finance: {role: '정산·회계 담당', actions: ['이월 항목 대사', '세금계산서 대조', '월마감 체크리스트', '미수금 정리']},
    research: {role: '리서치 담당', actions: ['주간 리서치 요약', '경쟁사 모니터링', '논문 정리', '뉴스 브리핑']},
    dev: {role: '개발 담당', actions: ['PR 리뷰 요약', '이슈 트리아지', '릴리스 노트 초안', '로그 분석']},
    general: {role: '일반 업무 담당', actions: ['메일 정리', '일정 브리핑', '문서 요약', '링크 모음']},
  };
  const HOURS = ['오전', '오후', '저녁', '야간'];
  const PATTERNS = [
    {id: 'p1', hour: 0, task: '월마감 체크리스트', why: '최근 3주 동안 이 시간대에 같은 작업을 반복했습니다.'},
    {id: 'p2', hour: 1, task: '거래명세서 대조', why: '오후에 가장 자주 실행한 작업입니다.'},
    {id: 'p3', hour: 2, task: '주간 리서치 요약', why: '지난 4주간 저녁마다 실행한 기록이 있습니다.'},
    {id: 'p4', hour: 3, task: '로그 분석', why: '야간에 반복해서 열어본 작업입니다.'},
  ];
  const STEPS = [
    {id: 'extract', risk: false, source: '정산 문서', action: '읽기',
      text: {brief: '이월 항목 7건 추출', standard: '정산 문서에서 이월 항목 7건 추출', detail: '정산 문서를 열어 이월 항목 7건을 찾아 표로 정리합니다.'}},
    {id: 'compare', risk: false, source: '에이전트 작업 탭', action: '읽기',
      text: {brief: '거래명세서 대조', standard: '거래명세서 PDF를 작업 탭으로 열어 대조', detail: '거래명세서를 열어 앞 단계에서 정리한 항목과 하나씩 대조합니다.'}},
    {id: 'send', risk: true, source: 'mail.mesoft.kr', action: '외부 전송',
      text: {brief: '불일치 2건 회신', standard: '불일치 2건을 재무팀에 메일로 회신', detail: '대조 과정에서 나온 불일치 2건을 정리해 재무팀 앞으로 회신 메일을 보냅니다.'},
      gate: {summary: '받는 사람 finance@mesoft.kr · 제목 “이월 항목 불일치 2건”', detail: '외부 전송 전에 받는 사람과 본문을 확인하세요.'}},
    {id: 'download', risk: true, source: '다운로드', action: '로컬 저장',
      text: {brief: '결과 CSV 저장', standard: '대사 결과 CSV를 다운로드 폴더에 저장', detail: '대사 결과 전체를 CSV 파일로 내려받아 로컬에 저장합니다.'},
      gate: {summary: '파일명 meweb-reconciliation.csv · 다운로드 폴더', detail: '로컬 파일을 생성하기 전에 저장 위치를 확인하세요.'}},
  ];
  const AUTONOMY = {
    always: {label: '모든 단계 승인', className: 'always', note: '읽기를 포함한 모든 단계에서 승인받습니다.', reason: '모든 단계 승인 설정에 따라 실행 전에 멈췄습니다.'},
    risky: {label: '위험 단계만 승인', className: 'risky', note: '외부 전송과 로컬 저장처럼 영향을 남기는 단계에서만 승인받습니다.', reason: '되돌리기 어려운 단계이므로 실행 전에 멈췄습니다.'},
    auto: {label: '자동 진행', className: 'auto', note: '모든 단계를 자동 실행하되 기록과 되돌리기를 제공합니다.', reason: '자동 진행 설정에 따라 승인 없이 실행했습니다.'},
  };

  function defaults() {
    return {
      appearance: {theme: 'midnight', accent: 'mint', density: 'default', fontScale: 100, rail: 'left', railWidth: 220},
      persona: {avatar: 'ribbon', name: '미웹', domain: 'finance', tone: 'standard', autonomy: 'risky', showReason: true},
      workflow: {custom: [], pinned: ['이월 항목 대사', '월마감 체크리스트'], removed: [], shortcuts: true},
      adaptive: {enabled: true, basis: 'time', hour: 0, forgotten: []},
      start: {engine: 'google', showLinks: true, showTasks: true, links: [
        {name: '정산 문서', url: 'https://docs.mesoft.kr', color: '#7ee0c0'},
        {name: '거래명세서', url: 'https://files.mesoft.kr', color: '#e8ab3f'},
        {name: '메일', url: 'https://mail.mesoft.kr', color: '#8b9bf9'},
        {name: '사내 위키', url: 'https://wiki.mesoft.kr', color: '#b18cf5'},
        {name: '대시보드', url: 'https://dash.mesoft.kr', color: '#4fd1e0'},
        {name: '캘린더', url: 'https://cal.mesoft.kr', color: '#f2857e'},
      ]},
      task: {name: '이월 항목 대사', status: 'running', decisions: {}, audit: [
        {at: new Date().toISOString(), text: '작업을 시작했습니다.'},
        {at: new Date().toISOString(), text: '읽기 단계 2개를 로컬에서 완료했습니다.'},
      ]},
    };
  }

  function loadState() {
    const base = defaults();
    try {
      const parsed = JSON.parse(localStorage.getItem(STORAGE_KEY) || 'null');
      if (!parsed || typeof parsed !== 'object') return base;
      for (const key of Object.keys(base)) {
        if (parsed[key] && typeof parsed[key] === 'object') Object.assign(base[key], parsed[key]);
      }
      if (!Array.isArray(base.task.audit)) base.task.audit = [];
      if (!base.task.decisions || typeof base.task.decisions !== 'object') base.task.decisions = {};
    } catch (_error) {}
    return base;
  }

  let state = loadState();
  let currentView = 'start';
  let settingsSection = 'agent';
  let engineMenuOpen = false;
  let auditOpen = false;
  let toastTimer = 0;

  function save() {
    const serialized = JSON.stringify(state);
    localStorage.setItem(STORAGE_KEY, serialized);
    // eslint-disable-next-line no-restricted-properties
    chrome.send('mewebAgentSaveState', [serialized]);
  }
  function esc(value) { return String(value).replace(/[&<>"']/g, character => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[character])); }
  function getPath(path) { return path.split('.').reduce((value, key) => value[key], state); }
  function setPath(path, value) {
    const keys = path.split('.');
    const last = keys.pop();
    keys.reduce((target, key) => target[key], state)[last] = value;
  }
  function engine() { return ENGINES.find(item => item.id === state.start.engine) || ENGINES[0]; }
  function domain() { return DOMAINS[state.persona.domain] || DOMAINS.finance; }
  function autonomy() { return AUTONOMY[state.persona.autonomy] || AUTONOMY.risky; }
  function toast(message) {
    const element = $('#toast');
    element.textContent = message;
    element.classList.add('show');
    clearTimeout(toastTimer);
    toastTimer = setTimeout(() => element.classList.remove('show'), 2100);
  }
  function record(text) {
    state.task.audit.unshift({at: new Date().toISOString(), text});
    state.task.audit = state.task.audit.slice(0, 100);
    save();
  }

  function allActions() {
    const builtIn = domain().actions.filter(action => !state.workflow.removed.includes(action));
    const custom = state.workflow.custom.filter(action => !builtIn.includes(action));
    const all = builtIn.concat(custom);
    const pinned = state.workflow.pinned.filter(action => all.includes(action));
    return pinned.concat(all.filter(action => !pinned.includes(action)));
  }

  function resolvedSteps() {
    let waitingBehindGate = false;
    return STEPS.map((step, index) => {
      const decision = state.task.decisions[step.id];
      if (decision === 'approved') return {...step, state: 'done', label: '승인 후 완료', approved: true};
      if (decision === 'skipped') return {...step, state: 'done', label: '건너뜀', skipped: true};
      if (decision === 'undone') return {...step, state: 'gate', label: '되돌림', undone: true};
      if (state.persona.autonomy === 'auto') return {...step, state: 'done', label: step.risk ? '자동 실행' : '완료', automatic: true};
      if (waitingBehindGate) return {...step, state: 'wait', label: '대기'};
      const requiresApproval = state.persona.autonomy === 'always' || step.risk;
      if (requiresApproval) {
        waitingBehindGate = true;
        return {...step, state: 'gate', label: '승인 대기'};
      }
      return {...step, state: 'done', label: index < 2 ? '완료' : '자동 완료'};
    });
  }

  function taskProgress() {
    const steps = resolvedSteps();
    const done = steps.filter(step => step.state === 'done').length;
    const gates = steps.filter(step => step.state === 'gate').length;
    const complete = done === steps.length;
    return {steps, done, gates, total: steps.length, complete};
  }

  function applyRoot() {
    const root = document.documentElement;
    root.dataset.theme = state.appearance.theme;
    root.dataset.accent = state.appearance.accent;
    root.dataset.density = state.appearance.density;
    root.dataset.rail = state.appearance.rail;
    root.style.setProperty('--font-scale', String(state.appearance.fontScale / 100));
    root.style.setProperty('--rail-width', `${state.appearance.railWidth}px`);
  }

  function renderProfile() {
    setHtml($('#profileAvatar'), AVATARS[state.persona.avatar] || AVATARS.ribbon);
    $('#profileName').textContent = state.persona.name || '이름 없음';
    $('#profileRole').textContent = domain().role;
    const chip = $('#autonomyChip');
    chip.textContent = autonomy().label;
    chip.className = `autonomy ${autonomy().className}`;
  }

  function renderNavigation() {
    $$('[data-view-target]').forEach(button => button.setAttribute('aria-current', button.dataset.viewTarget === currentView ? 'page' : 'false'));
    $('#startView').hidden = currentView !== 'start';
    $('#workView').hidden = currentView !== 'work';
    $('#settingsView').hidden = currentView !== 'settings';
    $('#auditButton').hidden = currentView !== 'work';
    $('#pauseButton').hidden = currentView !== 'work';
    const crumbs = {
      start: '<b>시작 화면</b> · 검색, 바로가기와 진행 중인 작업',
      work: `<b>${esc(state.task.name)}</b> · 승인 기반 에이전트 워크스페이스`,
      settings: `<b>환경설정</b> · ${esc({appearance:'모양', search:'검색엔진', startup:'시작 화면', agent:'에이전트', privacy:'개인정보 보호', downloads:'다운로드'}[settingsSection])}`,
    };
    setHtml($('#crumb'), crumbs[currentView]);
    $('#auditPanel').hidden = !auditOpen || currentView !== 'work';
  }

  function renderStart() {
    const selectedEngine = engine();
    $('#engineDot').style.background = selectedEngine.color;
    $('#engineName').textContent = selectedEngine.name;
    $('#searchInput').placeholder = `${selectedEngine.name}에서 검색하거나 주소 입력`;
    setHtml($('#engineMenu'), ENGINES.map(item => `<button data-engine="${item.id}" aria-pressed="${item.id === selectedEngine.id}"><span class="engine-dot" style="background:${item.color}"></span>${esc(item.name)}<span class="host">${esc(item.host)}</span></button>`).join(''));
    $('#engineMenu').hidden = !engineMenuOpen;

    const linkGrid = $('#linkGrid');
    linkGrid.hidden = !state.start.showLinks;
    setHtml(linkGrid, state.start.links.map((link, index) => `<button class="link" data-open-link="${index}" title="${esc(link.url)}"><span class="link-icon" style="background:${esc(link.color)}">${esc(link.name.slice(0, 1))}</span><span class="link-name">${esc(link.name)}</span><span class="remove" data-remove-link="${index}" title="바로가기 삭제">×</span></button>`).join('') + '<button class="link add" id="showLinkFormButton"><span class="link-icon">＋</span><span class="link-name">바로가기 추가</span></button>');

    const progress = taskProgress();
    $('#taskSection').hidden = !state.start.showTasks;
    setHtml($('#startTasks'), `<button class="task-card" data-view-target="work"><div class="task-top"><span class="task-name">${esc(state.task.name)}</span><span class="chip ${progress.complete ? 'done' : progress.gates ? 'gate' : 'run'}">${progress.complete ? '완료' : progress.gates ? '승인 대기' : '진행 중'}</span></div><div class="progress"><i style="width:${Math.round(progress.done / progress.total * 100)}%"></i></div><div class="task-meta"><span>완료 <b>${progress.done}</b></span><span>승인 대기 <b>${progress.gates}</b></span><span>대기 <b>${progress.total - progress.done - progress.gates}</b></span><span>${progress.total}단계</span></div></button><button class="task-card" data-secondary-task="주간 리서치 요약"><div class="task-top"><span class="task-name">주간 리서치 요약</span><span class="chip done">완료</span></div><div class="progress"><i style="width:100%"></i></div><div class="task-meta"><span>완료 <b>3</b></span><span>3단계</span></div></button>`);

    const pattern = activePattern();
    $('#suggestionSection').hidden = !pattern;
    setHtml($('#suggestion'), pattern ? `<div class="suggestion"><small>${HOURS[state.adaptive.hour]} 패턴</small><button data-start-action="${esc(pattern.task)}">${esc(pattern.task)}</button><details><summary>왜 추천됐나요?</summary><p>${esc(pattern.why)}</p><button class="btn danger" data-forget-pattern="${pattern.id}">이 패턴 지우기</button></details></div>` : '');
    setHtml($('#quickActions'), allActions().slice(0, 6).map((action, index) => `<button class="quick" data-start-action="${esc(action)}"><span class="label">${esc(action)}</span>${state.workflow.shortcuts && state.workflow.pinned.includes(action) ? `<kbd>⌘${index + 1}</kbd>` : ''}</button>`).join(''));
  }

  function activePattern() {
    if (!state.adaptive.enabled) return null;
    const available = PATTERNS.filter(pattern => !state.adaptive.forgotten.includes(pattern.id));
    if (!available.length) return null;
    if (state.adaptive.basis === 'time') return available.find(pattern => pattern.hour === state.adaptive.hour) || null;
    const action = allActions()[0];
    return action ? {id: available[0].id, task: action, why: state.adaptive.basis === 'recent' ? '가장 최근에 실행한 작업입니다.' : '이번 달에 가장 많이 실행한 작업입니다.'} : null;
  }

  function renderWork() {
    const progress = taskProgress();
    $('#workTitle').textContent = state.task.name;
    const status = $('#runStatus');
    status.className = `run-status ${state.task.status === 'paused' ? 'paused' : state.persona.autonomy === 'auto' ? 'auto' : ''}`;
    $('#runStatusText').textContent = state.task.status === 'paused' ? '일시 정지됨 · 로컬 실행 엔진' : progress.complete ? '완료 · 로컬 실행 엔진' : progress.gates ? '승인 대기 · 로컬 실행 엔진' : '실행 중 · 로컬 실행 엔진';
    $('#pauseButton').textContent = state.task.status === 'paused' ? '계속 실행' : '일시 정지';
    const railChip = $('#railTaskChip');
    railChip.textContent = progress.complete ? '완료' : progress.gates ? '승인 대기' : state.task.status === 'paused' ? '정지됨' : '진행 중';
    railChip.className = `chip ${progress.complete ? 'done' : progress.gates ? 'gate' : 'run'}`;

    setHtml($('#steps'), progress.steps.map(step => {
      const toneText = step.text[state.persona.tone] || step.text.standard;
      let card = '';
      if (step.state === 'gate') {
        const detail = step.gate || {summary: `${step.action} 단계`, detail: '실행 전에 내용을 확인하세요.'};
        card = `<div class="gate-card" id="approvalGate-${step.id}"><p><b>${esc(detail.summary)}</b><br>${esc(detail.detail)}</p><div class="gate-actions"><button class="btn primary" data-approve-step="${step.id}" ${state.task.status === 'paused' ? 'disabled' : ''}>확인하고 승인</button><button class="btn" data-skip-step="${step.id}" ${state.task.status === 'paused' ? 'disabled' : ''}>건너뛰기</button>${step.undone ? `<button class="btn" data-redo-step="${step.id}">다시 실행</button>` : ''}</div></div>`;
      } else if (step.state === 'done' && step.risk) {
        card = `<div class="gate-card approved"><p><b>${esc(step.label)}</b> · 감사 기록에 남았습니다.</p><div class="gate-actions"><button class="btn" data-undo-step="${step.id}">되돌리기</button></div></div>`;
      }
      const reason = state.persona.showReason && (step.state === 'gate' || step.automatic) ? `<div class="step-reason">${esc(autonomy().reason)}</div>` : '';
      return `<div class="step ${step.state}" data-step-id="${step.id}"><span class="step-mark"></span><div><div class="step-title">${esc(toneText)} <span class="chip ${step.state === 'done' ? 'done' : step.state === 'gate' ? 'gate' : 'run'}">${esc(step.label)}</span></div><div class="step-meta">${esc(step.source)} · ${esc(step.action)}</div>${reason}${card}</div></div>`;
    }).join(''));
  }

  function optionGroup(path, options) {
    const current = String(getPath(path));
    return `<div class="setting-control" data-option-path="${path}">${options.map(option => `<button class="option" data-option-value="${option[0]}" aria-pressed="${String(option[0]) === current}">${option[2] || option[1]}</button>`).join('')}</div>`;
  }
  function row(label, help, control) { return `<div class="setting-row"><div class="setting-label">${label}${help ? `<small>${help}</small>` : ''}</div>${control}</div>`; }

  function renderSettings() {
    $$('[data-settings-target]').forEach(button => button.setAttribute('aria-current', button.dataset.settingsTarget === settingsSection ? 'page' : 'false'));
    const content = $('#settingsContent');
    if (settingsSection === 'appearance') {
      setHtml(content, `<h1>모양</h1><p class="lede">브라우저 워크스페이스의 테마와 레일 배치를 조정합니다.</p><section class="settings-card"><h2>화면</h2>${row('테마', '워크스페이스 전체에 적용됩니다.', optionGroup('appearance.theme', [['midnight','midnight','미드나잇'],['navy','navy','딥 네이비'],['light','light','라이트']]))}${row('강조 색', '', `<div class="setting-control" data-option-path="appearance.accent">${ACCENTS.map(accent => `<button class="option" data-option-value="${accent[0]}" aria-pressed="${accent[0] === state.appearance.accent}"><span class="swatch" style="background:${accent[1]}"></span>${accent[2]}</button>`).join('')}</div>`)}${row('밀도', '', optionGroup('appearance.density', [['compact','compact','조밀하게'],['default','default','기본'],['comfy','comfy','편안하게']]))}${row('글자 크기', '', `<div class="setting-control"><input type="range" id="fontScaleInput" min="85" max="125" step="5" value="${state.appearance.fontScale}"><span class="value" id="fontScaleValue">${state.appearance.fontScale}%</span></div>`)}${row('작업 레일 위치', '', optionGroup('appearance.rail', [['left','left','왼쪽'],['right','right','오른쪽']]))}${row('작업 레일 너비', '', `<div class="setting-control"><input type="range" id="railWidthInput" min="180" max="300" step="2" value="${state.appearance.railWidth}"><span class="value" id="railWidthValue">${state.appearance.railWidth}px</span></div>`)}</section>`);
    } else if (settingsSection === 'search') {
      setHtml(content, `<h1>검색엔진</h1><p class="lede">시작 화면 검색에 사용할 서비스를 선택합니다.</p><section class="settings-card"><h2>기본 검색엔진</h2>${ENGINES.map(item => row(`<span style="color:${item.color};font-weight:700">●</span> ${esc(item.name)}`, item.host, item.id === state.start.engine ? '<span class="value">기본</span>' : `<button class="btn" data-engine="${item.id}">기본으로 설정</button>`)).join('')}</section>`);
    } else if (settingsSection === 'startup') {
      setHtml(content, `<h1>시작 화면</h1><p class="lede">새 탭에서 보여줄 정보를 선택합니다.</p><section class="settings-card"><h2>구성</h2>${row('바로가기 표시', '직접 추가하고 삭제한 링크가 프로필에 저장됩니다.', `<button class="toggle" id="showLinksToggle" aria-pressed="${state.start.showLinks}"></button>`)}${row('진행 중인 작업 표시', '', `<button class="toggle" id="showTasksToggle" aria-pressed="${state.start.showTasks}"></button>`)}${row('저장된 바로가기', '', `<span class="value">${state.start.links.length}개</span>`)}</section>`);
    } else if (settingsSection === 'agent') {
      const actions = allActions();
      const patterns = PATTERNS.filter(pattern => !state.adaptive.forgotten.includes(pattern.id));
      setHtml(content, `<h1>에이전트</h1><p class="lede">외형, 역할, 승인 범위와 학습 기록은 이 프로필에만 저장됩니다.</p>
        <section class="settings-card"><h2>페르소나</h2>
          ${row('아바타', '', `<div class="avatar-options">${Object.keys(AVATARS).map(key => `<button class="avatar-option" data-avatar="${key}" aria-pressed="${key === state.persona.avatar}">${AVATARS[key]}</button>`).join('')}</div>`)}
          ${row('이름', '', `<div class="setting-control"><input class="text-input" id="personaNameInput" maxlength="16" value="${esc(state.persona.name)}"></div>`)}
          ${row('업무 역할', '', optionGroup('persona.domain', [['finance','finance','정산'],['research','research','리서치'],['dev','dev','개발'],['general','general','일반']]))}
          ${row('설명 방식', '', optionGroup('persona.tone', [['brief','brief','간결'],['standard','standard','표준'],['detail','detail','상세']]))}
          ${row('자율성', '승인 게이트의 범위를 직접 결정합니다.', optionGroup('persona.autonomy', [['always','always','모두 승인'],['risky','risky','위험만 승인'],['auto','auto','자동 진행']]))}
          <div class="setting-row"><div class="note">${esc(autonomy().note)}</div></div>
          ${row('판단 이유 표시', '', `<button class="toggle" id="showReasonToggle" aria-pressed="${state.persona.showReason}"></button>`)}
        </section>
        <section class="settings-card"><h2>워크플로</h2>
          <div class="setting-row"><div class="list">${actions.map((action, index) => `<div class="list-row"><button class="${state.workflow.pinned.includes(action) ? 'star' : ''}" data-pin-action="${esc(action)}">${state.workflow.pinned.includes(action) ? '★' : '☆'}</button><span class="label">${esc(action)}</span>${state.workflow.shortcuts && state.workflow.pinned.includes(action) ? `<kbd>⌘${index + 1}</kbd>` : ''}<button class="delete" data-remove-action="${esc(action)}">×</button></div>`).join('') || '<span class="lede">등록된 작업이 없습니다.</span>'}</div></div>
          <div class="setting-row"><div class="add-row"><input class="text-input" id="workflowInput" maxlength="24" placeholder="새 빠른 실행 작업"><button class="btn" id="addWorkflowButton">추가</button></div></div>
          ${row('고정 작업 단축키', '고정된 작업을 ⌘1~6으로 시작합니다.', `<button class="toggle" id="shortcutToggle" aria-pressed="${state.workflow.shortcuts}"></button>`)}
        </section>
        <section class="settings-card"><h2>적응형 추천</h2>
          ${row('추천 사용', '브라우징 내용이 아니라 이 워크스페이스의 실행 패턴만 사용합니다.', `<button class="toggle" id="adaptiveToggle" aria-pressed="${state.adaptive.enabled}"></button>`)}
          ${row('추천 기준', '', optionGroup('adaptive.basis', [['time','time','시간대'],['recent','recent','최근 실행'],['frequent','frequent','자주 실행']]))}
          ${row('시간대 미리보기', '', `<div class="setting-control"><input type="range" id="hourInput" min="0" max="3" value="${state.adaptive.hour}"><span class="value" id="hourValue">${HOURS[state.adaptive.hour]}</span></div>`)}
          <div class="setting-row"><div class="list">${patterns.map(pattern => `<div class="list-row"><span class="label">${HOURS[pattern.hour]} · ${esc(pattern.task)}</span><button class="delete" data-forget-pattern="${pattern.id}">×</button></div>`).join('') || '<span class="lede">학습된 패턴이 없습니다.</span>'}</div></div>
          ${row('학습 기록', '', '<button class="btn danger" id="forgetAllButton">모두 지우기</button>')}
        </section>`);
    } else {
      const title = settingsSection === 'privacy' ? '개인정보 보호' : '다운로드';
      setHtml(content, `<h1>${title}</h1><p class="lede">기존 Chromium 설정을 그대로 사용합니다.</p><div class="stub">이 항목은 MEWEB의 기존 <b>chrome://settings</b>에서 관리됩니다.<br><button class="btn" id="openChromeSettingsButton">Chromium 설정 열기</button></div>`);
    }
  }

  function renderAudit() {
    setHtml($('#auditList'), state.task.audit.length ? state.task.audit.map(entry => `<div class="audit-entry"><time>${new Date(entry.at).toLocaleString('ko-KR')}</time><span>${esc(entry.text)}</span></div>`).join('') : '<div class="stub">기록이 없습니다.</div>');
  }

  function renderAll() {
    applyRoot(); renderProfile(); renderStart(); renderWork(); renderSettings(); renderAudit(); renderNavigation();
  }

  function switchView(view) {
    currentView = view;
    if (view !== 'work') auditOpen = false;
    engineMenuOpen = false;
    renderAll();
  }

  function setEngine(id) {
    if (!ENGINES.some(item => item.id === id)) return;
    state.start.engine = id;
    engineMenuOpen = false;
    save(); renderAll(); toast(`기본 검색엔진을 ${engine().name}(으)로 바꿨습니다.`);
  }

  function normalizedUrl(value) {
    const trimmed = value.trim();
    if (!trimmed) return null;
    if (/^[a-z][a-z0-9+.-]*:\/\//i.test(trimmed)) {
      try {
        const url = new URL(trimmed);
        return ['http:', 'https:'].includes(url.protocol) ? url.href : null;
      } catch (_error) { return null; }
    }
    if (/^(localhost|127\.0\.0\.1|\[[0-9a-f:]+\])(?::\d+)?(?:\/|$)/i.test(trimmed) || (/^[^\s]+\.[^\s]+/.test(trimmed) && !trimmed.includes(' '))) {
      try { return new URL(`https://${trimmed}`).href; } catch (_error) { return null; }
    }
    return null;
  }

  function openUrl(url) {
    const target = normalizedUrl(url);
    if (!target) { toast('HTTP 또는 HTTPS 주소를 확인하세요.'); return false; }
    const anchor = document.createElement('a');
    anchor.href = target;
    anchor.target = '_blank';
    anchor.rel = 'noopener';
    anchor.click();
    return true;
  }

  function runSearch() {
    const input = $('#searchInput');
    const query = input.value.trim();
    if (!query) { input.focus(); return; }
    const direct = normalizedUrl(query);
    const target = direct || `${engine().query}${encodeURIComponent(query)}`;
    record(direct ? `주소를 새 탭에서 열었습니다: ${target}` : `${engine().name}에서 검색했습니다: ${query}`);
    openUrl(target);
    input.value = '';
  }

  function startAction(name) {
    state.task = {name, status: 'running', decisions: {}, audit: [{at: new Date().toISOString(), text: `“${name}” 작업을 시작했습니다.`}]};
    save(); switchView('work'); toast(`“${name}” 작업을 시작했습니다.`);
  }

  function updateDecision(stepId, decision) {
    const step = STEPS.find(item => item.id === stepId);
    if (!step || state.task.status === 'paused') { if (state.task.status === 'paused') toast('작업을 계속 실행한 뒤 승인하세요.'); return; }
    state.task.decisions[stepId] = decision;
    const labels = {approved: '승인 후 실행했습니다.', skipped: '건너뛰었습니다.', undone: '실행을 되돌렸습니다.'};
    record(`${step.text.standard}: ${labels[decision]}`);
    renderAll();
    toast(labels[decision]);
  }

  function addLink() {
    const name = $('#linkNameInput').value.trim();
    const urlValue = $('#linkUrlInput').value.trim();
    const url = normalizedUrl(urlValue);
    if (!name || !url) { toast('이름과 올바른 HTTP 또는 HTTPS 주소를 입력하세요.'); return; }
    const color = ACCENTS[state.start.links.length % ACCENTS.length][1];
    state.start.links.push({name, url, color});
    save();
    $('#linkNameInput').value = '';
    $('#linkUrlInput').value = '';
    $('#linkForm').hidden = true;
    renderAll(); toast(`“${name}” 바로가기를 추가했습니다.`);
  }

  function toggleActionPin(action) {
    const index = state.workflow.pinned.indexOf(action);
    if (index === -1) state.workflow.pinned.push(action); else state.workflow.pinned.splice(index, 1);
    save(); renderAll();
  }

  function removeAction(action) {
    const customIndex = state.workflow.custom.indexOf(action);
    if (customIndex >= 0) state.workflow.custom.splice(customIndex, 1);
    else if (!state.workflow.removed.includes(action)) state.workflow.removed.push(action);
    state.workflow.pinned = state.workflow.pinned.filter(item => item !== action);
    save(); renderAll();
  }

  function addWorkflow() {
    const input = $('#workflowInput');
    const action = input.value.trim();
    if (!action) return;
    if (allActions().includes(action)) { toast('이미 등록된 작업입니다.'); return; }
    state.workflow.removed = state.workflow.removed.filter(item => item !== action);
    state.workflow.custom.push(action);
    input.value = '';
    save(); renderAll(); toast(`“${action}” 작업을 추가했습니다.`);
  }

  document.addEventListener('click', event => {
    const removeLinkTarget = event.target.closest('[data-remove-link]');
    if (removeLinkTarget) {
      event.stopPropagation();
      const [removed] = state.start.links.splice(Number(removeLinkTarget.dataset.removeLink), 1);
      save(); renderAll();
      if (removed) toast(`“${removed.name}” 바로가기를 삭제했습니다.`);
      return;
    }
    const button = event.target.closest('button');
    if (!button) {
      if (engineMenuOpen && !event.target.closest('#engineMenu')) { engineMenuOpen = false; renderStart(); }
      return;
    }
    if (button.dataset.viewTarget) { switchView(button.dataset.viewTarget); return; }
    if (button.dataset.settingsTarget) { settingsSection = button.dataset.settingsTarget; renderSettings(); renderNavigation(); return; }
    if (button.dataset.engine) { setEngine(button.dataset.engine); return; }
    if (button.dataset.startAction) { startAction(button.dataset.startAction); return; }
    if (button.dataset.secondaryTask) { toast(`“${button.dataset.secondaryTask}” 작업은 읽기 전용 기록입니다.`); return; }
    if (button.dataset.approveStep || button.dataset.redoStep) { updateDecision(button.dataset.approveStep || button.dataset.redoStep, 'approved'); return; }
    if (button.dataset.skipStep) { updateDecision(button.dataset.skipStep, 'skipped'); return; }
    if (button.dataset.undoStep) { updateDecision(button.dataset.undoStep, 'undone'); return; }
    if (button.dataset.openLink !== undefined) { const link = state.start.links[Number(button.dataset.openLink)]; if (link) { record(`바로가기를 열었습니다: ${link.name}`); openUrl(link.url); } return; }
    if (button.dataset.optionValue !== undefined) {
      const group = button.closest('[data-option-path]');
      if (!group) return;
      setPath(group.dataset.optionPath, button.dataset.optionValue);
      if (group.dataset.optionPath === 'persona.autonomy') {
        state.task.decisions = {};
        record(`자율성을 “${autonomy().label}”로 변경하고 승인 상태를 초기화했습니다.`);
      } else save();
      renderAll(); return;
    }
    if (button.dataset.avatar) { state.persona.avatar = button.dataset.avatar; save(); renderAll(); return; }
    if (button.dataset.pinAction) { toggleActionPin(button.dataset.pinAction); return; }
    if (button.dataset.removeAction) { removeAction(button.dataset.removeAction); return; }
    if (button.dataset.forgetPattern) { if (!state.adaptive.forgotten.includes(button.dataset.forgetPattern)) state.adaptive.forgotten.push(button.dataset.forgetPattern); save(); renderAll(); toast('학습 패턴을 지웠습니다.'); return; }

    switch (button.id) {
      case 'engineButton': engineMenuOpen = !engineMenuOpen; renderStart(); break;
      case 'searchButton': runSearch(); break;
      case 'profileButton': settingsSection = 'agent'; switchView('settings'); break;
      case 'resetButton': state = defaults(); save(); currentView = 'start'; settingsSection = 'agent'; auditOpen = false; renderAll(); toast('기본값으로 되돌렸습니다.'); break;
      case 'showLinkFormButton': $('#linkForm').hidden = false; $('#linkNameInput').focus(); break;
      case 'cancelLinkButton': $('#linkForm').hidden = true; break;
      case 'addLinkButton': addLink(); break;
      case 'auditButton': auditOpen = !auditOpen; renderNavigation(); break;
      case 'closeAuditButton': auditOpen = false; renderNavigation(); break;
      case 'pauseButton': state.task.status = state.task.status === 'paused' ? 'running' : 'paused'; record(state.task.status === 'paused' ? '작업을 일시 정지했습니다.' : '작업을 계속 실행했습니다.'); renderAll(); break;
      case 'showLinksToggle': state.start.showLinks = !state.start.showLinks; save(); renderAll(); break;
      case 'showTasksToggle': state.start.showTasks = !state.start.showTasks; save(); renderAll(); break;
      case 'showReasonToggle': state.persona.showReason = !state.persona.showReason; save(); renderAll(); break;
      case 'shortcutToggle': state.workflow.shortcuts = !state.workflow.shortcuts; save(); renderAll(); break;
      case 'adaptiveToggle': state.adaptive.enabled = !state.adaptive.enabled; save(); renderAll(); break;
      case 'forgetAllButton': state.adaptive.forgotten = PATTERNS.map(pattern => pattern.id); save(); renderAll(); toast('학습 기록을 모두 지웠습니다.'); break;
      case 'addWorkflowButton': addWorkflow(); break;
      case 'openChromeSettingsButton': window.location.assign('chrome://settings/'); break;
    }
  });

  document.addEventListener('input', event => {
    if (event.target.id === 'personaNameInput') { state.persona.name = event.target.value; save(); renderProfile(); }
    if (event.target.id === 'fontScaleInput') { state.appearance.fontScale = Number(event.target.value); save(); applyRoot(); $('#fontScaleValue').textContent = `${event.target.value}%`; }
    if (event.target.id === 'railWidthInput') { state.appearance.railWidth = Number(event.target.value); save(); applyRoot(); $('#railWidthValue').textContent = `${event.target.value}px`; }
    if (event.target.id === 'hourInput') { state.adaptive.hour = Number(event.target.value); save(); $('#hourValue').textContent = HOURS[state.adaptive.hour]; renderStart(); }
  });

  document.addEventListener('keydown', event => {
    if (event.target.id === 'searchInput' && event.key === 'Enter') { event.preventDefault(); runSearch(); return; }
    if ((event.target.id === 'linkNameInput' || event.target.id === 'linkUrlInput') && event.key === 'Enter') { event.preventDefault(); addLink(); return; }
    if (event.target.id === 'workflowInput' && event.key === 'Enter') { event.preventDefault(); addWorkflow(); return; }
    if (event.key === 'Escape') {
      if (engineMenuOpen) { engineMenuOpen = false; renderStart(); }
      else if (auditOpen) { auditOpen = false; renderNavigation(); }
      else if (currentView !== 'start') switchView('start');
      return;
    }
    if ((event.metaKey || event.ctrlKey) && event.key === ',') { event.preventDefault(); settingsSection = 'agent'; switchView('settings'); return; }
    if ((event.metaKey || event.ctrlKey) && /^[1-6]$/.test(event.key) && state.workflow.shortcuts) {
      const pinned = allActions().filter(action => state.workflow.pinned.includes(action)).slice(0, 6);
      const action = pinned[Number(event.key) - 1];
      if (action) { event.preventDefault(); startAction(action); }
    }
  });

  window.mewebAgentTest = {
    getState: () => JSON.parse(JSON.stringify(state)),
    getProgress: () => taskProgress(),
    reset: () => { state = defaults(); save(); renderAll(); },
    openView: view => switchView(view),
    setAutonomy: value => { state.persona.autonomy = value; state.task.decisions = {}; save(); renderAll(); },
    persist: () => {
      // eslint-disable-next-line no-restricted-properties
      chrome.send('mewebAgentSaveState', [JSON.stringify(state)]);
      return true;
    },
  };

  renderAll();
  window.mewebAgentWorkspaceLoadState = serialized => {
    try {
      const parsed = JSON.parse(serialized || 'null');
      if (!parsed || typeof parsed !== 'object') return;
      const restored = defaults();
      for (const key of Object.keys(restored)) {
        if (parsed[key] && typeof parsed[key] === 'object') {
          Object.assign(restored[key], parsed[key]);
        }
      }
      if (!Array.isArray(restored.task.audit)) restored.task.audit = [];
      if (!restored.task.decisions ||
          typeof restored.task.decisions !== 'object') {
        restored.task.decisions = {};
      }
      state = restored;
      localStorage.setItem(STORAGE_KEY, JSON.stringify(state));
      renderAll();
    } catch (_error) {}
  };
  // eslint-disable-next-line no-restricted-properties
  chrome.send('mewebAgentLoadState');
})();
