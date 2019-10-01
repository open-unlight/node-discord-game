#include "application.h"

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

void OnOAuth2Token(void* data, enum EDiscordResult result, struct DiscordOAuth2Token* token) {
  Promise* promise = data;
  napi_value ret;
  if (result != DiscordResult_Ok) {
    NAPI_REQUIRE(napi_create_uint32(promise->env, (uint32_t)result, &ret));
    NAPI_REQUIRE(napi_reject_deferred(promise->env, promise->deferred, ret));
    return;
  }

  // TODO: Return OAuth2Token Object
  NAPI_REQUIRE(napi_create_string_utf8(promise->env, token->access_token, NAPI_AUTO_LENGTH, &ret));
  NAPI_REQUIRE(napi_resolve_deferred(promise->env, promise->deferred, ret));

  promise->env = NULL;
  promise->deferred = NULL;
  free(promise);
}

napi_value GetOAuth2Token(napi_env env, napi_callback_info info) {
  void* data;
  AddonState* state;

  NAPI_REQUIRE(napi_get_cb_info(env, info, 0, NULL, NULL, &data));
  state = data;

  napi_value ret;
  napi_deferred deferred;
  NAPI_REQUIRE(napi_create_promise(env, &deferred, &ret));

  if (!state->initialized) {
    napi_value undefiend;
    NAPI_REQUIRE(napi_get_undefined(env, &undefiend));
    NAPI_REQUIRE(napi_reject_deferred(env, deferred, undefiend));

    return ret;
  }

  Promise *promise = malloc(sizeof(*promise));

  promise->env = env;
  promise->deferred = deferred;
  promise->state = state;

  state->app.application->get_oauth2_token(state->app.application, promise, OnOAuth2Token);

  return ret;
}

napi_value Application_Init(napi_env env, AddonState* state) {
  napi_value exports;
  NAPI_REQUIRE(napi_create_object(env, &exports));

  napi_property_descriptor desc[] = {
    { "locale", NULL, NULL, GetApplicationLocale, NULL, NULL, napi_default, state },
    { "branch", NULL, NULL, GetApplicationBranch, NULL, NULL, napi_default, state },
    { "getOAuth2Token", NULL, GetOAuth2Token, NULL, NULL, NULL, napi_default, state }
  };

  NAPI_REQUIRE(napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

  return exports;
}
