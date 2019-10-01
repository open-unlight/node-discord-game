#include "activity.h"

void OnActivityUpdated(void* data, enum EDiscordResult result) {
  Promise *promise = data;
  napi_value undefined;
  napi_get_undefined(promise->env, &undefined);

  if (result == DiscordResult_Ok) {
    napi_resolve_deferred(promise->env, promise->deferred, undefined);
  } else {
    napi_reject_deferred(promise->env, promise->deferred, undefined);
  }

  promise->env = NULL;
  promise->deferred = NULL;
  free(promise);
}

napi_value UpdateActivity(napi_env env, napi_callback_info info) {
  void* data;
  AddonState* state;
  size_t argc = 4;
  napi_value argv[argc];

  NAPI_REQUIRE(napi_get_cb_info(env, info, &argc, argv, NULL, &data));
  state = data;

  // TODO: Add full activity support
  struct DiscordActivity activity;
  double timestamp;
  memset(&activity, 0, sizeof(activity));
  NAPI_REQUIRE(napi_get_value_string_utf8(env, argv[0], activity.details, 0, NULL));
  NAPI_REQUIRE(napi_get_value_string_utf8(env, argv[1], activity.state, 0, NULL));
  NAPI_REQUIRE(napi_get_value_string_utf8(env, argv[2], activity.assets.large_image, 0, NULL));
  NAPI_REQUIRE(napi_get_date_value(env, argv[3], &timestamp));
  activity.timestamps.start = (unsigned long)timestamp;

  napi_value ret;
  napi_deferred deferred;
  NAPI_REQUIRE(napi_create_promise(env, &deferred, &ret));

  Promise *promise = malloc(sizeof(*promise));
  promise->env = env;
  promise->deferred = deferred;
  promise->state = state;

  state->app.activities->update_activity(state->app.activities, &activity, promise, OnActivityUpdated);

  return ret;
}

napi_value Activity_Init(napi_env env, AddonState* state) {
  napi_value exports;
  NAPI_REQUIRE(napi_create_object(env, &exports));

  napi_property_descriptor desc[] = {
    { "update", NULL, UpdateActivity, NULL, NULL, NULL, napi_default, state },
  };

  NAPI_REQUIRE(napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

  return exports;
}
