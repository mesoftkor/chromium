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
  let modelAuthRequestSequence = 0;
  const pendingModelAuthRequests = new Map();
  window.mewebModelAuthResponse = (requestId, result) => {
    const resolve = pendingModelAuthRequests.get(requestId);
    if (!resolve) return;
    pendingModelAuthRequests.delete(requestId);
    resolve(result);
  };
  const sendModelAuthRequest = (method, ...args) => new Promise(resolve => {
    const requestId = `meweb-model-auth-${++modelAuthRequestSequence}`;
    pendingModelAuthRequests.set(requestId, resolve);
    // eslint-disable-next-line no-restricted-properties
    chrome.send(method, [requestId, ...args]);
  });
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
  const MODEL_PROFILES = {
    'openai/gpt-4o-mini': {
      provider: 'openai', providerLabel: 'OpenAI', name: 'gpt-4o-mini',
      endpoint: 'https://api.openai.com/v1/responses',
      temperature: 0.2, maxContextTokens: 4096, maxOutputTokens: 4096,
      credentialReference: 'OPENAI_API_KEY', toolFormat: 'tools',
      defaultAuthenticationMethod: 'oauth_wif',
      allowedAuthenticationMethods: ['oauth_wif', 'api_key'],
    },
    'openai/gpt-4.1': {
      provider: 'openai', providerLabel: 'OpenAI', name: 'gpt-4.1',
      endpoint: 'https://api.openai.com/v1/responses',
      temperature: 0.1, maxContextTokens: 8192, maxOutputTokens: 4096,
      credentialReference: 'OPENAI_API_KEY', toolFormat: 'tools',
      defaultAuthenticationMethod: 'oauth_wif',
      allowedAuthenticationMethods: ['oauth_wif', 'api_key'],
    },
    'anthropic/claude-sonnet-4-6': {
      provider: 'anthropic', providerLabel: 'Anthropic',
      name: 'claude-sonnet-4-6',
      endpoint: 'https://api.anthropic.com/v1/messages',
      temperature: 0.2, maxContextTokens: 1000000, maxOutputTokens: 4096,
      credentialReference: 'ANTHROPIC_API_KEY', toolFormat: 'tool_use',
      defaultAuthenticationMethod: 'cli_oauth',
      allowedAuthenticationMethods: ['cli_oauth', 'oauth_wif', 'api_key'],
    },
    'gemini/gemini-3.6-flash': {
      provider: 'gemini', providerLabel: 'Gemini', name: 'gemini-3.6-flash',
      endpoint: 'https://generativelanguage.googleapis.com/v1beta/models/gemini-3.6-flash:generateContent',
      temperature: 0.2, maxContextTokens: 1000000, maxOutputTokens: 4096,
      credentialReference: 'GEMINI_API_KEY',
      toolFormat: 'functionDeclarations',
      defaultAuthenticationMethod: 'api_key',
      allowedAuthenticationMethods: ['api_key', 'oauth_access_token'],
    },
    'ollama/qwen2.5-14b': {
      provider: 'ollama', providerLabel: 'Ollama Local', name: 'qwen2.5-14b',
      endpoint: 'http://127.0.0.1:11434/api/chat',
      temperature: 0.2, maxContextTokens: 32768, maxOutputTokens: 4096,
      credentialReference: null, toolFormat: 'json_mode',
      defaultAuthenticationMethod: 'none',
      allowedAuthenticationMethods: ['none'],
    },
    'ollama/gpt-oss-120b-cloud': {
      provider: 'ollama', providerLabel: 'Ollama Cloud',
      name: 'gpt-oss:120b', endpoint: 'https://ollama.com/api/chat',
      temperature: 0.2, maxContextTokens: 131072, maxOutputTokens: 4096,
      credentialReference: 'OLLAMA_API_KEY', toolFormat: 'tools',
      defaultAuthenticationMethod: 'api_key',
      allowedAuthenticationMethods: ['api_key'],
    },
    'openrouter/auto': {
      provider: 'openrouter', providerLabel: 'OpenRouter',
      name: 'openrouter/auto',
      endpoint: 'https://openrouter.ai/api/v1/chat/completions',
      temperature: 0.2, maxContextTokens: 2000000, maxOutputTokens: 4096,
      credentialReference: 'OPENROUTER_API_KEY', toolFormat: 'tools',
      defaultAuthenticationMethod: 'api_key',
      allowedAuthenticationMethods: ['api_key', 'oauth_pkce'],
    },
  };
  const MODEL_PROFILE_IDS = Object.keys(MODEL_PROFILES);
  const LEGACY_MODEL_PROFILE_IDS = {
    'anthropic/claude-3-5-sonnet': 'anthropic/claude-sonnet-4-6',
    'google/gemini-1-5-pro': 'gemini/gemini-3.6-flash',
    'local/qwen2.5-14b': 'ollama/qwen2.5-14b',
  };
  const MODEL_AUTHENTICATION = {
    openai: {
      defaultMethod: 'oauth_wif',
      methods: {
        oauth_wif: {
          label: 'OAuth 2.0 WIF', accountType: 'service_account',
          interactive: false, brokerReference: 'meweb-native-auth://openai',
        },
        api_key: {
          label: 'API 키', accountType: 'service_account',
          interactive: false, brokerReference: 'OPENAI_API_KEY',
        },
      },
    },
    anthropic: {
      defaultMethod: 'cli_oauth',
      methods: {
        cli_oauth: {
          label: 'Anthropic 계정 OAuth', accountType: 'user',
          interactive: true, brokerReference: 'meweb-native-auth://anthropic',
        },
        oauth_wif: {
          label: 'OAuth 2.0 WIF', accountType: 'service_account',
          interactive: false, brokerReference: 'meweb-native-auth://anthropic',
        },
        api_key: {
          label: 'API 키', accountType: 'service_account',
          interactive: false, brokerReference: 'ANTHROPIC_API_KEY',
        },
      },
    },
    gemini: {
      defaultMethod: 'api_key',
      methods: {
        api_key: {
          label: 'API 키', accountType: 'service_account',
          interactive: true, brokerReference: 'meweb-native-auth://gemini',
        },
        oauth_access_token: {
          label: 'Google OAuth', accountType: 'user',
          interactive: true, brokerReference: 'meweb-native-auth://gemini',
        },
      },
    },
    ollama: {
      defaultMethod: 'none',
      methods: {
        none: {
          label: '로컬 연결', accountType: 'local',
          interactive: false, brokerReference: 'meweb-native-auth://ollama',
        },
        api_key: {
          label: 'Ollama Cloud API 키', accountType: 'service_account',
          interactive: true, brokerReference: 'meweb-native-auth://ollama',
        },
      },
    },
    openrouter: {
      defaultMethod: 'api_key',
      methods: {
        api_key: {
          label: 'API 키', accountType: 'user', interactive: true,
          brokerReference: 'meweb-native-auth://openrouter',
        },
        oauth_pkce: {
          label: 'OAuth PKCE 키', accountType: 'user', interactive: true,
          brokerReference: 'meweb-native-auth://openrouter',
        },
      },
    },
  };
  const MODEL_TOOL_CHOICES = ['required', 'auto', 'none'];
  const MODEL_SELECTOR_PROFILES = ['naver-desktop-v1', 'naver-mobile-v1'];
  const MODEL_FAIL_POLICIES = [
    'ask_human', 'retry_once_then_ask', 'strict_stop'];
  const SECRET_FIELD_SUFFIXES = [
    'apikey', 'accesstoken', 'refreshtoken', 'clientsecret',
    'password', 'authorization',
  ];

  function isSecretFieldName(value) {
    const normalized = String(value).replace(/[^a-z0-9]/gi, '').toLowerCase();
    return normalized === 'secret' ||
        SECRET_FIELD_SUFFIXES.some(suffix => normalized.endsWith(suffix));
  }

  function isLoopbackEndpoint(value) {
    try {
      const url = new URL(String(value));
      const host = url.hostname.replace(/^\[|\]$/g, '').toLowerCase();
      return ['http:', 'https:'].includes(url.protocol) &&
          ['127.0.0.1', 'localhost', '::1'].includes(host) &&
          !url.username && !url.password;
    } catch (_error) {
      return false;
    }
  }

  function containsSecretField(value) {
    if (!value || typeof value !== 'object') return false;
    return Object.entries(value).some(([key, child]) =>
      isSecretFieldName(key) || containsSecretField(child));
  }

  function modelDefaults(profileId = 'openai/gpt-4o-mini') {
    const migrated = LEGACY_MODEL_PROFILE_IDS[profileId] || profileId;
    const selected = MODEL_PROFILES[migrated] ? migrated : MODEL_PROFILE_IDS[0];
    const profile = MODEL_PROFILES[selected];
    return {
      selected,
      authenticationMethod:
          profile.defaultAuthenticationMethod ||
          MODEL_AUTHENTICATION[profile.provider].defaultMethod,
      endpoint: profile.endpoint,
      temperature: profile.temperature,
      maxOutputTokens: profile.maxOutputTokens,
      toolChoice: 'auto',
      selectorProfile: 'naver-desktop-v1',
      failPolicy: 'ask_human',
      retryLimit: 2,
      requireHumanPublishApproval: true,
    };
  }

  function sanitizeModelSettings(candidate) {
    const source = candidate && typeof candidate === 'object' ? candidate : {};
    const migrated = LEGACY_MODEL_PROFILE_IDS[source.selected] || source.selected;
    const selected = MODEL_PROFILES[migrated] ?
        migrated : 'openai/gpt-4o-mini';
    const profile = MODEL_PROFILES[selected];
    const defaultsForProfile = modelDefaults(selected);
    const temperature = Number(source.temperature);
    const maxOutputTokens = Number(source.maxOutputTokens);
    const retryLimit = Number(source.retryLimit);
    const authentication = MODEL_AUTHENTICATION[profile.provider];
    return {
      selected,
      authenticationMethod:
          authentication.methods[source.authenticationMethod] &&
              profile.allowedAuthenticationMethods.includes(
                  source.authenticationMethod) ?
          source.authenticationMethod : profile.defaultAuthenticationMethod,
      endpoint: profile.provider === 'ollama' &&
              profile.allowedAuthenticationMethods.includes('none') &&
              isLoopbackEndpoint(source.endpoint) ?
          String(source.endpoint) : profile.endpoint,
      temperature: Number.isFinite(temperature) &&
              temperature >= 0 && temperature <= 1 ?
          temperature : defaultsForProfile.temperature,
      maxOutputTokens: Number.isInteger(maxOutputTokens) &&
              maxOutputTokens >= 256 &&
              maxOutputTokens <= profile.maxContextTokens ?
          maxOutputTokens : defaultsForProfile.maxOutputTokens,
      toolChoice: MODEL_TOOL_CHOICES.includes(source.toolChoice) ?
          source.toolChoice : defaultsForProfile.toolChoice,
      selectorProfile:
          MODEL_SELECTOR_PROFILES.includes(source.selectorProfile) ?
          source.selectorProfile : defaultsForProfile.selectorProfile,
      failPolicy: MODEL_FAIL_POLICIES.includes(source.failPolicy) ?
          source.failPolicy : defaultsForProfile.failPolicy,
      retryLimit: Number.isInteger(retryLimit) &&
              retryLimit >= 0 && retryLimit <= 5 ?
          retryLimit : defaultsForProfile.retryLimit,
      requireHumanPublishApproval: true,
    };
  }

  function validateModelSettings(candidate) {
    const errors = [];
    if (!candidate || typeof candidate !== 'object') {
      return {valid: false, errors: ['모델 설정이 객체가 아닙니다.']};
    }
    if (containsSecretField(candidate)) {
      errors.push('API 키·토큰·비밀번호는 모델 설정에 저장할 수 없습니다.');
    }
    const profile = MODEL_PROFILES[candidate.selected];
    if (!profile) errors.push('지원하지 않는 모델 프로필입니다.');
    if (profile) {
      const authentication = MODEL_AUTHENTICATION[profile.provider];
      if (!authentication.methods[candidate.authenticationMethod] ||
          !profile.allowedAuthenticationMethods.includes(
              candidate.authenticationMethod)) {
        errors.push('선택한 공급자에서 지원하지 않는 인증 방식입니다.');
      }
      if (profile.provider === 'ollama' &&
          profile.allowedAuthenticationMethods.includes('none')) {
        if (!isLoopbackEndpoint(candidate.endpoint)) {
          errors.push('로컬 모델 주소는 localhost 또는 loopback만 허용합니다.');
        }
      } else if (candidate.endpoint !== profile.endpoint) {
        errors.push('클라우드 모델 주소는 검증된 기본 주소만 허용합니다.');
      }
      const outputTokens = Number(candidate.maxOutputTokens);
      if (!Number.isInteger(outputTokens) || outputTokens < 256 ||
          outputTokens > profile.maxContextTokens) {
        errors.push(`출력 토큰은 256~${profile.maxContextTokens} 범위여야 합니다.`);
      }
    }
    const temperature = Number(candidate.temperature);
    if (!Number.isFinite(temperature) || temperature < 0 || temperature > 1) {
      errors.push('온도는 0~1 범위여야 합니다.');
    }
    if (!MODEL_TOOL_CHOICES.includes(candidate.toolChoice)) {
      errors.push('도구 호출 정책이 올바르지 않습니다.');
    }
    if (!MODEL_SELECTOR_PROFILES.includes(candidate.selectorProfile)) {
      errors.push('UI 셀렉터 프로필이 올바르지 않습니다.');
    }
    if (!MODEL_FAIL_POLICIES.includes(candidate.failPolicy)) {
      errors.push('실패 정책이 올바르지 않습니다.');
    }
    const retryLimit = Number(candidate.retryLimit);
    if (!Number.isInteger(retryLimit) || retryLimit < 0 || retryLimit > 5) {
      errors.push('동작별 재시도 횟수는 0~5 범위여야 합니다.');
    }
    if (candidate.requireHumanPublishApproval !== true) {
      errors.push('발행 전 사용자 승인 보호는 끌 수 없습니다.');
    }
    return {valid: errors.length === 0, errors};
  }

  function effectiveModelRuntime(candidate = state?.model) {
    const settings = sanitizeModelSettings(candidate);
    const profile = MODEL_PROFILES[settings.selected];
    const authentication =
        MODEL_AUTHENTICATION[profile.provider].methods[
            settings.authenticationMethod];
    const credentialReference = authentication.brokerReference ||
        profile.credentialReference;
    return {
      schema_version: 'meweb-agent-model-runtime-v1',
      selected_model: {
        id: settings.selected,
        provider: profile.provider,
        name: profile.name,
        endpoint: settings.endpoint,
        temperature: settings.temperature,
        max_context_tokens: profile.maxContextTokens,
        max_output_tokens: settings.maxOutputTokens,
        tool_choice: settings.toolChoice,
        tool_format: profile.toolFormat,
        credential_reference: credentialReference,
        authentication: {
          method: settings.authenticationMethod,
          account_type: authentication.accountType,
          interactive_login: authentication.interactive,
          broker_reference: authentication.brokerReference,
          native_broker: true,
          provider_connection_required:
              settings.authenticationMethod !== 'none',
          token_persisted_by_meweb: false,
        },
      },
      selected_selector_profile: settings.selectorProfile,
      runtime: {
        fail_policy: settings.failPolicy,
        retry_limit_per_action: settings.retryLimit,
        require_human_publish_approval: true,
      },
      prompt: {
        document: 'docs/agent-browser-system-prompt.md',
        model_agnostic: true,
      },
      security: {
        credentials_persisted: false,
        oauth_access_tokens_memory_only: true,
        python_runtime_required: false,
        cloud_endpoints_locked: true,
        local_endpoint_loopback_only: true,
        publish_click_guard: true,
      },
    };
  }

  function defaults() {
    return {
      appearance: {theme: 'midnight', accent: 'mint', density: 'default', fontScale: 100, rail: 'left', railWidth: 220},
      persona: {avatar: 'ribbon', name: '미웹', domain: 'finance', tone: 'standard', autonomy: 'risky', showReason: true},
      workflow: {custom: [], pinned: ['이월 항목 대사', '월마감 체크리스트'], removed: [], shortcuts: true},
      adaptive: {enabled: true, basis: 'time', hour: 0, forgotten: []},
      model: modelDefaults(),
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
      base.model = sanitizeModelSettings(parsed.model);
      if (!Array.isArray(base.task.audit)) base.task.audit = [];
      if (!base.task.decisions || typeof base.task.decisions !== 'object') base.task.decisions = {};
    } catch (_error) {}
    return base;
  }

  let state = loadState();
  let modelAuthConnection = {
    key: '', status: 'unknown', connected: false,
    message: '연결 상태를 확인하지 않았습니다.',
  };
  let modelInference = {
    key: '', status: 'idle', completed: false,
    message: '아직 모델 응답 시험을 실행하지 않았습니다.',
    text: '', tool_calls: [],
    usage: {input_tokens: 0, output_tokens: 0, total_tokens: 0},
  };
  let modelTestPrompt = 'MEWEB 모델 연결 시험입니다. 한 문장으로 응답하세요.';
  let currentView = 'start';
  let settingsSection = 'agent';
  let engineMenuOpen = false;
  let auditOpen = false;
  let toastTimer = 0;

  function save() {
    state.model = sanitizeModelSettings(state.model);
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
  function modelProfile() {
    return MODEL_PROFILES[state.model.selected] || MODEL_PROFILES[MODEL_PROFILE_IDS[0]];
  }
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
    $('#modelChip').textContent =
        `${modelProfile().providerLabel} · ${modelProfile().name}`;
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
      settings: `<b>환경설정</b> · ${esc({appearance:'모양', search:'검색엔진', startup:'시작 화면', agent:'에이전트', models:'AI 모델', privacy:'개인정보 보호', downloads:'다운로드', updates:'업데이트·복구'}[settingsSection])}`,
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
    } else if (settingsSection === 'models') {
      const selectedProfile = modelProfile();
      const endpointEditable = selectedProfile.provider === 'ollama' &&
          selectedProfile.allowedAuthenticationMethods.includes('none');
      const authentication = MODEL_AUTHENTICATION[selectedProfile.provider];
      const authenticationMethod =
          authentication.methods[state.model.authenticationMethod];
      const authenticationOptions = Object.entries(authentication.methods)
          .filter(([id]) =>
            selectedProfile.allowedAuthenticationMethods.includes(id))
          .map(([id, definition]) => [id, id, definition.label]);
      let authenticationHelp = '이 모델은 별도 로그인이 필요하지 않습니다.';
      if (state.model.authenticationMethod === 'oauth_wif') {
        authenticationHelp = selectedProfile.provider === 'openai' ?
            'OpenAI 조직의 Identity Provider와 서비스 계정 매핑을 사용합니다. 일반 OpenAI 사용자 계정 로그인은 API에서 제공되지 않습니다.' :
            'Anthropic 조직의 Federation Rule과 서비스 계정 매핑을 사용합니다.';
      } else if (state.model.authenticationMethod === 'cli_oauth') {
        authenticationHelp = 'Anthropic 공식 ant auth login 후 ant auth print-credentials --access-token으로 받은 단기 토큰을 현재 앱 세션에 연결합니다.';
      } else if (state.model.authenticationMethod === 'oauth_access_token') {
        authenticationHelp = 'Google OAuth 데스크톱 앱 흐름에서 발급한 액세스 토큰을 현재 앱 세션에만 연결합니다.';
      } else if (state.model.authenticationMethod === 'oauth_pkce') {
        authenticationHelp = 'OpenRouter 공식 PKCE 흐름이 반환한 사용자 제어 API 키를 현재 앱 세션에 연결합니다.';
      } else if (state.model.authenticationMethod === 'api_key') {
        authenticationHelp = `${selectedProfile.credentialReference} 또는 아래 입력값을 C++23 네이티브 브로커가 현재 앱 세션 메모리에서만 사용합니다.`;
      }
      const authKey = `${selectedProfile.provider}/${state.model.authenticationMethod}`;
      const displayedAuthStatus = modelAuthConnection.key === authKey ?
          modelAuthConnection : {
            status: 'unknown', connected: false,
            message: '연결 상태를 확인하지 않았습니다.',
          };
      const displayedInference = modelInference.key === authKey ?
          modelInference : {
            status: 'idle', completed: false,
            message: '현재 공급자에서 모델 응답 시험을 실행하지 않았습니다.',
            text: '', tool_calls: [],
            usage: {input_tokens: 0, output_tokens: 0, total_tokens: 0},
          };
      const inferencePreview = JSON.stringify({
        status: displayedInference.status,
        text: displayedInference.text || '',
        tool_calls: displayedInference.tool_calls || [],
        finish_reason: displayedInference.finish_reason || '',
        usage: displayedInference.usage,
      }, null, 2);
      const needsCredentialInput = !['none', 'oauth_wif'].includes(
          state.model.authenticationMethod);
      const credentialPlaceholder = state.model.authenticationMethod ===
              'cli_oauth' ? 'Anthropic OAuth 액세스 토큰' :
          state.model.authenticationMethod === 'oauth_access_token' ?
              'Google OAuth 액세스 토큰' :
          state.model.authenticationMethod === 'oauth_pkce' ?
              'OpenRouter PKCE로 발급된 키' :
              `${selectedProfile.providerLabel} API 키`;
      const runtimePreview = JSON.stringify(effectiveModelRuntime(), null, 2);
      setHtml(content, `<h1>AI 모델</h1><p class="lede">모델 공급자를 바꿔도 같은 시스템 프롬프트와 안전 정책을 사용합니다.</p>
        <section class="settings-card"><h2>모델 프로필</h2>
          <div class="setting-row"><div class="model-grid">${MODEL_PROFILE_IDS.map(id => {
            const profile = MODEL_PROFILES[id];
            return `<button class="model-card" data-model-profile="${esc(id)}" aria-pressed="${id === state.model.selected}"><b>${esc(profile.providerLabel)} · ${esc(profile.name)}</b><small>컨텍스트 ${profile.maxContextTokens.toLocaleString()} · ${esc(profile.toolFormat)}</small></button>`;
          }).join('')}</div></div>
          ${row('API 주소', endpointEditable ? '로컬 모델은 localhost 또는 loopback 주소만 허용합니다.' : '클라우드 공급자의 검증된 기본 주소로 잠겨 있습니다.', `<div class="setting-control"><input class="text-input" id="modelEndpointInput" maxlength="300" value="${esc(state.model.endpoint)}" ${endpointEditable ? '' : 'disabled'}></div>`)}
        </section>
        <section class="settings-card" data-model-authentication="${esc(selectedProfile.provider)}"><h2>공급자 로그인</h2>
          ${row('인증 방식', 'OpenAI와 Anthropic은 공식 지원 방식만 사용합니다.', optionGroup('model.authenticationMethod', authenticationOptions))}
          <div class="setting-row"><div class="note"><b>${esc(authenticationMethod.label)} · 비밀값은 Chromium Preferences와 localStorage에 저장하지 않습니다.</b><br>${esc(authenticationHelp)}</div></div>
          ${row('네이티브 연결', 'Python 없이 Chromium C++23 브로커가 검증합니다.', `<div class="setting-control">${needsCredentialInput ? `<input class="text-input" type="password" id="modelCredentialInput" maxlength="16384" autocomplete="off" autocapitalize="off" spellcheck="false" placeholder="${esc(credentialPlaceholder)}">` : ''}<button class="btn primary" id="connectModelProviderButton">연결</button><button class="btn" id="checkModelProviderButton">상태 확인</button><button class="btn" id="disconnectModelProviderButton" ${displayedAuthStatus.connected ? '' : 'disabled'}>연결 해제</button></div>`)}
          <div class="setting-row"><div class="validation ${displayedAuthStatus.status === 'error' ? 'error' : ''}" id="modelAuthStatus" data-status="${esc(displayedAuthStatus.status)}">${esc(displayedAuthStatus.message)}</div></div>
        </section>
        <section class="settings-card"><h2>실제 모델 응답 시험</h2>
          ${row('시험 프롬프트', '입력과 응답은 프로필 설정에 저장하지 않습니다.', `<div class="setting-control"><textarea class="text-input model-prompt" id="modelTestPromptInput" maxlength="65536" rows="3" spellcheck="false">${esc(modelTestPrompt)}</textarea></div>`)}
          <div class="setting-row"><div class="setting-control"><button class="btn primary" id="testModelResponseButton" ${displayedAuthStatus.connected && displayedInference.status !== 'running' ? '' : 'disabled'}>응답 보내기</button><button class="btn" id="cancelModelResponseButton" ${displayedInference.status === 'running' ? '' : 'disabled'}>요청 취소</button></div><div class="validation ${displayedInference.status === 'error' ? 'error' : ''}" id="modelInferenceStatus" data-status="${esc(displayedInference.status)}">${esc(displayedInference.message)}</div></div>
          <div class="setting-row"><pre class="code-preview" id="modelInferenceResult">${esc(inferencePreview)}</pre></div>
        </section>
        <section class="settings-card"><h2>생성·도구 설정</h2>
          ${row('온도', '결정적인 브라우저 조작을 위해 0.1~0.3을 권장합니다.', `<div class="setting-control"><input type="range" id="modelTemperatureInput" min="0" max="1" step="0.1" value="${state.model.temperature}"><span class="value" id="modelTemperatureValue">${state.model.temperature.toFixed(1)}</span></div>`)}
          ${row('최대 출력 토큰', `선택 모델 컨텍스트 한도 ${selectedProfile.maxContextTokens.toLocaleString()} 안에서 설정합니다.`, `<div class="setting-control"><input class="text-input" type="number" id="modelOutputTokensInput" min="256" max="${selectedProfile.maxContextTokens}" step="256" value="${state.model.maxOutputTokens}"></div>`)}
          ${row('도구 호출', 'required는 도구 사용 강제, auto는 모델 판단, none은 텍스트 전용입니다.', optionGroup('model.toolChoice', [['required','required','필수'],['auto','auto','자동'],['none','none','사용 안 함']]))}
          ${row('UI 셀렉터', '네이버 데스크톱과 모바일 프로필을 분리합니다.', optionGroup('model.selectorProfile', [['naver-desktop-v1','naver-desktop-v1','데스크톱'],['naver-mobile-v1','naver-mobile-v1','모바일']]))}
        </section>
        <section class="settings-card"><h2>실패·승인 정책</h2>
          ${row('실패 정책', '모르는 상태를 추측하지 않고 사용자에게 넘기는 방식을 결정합니다.', optionGroup('model.failPolicy', [['ask_human','ask_human','사용자 확인'],['retry_once_then_ask','retry_once_then_ask','1회 재시도'],['strict_stop','strict_stop','즉시 중단']]))}
          ${row('동작별 재시도', '0~5회. 같은 상태를 재관찰한 뒤에만 다시 시도합니다.', `<div class="setting-control"><input type="range" id="modelRetryInput" min="0" max="5" step="1" value="${state.model.retryLimit}"><span class="value" id="modelRetryValue">${state.model.retryLimit}회</span></div>`)}
          ${row('발행 전 사용자 승인', '배포팩 1차 안전 경계이며 어떤 모델에서도 끌 수 없습니다.', '<span class="value">항상 사용</span>')}
          <div class="setting-row"><div class="setting-control"><button class="btn primary" id="applyModelSettingsButton">설정 검증·적용</button><button class="btn" id="resetModelSettingsButton">모델 기본값</button></div><div class="validation" id="modelValidationStatus">현재 설정은 공급자 중립 런타임 형식으로 변환됩니다.</div></div>
        </section>
        <section class="settings-card"><h2>런타임 미리보기</h2><div class="setting-row"><pre class="code-preview" id="modelRuntimePreview">${esc(runtimePreview)}</pre></div></section>`);
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
    } else if (settingsSection === 'updates') {
      setHtml(content, `<h1>업데이트·복구</h1><p class="lede">MEWEB는 새 버전을 서명과 해시로 확인한 뒤 설치합니다.</p>
        <section class="settings-card"><h2>업데이트</h2>
          ${row('업데이트 확인', '다운로드가 중단됐다면 다시 확인하면 처음부터 안전하게 재시도합니다.', '<button class="btn primary" id="openUpdateSettingsButton">업데이트 확인 열기</button>')}
          ${row('설치 보호', '손상됐거나 서명이 다른 업데이트는 설치하지 않으며 현재 버전을 유지합니다.', '<span class="value">사용 중</span>')}
          ${row('복구 사본', '설치 직전 버전은 검증이 끝날 때까지 롤백 사본으로 유지합니다.', '<span class="value">자동</span>')}
        </section>
        <section class="settings-card"><h2>비정상 종료 복구</h2>
          <div class="setting-row"><div class="note">브라우저가 비정상 종료되면 다음 실행에서 <b>복원</b>을 선택해 이전 창과 탭을 되살릴 수 있습니다.</div></div>
        </section>
        <section class="settings-card"><h2>진단 안내</h2>
          ${row('업데이터 로그', '확인·다운로드 오류와 재시도 기록', '<code>~/Library/Application Support/MESOFT/MEWEBUpdater/updater.log</code>')}
          ${row('설치 로그', '설치·검증·롤백 결과와 MEWEB-U 오류 코드', '<code>~/Library/Logs/MEWEB/updater-install.jsonl</code>')}
          <div class="setting-row"><div class="note"><b>MEWEB-U100~U106</b>: 업데이트 파일 또는 대상 오류<br><b>MEWEB-U107~U110</b>: 서명·버전·설치 검증 오류<br><b>MEWEB-U111~U115</b>: 설치 또는 롤백 오류<br>오류가 반복되면 위 로그와 표시된 코드를 함께 전달하세요.</div></div>
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

  function selectModelProfile(profileId) {
    if (!MODEL_PROFILES[profileId]) return;
    const previous = state.model.selected;
    state.model = modelDefaults(profileId);
    record(`AI 모델을 ${previous}에서 ${profileId}(으)로 변경했습니다.`);
    renderAll();
    toast(`${MODEL_PROFILES[profileId].name} 모델을 선택했습니다.`);
  }

  function modelCandidateFromControls() {
    return {
      ...state.model,
      endpoint: $('#modelEndpointInput')?.value || state.model.endpoint,
      temperature: Number(
          $('#modelTemperatureInput')?.value ?? state.model.temperature),
      maxOutputTokens: Number(
          $('#modelOutputTokensInput')?.value ?? state.model.maxOutputTokens),
      retryLimit: Number($('#modelRetryInput')?.value ?? state.model.retryLimit),
      requireHumanPublishApproval: true,
    };
  }

  function applyModelSettings() {
    const candidate = modelCandidateFromControls();
    const validation = validateModelSettings(candidate);
    const status = $('#modelValidationStatus');
    if (!validation.valid) {
      status.textContent = validation.errors.join(' ');
      status.className = 'validation error';
      toast('모델 설정을 적용하지 않았습니다.');
      return false;
    }
    state.model = sanitizeModelSettings(candidate);
    record(`AI 모델 설정을 검증해 적용했습니다: ${state.model.selected}`);
    renderAll();
    toast('모델 설정 검증과 적용을 완료했습니다.');
    return true;
  }

  function selectedAuthentication() {
    const profile = modelProfile();
    return {
      provider: profile.provider,
      method: state.model.authenticationMethod,
      endpoint: state.model.endpoint,
      key: `${profile.provider}/${state.model.authenticationMethod}`,
    };
  }

  function applyModelAuthResult(result) {
    const selected = selectedAuthentication();
    if (!result || result.provider !== selected.provider ||
        result.method !== selected.method) {
      return false;
    }
    modelAuthConnection = {
      key: selected.key,
      status: result.status || 'error',
      connected: result.connected === true,
      message: result.message || '공급자 연결 결과가 없습니다.',
    };
    renderSettings();
    return modelAuthConnection.connected;
  }

  async function checkModelProviderConnection() {
    const selected = selectedAuthentication();
    try {
      const result = await sendModelAuthRequest(
          'mewebModelAuthStatus', selected.provider, selected.method);
      applyModelAuthResult(result);
      return result;
    } catch (_error) {
      applyModelAuthResult({
        ...selected, status: 'error', connected: false,
        message: '네이티브 인증 브로커의 상태를 확인하지 못했습니다.',
      });
      return null;
    }
  }

  async function connectModelProvider() {
    const selected = selectedAuthentication();
    const input = $('#modelCredentialInput');
    const credential = input?.value || '';
    if (input) input.value = '';
    modelAuthConnection = {
      key: selected.key, status: 'connecting', connected: false,
      message: '공급자 연결과 자격 증명을 검증하고 있습니다.',
    };
    renderSettings();
    try {
      const result = await sendModelAuthRequest(
          'mewebModelAuthConnect', selected.provider, selected.method,
          credential, selected.endpoint);
      applyModelAuthResult(result);
      if (result?.connected) {
        record(`AI 모델 공급자를 네이티브 브로커에 연결했습니다: ${selected.key}`);
        toast('모델 공급자 연결을 확인했습니다.');
      } else {
        toast('모델 공급자 연결을 완료하지 못했습니다.');
      }
      return result;
    } catch (_error) {
      applyModelAuthResult({
        ...selected, status: 'error', connected: false,
        message: '네이티브 인증 브로커 호출에 실패했습니다.',
      });
      return null;
    }
  }

  async function disconnectModelProvider() {
    const selected = selectedAuthentication();
    try {
      const result = await sendModelAuthRequest(
          'mewebModelAuthDisconnect', selected.provider, selected.method);
      applyModelAuthResult(result);
      record(`AI 모델 공급자의 메모리 연결을 해제했습니다: ${selected.key}`);
      toast('모델 공급자 연결을 해제했습니다.');
      return result;
    } catch (_error) {
      return null;
    }
  }

  async function generateModelResponse(promptOverride, tools = []) {
    const selected = selectedAuthentication();
    const runtime = effectiveModelRuntime();
    const promptInput = $('#modelTestPromptInput');
    const candidatePrompt = promptOverride === undefined ?
        promptInput?.value : promptOverride;
    const prompt = String(candidatePrompt || '').trim();
    if (!prompt || prompt.length > 65536) {
      modelInference = {
        key: selected.key, status: 'error', completed: false,
        message: '시험 프롬프트는 1~65,536자여야 합니다.', text: '',
        tool_calls: [],
        usage: {input_tokens: 0, output_tokens: 0, total_tokens: 0},
      };
      renderSettings();
      return modelInference;
    }
    modelTestPrompt = prompt;
    modelInference = {
      key: selected.key, status: 'running', completed: false,
      message: `${runtime.selected_model.provider} 모델에 요청하고 있습니다.`,
      text: '', tool_calls: [],
      usage: {input_tokens: 0, output_tokens: 0, total_tokens: 0},
    };
    renderSettings();
    try {
      const result = await sendModelAuthRequest(
          'mewebModelGenerate', runtime.selected_model.provider,
          runtime.selected_model.authentication.method,
          runtime.selected_model.endpoint, runtime.selected_model.name,
          'MEWEB 연결 시험입니다. 안전 정책을 지키고 간결하게 응답하세요.',
          prompt, runtime.selected_model.max_output_tokens,
          runtime.selected_model.temperature, runtime.selected_model.tool_choice,
          JSON.stringify(Array.isArray(tools) ? tools : []));
      modelInference = {...result, key: selected.key};
      renderSettings();
      if (result?.completed) {
        record(`AI 모델의 실제 응답을 확인했습니다: ${selected.key}`);
        toast('모델 응답을 확인했습니다.');
      } else if (result?.status !== 'cancelled') {
        toast('모델 응답 시험을 완료하지 못했습니다.');
      }
      return result;
    } catch (_error) {
      modelInference = {
        key: selected.key, status: 'error', completed: false,
        message: '네이티브 추론 어댑터 호출에 실패했습니다.', text: '',
        tool_calls: [],
        usage: {input_tokens: 0, output_tokens: 0, total_tokens: 0},
      };
      renderSettings();
      return modelInference;
    }
  }

  async function cancelModelResponse() {
    const result = await sendModelAuthRequest('mewebModelCancel');
    if (result?.status === 'cancelled') {
      modelInference = {...result, key: selectedAuthentication().key};
      renderSettings();
      toast('모델 요청을 취소했습니다.');
    }
    return result;
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
    if (button.dataset.modelProfile) { selectModelProfile(button.dataset.modelProfile); return; }
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
      case 'applyModelSettingsButton': applyModelSettings(); break;
      case 'resetModelSettingsButton': state.model = modelDefaults(); record('AI 모델 설정을 기본값으로 되돌렸습니다.'); renderAll(); toast('모델 설정을 기본값으로 되돌렸습니다.'); break;
      case 'connectModelProviderButton': connectModelProvider(); break;
      case 'checkModelProviderButton': checkModelProviderConnection(); break;
      case 'disconnectModelProviderButton': disconnectModelProvider(); break;
      case 'testModelResponseButton': generateModelResponse(); break;
      case 'cancelModelResponseButton': cancelModelResponse(); break;
      case 'openChromeSettingsButton': window.location.assign('chrome://settings/'); break;
      case 'openUpdateSettingsButton': window.location.assign('chrome://settings/help'); break;
    }
  });

  document.addEventListener('input', event => {
    if (event.target.id === 'personaNameInput') { state.persona.name = event.target.value; save(); renderProfile(); }
    if (event.target.id === 'fontScaleInput') { state.appearance.fontScale = Number(event.target.value); save(); applyRoot(); $('#fontScaleValue').textContent = `${event.target.value}%`; }
    if (event.target.id === 'railWidthInput') { state.appearance.railWidth = Number(event.target.value); save(); applyRoot(); $('#railWidthValue').textContent = `${event.target.value}px`; }
    if (event.target.id === 'hourInput') { state.adaptive.hour = Number(event.target.value); save(); $('#hourValue').textContent = HOURS[state.adaptive.hour]; renderStart(); }
    if (event.target.id === 'modelTemperatureInput') { $('#modelTemperatureValue').textContent = Number(event.target.value).toFixed(1); }
    if (event.target.id === 'modelRetryInput') { $('#modelRetryValue').textContent = `${event.target.value}회`; }
    if (event.target.id === 'modelTestPromptInput') { modelTestPrompt = event.target.value; }
  });

  document.addEventListener('keydown', event => {
    if (event.target.id === 'searchInput' && event.key === 'Enter') { event.preventDefault(); runSearch(); return; }
    if ((event.target.id === 'linkNameInput' || event.target.id === 'linkUrlInput') && event.key === 'Enter') { event.preventDefault(); addLink(); return; }
    if (event.target.id === 'workflowInput' && event.key === 'Enter') { event.preventDefault(); addWorkflow(); return; }
    if ((event.target.id === 'modelEndpointInput' || event.target.id === 'modelOutputTokensInput') && event.key === 'Enter') { event.preventDefault(); applyModelSettings(); return; }
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
    getModelRuntimeConfig: () => JSON.parse(JSON.stringify(effectiveModelRuntime())),
    getModelAuthStatus: () => JSON.parse(JSON.stringify(modelAuthConnection)),
    connectModelProvider: credential => {
      const input = $('#modelCredentialInput');
      if (input) input.value = credential || '';
      return connectModelProvider();
    },
    checkModelProviderConnection: () => checkModelProviderConnection(),
    disconnectModelProvider: () => disconnectModelProvider(),
    generateModelResponse: (prompt, tools = []) =>
      generateModelResponse(prompt, tools),
    cancelModelResponse: () => cancelModelResponse(),
    getModelInference: () => JSON.parse(JSON.stringify(modelInference)),
    validateModelSettings: value => JSON.parse(JSON.stringify(validateModelSettings(value))),
    setModelProfile: value => selectModelProfile(value),
    importModelSettings: value => {
      const validation = validateModelSettings(value);
      if (!validation.valid) return validation;
      state.model = sanitizeModelSettings(value);
      save(); renderAll();
      return {valid: true, errors: []};
    },
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
      restored.model = sanitizeModelSettings(parsed.model);
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
