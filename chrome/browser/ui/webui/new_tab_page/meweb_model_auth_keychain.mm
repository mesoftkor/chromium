// Copyright 2026 The MEWEB Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/new_tab_page/meweb_model_auth_keychain.h"

#import <Foundation/Foundation.h>
#import <Security/Security.h>

#include "base/apple/bridging.h"
#include "base/apple/scoped_cftyperef.h"
#include "base/containers/span.h"
#include "base/strings/sys_string_conversions.h"
#include "crypto/apple/keychain_v2.h"

namespace meweb_model_auth {
namespace {

constexpr char kServiceName[] = "com.mesoft.meweb.model-auth";

base::apple::ScopedCFTypeRef<CFDictionaryRef> MakeDeleteQuery(
    std::string_view account) {
  NSDictionary* query = @{
    base::apple::CFToNSPtrCast(kSecClass) :
        base::apple::CFToNSPtrCast(kSecClassGenericPassword),
    base::apple::CFToNSPtrCast(kSecAttrService) :
        base::SysUTF8ToNSString(kServiceName),
    base::apple::CFToNSPtrCast(kSecAttrAccount) :
        base::SysUTF8ToNSString(account),
  };
  return base::apple::ScopedCFTypeRef<CFDictionaryRef>(
      base::apple::NSToCFOwnershipCast(query));
}

}  // namespace

bool SaveCredential(std::string_view account, std::string_view credential) {
  return crypto::apple::KeychainV2::GetInstance().AddGenericPassword(
             kServiceName, account, base::as_byte_span(credential)) == noErr;
}

KeychainReadResult ReadCredential(std::string_view account) {
  auto password = crypto::apple::KeychainV2::GetInstance().FindGenericPassword(
      kServiceName, account);
  if (!password.has_value()) {
    return {.status = password.error() == errSecItemNotFound
                          ? KeychainReadStatus::kNotFound
                          : KeychainReadStatus::kError};
  }
  return {.status = KeychainReadStatus::kSuccess,
          .credential = std::string(password->begin(), password->end())};
}

bool DeleteCredential(std::string_view account) {
  const OSStatus status = crypto::apple::KeychainV2::GetInstance().ItemDelete(
      MakeDeleteQuery(account).get());
  return status == noErr || status == errSecItemNotFound;
}

}  // namespace meweb_model_auth
