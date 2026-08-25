// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/new_tab_page/new_tab_page_ui.h"

#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "base/command_line.h"
#include "base/environment.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/logging.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/metrics/histogram_functions.h"
#include "base/notreached.h"
#include "base/strings/strcat.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "base/time/time.h"
#include "base/values.h"
#include "build/build_config.h"
#include "build/branding_buildflags.h"
#include "chrome/browser/autocomplete/aim_eligibility_service_factory.h"
#include "chrome/browser/browser_features.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/buildflags.h"
#include "chrome/browser/contextual_search/contextual_search_service_factory.h"
#include "chrome/browser/enterprise/util/managed_browser_utils.h"
#include "chrome/browser/new_tab_page/feature_promo_helper/new_tab_page_feature_promo_helper.h"
#include "chrome/browser/new_tab_page/modules/file_suggestion/drive_service.h"
#include "chrome/browser/new_tab_page/modules/file_suggestion/drive_suggestion_handler.h"
#include "chrome/browser/new_tab_page/modules/file_suggestion/microsoft_files_page_handler.h"
#include "chrome/browser/new_tab_page/modules/new_tab_page_modules.h"
#include "chrome/browser/new_tab_page/modules/v2/authentication/microsoft_auth_page_handler.h"
#include "chrome/browser/new_tab_page/modules/v2/calendar/google_calendar_page_handler.h"
#include "chrome/browser/new_tab_page/modules/v2/calendar/outlook_calendar_page_handler.h"
#include "chrome/browser/new_tab_page/modules/v2/most_relevant_tab_resumption/most_relevant_tab_resumption_page_handler.h"
#include "chrome/browser/new_tab_page/modules/v2/tab_groups/tab_groups_page_handler.h"
#include "chrome/browser/new_tab_page/new_tab_page_util.h"
#include "chrome/browser/new_tab_page/prefs/ntp_pref_names.h"
#include "chrome/browser/optimization_guide/optimization_guide_keyed_service.h"
#include "chrome/browser/optimization_guide/optimization_guide_keyed_service_factory.h"
#include "chrome/browser/page_image_service/image_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search/background/ntp_custom_background_service_factory.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/search_provider_logos/logo_service_factory.h"
#include "chrome/browser/segmentation_platform/segmentation_platform_service_factory.h"
#include "chrome/browser/signin/identity_manager_factory.h"
#include "chrome/browser/sync/sync_service_factory.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/user_education/browser_user_education_interface.h"
#include "chrome/browser/ui/views/side_panel/customize_chrome/customize_chrome_utils.h"
#include "chrome/browser/ui/views/side_panel/customize_chrome/side_panel_controller_views.h"
#include "chrome/browser/ui/webui/browser_command/browser_command_handler.h"
#include "chrome/browser/ui/webui/cr_components/composebox/composebox_handler.h"
#include "chrome/browser/ui/webui/cr_components/most_visited/most_visited_handler.h"
#include "chrome/browser/ui/webui/cr_components/searchbox/searchbox_handler.h"
#include "chrome/browser/ui/webui/customize_buttons/customize_buttons_handler.h"
#include "chrome/browser/ui/webui/favicon_source.h"
#include "chrome/browser/ui/webui/new_tab_page/action_chips/action_chips_generator.h"
#include "chrome/browser/ui/webui/new_tab_page/action_chips/action_chips_handler.h"
#include "chrome/browser/ui/webui/new_tab_page/action_chips/action_chips_metrics.h"
#include "chrome/browser/ui/webui/new_tab_page/action_chips/tab_id_generator.h"
#include "chrome/browser/ui/webui/new_tab_page/composebox/variations/composebox_fieldtrial.h"
#include "chrome/browser/ui/webui/new_tab_page/new_tab_page_handler.h"
#include "chrome/browser/ui/webui/new_tab_page/untrusted_source.h"
#include "chrome/browser/ui/webui/page_not_available_for_guest/page_not_available_for_guest_ui.h"
#include "chrome/browser/ui/webui/plural_string_handler.h"
#include "chrome/browser/ui/webui/sanitized_image/sanitized_image_source.h"
#include "chrome/browser/ui/webui/searchbox/realbox_handler.h"
#include "chrome/browser/ui/webui/theme_source.h"
#include "chrome/browser/ui/webui/webui_embedding_context.h"
#include "chrome/browser/user_education/user_education_service.h"
#include "chrome/browser/user_education/user_education_service_factory.h"
#include "chrome/common/channel_info.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/search/instant_types.h"
#include "chrome/common/url_constants.h"
#include "chrome/grit/branded_strings.h"
#include "chrome/grit/generated_resources.h"
#include "chrome/grit/new_tab_page_resources.h"
#include "chrome/grit/new_tab_page_resources_map.h"
#include "chrome/grit/theme_resources.h"
#include "components/commerce/core/commerce_feature_list.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/contextual_search/contextual_search_metrics_recorder.h"
#include "components/contextual_search/contextual_search_service.h"
#include "components/favicon_base/favicon_url_parser.h"
#include "components/google/core/common/google_util.h"
#include "components/grit/components_scaled_resources.h"
#include "components/history_clusters/core/features.h"
#include "components/lens/lens_features.h"
#include "components/lens/lens_overlay_invocation_source.h"
#include "components/lens/lens_url_utils.h"
#include "components/ntp_tiles/features.h"
#include "components/ntp_tiles/most_visited_sites.h"
#include "components/ntp_tiles/pref_names.h"
#include "components/ntp_tiles/tile_type.h"
#include "components/omnibox/browser/aim_eligibility_service.h"
#include "components/omnibox/browser/omnibox_prefs.h"
#include "components/omnibox/common/composebox_features.h"
#include "components/page_image_service/image_service.h"
#include "components/page_image_service/image_service_handler.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/search/ntp_features.h"
#include "components/search_engines/template_url_service.h"
#include "components/signin/public/identity_manager/accounts_in_cookie_jar_info.h"
#include "components/signin/public/identity_manager/identity_manager.h"
#include "components/strings/grit/components_strings.h"
#include "components/sync/service/sync_service.h"
#include "components/user_education/common/ntp_promo/ntp_promo_controller.h"
#include "components/user_education/common/user_education_features.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "google_apis/gaia/core_account_id.h"
#include "media/base/media_switches.h"
#include "mojo/public/cpp/base/big_buffer.h"
#include "net/base/net_errors.h"
#include "net/base/url_util.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_request_headers.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "services/network/public/mojom/content_security_policy.mojom.h"
#include "services/network/public/mojom/url_response_head.mojom.h"
#include "skia/ext/skia_utils_base.h"
#include "third_party/omnibox_proto/chrome_aim_entry_point.pb.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/webui/web_ui_util.h"
#include "ui/color/color_provider.h"
#include "ui/native_theme/native_theme.h"
#include "ui/webui/resources/grit/webui_resources.h"
#include "ui/webui/resources/grit/webui_resources_map.h"
#include "ui/webui/tracked_element/tracked_element_handler_document_singleton.h"
#include "ui/webui/webui_allowlist.h"
#include "ui/webui/webui_util.h"
#include "url/origin.h"
#include "url/url_util.h"

#if !BUILDFLAG(IS_ANDROID)
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/tabs/public/tab_features.h"
#include "chrome/browser/ui/webui/new_tab_page/ntp_promo/ntp_promo_handler.h"
#endif  // !BUILDFLAG(IS_ANDROID)

#if !BUILDFLAG(OPTIMIZE_WEBUI)
#include "chrome/grit/new_tab_shared_resources.h"
#include "chrome/grit/new_tab_shared_resources_map.h"
#endif  // !BUILDFLAG(OPTIMIZE_WEBUI)

#if !defined(OFFICIAL_BUILD)
#include "chrome/browser/ui/webui/new_tab_page/foo/foo_handler.h"
#endif

using content::BrowserContext;
using content::WebContents;

DEFINE_CLASS_ELEMENT_IDENTIFIER_VALUE(NewTabPageUI,
                                      kRealboxContextualEntrypointElementId);

// TODO(b/507919199): Remove this definition once Android implements the NTP
// customize chrome handler.
#if BUILDFLAG(IS_ANDROID)
DEFINE_CLASS_ELEMENT_IDENTIFIER_VALUE(CustomizeButtonsHandler,
                                      kCustomizeChromeButtonElementId);
#endif

bool NewTabPageUIConfig::IsWebUIEnabled(
    content::BrowserContext* browser_context) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  return !profile->IsOffTheRecord();
}

std::unique_ptr<content::WebUIController>
NewTabPageUIConfig::CreateWebUIController(content::WebUI* web_ui,
                                          const GURL& url) {
  Profile* profile = Profile::FromWebUI(web_ui);
  if (profile->IsGuestSession()) {
    return std::make_unique<PageNotAvailableForGuestUI>(
        web_ui, chrome::kChromeUINewTabPageHost);
  }
  return std::make_unique<NewTabPageUI>(web_ui);
}

namespace {

constexpr char kPrevNavigationTimePrefName[] = "NewTabPage.PrevNavigationTime";
constexpr char kMewebAgentWorkspaceStatePref[] =
    "meweb.agent_workspace_state";
constexpr size_t kMaxMewebAgentWorkspaceStateBytes = 512 * 1024;
constexpr size_t kMaxMewebModelAuthResponseBytes = 256 * 1024;
constexpr size_t kMaxMewebModelInferenceResponseBytes = 1024 * 1024;
constexpr size_t kMaxMewebIdentityTokenBytes = 64 * 1024;
constexpr size_t kMaxMewebCredentialBytes = 16 * 1024;
constexpr size_t kMaxMewebModelPromptBytes = 64 * 1024;
constexpr size_t kMaxMewebModelToolsBytes = 64 * 1024;
constexpr char kMewebModelTestOriginSwitch[] = "meweb-model-test-origin";
// The value for the "udm" (Unified Drilldown Mode) query parameter.
// value "50" triggers AI mode as opposed to traditional search.
constexpr char kAIMDisplayMode[] = "50";
// The value for the "atvm" (AIM Threads Visibility Mode) query parameter.
// value "3" corresponds to Threads Visibility Mode "Always Open".
constexpr char kAIMThreadsVisibilityMode[] = "3";

class MewebAgentWorkspaceHandler : public content::WebUIMessageHandler {
 public:
  explicit MewebAgentWorkspaceHandler(Profile* profile) : profile_(profile) {}

  MewebAgentWorkspaceHandler(const MewebAgentWorkspaceHandler&) = delete;
  MewebAgentWorkspaceHandler& operator=(const MewebAgentWorkspaceHandler&) =
      delete;

  void RegisterMessages() override {
    web_ui()->RegisterMessageCallback(
        "mewebAgentSaveState",
        base::BindRepeating(&MewebAgentWorkspaceHandler::HandleSaveState,
                            base::Unretained(this)));
    web_ui()->RegisterMessageCallback(
        "mewebAgentLoadState",
        base::BindRepeating(&MewebAgentWorkspaceHandler::HandleLoadState,
                            base::Unretained(this)));
    web_ui()->RegisterMessageCallback(
        "mewebModelAuthStatus",
        base::BindRepeating(&MewebAgentWorkspaceHandler::HandleModelAuthStatus,
                            base::Unretained(this)));
    web_ui()->RegisterMessageCallback(
        "mewebModelAuthConnect",
        base::BindRepeating(&MewebAgentWorkspaceHandler::HandleModelAuthConnect,
                            base::Unretained(this)));
    web_ui()->RegisterMessageCallback(
        "mewebModelAuthDisconnect",
        base::BindRepeating(
            &MewebAgentWorkspaceHandler::HandleModelAuthDisconnect,
            base::Unretained(this)));
    web_ui()->RegisterMessageCallback(
        "mewebModelGenerate",
        base::BindRepeating(&MewebAgentWorkspaceHandler::HandleModelGenerate,
                            base::Unretained(this)));
    web_ui()->RegisterMessageCallback(
        "mewebModelCancel",
        base::BindRepeating(&MewebAgentWorkspaceHandler::HandleModelCancel,
                            base::Unretained(this)));
  }

 private:
  struct MemoryCredential {
    std::string value;
    base::Time expires_at;
  };

  struct PendingInference {
    base::Value callback_id;
    std::string provider;
    std::string model;
  };

  static std::string CredentialKey(std::string_view provider,
                                   std::string_view method) {
    return base::StrCat({provider, "/", method});
  }

  static bool IsSupportedAuthentication(std::string_view provider,
                                        std::string_view method) {
    if (provider == "openai") {
      return method == "oauth_wif" || method == "api_key";
    }
    if (provider == "anthropic") {
      return method == "cli_oauth" || method == "oauth_wif" ||
             method == "api_key";
    }
    if (provider == "gemini") {
      return method == "oauth_access_token" || method == "api_key";
    }
    if (provider == "ollama") {
      return method == "none" || method == "api_key";
    }
    if (provider == "openrouter") {
      return method == "oauth_pkce" || method == "api_key";
    }
    return false;
  }

  static bool IsLoopbackHost(std::string_view host) {
    return host == "127.0.0.1" || host == "localhost" || host == "[::1]" ||
           host == "::1";
  }

  static bool IsAllowedEndpoint(std::string_view provider, const GURL& url) {
    if (!url.is_valid() || url.has_username() || url.has_password()) {
      return false;
    }
    if (provider == "ollama") {
      return ((url.SchemeIsHTTPOrHTTPS() && IsLoopbackHost(url.host())) ||
              url.DeprecatedGetOriginAsURL() == GURL("https://ollama.com/"));
    }
    static constexpr std::pair<std::string_view, std::string_view>
        kAllowedOrigins[] = {
            {"openai", "https://api.openai.com/"},
            {"anthropic", "https://api.anthropic.com/"},
            {"gemini", "https://generativelanguage.googleapis.com/"},
            {"openrouter", "https://openrouter.ai/"},
        };
    for (const auto& [allowed_provider, origin] : kAllowedOrigins) {
      if (provider == allowed_provider) {
        return url.DeprecatedGetOriginAsURL() == GURL(origin);
      }
    }
    return false;
  }

  static std::optional<GURL> ModelTestOrigin() {
#if !defined(OFFICIAL_BUILD)
    const std::string value =
        base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
            kMewebModelTestOriginSwitch);
    const GURL url(value);
    if (!value.empty() && url.is_valid() && url.SchemeIsHTTPOrHTTPS() &&
        IsLoopbackHost(url.host()) && !url.has_username() &&
        !url.has_password()) {
      return url.DeprecatedGetOriginAsURL();
    }
#endif
    return std::nullopt;
  }

  static std::string CredentialEnvironmentVariable(
      std::string_view provider,
      std::string_view method) {
    if (method == "api_key") {
      if (provider == "openai") return "OPENAI_API_KEY";
      if (provider == "anthropic") return "ANTHROPIC_API_KEY";
      if (provider == "gemini") return "GEMINI_API_KEY";
      if (provider == "ollama") return "OLLAMA_API_KEY";
      if (provider == "openrouter") return "OPENROUTER_API_KEY";
    }
    if (provider == "anthropic" && method == "cli_oauth") {
      return "ANTHROPIC_OAUTH_TOKEN";
    }
    if (provider == "gemini" && method == "oauth_access_token") {
      return "GOOGLE_OAUTH_ACCESS_TOKEN";
    }
    if (provider == "openrouter" && method == "oauth_pkce") {
      return "OPENROUTER_OAUTH_KEY";
    }
    return std::string();
  }

  static GURL ValidationUrl(std::string_view provider, const GURL& endpoint) {
    if (auto test_origin = ModelTestOrigin()) {
      return test_origin->Resolve(base::StrCat({provider, "/models"}));
    }
    if (provider == "openai") return GURL("https://api.openai.com/v1/models");
    if (provider == "anthropic") {
      return GURL("https://api.anthropic.com/v1/models");
    }
    if (provider == "gemini") {
      return GURL("https://generativelanguage.googleapis.com/v1beta/models");
    }
    if (provider == "openrouter") {
      return GURL("https://openrouter.ai/api/v1/models");
    }
    if (provider == "ollama") {
      return endpoint.DeprecatedGetOriginAsURL().Resolve("api/version");
    }
    return GURL();
  }

