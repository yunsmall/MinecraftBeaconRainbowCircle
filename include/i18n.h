#pragma once

#include <string>
#include <string_view>

#include <libintl.h>

#define GETTEXT_DOMAIN "minecraft_beacon_circle"
#define GETTEXT_OUTPUT_DIR "./locale"

#define _(STRING) gettext(STRING)

// std::string get_locale_name();

void set_system_i18n();

void setup_i18n(const std::string_view locale);
