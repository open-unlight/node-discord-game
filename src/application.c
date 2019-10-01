#include "discord_game.h"

napi_value GetApplicationLocale(napi_env env, napi_callback_info info) {
  void* data;
  AddonState* state;

  NAPI_REQUIRE(napi_get_cb_info(env, info, 0, NULL, NULL, &data));
  state = data;

  napi_value ret;
  if (!state->initialized) {
    NAPI_REQUIRE(napi_get_boolean(env, false, &ret));
    return ret;
  }

  DiscordLocale current_locale;
  state->app.application->get_current_locale(state->app.application, &current_locale);

  NAPI_REQUIRE(napi_create_string_utf8(env, current_locale, NAPI_AUTO_LENGTH, &ret));

  return ret;
}

napi_value GetApplicationBranch(napi_env env, napi_callback_info info) {
  void* data;
  AddonState* state;

  NAPI_REQUIRE(napi_get_cb_info(env, info, 0, NULL, NULL, &data));
  state = data;

  napi_value ret;
  if (!state->initialized) {
    NAPI_REQUIRE(napi_get_boolean(env, false, &ret));
    return ret;
  }

  DiscordBranch current_branch;
  state->app.application->get_current_branch(state->app.application, &current_branch);

  NAPI_REQUIRE(napi_create_string_utf8(env, current_branch, NAPI_AUTO_LENGTH, &ret));

  return ret;
}

napi_value Application_Init(napi_env env, AddonState* state) {
  napi_value exports;
  NAPI_REQUIRE(napi_create_object(env, &exports));

  napi_property_descriptor desc[] = {
    { "locale", NULL, NULL, GetApplicationLocale, NULL, NULL, napi_default, state },
    { "branch", NULL, NULL, GetApplicationBranch, NULL, NULL, napi_default, state }
  };

  NAPI_REQUIRE(napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

  return exports;
}
