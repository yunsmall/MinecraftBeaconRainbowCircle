#include "i18n.h"

#include <filesystem>
#include <iostream>
#include <string>

#include "utils.h"

#if WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

void set_system_i18n() {
    // WCHAR buffer[LOCALE_NAME_MAX_LENGTH];
    // GetUserDefaultLocaleName(buffer, std::size(buffer));
    // std::wcout << "locale name:" << buffer << std::endl;
    // #if WIN32
    //     _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
    //     SetThreadLocale(GetUserDefaultLCID());
    // #else
    //     setlocale(LC_MESSAGES, "");
    // #endif
    setlocale(LC_MESSAGES, "");
    auto executable_path = get_executable_path();
    // std::cout << "executable_path:" << executable_path << std::endl;
    auto directory_path = executable_path.parent_path();
    auto locale_path = (directory_path / GETTEXT_OUTPUT_DIR).lexically_normal();
    // std::cout << "locale_path:" << locale_path << std::endl;
    bindtextdomain(GETTEXT_DOMAIN, locale_path.string().c_str());
    bind_textdomain_codeset(GETTEXT_DOMAIN, "UTF-8");
    textdomain(GETTEXT_DOMAIN);
}


// std::string get_locale_name() {
//     LANGID langID = GetUserDefaultUILanguage();
//     char locale[10];
//     snprintf(locale, sizeof(locale), "%s_%s",
//         get_lang_name(langID),  // 如 "zh"
//         get_country_name(langID) // 如 "CN"
//     );
// }


void setup_i18n(const std::string_view locale) {
#if WIN32
    // LocaleNameToLCID requires a LPCWSTR so we need to convert from char to wchar_t
    const auto wStringSize = MultiByteToWideChar(CP_UTF8, 0, locale.data(), static_cast<int>(locale.length()), nullptr,
                                                 0);
    std::wstring localeName;
    localeName.reserve(wStringSize);
    MultiByteToWideChar(CP_UTF8, 0, locale.data(), static_cast<int>(locale.length()), localeName.data(), wStringSize);

    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
    const auto localeId = LocaleNameToLCID(localeName.c_str(), LOCALE_ALLOW_NEUTRAL_NAMES);
    SetThreadLocale(localeId);
#else
    setlocale(LC_MESSAGES, locale.data());
#endif


    bindtextdomain(GETTEXT_DOMAIN, GETTEXT_OUTPUT_DIR);
    bind_textdomain_codeset(GETTEXT_DOMAIN, "UTF-8");
    textdomain(GETTEXT_DOMAIN);
}