  static GURL InferenceUrl(std::string_view provider, const GURL& endpoint) {
    if (auto test_origin = ModelTestOrigin()) {
      return test_origin->Resolve(base::StrCat({provider, "/generate"}));
    }
    if (provider == "openai") {
      return GURL("https://api.openai.com/v1/responses");
    }
    if (provider == "anthropic") {
      return GURL("https://api.anthropic.com/v1/messages");
    }
    if (provider == "gemini") return endpoint;
    if (provider == "openrouter") {
      return GURL("https://openrouter.ai/api/v1/chat/completions");
    }
    if (provider == "ollama") {
      return endpoint.DeprecatedGetOriginAsURL().Resolve("api/chat");
    }
    return GURL();
  }

  base::DictValue AuthResult(std::string_view provider,
                             std::string_view method,
                             bool connected,
                             std::string_view status,
                             std::string_view message) const {
    base::DictValue result;
    result.Set("provider", provider);
    result.Set("method", method);
    result.Set("connected", connected);
    result.Set("status", status);
    result.Set("message", message);
    result.Set("credential_persisted_by_meweb", false);
    result.Set("broker", "chromium_native_cxx23");
    return result;
  }

  void Reply(const base::Value& callback_id, base::DictValue result) {
    AllowJavascript();
    CallJavascriptFunction("mewebModelAuthResponse", callback_id,
                           base::Value(std::move(result)));
  }

  void HandleModelAuthStatus(const base::ListValue& args) {
    if (args.size() != 3 || !args[0].is_string() || !args[1].is_string() ||
        !args[2].is_string()) {
      return;
    }
    const std::string& provider = args[1].GetString();
    const std::string& method = args[2].GetString();
    if (!IsSupportedAuthentication(provider, method)) {
      Reply(args[0], AuthResult(provider, method, false, "error",
                                "지원하지 않는 인증 방식입니다."));
      return;
    }
    const std::string key = CredentialKey(provider, method);
    auto credential = credentials_.find(key);
    if (credential != credentials_.end() &&
        (credential->second.expires_at.is_max() ||
         credential->second.expires_at > base::Time::Now())) {
      Reply(args[0], AuthResult(provider, method, true, "connected",
                                "네이티브 메모리 브로커에 연결되어 있습니다."));
      return;
    }
    if (credential != credentials_.end()) credentials_.erase(credential);
    Reply(args[0], AuthResult(provider, method, false, "disconnected",
                              "현재 앱 세션에 연결된 자격 증명이 없습니다."));
  }

  void HandleModelAuthConnect(const base::ListValue& args) {
    if (args.size() != 5 || !args[0].is_string() || !args[1].is_string() ||
        !args[2].is_string() || !args[3].is_string() ||
        !args[4].is_string()) {
      return;
    }
    const std::string& provider = args[1].GetString();
    const std::string& method = args[2].GetString();
    std::string credential = args[3].GetString();
    const GURL endpoint(args[4].GetString());
    if (!IsSupportedAuthentication(provider, method) ||
        !IsAllowedEndpoint(provider, endpoint)) {
      Reply(args[0], AuthResult(provider, method, false, "error",
                                "공급자 또는 API 주소가 허용되지 않습니다."));
      return;
    }
    if (auth_loader_) {
      Reply(args[0], AuthResult(provider, method, false, "busy",
                                "다른 공급자 연결을 확인하고 있습니다."));
      return;
    }
    if (method == "oauth_wif") {
      BeginWifExchange(args[0].Clone(), provider, method);
      return;
    }
    if (credential.empty()) {
      const std::string variable =
          CredentialEnvironmentVariable(provider, method);
      if (!variable.empty()) {
        auto environment = base::Environment::Create();
        if (auto value = environment->GetVar(variable)) {
          credential = std::move(*value);
        }
      }
    }
    if (method != "none" &&
        (credential.empty() || credential.size() > kMaxMewebCredentialBytes)) {
      Reply(args[0], AuthResult(provider, method, false, "error",
                                "자격 증명이 없거나 허용 크기를 초과했습니다."));
      return;
    }
    BeginCredentialValidation(args[0].Clone(), provider, method,
                              std::move(credential), endpoint);
  }

  void HandleModelAuthDisconnect(const base::ListValue& args) {
    if (args.size() != 3 || !args[0].is_string() || !args[1].is_string() ||
        !args[2].is_string()) {
      return;
    }
    const std::string& provider = args[1].GetString();
    const std::string& method = args[2].GetString();
    credentials_.erase(CredentialKey(provider, method));
    Reply(args[0], AuthResult(provider, method, false, "disconnected",
                              "현재 앱 세션의 연결을 해제했습니다."));
  }

  static void AppendMessage(base::ListValue* messages,
                            std::string_view role,
                            std::string_view content) {
    if (content.empty()) return;
    base::DictValue message;
    message.Set("role", role);
    message.Set("content", content);
    messages->Append(std::move(message));
  }

  static std::string JsonString(const base::Value* value) {
    std::string serialized = "{}";
    if (value) base::JSONWriter::Write(*value, &serialized);
    return serialized;
  }

  static base::ListValue AdaptTools(std::string_view provider,
                                    const base::ListValue& tools) {
    base::ListValue adapted;
    for (const auto& item : tools) {
      if (!item.is_dict()) continue;
      const auto& tool = item.GetDict();
      const std::string* name = tool.FindString("name");
      if (!name || name->empty() || name->size() > 128) continue;
      const std::string* description = tool.FindString("description");
      const base::Value* parameters = tool.Find("parameters");
      base::DictValue function;
      function.Set("name", *name);
      if (description) function.Set("description", *description);
      function.Set("parameters",
                   parameters && parameters->is_dict()
                       ? parameters->Clone()
                       : base::Value(base::DictValue()));
      if (provider == "openai") {
        function.Set("type", "function");
        function.Set("strict", true);
        adapted.Append(std::move(function));
      } else if (provider == "anthropic") {
        base::DictValue anthropic_tool;
        anthropic_tool.Set("name", *name);
        if (description) anthropic_tool.Set("description", *description);
        anthropic_tool.Set(
            "input_schema",
            parameters && parameters->is_dict()
                ? parameters->Clone()
                : base::Value(base::DictValue()));
        adapted.Append(std::move(anthropic_tool));
      } else {
        base::DictValue wrapper;
        wrapper.Set("type", "function");
        wrapper.Set("function", std::move(function));
        adapted.Append(std::move(wrapper));
      }
    }
    return adapted;
  }

  static std::optional<base::DictValue> BuildInferencePayload(
      std::string_view provider,
      std::string_view model,
      std::string_view instructions,
      std::string_view prompt,
      int max_output_tokens,
      double temperature,
      std::string_view tool_choice,
      const base::ListValue& tools) {
    base::DictValue payload;
    payload.Set("model", model);
    if (provider == "openai") {
      payload.Set("input", prompt);
      if (!instructions.empty()) payload.Set("instructions", instructions);
      payload.Set("max_output_tokens", max_output_tokens);
      payload.Set("temperature", temperature);
      payload.Set("store", false);
      auto adapted = AdaptTools(provider, tools);
      if (!adapted.empty()) {
        payload.Set("tools", std::move(adapted));
        payload.Set("tool_choice", tool_choice);
      }
      return payload;
    }
    if (provider == "anthropic") {
      base::ListValue messages;
      AppendMessage(&messages, "user", prompt);
      payload.Set("messages", std::move(messages));
      if (!instructions.empty()) payload.Set("system", instructions);
      payload.Set("max_tokens", max_output_tokens);
      payload.Set("temperature", temperature);
      auto adapted = AdaptTools(provider, tools);
      if (!adapted.empty()) {
        payload.Set("tools", std::move(adapted));
        base::DictValue choice;
        choice.Set("type", tool_choice == "required" ? "any" : "auto");
        payload.Set("tool_choice", std::move(choice));
      }
      return payload;
    }
    if (provider == "gemini") {
      payload.Remove("model");
      base::DictValue user_part;
      user_part.Set("text", prompt);
      base::ListValue user_parts;
      user_parts.Append(std::move(user_part));
      base::DictValue content;
      content.Set("role", "user");
      content.Set("parts", std::move(user_parts));
      base::ListValue contents;
      contents.Append(std::move(content));
      payload.Set("contents", std::move(contents));
      if (!instructions.empty()) {
        base::DictValue system_part;
        system_part.Set("text", instructions);
        base::ListValue system_parts;
        system_parts.Append(std::move(system_part));
        base::DictValue system_instruction;
        system_instruction.Set("parts", std::move(system_parts));
        payload.Set("systemInstruction", std::move(system_instruction));
      }
      base::DictValue generation_config;
      generation_config.Set("maxOutputTokens", max_output_tokens);
      generation_config.Set("temperature", temperature);
      payload.Set("generationConfig", std::move(generation_config));
      if (!tools.empty()) {
        base::ListValue declarations;
        for (const auto& item : tools) {
          if (!item.is_dict()) continue;
          const auto& tool = item.GetDict();
          const std::string* name = tool.FindString("name");
          if (!name || name->empty()) continue;
          base::DictValue declaration;
          declaration.Set("name", *name);
          if (const std::string* description =
                  tool.FindString("description")) {
            declaration.Set("description", *description);
          }
          const base::Value* parameters = tool.Find("parameters");
          declaration.Set("parametersJsonSchema",
                          parameters && parameters->is_dict()
                              ? parameters->Clone()
                              : base::Value(base::DictValue()));
          declarations.Append(std::move(declaration));
        }
        if (!declarations.empty()) {
          base::DictValue functions;
          functions.Set("functionDeclarations", std::move(declarations));
          base::ListValue gemini_tools;
          gemini_tools.Append(std::move(functions));
          payload.Set("tools", std::move(gemini_tools));
          base::DictValue function_calling;
          function_calling.Set(
              "mode", tool_choice == "required" ? "ANY" :
                      tool_choice == "none" ? "NONE" : "AUTO");
          base::DictValue tool_config;
          tool_config.Set("functionCallingConfig", std::move(function_calling));
          payload.Set("toolConfig", std::move(tool_config));
        }
      }
      return payload;
    }
    if (provider == "ollama" || provider == "openrouter") {
      base::ListValue messages;
      AppendMessage(&messages, "system", instructions);
      AppendMessage(&messages, "user", prompt);
      payload.Set("messages", std::move(messages));
      if (provider == "ollama") {
        payload.Set("stream", false);
        base::DictValue options;
        options.Set("temperature", temperature);
        options.Set("num_predict", max_output_tokens);
        payload.Set("options", std::move(options));
      } else {
        payload.Set("max_tokens", max_output_tokens);
        payload.Set("temperature", temperature);
      }
      auto adapted = AdaptTools(provider, tools);
      if (!adapted.empty()) {
        payload.Set("tools", std::move(adapted));
        if (provider == "openrouter") payload.Set("tool_choice", tool_choice);
      }
      return payload;
    }
    return std::nullopt;
  }

  base::DictValue InferenceResult(std::string_view provider,
                                  std::string_view model,
                                  std::string_view status,
                                  std::string_view message) const {
    base::DictValue result;
    result.Set("provider", provider);
    result.Set("model", model);
    result.Set("status", status);
    result.Set("completed", status == "completed");
    result.Set("message", message);
    result.Set("text", "");
    result.Set("tool_calls", base::ListValue());
    result.Set("finish_reason", "");
    base::DictValue usage;
    usage.Set("input_tokens", 0);
    usage.Set("output_tokens", 0);
    usage.Set("total_tokens", 0);
    result.Set("usage", std::move(usage));
    result.Set("adapter", "chromium_native_cxx23");
    result.Set("credential_persisted_by_meweb", false);
    return result;
  }

  void HandleModelGenerate(const base::ListValue& args) {
    if (args.size() != 11 || !args[0].is_string() ||
        !args[1].is_string() || !args[2].is_string() ||
        !args[3].is_string() || !args[4].is_string() ||
        !args[5].is_string() || !args[6].is_string() ||
        !args[7].is_int() ||
        (!args[8].is_double() && !args[8].is_int()) ||
        !args[9].is_string() || !args[10].is_string()) {
      return;
    }
    const std::string& provider = args[1].GetString();
    const std::string& method = args[2].GetString();
    const GURL endpoint(args[3].GetString());
    const std::string& model = args[4].GetString();
    const std::string& instructions = args[5].GetString();
    const std::string& prompt = args[6].GetString();
    const int max_output_tokens = args[7].GetInt();
    const double temperature = args[8].is_double() ?
        args[8].GetDouble() : args[8].GetInt();
    const std::string& tool_choice = args[9].GetString();
    const std::string& tools_json = args[10].GetString();
    auto error = [&](std::string_view message) {
      Reply(args[0], InferenceResult(provider, model, "error", message));
    };
    if (!IsSupportedAuthentication(provider, method) ||
        !IsAllowedEndpoint(provider, endpoint)) {
      error("공급자, 인증 방식 또는 API 주소가 허용되지 않습니다.");
      return;
    }
    if (inference_loader_) {
      error("다른 모델 요청이 실행 중입니다.");
      return;
    }
    if (model.empty() || model.size() > 256 || prompt.empty() ||
        prompt.size() > kMaxMewebModelPromptBytes ||
        instructions.size() > kMaxMewebModelPromptBytes ||
        max_output_tokens < 1 || max_output_tokens > 131072 ||
        temperature < 0 || temperature > 1 ||
        (tool_choice != "required" && tool_choice != "auto" &&
         tool_choice != "none") ||
        tools_json.size() > kMaxMewebModelToolsBytes) {
      error("모델 요청 값이 허용 범위를 벗어났습니다.");
      return;
    }
    const std::string credential_key = CredentialKey(provider, method);
    auto credential = credentials_.find(credential_key);
    if (credential == credentials_.end() ||
        (!credential->second.expires_at.is_max() &&
         credential->second.expires_at <= base::Time::Now())) {
      if (credential != credentials_.end()) credentials_.erase(credential);
      error("먼저 현재 모델 공급자 연결을 완료하세요.");
      return;
    }
    auto parsed_tools = base::JSONReader::Read(tools_json, 0);
    if (!parsed_tools || !parsed_tools->is_list()) {
      error("도구 정의가 JSON 배열이 아닙니다.");
      return;
    }
    auto payload = BuildInferencePayload(
        provider, model, instructions, prompt, max_output_tokens, temperature,
        tool_choice, parsed_tools->GetList());
    if (!payload) {
      error("지원하지 않는 모델 공급자입니다.");
      return;
    }
    std::string body;
    if (!base::JSONWriter::Write(*payload, &body)) {
      error("모델 요청 JSON을 만들 수 없습니다.");
      return;
    }
    auto request = std::make_unique<network::ResourceRequest>();
    request->url = InferenceUrl(provider, endpoint);
    request->method = "POST";
    request->credentials_mode = network::mojom::CredentialsMode::kOmit;
    const std::string& secret = credential->second.value;
    if (method != "none") {
      if (provider == "anthropic" && method == "api_key") {
        request->headers.SetHeader("x-api-key", secret);
      } else if (provider == "gemini" && method == "api_key") {
        request->headers.SetHeader("x-goog-api-key", secret);
      } else {
        request->headers.SetHeader(net::HttpRequestHeaders::kAuthorization,
                                   base::StrCat({"Bearer ", secret}));
      }
    }
    if (provider == "anthropic") {
      request->headers.SetHeader("anthropic-version", "2023-06-01");
    }
    StartInference(std::move(request), std::move(body),
                   PendingInference{args[0].Clone(), provider, model});
  }

  void HandleModelCancel(const base::ListValue& args) {
    if (args.size() != 1 || !args[0].is_string()) return;
    auto result = InferenceResult("", "", "idle",
                                  "실행 중인 모델 요청이 없습니다.");
    if (pending_inference_) {
      PendingInference cancelled = std::move(*pending_inference_);
      pending_inference_.reset();
      inference_loader_.reset();
      Reply(cancelled.callback_id,
            InferenceResult(cancelled.provider, cancelled.model, "cancelled",
                            "사용자가 모델 요청을 취소했습니다."));
      result = InferenceResult(cancelled.provider, cancelled.model,
                               "cancelled", "모델 요청을 취소했습니다.");
    }
    Reply(args[0], std::move(result));
  }

