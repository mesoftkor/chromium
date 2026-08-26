// Copyright 2026 The MEWEB Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NEW_TAB_PAGE_MEWEB_MODEL_AUTH_KEYCHAIN_H_
#define CHROME_BROWSER_UI_WEBUI_NEW_TAB_PAGE_MEWEB_MODEL_AUTH_KEYCHAIN_H_

#include <string>
#include <string_view>

namespace meweb_model_auth {

enum class KeychainReadStatus {
  kSuccess,
  kNotFound,
  kError,
};

struct KeychainReadResult {
  KeychainReadStatus status = KeychainReadStatus::kError;
  std::string credential;
};

// Stores model credentials as generic-password items in the user's macOS
// login Keychain. The caller supplies an opaque, profile-scoped account name.
bool SaveCredential(std::string_view account, std::string_view credential);
KeychainReadResult ReadCredential(std::string_view account);

// Missing entries count as successfully deleted.
bool DeleteCredential(std::string_view account);

}  // namespace meweb_model_auth

#endif  // CHROME_BROWSER_UI_WEBUI_NEW_TAB_PAGE_MEWEB_MODEL_AUTH_KEYCHAIN_H_
