#include "activity.h"

void OnActivityUpdated(void* data, enum EDiscordResult result) {
  Promise *promise = data;
  napi_value undefined;
  napi_get_undefined(promise->env, &undefined);

  if (result == DiscordResult_Ok) {
    napi_resolve_deferred(promise->env, promise->deferred, undefined);
  } else {
    napi_value result;
    NAPI_REQUIRE(napi_create_uint32(promise->env, (uint32_t)sizeof(result), &result));
    napi_reject_deferred(promise->env, promise->deferred, result);
  }

  promise->env = NULL;
  promise->deferred = NULL;
  free(promise);
}

void SetActivityAssets(napi_env env, napi_value assets, struct DiscordActivity* activity) {
  NAPI_HAS_PROPERTY_DO(assets, largeImage) {
    napi_value large_image;
    NAPI_REQUIRE(napi_get_named_property(env, assets, "largeImage", &large_image));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, large_image, activity->assets.large_image, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(assets, largeText) {
    napi_value large_text;
    NAPI_REQUIRE(napi_get_named_property(env, assets, "largeText", &large_text));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, large_text, activity->assets.large_text, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(assets, smallImage) {
    napi_value small_image;
    NAPI_REQUIRE(napi_get_named_property(env, assets, "smallImage", &small_image));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, small_image, activity->assets.small_image, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(assets, smallText) {
    napi_value small_text;
    NAPI_REQUIRE(napi_get_named_property(env, assets, "smallText", &small_text));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, small_text, activity->assets.small_text, 0, NULL));
  }
}

void SetActivityTimestamps(napi_env env, napi_value timestamps, struct DiscordActivity* activity) {
  NAPI_HAS_PROPERTY_DO(timestamps, startAt) {
    napi_value start_at;
    double start_timestamp;
    NAPI_REQUIRE(napi_get_named_property(env, timestamps, "startAt", &start_at));
    NAPI_REQUIRE(napi_get_date_value(env, start_at, &start_timestamp));
    activity->timestamps.start = (unsigned long)start_timestamp;
  }

  NAPI_HAS_PROPERTY_DO(timestamps, endAt) {
    napi_value end_at;
    double end_timestamp;
    NAPI_REQUIRE(napi_get_named_property(env, timestamps, "endAt", &end_at));
    NAPI_REQUIRE(napi_get_date_value(env, end_at, &end_timestamp));
    activity->timestamps.end = (unsigned long)end_timestamp;
  }
}

void SetActivitySecrets(napi_env env, napi_value secrets, struct DiscordActivity* activity) {
  NAPI_HAS_PROPERTY_DO(secrets, match) {
    napi_value match;
    NAPI_REQUIRE(napi_get_named_property(env, secrets, "match", &match));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, match, activity->secrets.match, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(secrets, join) {
    napi_value join;
    NAPI_REQUIRE(napi_get_named_property(env, secrets, "join", &join));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, join, activity->secrets.join, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(secrets, spectate) {
    napi_value spectate;
    NAPI_REQUIRE(napi_get_named_property(env, secrets, "spectate", &spectate));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, spectate, activity->secrets.spectate, 0, NULL));
  }
}

void SetActivityParty(napi_env env, napi_value party, struct DiscordActivity* activity) {
  NAPI_HAS_PROPERTY_DO(party, id) {
    napi_value id;
    NAPI_REQUIRE(napi_get_named_property(env, party, "id", &id));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, id, activity->party.id, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(party, currentSize) {
    napi_value current_size;
    int32_t current_size_int32;
    NAPI_REQUIRE(napi_get_named_property(env, party, "currentSize", &current_size));
    NAPI_REQUIRE(napi_get_value_int32(env, current_size, &current_size_int32));
    activity->party.size.current_size = current_size_int32;
  }

  NAPI_HAS_PROPERTY_DO(party, maxSize) {
    napi_value max_size;
    int32_t max_size_int32;
    NAPI_REQUIRE(napi_get_named_property(env, party, "maxSize", &max_size));
    NAPI_REQUIRE(napi_get_value_int32(env, max_size, &max_size_int32));
    activity->party.size.max_size = max_size_int32;
  }
}

napi_value UpdateActivity(napi_env env, napi_callback_info info) {
  void* data;
  AddonState* state;
  size_t argc = 1;
  napi_value settings;

  NAPI_REQUIRE(napi_get_cb_info(env, info, &argc, &settings, NULL, &data));
  state = data;

  struct DiscordActivity activity;
  memset(&activity, 0, sizeof(activity));

  NAPI_HAS_PROPERTY_DO(settings, details) {
    napi_value details;
    NAPI_REQUIRE(napi_get_named_property(env, settings, "details", &details));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, details, activity.details, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(settings, state) {
    napi_value state;
    NAPI_REQUIRE(napi_get_named_property(env, settings, "state", &state));
    NAPI_REQUIRE(napi_get_value_string_utf8(env, state, activity.state, 0, NULL));
  }

  NAPI_HAS_PROPERTY_DO(settings, assets) {
    napi_value assets;
    NAPI_REQUIRE(napi_get_named_property(env, settings, "assets", &assets));
    SetActivityAssets(env, assets, &activity);
  }

  NAPI_HAS_PROPERTY_DO(settings, timestamps) {
    napi_value timestamps;
    NAPI_REQUIRE(napi_get_named_property(env, settings, "timestamps", &timestamps));
    SetActivityTimestamps(env, timestamps, &activity);
  }

  NAPI_HAS_PROPERTY_DO(settings, secrets) {
    napi_value secrets;
    NAPI_REQUIRE(napi_get_named_property(env, settings, "secrets", &secrets));
    SetActivitySecrets(env, secrets, &activity);
  }

  NAPI_HAS_PROPERTY_DO(settings, party) {
    napi_value party;
    NAPI_REQUIRE(napi_get_named_property(env, settings, "party", &party));
    SetActivityParty(env, party, &activity);
  }

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