  void BeginCredentialValidation(base::Value callback_id,
                                 std::string provider,
                                 std::string method,
                                 std::string credential,
                                 const GURL& endpoint) {
    auto request = std::make_unique<network::ResourceRequest>();
    request->url = ValidationUrl(provider, endpoint);
    request->method = "GET";
    request->credentials_mode = network::mojom::CredentialsMode::kOmit;
    if (method != "none") {
      if (provider == "anthropic" && method == "api_key") {
        request->headers.SetHeader("x-api-key", credential);
      } else if (provider == "gemini" && method == "api_key") {
        request->headers.SetHeader("x-goog-api-key", credential);
      } else {
        request->headers.SetHeader(net::HttpRequestHeaders::kAuthorization,
                                   base::StrCat({"Bearer ", credential}));
      }
      if (provider == "anthropic") {
        request->headers.SetHeader("anthropic-version", "2023-06-01");
      }
    }
    StartRequest(
        std::move(request), std::string(),
        base::BindOnce(&MewebAgentWorkspaceHandler::OnCredentialValidated,
                       weak_ptr_factory_.GetWeakPtr(), std::move(callback_id),
                       std::move(provider), std::move(method),
                       std::move(credential)));
  }

  void OnCredentialValidated(base::Value callback_id,
                             std::string provider,
                             std::string method,
                             std::string credential,
                             bool success,
                             std::optional<std::string> body) {
    if (!success) {
      Reply(callback_id,
            AuthResult(provider, method, false, "error",
                       "공급자 연결 검증에 실패했습니다. 주소와 자격 증명을 확인하세요."));
      return;
    }
    credentials_.insert_or_assign(
        CredentialKey(provider, method),
        MemoryCredential{std::move(credential), base::Time::Max()});
    Reply(callback_id,
          AuthResult(provider, method, true, "connected",
                     provider == "ollama" ?
                         "Ollama API 연결과 버전 응답을 확인했습니다." :
                         "공급자 API 연결과 자격 증명을 확인했습니다."));
  }

  bool ReadIdentityToken(const std::string& path_value,
                         std::string* token,
                         std::string* error) {
    const base::FilePath path = base::FilePath::FromUTF8Unsafe(path_value);
    if (path.empty() || base::IsLink(path)) {
      *error = "ID 토큰은 심볼릭 링크가 아닌 파일이어야 합니다.";
      return false;
    }
#if BUILDFLAG(IS_POSIX)
    int permissions = 0;
    if (!base::GetPosixFilePermissions(path, &permissions) ||
        (permissions & (base::FILE_PERMISSION_GROUP_MASK |
                        base::FILE_PERMISSION_OTHERS_MASK))) {
      *error = "ID 토큰 파일 권한은 소유자 전용이어야 합니다.";
      return false;
    }
#endif
    if (!base::ReadFileToStringWithMaxSize(
            path, token, kMaxMewebIdentityTokenBytes) || token->empty()) {
      *error = "ID 토큰 파일을 안전하게 읽을 수 없습니다.";
      return false;
    }
    base::TrimWhitespaceASCII(*token, base::TRIM_ALL, token);
    return !token->empty();
  }

  void BeginWifExchange(base::Value callback_id,
                        std::string provider,
                        std::string method) {
    auto environment = base::Environment::Create();
    auto required = [&](std::string_view name, std::string* output) {
      const std::string variable_name(name);
      auto value = environment->GetVar(variable_name);
      if (!value || value->empty()) return false;
      *output = std::move(*value);
      return true;
    };
    std::string token_path;
    std::string subject_token;
    std::string first_id;
    std::string organization_id;
    std::string service_account_id;
    std::string workspace_id;
    base::DictValue payload;
    GURL token_endpoint;
    if (provider == "openai") {
      if (!required("OPENAI_IDENTITY_TOKEN_FILE", &token_path) ||
          !required("OPENAI_IDENTITY_PROVIDER_ID", &first_id) ||
          !required("OPENAI_SERVICE_ACCOUNT_ID", &service_account_id)) {
        Reply(callback_id,
              AuthResult(provider, method, false, "missing_configuration",
                         "OpenAI WIF 환경 설정 3개가 필요합니다."));
        return;
      }
      payload.Set("grant_type",
                  "urn:ietf:params:oauth:grant-type:token-exchange");
      payload.Set("subject_token_type",
                  "urn:ietf:params:oauth:token-type:jwt");
      payload.Set("identity_provider_id", first_id);
      payload.Set("service_account_id", service_account_id);
      token_endpoint = GURL("https://auth.openai.com/oauth/token");
    } else {
      if (!required("ANTHROPIC_IDENTITY_TOKEN_FILE", &token_path) ||
          !required("ANTHROPIC_FEDERATION_RULE_ID", &first_id) ||
          !required("ANTHROPIC_ORGANIZATION_ID", &organization_id) ||
          !required("ANTHROPIC_SERVICE_ACCOUNT_ID", &service_account_id)) {
        Reply(callback_id,
              AuthResult(provider, method, false, "missing_configuration",
                         "Anthropic WIF 환경 설정 4개가 필요합니다."));
        return;
      }
      payload.Set("grant_type",
                  "urn:ietf:params:oauth:grant-type:jwt-bearer");
      payload.Set("federation_rule_id", first_id);
      payload.Set("organization_id", organization_id);
      payload.Set("service_account_id", service_account_id);
      if (auto value = environment->GetVar("ANTHROPIC_WORKSPACE_ID");
          value && !value->empty()) {
        workspace_id = std::move(*value);
        payload.Set("workspace_id", workspace_id);
      }
      token_endpoint = GURL("https://api.anthropic.com/v1/oauth/token");
    }
    std::string read_error;
    if (!ReadIdentityToken(token_path, &subject_token, &read_error)) {
      Reply(callback_id,
            AuthResult(provider, method, false, "invalid_token_file",
                       read_error));
      return;
    }
    payload.Set(provider == "openai" ? "subject_token" : "assertion",
                subject_token);
    std::string body;
    if (!base::JSONWriter::Write(payload, &body)) {
      Reply(callback_id, AuthResult(provider, method, false, "error",
                                    "WIF 요청을 만들 수 없습니다."));
      return;
    }
    auto request = std::make_unique<network::ResourceRequest>();
    request->url = token_endpoint;
    request->method = "POST";
    request->credentials_mode = network::mojom::CredentialsMode::kOmit;
    StartRequest(
        std::move(request), std::move(body),
        base::BindOnce(&MewebAgentWorkspaceHandler::OnWifExchanged,
                       weak_ptr_factory_.GetWeakPtr(), std::move(callback_id),
                       std::move(provider), std::move(method)));
  }

  void OnWifExchanged(base::Value callback_id,
                      std::string provider,
                      std::string method,
                      bool success,
                      std::optional<std::string> body) {
    if (!success || !body) {
      Reply(callback_id,
            AuthResult(provider, method, false, "error",
                       "WIF 토큰 교환이 공급자에서 거부되었습니다."));
      return;
    }
    auto parsed = base::JSONReader::Read(*body, 0);
    if (!parsed || !parsed->is_dict()) {
      Reply(callback_id, AuthResult(provider, method, false, "error",
                                    "WIF 응답 형식이 올바르지 않습니다."));
      return;
    }
    const std::string* access_token =
        parsed->GetDict().FindString("access_token");
    const std::optional<int> expires_in =
        parsed->GetDict().FindInt("expires_in");
    if (!access_token || access_token->empty() || !expires_in ||
        *expires_in <= 0 || *expires_in > 3600) {
      Reply(callback_id, AuthResult(provider, method, false, "error",
                                    "WIF 응답의 토큰 만료값이 올바르지 않습니다."));
      return;
    }
    credentials_.insert_or_assign(
        CredentialKey(provider, method),
        MemoryCredential{*access_token,
                         base::Time::Now() + base::Seconds(*expires_in)});
    auto result = AuthResult(provider, method, true, "connected",
                             "단기 WIF 토큰을 메모리에 연결했습니다.");
    result.Set("expires_in", *expires_in);
    Reply(callback_id, std::move(result));
  }

  using RequestCallback = base::OnceCallback<void(
      bool success, std::optional<std::string> response_body)>;

  void StartRequest(std::unique_ptr<network::ResourceRequest> request,
                    std::string upload_body,
                    RequestCallback callback) {
    constexpr net::NetworkTrafficAnnotationTag traffic_annotation =
        net::DefineNetworkTrafficAnnotation("meweb_model_auth_broker", R"(
          semantics {
            sender: "MEWEB model authentication broker"
            description:
              "Validates a model provider credential or exchanges a workload "
              "identity token after the user selects Connect in MEWEB settings."
            trigger: "The user selects Connect for an AI model provider."
            data:
              "A provider API credential or workload identity token. The "
              "credential is held in process memory and is not persisted by MEWEB."
            destination: OTHER
          }
          policy {
            cookies_allowed: NO
            setting:
              "The request is made only from the AI model provider settings "
              "screen and can be disconnected there."
            policy_exception_justification:
              "This is a user-selected model provider connection."
          })");
    auth_loader_ =
        network::SimpleURLLoader::Create(std::move(request), traffic_annotation);
    if (!upload_body.empty()) {
      auth_loader_->AttachStringForUpload(std::move(upload_body),
                                          "application/json");
    }
    auth_loader_->DownloadToString(
        profile_->GetURLLoaderFactory().get(),
        base::BindOnce(&MewebAgentWorkspaceHandler::OnRequestFinished,
                       weak_ptr_factory_.GetWeakPtr(), std::move(callback)),
        kMaxMewebModelAuthResponseBytes);
  }

  void OnRequestFinished(RequestCallback callback,
                         std::optional<std::string> body) {
    bool success = auth_loader_ && auth_loader_->NetError() == net::OK &&
                   auth_loader_->ResponseInfo() &&
                   auth_loader_->ResponseInfo()->headers &&
                   auth_loader_->ResponseInfo()->headers->response_code() >=
                       200 &&
                   auth_loader_->ResponseInfo()->headers->response_code() <=
                       299 &&
                   body.has_value();
    auth_loader_.reset();
    std::move(callback).Run(success, std::move(body));
  }

  static void AppendText(std::string* output, std::string_view text) {
    if (text.empty()) return;
    if (!output->empty()) output->append("\n");
    output->append(text);
  }

  static std::string ArgumentsString(const base::Value* value) {
    return value && value->is_string() ? value->GetString() : JsonString(value);
  }

  static void AppendToolCall(base::ListValue* output,
                             std::string_view id,
                             std::string_view name,
                             const base::Value* arguments) {
    if (name.empty()) return;
    base::DictValue call;
    call.Set("id", id);
    call.Set("name", name);
    call.Set("arguments", ArgumentsString(arguments));
    output->Append(std::move(call));
  }

  static int UsageInt(const base::DictValue* usage,
                      std::initializer_list<std::string_view> names) {
    if (!usage) return 0;
    for (std::string_view name : names) {
      if (auto value = usage->FindInt(name)) return *value;
    }
    return 0;
  }

  std::optional<base::DictValue> ParseInferenceResponse(
      std::string_view provider,
      std::string_view model,
      std::string_view body) const {
    auto parsed = base::JSONReader::Read(body, 0);
    if (!parsed || !parsed->is_dict()) return std::nullopt;
    const auto& root = parsed->GetDict();
    std::string text;
    std::string finish_reason;
    base::ListValue tool_calls;
    int input_tokens = 0;
    int output_tokens = 0;
    int total_tokens = 0;

    if (provider == "openai") {
      if (const auto* output = root.FindList("output")) {
        for (const auto& item_value : *output) {
          if (!item_value.is_dict()) continue;
          const auto& item = item_value.GetDict();
          const std::string* type = item.FindString("type");
          if (type && *type == "message") {
            if (const auto* content = item.FindList("content")) {
              for (const auto& part_value : *content) {
                if (!part_value.is_dict()) continue;
                const auto& part = part_value.GetDict();
                if (part.FindString("type") &&
                    *part.FindString("type") == "output_text") {
                  if (const std::string* value = part.FindString("text")) {
                    AppendText(&text, *value);
                  }
                }
              }
            }
          } else if (type && *type == "function_call") {
            AppendToolCall(&tool_calls,
                           item.FindString("call_id") ?
                               *item.FindString("call_id") : "",
                           item.FindString("name") ?
                               *item.FindString("name") : "",
                           item.Find("arguments"));
          }
        }
      }
      if (const std::string* status = root.FindString("status")) {
        finish_reason = *status;
      }
      const auto* usage = root.FindDict("usage");
      input_tokens = UsageInt(usage, {"input_tokens"});
      output_tokens = UsageInt(usage, {"output_tokens"});
      total_tokens = UsageInt(usage, {"total_tokens"});
    } else if (provider == "anthropic") {
      if (const auto* content = root.FindList("content")) {
        for (const auto& part_value : *content) {
          if (!part_value.is_dict()) continue;
          const auto& part = part_value.GetDict();
          const std::string* type = part.FindString("type");
          if (type && *type == "text") {
            if (const std::string* value = part.FindString("text")) {
              AppendText(&text, *value);
            }
          } else if (type && *type == "tool_use") {
            AppendToolCall(&tool_calls,
                           part.FindString("id") ? *part.FindString("id") : "",
                           part.FindString("name") ?
                               *part.FindString("name") : "",
                           part.Find("input"));
          }
        }
      }
      if (const std::string* reason = root.FindString("stop_reason")) {
        finish_reason = *reason;
      }
      const auto* usage = root.FindDict("usage");
      input_tokens = UsageInt(usage, {"input_tokens"});
      output_tokens = UsageInt(usage, {"output_tokens"});
      total_tokens = input_tokens + output_tokens;
    } else if (provider == "gemini") {
      const auto* candidates = root.FindList("candidates");
      if (candidates && !candidates->empty() && (*candidates)[0].is_dict()) {
        const auto& candidate = (*candidates)[0].GetDict();
        if (const std::string* reason = candidate.FindString("finishReason")) {
          finish_reason = *reason;
        }
        if (const auto* content = candidate.FindDict("content")) {
          if (const auto* parts = content->FindList("parts")) {
            for (const auto& part_value : *parts) {
              if (!part_value.is_dict()) continue;
              const auto& part = part_value.GetDict();
              if (const std::string* value = part.FindString("text")) {
                AppendText(&text, *value);
              }
              if (const auto* call = part.FindDict("functionCall")) {
                AppendToolCall(&tool_calls, "",
                               call->FindString("name") ?
                                   *call->FindString("name") : "",
                               call->Find("args"));
              }
            }
          }
        }
      }
      const auto* usage = root.FindDict("usageMetadata");
      input_tokens = UsageInt(usage, {"promptTokenCount"});
      output_tokens = UsageInt(usage, {"candidatesTokenCount"});
      total_tokens = UsageInt(usage, {"totalTokenCount"});
    } else if (provider == "ollama" || provider == "openrouter") {
      const base::DictValue* message = nullptr;
      const base::DictValue* usage = nullptr;
      if (provider == "ollama") {
        message = root.FindDict("message");
        usage = &root;
        if (const std::string* reason = root.FindString("done_reason")) {
          finish_reason = *reason;
        }
        input_tokens = UsageInt(usage, {"prompt_eval_count"});
        output_tokens = UsageInt(usage, {"eval_count"});
        total_tokens = input_tokens + output_tokens;
      } else {
        const auto* choices = root.FindList("choices");
        if (choices && !choices->empty() && (*choices)[0].is_dict()) {
          const auto& choice = (*choices)[0].GetDict();
          message = choice.FindDict("message");
          if (const std::string* reason = choice.FindString("finish_reason")) {
            finish_reason = *reason;
          }
        }
        usage = root.FindDict("usage");
        input_tokens = UsageInt(usage, {"prompt_tokens", "input_tokens"});
        output_tokens = UsageInt(usage, {"completion_tokens", "output_tokens"});
        total_tokens = UsageInt(usage, {"total_tokens"});
      }
      if (message) {
        if (const std::string* value = message->FindString("content")) {
          AppendText(&text, *value);
        }
        if (const auto* calls = message->FindList("tool_calls")) {
          for (const auto& call_value : *calls) {
            if (!call_value.is_dict()) continue;
            const auto& call = call_value.GetDict();
            const auto* function = call.FindDict("function");
            if (!function) continue;
            AppendToolCall(&tool_calls,
                           call.FindString("id") ?
                               *call.FindString("id") : "",
                           function->FindString("name") ?
                               *function->FindString("name") : "",
                           function->Find("arguments"));
          }
        }
      }
    } else {
      return std::nullopt;
    }

    auto result = InferenceResult(provider, model, "completed",
                                  "모델 응답을 받았습니다.");
    result.Set("text", std::move(text));
    result.Set("tool_calls", std::move(tool_calls));
    result.Set("finish_reason", std::move(finish_reason));
    base::DictValue usage;
    usage.Set("input_tokens", input_tokens);
    usage.Set("output_tokens", output_tokens);
    usage.Set("total_tokens", total_tokens > 0 ? total_tokens :
                                                input_tokens + output_tokens);
    result.Set("usage", std::move(usage));
    return result;
  }

  void StartInference(std::unique_ptr<network::ResourceRequest> request,
                      std::string upload_body,
                      PendingInference pending) {
    constexpr net::NetworkTrafficAnnotationTag traffic_annotation =
        net::DefineNetworkTrafficAnnotation("meweb_model_inference", R"(
          semantics {
            sender: "MEWEB model inference adapter"
            description:
              "Sends a prompt to the AI model provider explicitly selected "
              "and connected by the user, then normalizes the response."
            trigger: "The user selects the model response test action."
            data:
              "The prompt, optional instructions and tool schemas entered or "
              "approved by the user. Credentials stay in process memory."
            destination: OTHER
          }
          policy {
            cookies_allowed: NO
            setting:
              "The provider and connection can be selected or disconnected "
              "from MEWEB AI model settings."
            policy_exception_justification:
              "This is a user-requested connection to a selected AI provider."
          })");
    pending_inference_ = std::move(pending);
    inference_loader_ =
        network::SimpleURLLoader::Create(std::move(request), traffic_annotation);
    inference_loader_->AttachStringForUpload(std::move(upload_body),
                                              "application/json");
    inference_loader_->DownloadToString(
        profile_->GetURLLoaderFactory().get(),
        base::BindOnce(&MewebAgentWorkspaceHandler::OnInferenceFinished,
                       weak_ptr_factory_.GetWeakPtr()),
        kMaxMewebModelInferenceResponseBytes);
  }

  void OnInferenceFinished(std::optional<std::string> body) {
    if (!pending_inference_) return;
    PendingInference pending = std::move(*pending_inference_);
    pending_inference_.reset();
    const int network_error =
        inference_loader_ ? inference_loader_->NetError() : net::ERR_FAILED;
    int http_status = 0;
    if (inference_loader_ && inference_loader_->ResponseInfo() &&
        inference_loader_->ResponseInfo()->headers) {
      http_status = inference_loader_->ResponseInfo()->headers->response_code();
    }
    inference_loader_.reset();
    if (network_error != net::OK || http_status < 200 || http_status > 299 ||
        !body) {
      auto result = InferenceResult(
          pending.provider, pending.model, "error",
          network_error == net::OK ?
              "모델 공급자가 요청을 거부했습니다." :
              "모델 공급자 네트워크 요청에 실패했습니다.");
      result.Set("http_status", http_status);
      Reply(pending.callback_id, std::move(result));
      return;
    }
    auto result = ParseInferenceResponse(pending.provider, pending.model, *body);
    if (!result) {
      Reply(pending.callback_id,
            InferenceResult(pending.provider, pending.model, "error",
                            "모델 공급자 응답 형식을 해석할 수 없습니다."));
      return;
    }
    Reply(pending.callback_id, std::move(*result));
  }
  void HandleSaveState(const base::ListValue& args) {
    if (args.size() == 1 && args[0].is_string()) {
      const std::string& serialized = args[0].GetString();
      if (serialized.size() <= kMaxMewebAgentWorkspaceStateBytes) {
        auto parsed = base::JSONReader::Read(serialized, 0);
        if (parsed && parsed->is_dict()) {
          profile_->GetPrefs()->SetString(kMewebAgentWorkspaceStatePref,
                                          serialized);
        }
      }
    }
  }

  void HandleLoadState(const base::ListValue& args) {
    if (!args.empty()) {
      return;
    }
    AllowJavascript();
    CallJavascriptFunction(
        "mewebAgentWorkspaceLoadState",
        base::Value(
            profile_->GetPrefs()->GetString(kMewebAgentWorkspaceStatePref)));
  }

  raw_ptr<Profile> profile_;
  std::map<std::string, MemoryCredential> credentials_;
  std::unique_ptr<network::SimpleURLLoader> auth_loader_;
  std::unique_ptr<network::SimpleURLLoader> inference_loader_;
  std::optional<PendingInference> pending_inference_;
  base::WeakPtrFactory<MewebAgentWorkspaceHandler> weak_ptr_factory_{this};
};

bool HasCredentials(Profile* profile) {
  auto* identity_manager = IdentityManagerFactory::GetForProfile(profile);
  return
      /* Can be null if Chrome signin is disabled. */ identity_manager &&
      !identity_manager->GetAccountsInCookieJar()
           .GetPotentiallyInvalidSignedInAccounts()
           .empty();
}

content::WebUIDataSource* CreateAndAddNewTabPageUiHtmlSource(
    Profile* profile,
    bool session_allows_drag_and_drop) {
  content::WebUIDataSource* source = content::WebUIDataSource::CreateAndAdd(
      profile, chrome::kChromeUINewTabPageHost);

  ui::Accelerator undo_accelerator(ui::VKEY_Z, ui::EF_PLATFORM_ACCELERATOR);
  source->AddString("undoDescription", l10n_util::GetStringFUTF16(
                                           IDS_UNDO_DESCRIPTION,
                                           undo_accelerator.GetShortcutText()));
  source->AddString("ntpPromoDismiss",
                    l10n_util::GetStringUTF16(IDS_NTP_PROMO_DISMISS));
  source->AddString("ntpPromoMenu",
                    l10n_util::GetStringUTF16(IDS_NTP_PROMO_MENU_TOOLTIP));
  source->AddString("ntpPromoMenuA11yLabel",
                    l10n_util::GetStringUTF16(IDS_NTP_PROMO_MENU_A11Y_LABEL));

  GURL google_base_url = GURL(TemplateURLServiceFactory::GetForProfile(profile)
                                  ->search_terms_data()
                                  .GoogleBaseURLValue());
  source->AddString("googleBaseUrl", google_base_url.spec());

  GURL threads_url = google_base_url.Resolve("/search");
  threads_url = net::AppendQueryParameter(threads_url, "udm", kAIMDisplayMode);
  threads_url = net::AppendQueryParameter(
      threads_url, "aep",
      base::NumberToString(omnibox::DESKTOP_CHROME_NTP_THREADS_ENTRY_POINT));
  threads_url =
      net::AppendQueryParameter(threads_url, "atvm", kAIMThreadsVisibilityMode);
  threads_url = lens::AppendInvocationSourceParamToURL(
      threads_url, lens::LensOverlayInvocationSource::kNtpContextualQuery,
      /*is_contextual_tasks=*/true);
  source->AddString("threadsUrl", threads_url.spec());

  source->AddInteger(
      "preconnectStartTimeThreshold",
      features::kNewTabPagePreconnectStartDelayOnMouseHoverByMilliSeconds
          .Get());
  source->AddInteger(
      "prefetchStartTimeThreshold",
      features::kNewTabPagePrefetchStartDelayOnMouseHoverByMilliSeconds.Get());
  source->AddBoolean(
      "prefetchTriggerEnabled",
      base::FeatureList::IsEnabled(features::kNewTabPageTriggerForPrefetch));
  source->AddBoolean(
      "prerenderOnPressEnabled",
      base::FeatureList::IsEnabled(features::kNewTabPageTriggerForPrerender2));

  source->AddInteger("maxTilesInCollapsedState",
                     ntp_features::GetMaxTilesInCollapsedState());
  source->AddInteger("maxShortcutsInExpandedState",
                     ntp_features::GetMaxShortcutsInExpandedState());
  source->AddInteger("maxMostVisitedTilesInExpandedState",
                     ntp_features::GetMaxMostVisitedTilesInExpandedState());
  source->AddInteger("maxEnterpriseShortcuts",
                     ntp_features::GetMaxEnterpriseShortcuts());

  source->AddBoolean("energyEffectEnabled",
                     base::FeatureList::IsEnabled(ntp_features::kEnergyEffect));
  source->AddString("energyEffectVariant",
                    base::FeatureList::IsEnabled(ntp_features::kEnergyEffect)
                        ? ntp_features::kEnergyEffectVariantParam.GetName(
                              ntp_features::kEnergyEffectVariantParam.Get())
                        : std::string());
  source->AddBoolean(
      "energyEffectAnimationEnabled",
      base::FeatureList::IsEnabled(ntp_features::kEnergyEffectAnimation));
  source->AddBoolean(
      "contextMenuAnimationLimitingEnabled",
      base::FeatureList::IsEnabled(omnibox::kContextMenuAnimationLimiting));
  bool ntp_next_features_enabled =
      ntp_realbox::IsNtpRealboxNextEnabled(profile) &&
      base::FeatureList::IsEnabled(ntp_features::kNtpNextFeatures);
  source->AddBoolean("ntpNextFeaturesEnabled", ntp_next_features_enabled);
  source->AddBoolean("ntpNextShowDismissalUIEnabled",
                     ntp_features::kNtpNextShowDismissalUIParam.Get());
  source->AddBoolean("ntpNextDisablementContextMenuEnabled",
                     ntp_features::kNtpNextDisablementContextMenuParam.Get());
  source->AddBoolean(

      "oneGoogleBarEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpOneGoogleBar));
  source->AddBoolean("shortcutsEnabled",
                     base::FeatureList::IsEnabled(ntp_features::kNtpShortcuts));
  source->AddBoolean("logoEnabled",
                     base::FeatureList::IsEnabled(ntp_features::kNtpLogo));
  source->AddBoolean(
      "animatedDoodlesEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpAnimatedDoodles));
  source->AddBoolean(
      "doodleMuralsEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpDoodleMurals));
  bool use_google_logo_26 = false;
#if BUILDFLAG(GOOGLE_CHROME_BRANDING)
  use_google_logo_26 =
      base::FeatureList::IsEnabled(ntp_features::kNtpGoogleLogo26);
#endif  // BUILDFLAG(GOOGLE_CHROME_BRANDING)
  source->AddBoolean("useGoogleLogo26", use_google_logo_26);
  source->AddBoolean(
      "middleSlotPromoEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpMiddleSlotPromo) &&
          profile->GetPrefs()->GetBoolean(prefs::kNtpPromoVisible));
  source->AddBoolean(
      "middleSlotPromoDismissalEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpMiddleSlotPromoDismissal));
  source->AddBoolean(
      "modulesDragAndDropEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpModulesDragAndDrop));
  source->AddBoolean("modulesLoadEnabled", base::FeatureList::IsEnabled(
                                               ntp_features::kNtpModulesLoad));
  source->AddInteger("modulesLoadTimeout",
                     ntp_features::GetModulesLoadTimeout().InMilliseconds());
  source->AddInteger("modulesMaxColumnCount",
                     ntp_features::GetModulesMaxColumnCount());
  source->AddInteger(
      "multipleLoadedModulesMaxModuleInstanceCount",
      ntp_features::GetMultipleLoadedModulesMaxModuleInstanceCount());
  source->AddBoolean(
      "mostRelevantTabResumptionAllowFaviconServerFallback",
      base::FeatureList::IsEnabled(
          ntp_features::
              kNtpMostRelevantTabResumptionAllowFaviconServerFallback));
  source->AddBoolean(
      "mostRelevantTabResumptionModuleFallbackToHost",
      base::FeatureList::IsEnabled(
          ntp_features::kNtpMostRelevantTabResumptionModuleFallbackToHost));
  source->AddBoolean("footerEnabled",
                     base::FeatureList::IsEnabled(ntp_features::kNtpFooter));
  source->AddBoolean(
      "showCustomizeButton",
      base::FeatureList::IsEnabled(ntp_features::kNtpCustomizeWebUiAndroid) ||
          !BUILDFLAG(IS_ANDROID));

  source->AddBoolean("ntpRealboxNextEnabled",
                     ntp_realbox::IsNtpRealboxNextEnabled(profile));
  // Fusebox being enabled on the NTP is the same as realbox next being
  // enabled. Add this param for reusable components that shouldn't rely on NTP
  // specific booleans.
  source->AddBoolean("isFuseboxEnabled",
                     ntp_realbox::IsNtpRealboxNextEnabled(profile));
  source->AddBoolean("searchboxCyclingPlaceholders",
                     ntp_realbox::IsNtpRealboxNextEnabled(profile) &&
                         base::FeatureList::IsEnabled(
                             ntp_realbox::kNtpRealboxCyclingPlaceholders));
  source->AddBoolean("multiLineEnabled",
                     ntp_realbox::IsNtpRealboxNextEnabled(profile) &&
                         ntp_realbox::kMultiLineEnabled.Get());
  source->AddBoolean(
      "caretAnimationEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpAnimatedCaret));
  source->AddBoolean(
      "voiceSearchCoherenceSearchboxEnabled",
      base::FeatureList::IsEnabled(omnibox::kVoiceSearchCoherenceSearchbox));
  source->AddBoolean(
      "voiceSearchCoherenceAnySearchboxExperimentEnabled",
      SearchboxHandler::GetVoiceSearchCoherenceAnySearchboxExperimentEnabled());
  source->AddBoolean(
      "voiceSearchCoherenceSearchboxWithLiveTranscriptionEnabled",
      omnibox::kVoiceSearchCoherenceSearchboxWithLiveTranscription.Get());

  static constexpr webui::LocalizedString kStrings[] = {
      {"doneButton", IDS_DONE},
      {"dismissButton", IDS_NTP_DISMISS},
      {"title", IDS_NEW_TAB_TITLE},
      {"undo", IDS_NEW_TAB_UNDO_THUMBNAIL_REMOVE},
      {"controlledSettingPolicy", IDS_CONTROLLED_SETTING_POLICY},
      {"disableSuggestion", IDS_NTP_ACTION_CHIP_DISABLE_TEXT},
      {"actionChipsUndoDisablementToastMessage",
       IDS_NTP_ACTION_CHIPS_UNDO_DISABLEMENT_TOAST_MESSAGE},

      // Custom Links.
      {"addLinkTitle", IDS_NTP_CUSTOM_LINKS_ADD_SHORTCUT_TITLE},
      {"editLinkTitle", IDS_NTP_CUSTOM_LINKS_EDIT_SHORTCUT},
      {"viewLinkTitle", IDS_NTP_CUSTOM_LINKS_SHORTCUT_DETAILS_TITLE},
      {"invalidUrl", IDS_NTP_CUSTOM_LINKS_INVALID_URL},
      {"linkAddedMsg", IDS_NTP_CONFIRM_MSG_SHORTCUT_ADDED},
      {"linkCancel", IDS_NTP_CUSTOM_LINKS_CANCEL},
      {"linkCantCreate", IDS_NTP_CUSTOM_LINKS_CANT_CREATE},
      {"linkCantEdit", IDS_NTP_CUSTOM_LINKS_CANT_EDIT},
      {"viewLink", IDS_NTP_CUSTOM_LINKS_DETAILS},
      {"linkDone", IDS_NTP_CUSTOM_LINKS_DONE},
      {"linkEditedMsg", IDS_NTP_CONFIRM_MSG_SHORTCUT_EDITED},
      {"linkRemove", IDS_NTP_CUSTOM_LINKS_REMOVE},
      {"linkRemoveA11y", IDS_NTP_MOST_VISITED_SITES_REMOVE},
      {"linkRemovedMsg", IDS_NTP_CONFIRM_MSG_SHORTCUT_REMOVED},
      {"shortcutMoreActions", IDS_NTP_CUSTOM_LINKS_MORE_ACTIONS},
      {"enterpriseShortcutSubtitle", IDS_NTP_ENTERPRISE_SHORTCUT_SUBTITLE},
      {"nameField", IDS_NTP_CUSTOM_LINKS_NAME},
      {"restoreDefaultLinks", IDS_NTP_CONFIRM_MSG_RESTORE_DEFAULTS},
      {"restoreDefaultEnterpriseShortcuts",
       IDS_NTP_CONFIRM_MSG_RESTORE_ENTERPRISE_DEFAULTS},
      {"restoreThumbnailsShort", IDS_NEW_TAB_RESTORE_THUMBNAILS_SHORT_LINK},
      {"shortcutAlreadyExists", IDS_NTP_CUSTOM_LINKS_ALREADY_EXISTS},
      {"urlField", IDS_NTP_CUSTOM_LINKS_URL},
      {"showMore", IDS_NTP_SHOW_MORE_BUTTON_LABEL},
      {"showLess", IDS_NTP_SHOW_LESS_BUTTON_LABEL},
      {"shortcutsInactivityRemovalMsg",
       IDS_NTP_MOST_VISITED_SHORTCUTS_INACTIVITY_REMOVAL},
      {"moduleInactivityRemovalMsg", IDS_NTP_MODULE_INACTIVITY_REMOVAL},
      {"modulesInactivityRemovalMsg", IDS_NTP_MODULES_INACTIVITY_REMOVAL},

      // Customize button and dialog.
      {"colorPickerLabel", IDS_NTP_CUSTOMIZE_COLOR_PICKER_LABEL},
      {"hueSliderTitle", IDS_NTP_CUSTOMIZE_COLOR_HUE_SLIDER_TITLE},
      {"hueSliderAriaLabel", IDS_NTP_CUSTOMIZE_COLOR_HUE_SLIDER_ARIA_LABEL},
      {"customizeButton", IDS_NTP_CUSTOMIZE_BUTTON_LABEL},
      {"customizeThisPage", IDS_NTP_CUSTOM_BG_CUSTOMIZE_NTP_LABEL},

      // Wallpaper search.
      {"customizeThisPageWallpaperSearch",
       IDS_NTP_CUSTOM_BG_CUSTOMIZE_NTP_WALLPAPER_SEARCH_LABEL},
      {"wallpaperSearchButton", IDS_NTP_WALLPAPER_SEARCH_PAGE_HEADER},

      // Voice search.
      // TODO(crbug.com/328827188): Consider moving the voice search overlay
      // code (here and elsewhere) into the searchbox directories or a new
      // component.
      {"audioError", IDS_NEW_TAB_VOICE_AUDIO_ERROR},
      {"close", IDS_NTP_CLOSE},
      {"languageError", IDS_NEW_TAB_VOICE_LANGUAGE_ERROR},
      {"learnMore", IDS_LEARN_MORE},
      {"learnMoreA11yLabel", IDS_NEW_TAB_VOICE_LEARN_MORE_ACCESSIBILITY_LABEL},
      {"networkError", IDS_NEW_TAB_VOICE_NETWORK_ERROR},
      {"noTranslation", IDS_NEW_TAB_VOICE_NO_TRANSLATION},
      {"noVoice", IDS_NEW_TAB_VOICE_NO_VOICE},
      {"otherError", IDS_NEW_TAB_VOICE_OTHER_ERROR},
      {"speak", IDS_NEW_TAB_VOICE_READY},
      {"tryAgain", IDS_NEW_TAB_VOICE_TRY_AGAIN},
      {"waiting", IDS_NEW_TAB_VOICE_WAITING},

      // Lens image search.
      // TODO(crbug.com/328827188): Consider moving the Lens upload dialog code
      // (here and elsewhere) into the searchbox directories or a new component.
      {"lensSearchUploadDialogCloseButtonLabel",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_CLOSE_BUTTON_LABEL},
      {"lensSearchUploadDialogTitle",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_TITLE_SHORT},
      {"lensSearchUploadDialogDragTitle",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_DRAG_TITLE},
      {"lensSearchUploadDialogUploadFileTitle",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_UPLOAD_FILE_TITLE},
      {"lensSearchUploadDialogOrText", IDS_LENS_SEARCH_UPLOAD_DIALOG_OR_TEXT},
      {"lensSearchUploadDialogTextPlaceholder",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_TEXT_PLACEHOLDER},
      {"lensSearchUploadDialogSearchButtonLabel",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_SEARCH_BUTTON_LABEL},
      {"lensSearchUploadDialogDragDropTitle",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_DRAG_DROP_TITLE},
      {"lensSearchUploadDialogLoadingText",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_LOADING_TEXT},
      {"lensSearchUploadDialogOfflineText",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_OFFLINE_TEXT},
      {"lensSearchUploadDialogOfflineSubtitleText",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_OFFLINE_SUBTITLE_TEXT},
      {"lensSearchUploadDialogOfflineButtonLabel",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_OFFLINE_BUTTON_LABEL},
      {"lensSearchUploadDialogErrorFileType",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_ERROR_FILE_TYPE},
      {"lensSearchUploadDialogErrorFileSize",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_ERROR_FILE_SIZE},
      {"lensSearchUploadDialogErrorMultipleFiles",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_ERROR_MULTIPLE_FILES},
      {"lensSearchUploadDialogValidationErrorScheme",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_VALIDATION_ERROR_SCHEME},
      {"lensSearchUploadDialogValidationErrorConformance",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_VALIDATION_ERROR_CONFORMANCE},
      {"lensSearchUploadDialogErrorMultipleUrls",
       IDS_LENS_SEARCH_UPLOAD_DIALOG_ERROR_MULTIPLE_URLS},

      // Logo/doodle.
      {"copyLink", IDS_NTP_DOODLE_SHARE_DIALOG_COPY_LABEL},
      {"doodleLink", IDS_NTP_DOODLE_SHARE_DIALOG_LINK_LABEL},
      {"email", IDS_NTP_DOODLE_SHARE_DIALOG_MAIL_LABEL},
      {"facebook", IDS_NTP_DOODLE_SHARE_DIALOG_FACEBOOK_LABEL},
      {"shareDoodle", IDS_NTP_DOODLE_SHARE_LABEL},
      {"twitter", IDS_NTP_DOODLE_SHARE_DIALOG_TWITTER_LABEL},

      // Theme.
      {"themeCreatedBy", IDS_NEW_TAB_ATTRIBUTION_INTRO},
      {"themeManagedDialogTitle", IDS_NTP_THEME_MANAGED_DIALOG_TITLE},
      {"themeManagedDialogBody", IDS_NTP_THEME_MANAGED_DIALOG_BODY},
      {"ok", IDS_OK},

      // Modules.
      {"dismissModuleToastMessage", IDS_NTP_MODULES_DISMISS_TOAST_MESSAGE},
      {"disableModuleToastMessage", IDS_NTP_MODULES_DISABLE_TOAST_MESSAGE},
      {"moduleHeaderMoreActionsMenu", IDS_NTP_MODULE_HEADER_MORE_ACTIONS_MENU},
      {"moduleInfoButtonTitle", IDS_NTP_MODULES_INFO_BUTTON_TITLE},
      {"modulesDismissButtonText", IDS_NTP_MODULES_DISMISS_BUTTON_TEXT},
      {"modulesDisableButtonText", IDS_NTP_MODULES_DISABLE_BUTTON_TEXT},
      {"modulesDisableButtonTextV2", IDS_NTP_MODULES_DISABLE_BUTTON_TEXT_V2},
      {"modulesCustomizeButtonText", IDS_NTP_MODULES_CUSTOMIZE_BUTTON_TEXT},
      {"modulesTasksInfo", IDS_NTP_MODULES_TASKS_INFO},
      {"modulesDisableToastMessage",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_DISABLE_TOAST_MESSAGE},
      {"modulesDriveDisableButtonText",
       IDS_NTP_MODULES_DRIVE_DISABLE_BUTTON_TEXT},
      {"modulesDriveDisableButtonTextV2",
       IDS_NTP_MODULES_DRIVE_DISABLE_BUTTON_TEXT_V2},
      {"modulesDriveMoreActionsButtonText",
       IDS_NTP_MODULES_DRIVE_MORE_ACTIONS_BUTTON_TEXT},
      {"modulesDriveSentence", IDS_NTP_MODULES_DRIVE_NAME},
      {"modulesFilesSentence", IDS_NTP_MODULES_FILES_SENTENCE},
      {"modulesDummyLower", IDS_NTP_MODULES_DUMMY_LOWER},
      {"modulesDriveTitle", IDS_NTP_MODULES_DRIVE_NAME},
      {"modulesDriveTitleV2", IDS_NTP_MODULES_DRIVE_NAME},
      {"modulesDriveInfo", IDS_NTP_MODULES_DRIVE_INFO},
      {"modulesDriveSeeMore", IDS_NTP_MODULES_DRIVE_SEE_MORE},
      {"modulesDriveSeeMoreAcc", IDS_NTP_MODULES_DRIVE_SEE_MORE_ACCNAME},
      {"modulesMicrosoftFilesInfo", IDS_NTP_MODULES_MICROSOFT_FILES_INFO},
      {"modulesMicrosoftFilesName", IDS_NTP_MODULES_MICROSOFT_FILES_NAME},
      {"modulesMicrosoftFilesDisableButtonText",
       IDS_NTP_MODULES_MICROSOFT_FILES_DISABLE_BUTTON_TEXT},
      {"modulesDummyTitle", IDS_NTP_MODULES_DUMMY_TITLE},
      {"modulesDismissForHoursButtonText",
       IDS_NTP_MODULES_DISMISS_FOR_HOURS_BUTTON_TEXT},
      {"modulesDismissForDaysButtonText",
       IDS_NTP_MODULES_DISMISS_FOR_DAYS_BUTTON_TEXT},
      {"modulesGoogleCalendarDismissToastMessage",
       IDS_NTP_MODULES_GOOGLE_CALENDAR_DISMISS_TOAST_MESSAGE},
      {"modulesGoogleCalendarDisableToastMessage",
       IDS_NTP_MODULES_GOOGLE_CALENDAR_DISABLE_TOAST_MESSAGE},
      {"moduleGoogleCalendarInfo", IDS_NTP_MODULES_GOOGLE_CALENDAR_INFO},
      {"modulesGoogleCalendarMoreActions",
       IDS_NTP_MODULES_GOOGLE_CALENDAR_MORE_ACTIONS},
      {"modulesGoogleCalendarTitle", IDS_NTP_MODULES_GOOGLE_CALENDAR_TITLE},
      {"modulesGoogleCalendarDisableButtonText",
       IDS_NTP_MODULES_GOOGLE_CALENDAR_DISABLE_BUTTON_TEXT},
      {"modulesMicrosoftSignOutButtonText",
       IDS_NTP_MODULES_MICROSOFT_SIGN_OUT_BUTTON_TEXT},
      {"modulesOutlookCalendarTitle", IDS_NTP_MODULES_OUTLOOK_CALENDAR_TITLE},
      {"modulesOutlookCalendarDisableButtonText",
       IDS_NTP_MODULES_OUTLOOK_CALENDAR_DISABLE_BUTTON_TEXT},
      {"modulesOutlookCalendarDismissToastMessage",
       IDS_NTP_MODULES_OUTLOOK_CALENDAR_DISMISS_TOAST_MESSAGE},
      {"modulesOutlookCalendarInfo", IDS_NTP_MODULES_OUTLOOK_CALENDAR_INFO},
      {"modulesCalendarJoinMeetingButtonText",
       IDS_NTP_MODULES_CALENDAR_JOIN_MEETING_BUTTON_TEXT},
      {"modulesCalendarJoinMeetingButtonAcc",
       IDS_NTP_MODULES_CALENDAR_JOIN_MEETING_BUTTON_ACCNAME},
      {"modulesCalendarInProgress", IDS_NTP_MODULES_CALENDAR_IN_PROGRESS},
      {"modulesCalendarInXMin", IDS_NTP_MODULES_CALENDAR_IN_X_MIN},
      {"modulesCalendarInXHr", IDS_NTP_MODULES_CALENDAR_IN_X_HR},
      {"modulesCalendarSeeMore", IDS_NTP_MODULES_CALENDAR_SEE_MORE},
      {"modulesCalendarSeeMoreAcc", IDS_NTP_MODULES_CALENDAR_SEE_MORE_ACCNAME},
      {"modulesKaleidoscopeTitle", IDS_NTP_MODULES_KALEIDOSCOPE_TITLE},
      {"modulesTasksInfoTitle", IDS_NTP_MODULES_SHOPPING_TASKS_INFO_TITLE},
      {"modulesTasksInfoClose", IDS_NTP_MODULES_SHOPPING_TASKS_INFO_CLOSE},
      {"modulesJourneysShowAll", IDS_NTP_MODULES_SHOW_ALL},
      {"modulesJourneysInfo", IDS_NTP_MODULES_HISTORY_CLUSTERS_INFO},
      {"modulesHistoryDoneButton",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_DONE_BUTTON},
      {"modulesHistoryWithDiscountInfo",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_WITH_DISCOUNT_INFO},
      {"modulesHistoryResumeBrowsingTitle",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_RESUME_BROWSING},
      {"modulesHistoryResumeBrowsingForTitle",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_RESUME_BROWSING_FOR},
      {"modulesThisTypeOfCardText",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_DISABLE_TOAST_NAME},
      {"modulesJourneyDisable",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_DISABLE_DROPDOWN_TEXT},
      {"modulesJourneysDismissButton",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_DISMISS_BUTTON},
      {"modulesJourneysShowAllButton",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_SHOW_ALL_BUTTON},
      {"modulesJourneysShowAllAcc", IDS_ACCNAME_SHOW_ALL},
      {"modulesJourneysSearchSuggAcc", IDS_ACCNAME_SEARCH_SUGG},
      {"modulesJourneysBookmarked",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_BOOKMARKED},
      {"modulesJourneysOpenAllInNewTabGroupButtonText",
       IDS_NTP_MODULES_HISTORY_CLUSTERS_OPEN_ALL_IN_NEW_TAB_GROUP_BUTTON_TEXT},
      {"modulesMoreActions", IDS_NTP_MODULES_MORE_ACTIONS},
      {"modulesSetupListDisableToastMessage",
       IDS_NTP_MODULES_SETUP_LIST_DISABLE_TOAST_MESSAGE},
      {"modulesSetupListDismissToastMessage",
       IDS_NTP_MODULES_SETUP_LIST_DISMISS_TOAST_MESSAGE},
      {"modulesSetupListInfo", IDS_NTP_MODULES_SETUP_LIST_INFO},
      {"modulesSetupListTitle", IDS_NTP_MODULES_SETUP_LIST_TITLE},
      {"modulesTabResumptionDismissButton",
       IDS_NTP_MODULES_TAB_RESUMPTION_DISMISS_BUTTON},
      {"modulesTabResumptionTitle",
       IDS_NTP_MODULES_MOST_RELEVANT_TAB_RESUMPTION_TITLE},
      {"modulesTabResumptionInfo", IDS_NTP_MODULES_TAB_RESUMPTION_INFO},
      {"modulesTabResumptionMultiDismiss",
       IDS_NTP_MODULES_TAB_RESUMPTION_MULTI_DISMISS},
      {"modulesTabResumptionSingleDismiss",
       IDS_NTP_MODULES_TAB_RESUMPTION_SINGLE_DISMISS},
      {"modulesTabResumptionDevicePrefix",
       IDS_NTP_MODULES_TAB_RESUMPTION_DEVICE_PREFIX},
      {"modulesMostRelevantTabResumptionDismissAll",
       IDS_NTP_MODULES_MOST_RELEVANT_TAB_RESUMPTION_DISMISS_BUTTON},
      {"modulesMostRelevantTabResumptionTitle",
       IDS_NTP_MODULES_MOST_RELEVANT_TAB_RESUMPTION_TITLE},
      {"modulesMostRelevantTabResumptionSeeMore",
       IDS_NTP_MODULES_MOST_RELEVANT_TAB_RESUMPTION_SEE_MORE},
      {"modulesMostRelevantTabResumptionSeeMoreAcc",
       IDS_NTP_MODULES_MOST_RELEVANT_TAB_RESUMPTION_SEE_MORE_ACCNAME},
      {"modulesMostRelevantTabResumptionMostRecent",
       IDS_TAB_RESUME_DECORATORS_MOST_RECENT},
      {"modulesMostRelevantTabResumptionFrequentlyVisited",
       IDS_TAB_RESUME_DECORATORS_FREQUENTLY_VISITED},
      {"modulesMostRelevantTabResumptionVisitedXAgo",
       IDS_TAB_RESUME_DECORATORS_VISITED_X_AGO},
      {"modulesMicrosoftAuthName",
       IDS_NTP_MODULES_MICROSOFT_AUTHENTICATION_NAME},
      {"modulesMicrosoftAuthDisable",
       IDS_NTP_MICROSOFT_AUTHENTICATION_DISABLE_BUTTON_TEXT},
      {"modulesMicrosoftAuthDismiss",
       IDS_NTP_MICROSOFT_AUTHENTICATION_DISMISS_BUTTON_TEXT},
      {"modulesMicrosoftAuthHeading", IDS_NTP_MICROSOFT_AUTHENTICATION_HEADING},
      {"modulesMicrosoftAuthSubheading",
       IDS_NTP_MICROSOFT_AUTHENTICATION_SUBHEADING},
      {"modulesMicrosoftAuthSignIn",
       IDS_NTP_MICROSOFT_AUTHENTICATION_SIGN_IN_BUTTON_TEXT},
      {"modulesTabGroupsDismissToastMessage",
       IDS_NTP_MODULES_TAB_GROUPS_DISMISS_TOAST_MESSAGE},
      {"modulesTabGroupsDisableToastMessage",
       IDS_NTP_MODULES_TAB_GROUPS_DISABLE_TOAST_MESSAGE},
      {"modulesTabGroupsInfo", IDS_NTP_MODULES_TAB_GROUPS_INFO},
      {"modulesTabGroupsCreateNewTabGroup", IDS_CREATE_NEW_TAB_GROUP},
      {"modulesTabGroupsTitle", IDS_NTP_MODULES_TAB_GROUPS_TITLE},
      {"modulesTabGroupsZeroStateTitle",
       IDS_NTP_MODULES_TAB_GROUPS_ZERO_STATE_TITLE},
      {"modulesTabGroupsZeroStateText",
       IDS_NTP_MODULES_TAB_GROUPS_ZERO_STATE_TEXT},

      // Middle slot promo.
      {"undoDismissPromoButtonToast", IDS_NTP_UNDO_DISMISS_PROMO_BUTTON_TOAST},
      {"mobilePromoDescription", IDS_NTP_MOBILE_PROMO_DESCRIPTION},
      {"mobilePromoHeader", IDS_NTP_MOBILE_PROMO_HEADER},
      {"mobilePromoQrCode", IDS_NTP_MOBILE_PROMO_QR_CODE_LABEL},

      // Threads rail.
      {"aimThreadsHistoryLabel", IDS_NTP_THREADS_HISTORY_LABEL},
      {"aimThreadsNewSearchLabel", IDS_NTP_THREADS_NEW_SEARCH_LABEL},

      // Webstore toast.
      {"webstoreThemesToastMessage", IDS_NTP_WEBSTORE_TOAST_MESSAGE},
      {"webstoreThemesToastButtonText", IDS_NTP_WEBSTORE_TOAST_BUTTON_TEXT},
  };

  source->AddLocalizedStrings(kStrings);

  source->AddBoolean(
      "hideDismissModules",
      base::FeatureList::IsEnabled(
          ntp_features::kNtpFeatureOptimizationDismissModulesRemoval));
  source->AddBoolean(
      "showDriveModuleSeeMoreLink",
      base::FeatureList::IsEnabled(ntp_features::kNtpDriveModuleLink));

  source->AddString(
      "calendarModuleDismissHours",
      base::NumberToString(
          ntp_features::kNtpCalendarModuleWindowEndDeltaParam.Get().InHours()));
  source->AddString(
      "fileSuggestionDismissHours",
      base::NumberToString(DriveService::kDismissDuration.InHours()));
  source->AddString(
      "tabGroupsModuleDismissHours",
      base::NumberToString(
          ntp_features::kNtpTabGroupsModuleWindowEndDeltaParam.Get()
              .InHours()));

  bool microsoft_module_enabled = IsMicrosoftModuleEnabledForProfile(profile);
  source->AddBoolean("microsoftModuleEnabled", microsoft_module_enabled);
  source->AddBoolean("modulesReloadable", microsoft_module_enabled);
  source->AddBoolean("waitToLoadModules", microsoft_module_enabled);

  // ComposeBox LoadTimeData
  auto composebox_config =
      ntp_composebox::FeatureConfig::Get().config.composebox();
  const std::string image_mime_types =
      composebox_config.image_upload().mime_types_allowed();
  source->AddString("composeboxImageFileTypes", image_mime_types);
  source->AddBoolean("lensSendRawFileMediaTypesEnabled",
                     lens::features::IsLensSendRawFileMediaTypesEnabled());
  const std::string attachment_mime_types =
      composebox_config.attachment_upload().mime_types_allowed();
  source->AddString("composeboxAttachmentFileTypes", attachment_mime_types);
  source->AddInteger("composeboxFileMaxSize",
                     composebox_config.attachment_upload().max_size_bytes());
  source->AddString(
      "composeboxSource",
      contextual_search::ContextualSearchMetricsRecorder::
          ContextualSearchSourceToString(
              contextual_search::ContextualSearchSource::kNewTabPage));

  auto* service = AimEligibilityServiceFactory::GetForProfile(profile);
  bool aim_eligible = service && service->IsAimEligible();

  source->AddBoolean(
      "searchboxShowComposeEntrypoint",
      (aim_eligible || ntp_composebox::IsNtpComposeboxEnabled(profile)));

  source->AddBoolean(
      "ntpRealboxDynamicAiModeButton",
      ntp_realbox::IsNtpRealboxNextEnabled(profile) &&
          base::FeatureList::IsEnabled(
              ntp_realbox::kNtpRealboxDynamicAiModeButton));

  if (ntp_realbox::IsNtpRealboxNextEnabled(profile)) {
    if (base::FeatureList::IsEnabled(
            ntp_realbox::kNtpRealboxCyclingPlaceholders)) {
      source->AddLocalizedString(
          "searchBoxPlaceholder",
          IDS_NTP_SEARCH_BOX_DYNAMIC_PLACEHOLDER_ASK_GOOGLE);
    } else {
      switch (ntp_realbox::kSteadyPlaceholder.Get()) {
        case ntp_realbox::PlaceholderText::ASK_OR_TYPE:
          source->AddString("searchBoxPlaceholder",
                            l10n_util::GetStringFUTF16(
                                IDS_WEBUI_OMNIBOX_PLACEHOLDER_TEXT, u"Google"));
          break;
        case ntp_realbox::PlaceholderText::ASK:
          source->AddLocalizedString(
              "searchBoxPlaceholder",
              IDS_NTP_SEARCH_BOX_DYNAMIC_PLACEHOLDER_ASK_GOOGLE);
          break;
        default:
          NOTREACHED();
      }
    }

  } else {
    source->AddLocalizedString("searchBoxPlaceholder",
                               IDS_GOOGLE_SEARCH_BOX_EMPTY_HINT_MD);
  }

  source->AddBoolean("keepMenuOpenOnTabSelectForRealbox",
                     omnibox::kKeepMenuOpenOnTabSelectForRealbox.Get());
  source->AddBoolean("composeboxShowContextMenu",
                     ntp_composebox::kShowContextMenu.Get());
  source->AddBoolean("composeboxShowContextMenuTabPreviews",
                     ntp_composebox::kShowContextMenuTabPreviews.Get());
  source->AddBoolean("composeboxContextMenuEnableMultiTabSelection",
                     ntp_composebox::kContextMenuEnableMultiTabSelection.Get());
  source->AddBoolean(
      "composeboxSkillsEnabled",
      base::FeatureList::IsEnabled(omnibox::kComposeboxSkillsNtp));
  source->AddBoolean(
      "contextManagementInComposeboxEnabled",
      base::FeatureList::IsEnabled(omnibox::kContextManagementInComposebox));
  source->AddBoolean(
      "tabFaviconChipsToCoinsEnabled",
      base::FeatureList::IsEnabled(omnibox::kContextManagementInComposebox) &&
          base::FeatureList::IsEnabled(omnibox::kTabFaviconChipsToCoins));
  source->AddBoolean("searchboxShowComposebox",
                     ntp_composebox::IsNtpComposeboxEnabled(profile));
  source->AddBoolean("composeboxShowZps", true);
  source->AddBoolean("composeboxShowTypedSuggest",
                     ntp_composebox::kShowComposeboxTypedSuggest.Get());
  source->AddBoolean("composeboxShowImageSuggest",
                     ntp_composebox::kShowComposeboxImageSuggestions.Get());


  source->AddBoolean("composeboxSmartComposeEnabled",
                     ntp_composebox::kShowSmartCompose.Get());

  source->AddBoolean("enableThreadsRail", base::FeatureList::IsEnabled(
                                              ntp_features::kNtpThreadsRail));

  // Action Chips LoadTimeData
  const auto* aim_eligibility_service =
      AimEligibilityServiceFactory::GetForProfile(profile);
  int num_tools_eligible = 0;
  if (aim_eligibility_service) {
    if (aim_eligibility_service->IsDeepSearchEligible()) {
      num_tools_eligible++;
    }
    if (aim_eligibility_service->IsCreateImagesEligible()) {
      num_tools_eligible++;
      if (base::FeatureList::IsEnabled(ntp_features::kNtpStarterChip)) {
        num_tools_eligible++;
      }
    }
    if (base::FeatureList::IsEnabled(ntp_features::kNtpNextCanvasChip) &&
        aim_eligibility_service->IsCanvasEligible()) {
      num_tools_eligible++;
    }
  }
  bool action_chips_eligible =
      base::FeatureList::IsEnabled(ntp_features::kNtpScaledActionChips)
          ? ntp_next_features_enabled
          : (aim_eligibility_service &&
             aim_eligibility_service->IsAimEligible() &&
             num_tools_eligible >= 2);
  bool show_action_chips =
      action_chips_eligible &&
      (!ntp_features::kNtpNextDisablementParam.Get() ||
       profile->GetPrefs()->GetBoolean(prefs::kNtpToolChipsVisible));
  if (!show_action_chips) {
    action_chips::RecordActionChipsAnyShown(false);
  }
  bool add_tab_upload_delay_on_action_chip_click =
      ntp_features::kAddTabUploadDelayOnActionChipClick.Get();
  source->AddBoolean("addTabUploadDelayOnActionChipClick",
                     add_tab_upload_delay_on_action_chip_click);
  source->AddBoolean("actionChipsEnabled", show_action_chips);
  source->AddBoolean(
      "ntpSmallActionChipsEnabled",
      base::FeatureList::IsEnabled(ntp_features::kNtpScaledActionChipsSmall));

  // User education browser promos.
  int browser_promo_limit = 0;
  int browser_completed_promo_limit = 0;
  switch (user_education::features::GetNtpBrowserPromoType()) {
    case user_education::features::NtpBrowserPromoType::kSimple:
      // Hard code the limit to 1 for now, as we removed the param accessor.
      browser_promo_limit = 1;
      break;
    case user_education::features::NtpBrowserPromoType::kNone:
      break;
  }
  source->AddInteger("browserPromoLimit", browser_promo_limit);
  source->AddInteger("browserPromoCompletedLimit",
                     browser_completed_promo_limit);

  source->AddLocalizedStrings(SearchboxHandler::GetWebUIDataSourceDict(
      profile, {.enable_voice_search = true,
                .enable_lens_search = profile->GetPrefs()->GetBoolean(
                    prefs::kLensDesktopNTPSearchEnabled),
                .session_allows_drag_and_drop = session_allows_drag_and_drop}));

  webui::SetupWebUIDataSource(source, kNewTabPageResources,
                              IDR_NEW_TAB_PAGE_NEW_TAB_PAGE_HTML);
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::TrustedTypes,
      base::StrCat(
          {webui::kDefaultTrustedTypesPolicies, " meweb-agent-workspace;"}));

#if !BUILDFLAG(OPTIMIZE_WEBUI)
  source->AddResourcePaths(kNewTabSharedResources);
#endif  // BUILDFLAG(OPTIMIZE_WEBUI)

#if BUILDFLAG(GOOGLE_CHROME_BRANDING)
  // Overrides the mapping installed by SetupWebUIDataSource() above, so the
  // stable logo path serves the 2026 version of the Google logo.
  if (use_google_logo_26) {
    source->AddResourcePath("icons/google_logo.svg",
                            IDR_NEW_TAB_PAGE_BRANDED_GOOGLE_LOGO_SVG);
  }
#endif  // BUILDFLAG(GOOGLE_CHROME_BRANDING)

  // Allow embedding of iframes for the doodle and
  // chrome-untrusted://new-tab-page for other external content and resources.
  // NOTE: Use caution when overriding content security policies as that cean
  // lead to subtle security bugs such as https://crbug.com/40057334.
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ChildSrc,
      base::StringPrintf("child-src https: %s %s %s;",
                         google_util::CommandLineGoogleBaseURL().spec().c_str(),
                         chrome::kChromeUIUntrustedNewTabPageUrl,
                         chrome::kChromeUIUntrustedNtpMicrosoftAuthURL));
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::MediaSrc,
      "media-src blob: data: 'self';");

  return source;
}

// Constants sent to the UI to ensure that the correct promo is shown and the
// correct metrics are recorded. The distinction between empty and disabled is
// that empty means that promos would have been shown, whereas disabled
// indicates that no promo is allowed for the current page.
// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
constexpr std::string_view kSimpleBrowserPromo = "simple";
constexpr std::string_view kEmptyBrowserPromo = "empty";
#endif
constexpr std::string_view kDisabledBrowserPromo = "disabled";

}  // namespace

// static
int NewTabPageUI::instance_count_ = 0;

NewTabPageUI::NewTabPageUI(content::WebUI* web_ui)
    : ui::MojoWebUIController(web_ui,
                              /*enable_chrome_send=*/true,
                              /*enable_chrome_histograms=*/true),
      content::WebContentsObserver(web_ui->GetWebContents()),
      page_factory_receiver_(this),
      customize_buttons_factory_receiver_(this),
      most_visited_page_factory_receiver_(this),
      composebox_page_factory_receiver_(this),
// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
      ntp_promo_handler_factory_receiver_(this),
#endif
      action_chips_handler_factory_receiver_(this),
      browser_command_factory_receiver_(this),
      searchbox_page_factory_receiver_(this),
      help_bubble_handler_factory_receiver_(this),
      profile_(Profile::FromWebUI(web_ui)),
// TODO(b/502297163): Implement for Android.
#if BUILDFLAG(IS_ANDROID)
      theme_service_(nullptr),
#else
      theme_service_(ThemeServiceFactory::GetForProfile(profile_)),
#endif
      ntp_custom_background_service_(
          NtpCustomBackgroundServiceFactory::GetForProfile(profile_)),
      ntp_custom_background_service_observation_(this),
      // We initialize navigation_start_time_ to a reasonable value to account
      // for the unlikely case where the NewTabPageHandler is created before we
      // received the DidStartNavigation event.
      navigation_start_time_(base::Time::Now()),
      navigation_start_time_ticks_(base::TimeTicks::Now()),
      module_id_details_(
          ntp::MakeModuleIdDetails(NewTabPageUI::IsManagedProfile(profile_),
                                   profile_)) {

  instance_count_++;
  base::UmaHistogramCounts100("NewTabPage.Count", instance_count_);
  bool session_allows_drag_and_drop = false;
  if (auto* session_handle = GetOrCreateContextualSessionHandle()) {
    session_allows_drag_and_drop =
        session_handle->CheckSearchContentSharingSettings(profile_->GetPrefs());
  }

  auto* source = CreateAndAddNewTabPageUiHtmlSource(
      profile_, session_allows_drag_and_drop);
  web_ui->AddMessageHandler(
      std::make_unique<MewebAgentWorkspaceHandler>(profile_));
// TODO(b/502297163): Implement for Android.
#if BUILDFLAG(IS_ANDROID)
  bool wallpaper_search_button_enabled = false;
#else
  bool wallpaper_search_button_enabled =
      base::FeatureList::IsEnabled(ntp_features::kNtpWallpaperSearchButton) &&
      customize_chrome::IsWallpaperSearchEnabledForProfile(profile_);
#endif
  source->AddBoolean("wallpaperSearchButtonEnabled",
                     wallpaper_search_button_enabled);
// TODO(b/502297163): Implement for Android.
#if BUILDFLAG(IS_ANDROID)
  bool should_animate_wallpaper_search_button = false;
#else
  int wallpaper_search_animation_shown_threshold =
      ntp_features::GetWallpaperSearchButtonAnimationShownThreshold();
  // Animate the button if the threshold is negative (unconditional) or if the
  // button has has been shown less times than the threshold.
  bool should_animate_wallpaper_search_button =
      wallpaper_search_animation_shown_threshold < 0 ||
      wallpaper_search_animation_shown_threshold >=
          profile_->GetPrefs()->GetInteger(
              prefs::kNtpWallpaperSearchButtonShownCount);
#endif
  source->AddBoolean(
      "wallpaperSearchButtonAnimationEnabled",
      wallpaper_search_button_enabled &&
          base::FeatureList::IsEnabled(
              ntp_features::kNtpWallpaperSearchButtonAnimation) &&
          should_animate_wallpaper_search_button);
  source->AddInteger("wallpaperSearchButtonHideCondition",
                     ntp_features::GetWallpaperSearchButtonHideCondition());

  // Add a handler to provide pluralized strings.
  auto plural_string_handler = std::make_unique<PluralStringHandler>();
  plural_string_handler->AddLocalizedString("modulesTabGroupsTabsText",
                                            IDS_SAVED_TAB_GROUP_TABS_COUNT);
  plural_string_handler->AddLocalizedString("sharingTabs",
                                            IDS_COMPOSE_SHARING_TABS);
  web_ui->AddMessageHandler(std::move(plural_string_handler));

  content::URLDataSource::Add(profile_,
                              std::make_unique<SanitizedImageSource>(profile_));
  content::URLDataSource::Add(
      profile_, std::make_unique<FaviconSource>(
                    profile_, chrome::FaviconUrlFormat::kFavicon2));
  content::URLDataSource::Add(profile_,
                              std::make_unique<UntrustedSource>(profile_));
  content::URLDataSource::Add(
      profile_,
      std::make_unique<ThemeSource>(profile_, /*serve_untrusted=*/true));
// TODO(b/502297163): Implement for Android.
#if BUILDFLAG(IS_ANDROID)
  content::URLDataSource::Add(profile_,
                              std::make_unique<ThemeSource>(profile_));
#endif

  web_ui->AddRequestableScheme(content::kChromeUIUntrustedScheme);

// Give OGB 3P Cookie Permissions. Only necessary on non-Ash builds. Granting
// 3P cookies on Ash causes b/314326552.
#if !BUILDFLAG(IS_CHROMEOS)
  WebUIAllowlist::GetOrCreate(profile_)->RegisterAutoGrantedThirdPartyCookies(
      url::Origin::Create(GURL(chrome::kChromeUIUntrustedNewTabPageUrl)),
      {
          ContentSettingsPattern::FromURL(GURL("https://ogs.google.com")),
          ContentSettingsPattern::FromURL(GURL("https://corp.google.com")),
      });
#endif  // !BUILDFLAG(IS_CHROMEOS)

  pref_change_registrar_.Init(profile_->GetPrefs());
  pref_change_registrar_.Add(
      ntp_prefs::kNtpCustomLinksVisible,
      base::BindRepeating(&NewTabPageUI::OnTileTypesChanged,
                          weak_ptr_factory_.GetWeakPtr()));
  pref_change_registrar_.Add(
      ntp_prefs::kNtpEnterpriseShortcutsVisible,
      base::BindRepeating(&NewTabPageUI::OnTileTypesChanged,
                          weak_ptr_factory_.GetWeakPtr()));
  pref_change_registrar_.Add(
      ntp_prefs::kNtpPersonalShortcutsVisible,
      base::BindRepeating(&NewTabPageUI::OnTileTypesChanged,
                          weak_ptr_factory_.GetWeakPtr()));
  pref_change_registrar_.Add(
      ntp_prefs::kNtpShortcutsVisible,
      base::BindRepeating(&NewTabPageUI::OnTilesVisibilityPrefChanged,
                          weak_ptr_factory_.GetWeakPtr()));
// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
  pref_change_registrar_.Add(
      ntp_tiles::prefs::kEnterpriseShortcutsPolicyList,
      base::BindRepeating(&NewTabPageUI::OnEnterpriseShortcutsPolicyChanged,
                          weak_ptr_factory_.GetWeakPtr()));

  // Store basic theme info in load time data to make the background color and
  // background image available as soon as the page loads to prevent a potential
  // white flicker.

  ntp_custom_background_service_observation_.Observe(
      ntp_custom_background_service_.get());
#endif

  // Populates the load time data with basic info.
  OnColorProviderChanged();
  OnCustomBackgroundImageUpdated();
  OnLoad();

  ui::TrackedElementHandlerDocumentSingleton::Register(
      this, std::vector<ui::ElementIdentifier>{
                CustomizeButtonsHandler::kCustomizeChromeButtonElementId,
                NewTabPageUI::kRealboxContextualEntrypointElementId});
}

WEB_UI_CONTROLLER_TYPE_IMPL(NewTabPageUI)

NewTabPageUI::~NewTabPageUI() {
  instance_count_--;
  // Deregister customize chrome entry on unified side panel, unless the
  // WebContents is showing another NewTabPageUI (e.g. in case of reloads).
  if (auto* web_ui = web_contents()->GetWebUI()) {
    if (web_ui->GetController() && web_ui->GetController()->GetType() &&
        web_ui->GetController()->GetAs<NewTabPageUI>()) {
      return;
    }
  }
}

// static
bool NewTabPageUI::IsNewTabPageOrigin(const GURL& url) {
  return url.DeprecatedGetOriginAsURL() ==
         chrome::ChromeUINewTabPageURLAsGURL().DeprecatedGetOriginAsURL();
}

// static
void NewTabPageUI::RegisterProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterTimePref(kPrevNavigationTimePrefName, base::Time());
  registry->RegisterStringPref(kMewebAgentWorkspaceStatePref, std::string());
  registry->RegisterBooleanPref(ntp_prefs::kNtpCustomLinksVisible, true);
  registry->RegisterBooleanPref(ntp_prefs::kNtpEnterpriseShortcutsVisible,
                                false);
  registry->RegisterBooleanPref(ntp_prefs::kNtpShortcutsVisible, true);
  registry->RegisterIntegerPref(ntp_prefs::kNtpShortcutsStalenessCount, 0);
  registry->RegisterTimePref(ntp_prefs::kNtpLastShortcutsStalenessUpdate,
                             base::Time());
  registry->RegisterBooleanPref(ntp_prefs::kNtpShortcutsAutoRemovalDisabled,
                                false);
  registry->RegisterBooleanPref(ntp_prefs::kNtpPersonalShortcutsVisible, true);
  registry->RegisterBooleanPref(ntp_prefs::kNtpShowAllMostVisitedTiles, false);
  registry->RegisterBooleanPref(prefs::kNtpPromoVisible, true);
  registry->RegisterTimePref(ntp_prefs::kNtpLastModuleStalenessUpdate,
                             base::Time());
  registry->RegisterDictionaryPref(ntp_prefs::kNtpModuleStalenessCountDict);
  registry->RegisterDictionaryPref(
      ntp_prefs::kNtpModulesAutoRemovalDisabledDict);
  registry->RegisterBooleanPref(ntp_prefs::kNtpAnimatedDoodlesEnabled, true);
  registry->RegisterBooleanPref(ntp_prefs::kNtpDoodleMuralsEnabled, true);
  registry->RegisterInt64Pref(ntp_prefs::kNtpMostVisitedTileHoverCount, 0);
  registry->RegisterInt64Pref(ntp_prefs::kNtpMostVisitedTileNavigationCount, 0);
}

// static
void NewTabPageUI::ResetProfilePrefs(PrefService* prefs) {
  ntp_tiles::MostVisitedSites::ResetProfilePrefs(prefs);
  prefs->SetBoolean(ntp_prefs::kNtpCustomLinksVisible, true);
  prefs->SetBoolean(ntp_prefs::kNtpEnterpriseShortcutsVisible, false);
  prefs->SetBoolean(ntp_prefs::kNtpShortcutsVisible, true);
  prefs->SetInteger(ntp_prefs::kNtpShortcutsStalenessCount, 0);
  prefs->SetTime(ntp_prefs::kNtpLastShortcutsStalenessUpdate, base::Time());
  prefs->SetBoolean(ntp_prefs::kNtpShortcutsAutoRemovalDisabled, false);
  prefs->SetBoolean(ntp_prefs::kNtpPersonalShortcutsVisible, true);
  prefs->SetBoolean(ntp_prefs::kNtpShowAllMostVisitedTiles, false);
  prefs->SetTime(ntp_prefs::kNtpLastModuleStalenessUpdate, base::Time());
  prefs->SetDict(ntp_prefs::kNtpModuleStalenessCountDict, base::DictValue());
  prefs->SetDict(ntp_prefs::kNtpModulesAutoRemovalDisabledDict,
                 base::DictValue());
  prefs->SetBoolean(ntp_prefs::kNtpAnimatedDoodlesEnabled, true);
  prefs->SetBoolean(ntp_prefs::kNtpDoodleMuralsEnabled, true);
  prefs->SetInt64(ntp_prefs::kNtpMostVisitedTileHoverCount, 0);
  prefs->SetInt64(ntp_prefs::kNtpMostVisitedTileNavigationCount, 0);
  prefs->SetDict(prefs::kContextMenuAnimationState, base::DictValue());
}

// static
void NewTabPageUI::MigrateDeprecatedUseMostVisitedTilesPref(
    PrefService* prefs) {
  // Skip migration if the new preference is already set.
  if (prefs->HasPrefPath(ntp_prefs::kNtpShortcutsType)) {
    return;
  }
  const base::Value* user_value =
      prefs->GetUserPrefValue(ntp_prefs::kNtpUseMostVisitedTiles);
  if (user_value) {
    if (user_value->is_bool()) {
      prefs->SetInteger(
          ntp_prefs::kNtpShortcutsType,
          user_value->GetBool()
              ? static_cast<int>(ntp_tiles::TileType::kTopSites)
              : static_cast<int>(ntp_tiles::TileType::kCustomLinks));
    }
    prefs->ClearPref(ntp_prefs::kNtpUseMostVisitedTiles);
  }
}

// static
void NewTabPageUI::MigrateDeprecatedShortcutsTypePref(PrefService* prefs) {
  // Skip migration if the new preferences are already set.
  if (prefs->HasPrefPath(ntp_prefs::kNtpCustomLinksVisible) ||
      prefs->HasPrefPath(ntp_prefs::kNtpEnterpriseShortcutsVisible)) {
    return;
  }
  const base::Value* user_value =
      prefs->GetUserPrefValue(ntp_prefs::kNtpShortcutsType);
  if (user_value) {
    if (user_value->is_int()) {
      switch (static_cast<ntp_tiles::TileType>(user_value->GetInt())) {
        case ntp_tiles::TileType::kTopSites:
          prefs->SetBoolean(ntp_prefs::kNtpCustomLinksVisible, false);
          prefs->SetBoolean(ntp_prefs::kNtpEnterpriseShortcutsVisible, false);
          break;
        case ntp_tiles::TileType::kCustomLinks:
          prefs->SetBoolean(ntp_prefs::kNtpCustomLinksVisible, true);
          prefs->SetBoolean(ntp_prefs::kNtpEnterpriseShortcutsVisible, false);
          break;
        case ntp_tiles::TileType::kEnterpriseShortcuts:
          prefs->SetBoolean(ntp_prefs::kNtpCustomLinksVisible, false);
          prefs->SetBoolean(ntp_prefs::kNtpEnterpriseShortcutsVisible, true);
          break;
      }
    }
    prefs->ClearPref(ntp_prefs::kNtpShortcutsType);
  }
}

// static
bool NewTabPageUI::IsManagedProfile(Profile* profile) {
  // TODO(crbug.com/40183609): Stop calling the private method
  // FindExtendedPrimaryAccountInfo().
  auto* identity_manager = IdentityManagerFactory::GetForProfile(profile);
  return /* Can be null if Chrome signin is disabled. */ identity_manager &&
         identity_manager
                 ->FindExtendedPrimaryAccountInfo(signin::ConsentLevel::kSignin)
                 .IsManaged() == signin::Tribool::kTrue;
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<new_tab_page::mojom::PageHandlerFactory>
        pending_receiver) {
  if (page_factory_receiver_.is_bound()) {
    page_factory_receiver_.reset();
  }

  page_factory_receiver_.Bind(std::move(pending_receiver));
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<searchbox::mojom::PageHandlerFactory>
        pending_receiver) {
  if (searchbox_page_factory_receiver_.is_bound()) {
    searchbox_page_factory_receiver_.reset();
  }
  searchbox_page_factory_receiver_.Bind(std::move(pending_receiver));
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<browser_command::mojom::CommandHandlerFactory>
        pending_receiver) {
  if (browser_command_factory_receiver_.is_bound()) {
    browser_command_factory_receiver_.reset();
  }
  browser_command_factory_receiver_.Bind(std::move(pending_receiver));
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<
        customize_buttons::mojom::CustomizeButtonsHandlerFactory>
        pending_receiver) {
  if (customize_buttons_factory_receiver_.is_bound()) {
    customize_buttons_factory_receiver_.reset();
  }
  customize_buttons_factory_receiver_.Bind(std::move(pending_receiver));
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<most_visited::mojom::MostVisitedPageHandlerFactory>
        pending_receiver) {
  if (most_visited_page_factory_receiver_.is_bound()) {
    most_visited_page_factory_receiver_.reset();
  }
  most_visited_page_factory_receiver_.Bind(std::move(pending_receiver));
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<file_suggestion::mojom::DriveSuggestionHandler>
        pending_receiver) {
  drive_handler_ = std::make_unique<DriveSuggestionHandler>(
      std::move(pending_receiver), profile_);
}

#if !defined(OFFICIAL_BUILD)
void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<foo::mojom::FooHandler> pending_page_handler) {
  foo_handler_ = std::make_unique<FooHandler>(std::move(pending_page_handler));
}
#endif

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<ntp::tab_groups::mojom::PageHandler>
        pending_page_handler) {
  tab_groups_handler_ = std::make_unique<TabGroupsPageHandler>(
      std::move(pending_page_handler), web_contents());
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<ntp::most_relevant_tab_resumption::mojom::PageHandler>
        pending_page_handler) {
  most_relevant_tab_resumption_handler_ =
      std::make_unique<MostRelevantTabResumptionPageHandler>(
          std::move(pending_page_handler), web_contents());
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<ntp::calendar::mojom::GoogleCalendarPageHandler>
        pending_page_handler) {
  google_calendar_handler_ = std::make_unique<GoogleCalendarPageHandler>(
      std::move(pending_page_handler), profile_);
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<ntp::calendar::mojom::OutlookCalendarPageHandler>
        pending_page_handler) {
  outlook_calendar_handler_ = std::make_unique<OutlookCalendarPageHandler>(
      std::move(pending_page_handler), profile_);
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<ntp::authentication::mojom::MicrosoftAuthPageHandler>
        pending_page_handler) {
  microsoft_auth_handler_ = std::make_unique<MicrosoftAuthPageHandler>(
      std::move(pending_page_handler), profile_);
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<file_suggestion::mojom::MicrosoftFilesPageHandler>
        pending_page_handler) {
  microsoft_files_handler_ = std::make_unique<MicrosoftFilesPageHandler>(
      std::move(pending_page_handler), profile_);
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<composebox::mojom::PageHandlerFactory>
        pending_receiver) {
  auto* aim_service = AimEligibilityServiceFactory::GetForProfile(profile_);
  bool aim_eligible = aim_service && aim_service->IsAimEligible();

  if (!aim_eligible &&
      !ntp_composebox::IsNtpComposeboxEnabled(profile_) &&
      !SearchboxHandler::GetVoiceSearchCoherenceAnySearchboxExperimentEnabled()) {
    return;
  }
  if (composebox_page_factory_receiver_.is_bound()) {
    composebox_page_factory_receiver_.reset();
  }
  composebox_page_factory_receiver_.Bind(std::move(pending_receiver));
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<page_image_service::mojom::PageImageServiceHandler>
        pending_page_handler) {
  base::WeakPtr<page_image_service::ImageService> image_service_weak;
  if (auto* image_service =
          page_image_service::ImageServiceFactory::GetForBrowserContext(
              profile_)) {
    image_service_weak = image_service->GetWeakPtr();
  }
  image_service_handler_ =
      std::make_unique<page_image_service::ImageServiceHandler>(
          std::move(pending_page_handler), std::move(image_service_weak));
}

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<help_bubble::mojom::HelpBubbleHandlerFactory>
        pending_receiver) {
  if (help_bubble_handler_factory_receiver_.is_bound()) {
    help_bubble_handler_factory_receiver_.reset();
  }
  help_bubble_handler_factory_receiver_.Bind(std::move(pending_receiver));
}

// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<ntp_promo::mojom::NtpPromoHandlerFactory>
        pending_receiver) {
  if (ntp_promo_handler_factory_receiver_.is_bound()) {
    ntp_promo_handler_factory_receiver_.reset();
  }
  ntp_promo_handler_factory_receiver_.Bind(std::move(pending_receiver));
}
#endif  // !BUILDFLAG(IS_ANDROID)

void NewTabPageUI::BindInterface(
    mojo::PendingReceiver<action_chips::mojom::ActionChipsHandlerFactory>
        pending_receiver) {
  if (action_chips_handler_factory_receiver_.is_bound()) {
    action_chips_handler_factory_receiver_.reset();
  }
  action_chips_handler_factory_receiver_.Bind(std::move(pending_receiver));
}

void NewTabPageUI::CreatePageHandler(
    mojo::PendingRemote<new_tab_page::mojom::Page> pending_page,
    mojo::PendingReceiver<new_tab_page::mojom::PageHandler>
        pending_page_handler) {
  DCHECK(pending_page.is_valid());

  page_handler_ = std::make_unique<NewTabPageHandler>(
      std::move(pending_page_handler), std::move(pending_page), profile_,
      ntp_custom_background_service_, theme_service_,
      LogoServiceFactory::GetForProfile(profile_),
      SyncServiceFactory::GetForProfile(profile_),
      segmentation_platform::SegmentationPlatformServiceFactory::GetForProfile(
          profile_),
      web_contents(), navigation_start_time_, navigation_start_time_ticks_,
      &module_id_details_);
}

void NewTabPageUI::ConnectToParentDocument(
    mojo::PendingRemote<new_tab_page::mojom::MicrosoftAuthUntrustedDocument>
        child_page) {
  page_handler_->ConnectToParentDocument(std::move(child_page));
}

void NewTabPageUI::CreateBrowserCommandHandler(
    mojo::PendingReceiver<browser_command::mojom::CommandHandler>
        pending_handler) {
// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
  using browser_command::mojom::Command;
  std::vector<Command> supported_commands = {
      Command::kOpenSafetyCheck,
      Command::kOpenSafeBrowsingEnhancedProtectionSettings,
      Command::kNoOpCommand,
  };
  promo_browser_command_handler_ = std::make_unique<BrowserCommandHandler>(
      std::move(pending_handler), profile_, supported_commands,
      web_ui()->GetWebContents());
#endif
}

void NewTabPageUI::CreateCustomizeButtonsHandler(
    mojo::PendingRemote<customize_buttons::mojom::CustomizeButtonsDocument>
        pending_page,
    mojo::PendingReceiver<customize_buttons::mojom::CustomizeButtonsHandler>
        pending_page_handler) {
// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
  customize_buttons_handler_ = std::make_unique<CustomizeButtonsHandler>(
      std::move(pending_page_handler), std::move(pending_page), web_ui(),
      webui::GetTabInterface(web_contents()),
      std::make_unique<NewTabPageFeaturePromoHelper>());
#endif
}

void NewTabPageUI::CreatePageHandler(
    mojo::PendingRemote<most_visited::mojom::MostVisitedPage> pending_page,
    mojo::PendingReceiver<most_visited::mojom::MostVisitedPageHandler>
        pending_page_handler) {
  DCHECK(pending_page.is_valid());
  most_visited_page_handler_ = std::make_unique<MostVisitedHandler>(
      std::move(pending_page_handler), std::move(pending_page), profile_,
      web_contents(), chrome::ChromeUINewTabPageURLAsGURL(),
      navigation_start_time_, navigation_start_time_ticks_);
  UpdateMostVisitedTileTypes();
  most_visited_page_handler_->SetShortcutsVisible(IsShortcutsVisible());
}

void NewTabPageUI::CreatePageHandler(
    mojo::PendingRemote<searchbox::mojom::Page> pending_page,
    mojo::PendingReceiver<searchbox::mojom::PageHandler> pending_page_handler) {
  realbox_handler_ = std::make_unique<RealboxHandler>(
      std::move(pending_page_handler), std::move(pending_page), profile_,
      web_contents(),
      base::BindRepeating(&NewTabPageUI::GetOrCreateContextualSessionHandle,
                          base::Unretained(this)));
}

void NewTabPageUI::CreatePageHandler(
    mojo::PendingReceiver<composebox::mojom::PageHandler> pending_page_handler,
    mojo::PendingRemote<searchbox::mojom::Page> pending_searchbox_page,
    mojo::PendingReceiver<searchbox::mojom::PageHandler>
        pending_searchbox_handler) {
  composebox_handler_ = std::make_unique<ComposeboxHandler>(
      std::move(pending_page_handler), std::move(pending_searchbox_handler),
      std::move(pending_searchbox_page), profile_, web_contents(),
      base::BindRepeating(&NewTabPageUI::GetOrCreateContextualSessionHandle,
                          base::Unretained(this)),
      base::BindRepeating(&NewTabPageUI::ClearContextualSessionHandle,
                          base::Unretained(this)));
}

void NewTabPageUI::CreateHelpBubbleHandler(
    mojo::PendingRemote<help_bubble::mojom::HelpBubbleClient> client,
    mojo::PendingReceiver<help_bubble::mojom::HelpBubbleHandler> handler) {
// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
  help_bubble_handler_ = std::make_unique<user_education::HelpBubbleHandler>(
      std::move(handler), std::move(client),
      ui::TrackedElementHandlerDocumentSingleton::GetOrCreate(
          web_ui()->GetRenderFrameHost()));
#endif
}

// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
void NewTabPageUI::CreateNtpPromoHandler(
    mojo::PendingRemote<ntp_promo::mojom::NtpPromoClient> client,
    mojo::PendingReceiver<ntp_promo::mojom::NtpPromoHandler> handler) {
  ntp_promo_handler_ = NtpPromoHandler::Create(
      std::move(client), std::move(handler), web_contents());
}
#endif  // !BUILDFLAG(IS_ANDROID)

void NewTabPageUI::CreateActionChipsHandler(
    mojo::PendingReceiver<action_chips::mojom::ActionChipsHandler> handler,
    mojo::PendingRemote<action_chips::mojom::Page> page) {
  action_chips_handler_ = std::make_unique<ActionChipsHandler>(
      std::move(handler), std::move(page), profile_, web_ui(),
      std::make_unique<ActionChipsGeneratorImpl>(profile_),
      base::BindRepeating(&NewTabPageUI::GetOrCreateContextualSessionHandle,
                          base::Unretained(this)));
}

// OnColorProviderChanged can be called during the destruction process and
// should not directly access any member variables.
void NewTabPageUI::OnColorProviderChanged() {
  base::DictValue update;
  if (!web_contents() || !web_ui()) {
    return;
  }
  const ui::ColorProvider& color_provider = web_contents()->GetColorProvider();
  auto background_color = color_provider.GetColor(kColorNewTabPageBackground);
  update.Set("backgroundColor", skia::SkColorToHexString(background_color));
  content::WebUIDataSource::Update(Profile::FromWebUI(web_ui()),
                                   chrome::kChromeUINewTabPageHost,
                                   std::move(update));
}

void NewTabPageUI::OnCustomBackgroundImageUpdated() {
  base::DictValue update;
  auto custom_background_url =
      (ntp_custom_background_service_
           ? ntp_custom_background_service_->GetCustomBackground()
           : std::optional<CustomBackground>())
          .value_or(CustomBackground())
          .custom_background_url;
  url::UriComponentEncoder encoded_url(custom_background_url.spec());
  update.Set(
      "backgroundImageUrl",
      encoded_url.view().length() > 0
          ? base::StrCat(
                {"chrome-untrusted://new-tab-page/custom_background_image?url=",
                 encoded_url.view()})
          : "");
  content::WebUIDataSource::Update(profile_, chrome::kChromeUINewTabPageHost,
                                   std::move(update));
}

contextual_search::ContextualSearchSessionHandle*
NewTabPageUI::GetOrCreateContextualSessionHandle() {
  if (!shared_session_handle_) {
    auto* contextual_search_service =
        ContextualSearchServiceFactory::GetForProfile(profile_);
    if (contextual_search_service) {
      shared_session_handle_ = contextual_search_service->CreateSession(
          ntp_composebox::CreateQueryControllerConfigParams(),
          contextual_search::ContextualSearchSource::kNewTabPage,
          lens::LensOverlayInvocationSource::kNtpContextualQuery);
      // TODO(crbug.com/469875247): Determine what to do with the return value
      // of this call, or move this call to a different location.
      shared_session_handle_->CheckSearchContentSharingSettings(
          profile_->GetPrefs());
    }
  }
  return shared_session_handle_.get();
}

void NewTabPageUI::ClearContextualSessionHandle() {
  shared_session_handle_.reset();
}

void NewTabPageUI::DidStartNavigation(
    content::NavigationHandle* navigation_handle) {
  if (navigation_handle->IsInPrimaryMainFrame() &&
      navigation_handle->GetURL() == chrome::ChromeUINewTabPageURLAsGURL()) {
    navigation_start_time_ = base::Time::Now();
    navigation_start_time_ticks_ = base::TimeTicks::Now();

    OnLoad();

    auto prev_navigation_time =
        profile_->GetPrefs()->GetTime(kPrevNavigationTimePrefName);
    if (!prev_navigation_time.is_null()) {
      base::UmaHistogramCustomTimes(
          "NewTabPage.TimeSinceLastNTP",
          navigation_start_time_ - prev_navigation_time, base::Seconds(1),
          base::Days(1), 100);
    }
    profile_->GetPrefs()->SetTime(kPrevNavigationTimePrefName,
                                  navigation_start_time_);
    base::UmaHistogramBoolean("NewTabPage.HasCredentials",
                              HasCredentials(profile_));
  }
}

bool NewTabPageUI::IsShortcutsVisible() const {
  return profile_->GetPrefs()->GetBoolean(ntp_prefs::kNtpShortcutsVisible);
}

void NewTabPageUI::UpdateMostVisitedTileTypes() {
  if (most_visited_page_handler_) {
    auto enabled_types = GetEnabledTileTypes(profile_);
    most_visited_page_handler_->EnableTileTypes(
        ntp_tiles::MostVisitedSites::EnableTileTypesOptions()
            .with_top_sites(
                enabled_types.contains(ntp_tiles::TileType::kTopSites))
            .with_custom_links(
                enabled_types.contains(ntp_tiles::TileType::kCustomLinks))
            .with_enterprise_shortcuts(enabled_types.contains(
                ntp_tiles::TileType::kEnterpriseShortcuts)));
  }
}

void NewTabPageUI::OnTileTypesChanged() {
  UpdateMostVisitedTileTypes();
}

void NewTabPageUI::OnTilesVisibilityPrefChanged() {
  if (most_visited_page_handler_) {
    most_visited_page_handler_->SetShortcutsVisible(IsShortcutsVisible());
  }
}

void NewTabPageUI::OnEnterpriseShortcutsPolicyChanged() {
  MaybeEnableEnterpriseShortcutsVisibility();
  OnTileTypesChanged();
}

void NewTabPageUI::OnLoad() {
  MaybeEnableEnterpriseShortcutsVisibility();
  base::DictValue update;
  update.Set("navigationStartTime",
             navigation_start_time_.InMillisecondsFSinceUnixEpoch());
  const bool modules_enabled = ntp::HasModulesEnabled(
      module_id_details_, IdentityManagerFactory::GetForProfile(profile_));
  update.Set("modulesEnabled", modules_enabled);

  // Set up the NTP promo, if any.
  update.Set("browserPromoType", GetNtpPromoType());

  content::WebUIDataSource::Update(profile_, chrome::kChromeUINewTabPageHost,
                                   std::move(update));
}

void NewTabPageUI::MaybeEnableEnterpriseShortcutsVisibility() {
// TODO(b/502297163): Implement for Android.
#if !BUILDFLAG(IS_ANDROID)
  // If enterprise shortcuts are available by policy and the user
  // has not previously set the visibility preference, then enable enterprise
  // shortcuts by default.
  if (!profile_->GetPrefs()
           ->GetList(ntp_tiles::prefs::kEnterpriseShortcutsPolicyList)
           .empty() &&
      !profile_->GetPrefs()->HasPrefPath(
          ntp_prefs::kNtpEnterpriseShortcutsVisible)) {
    profile_->GetPrefs()->SetBoolean(ntp_prefs::kNtpEnterpriseShortcutsVisible,
                                     true);
  }
#endif  // !BUILDFLAG(IS_ANDROID)
}

// static
base::RefCountedMemory* NewTabPageUI::GetFaviconResourceBytes(
    ui::ResourceScaleFactor scale_factor) {
  return static_cast<base::RefCountedMemory*>(
      ui::ResourceBundle::GetSharedInstance().LoadDataResourceBytesForScale(
          IDR_NTP_FAVICON, scale_factor));
}

std::string_view NewTabPageUI::GetNtpPromoType() {
// TODO(b/502297163): Implement for Android.
#if BUILDFLAG(IS_ANDROID)
  return kDisabledBrowserPromo;
#else
  auto* controller = UserEducationServiceFactory::GetForBrowserContext(profile_)
                         ->ntp_promo_controller();
  if (!controller) {
    return kDisabledBrowserPromo;
  }
  auto* user_education =
      BrowserUserEducationInterface::MaybeGetForWebContentsInTab(
          web_contents());
  if (!user_education) {
    return kDisabledBrowserPromo;
  }
  auto context =
      user_education->GetUserEducationContext(base::PassKey<NewTabPageUI>());

  switch (user_education::features::GetNtpBrowserPromoType()) {
    case user_education::features::NtpBrowserPromoType::kSimple:
      return controller->HasShowablePromo(context) ? kSimpleBrowserPromo
                                                   : kEmptyBrowserPromo;
    case user_education::features::NtpBrowserPromoType::kNone:
      return kDisabledBrowserPromo;
  }
#endif  // BUILDFLAG(IS_ANDROID)
}
