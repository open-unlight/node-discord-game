#include "discord_game.h"

static void Destroy(napi_env env, void* data, void* hint) {
  if (!data) {
    return;
  }

  AddonState* state = (AddonState*)data;
  Application *app = &state->app;

  if (state->initialized && app->core) {
    app->core->destroy(app->core);
  }

  free(state);
}

napi_value RunCallback(napi_env env, napi_callback_info info) {
  void *data;
  AddonState *state;
  napi_value ret;

  NAPI_REQUIRE(napi_get_cb_info(env, info, 0, NULL, NULL, &data));
  state = data;

  if (!state->initialized) {
    NAPI_REQUIRE(napi_get_boolean(env, false, &ret));
    return ret;
  }

  enum EDiscordResult result;
  result = state->app.core->run_callbacks(state->app.core);

  if (result != DiscordResult_Ok) {
    NAPI_REQUIRE(napi_get_boolean(env, false, &ret));
    return ret;
  }

  NAPI_REQUIRE(napi_get_boolean(env, true, &ret));
  return ret;
}

napi_value Create(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value argv[2];
  napi_value ret;
  void* data;
  AddonState *state;

  NAPI_REQUIRE(napi_get_cb_info(env, info, &argc, argv, NULL, &data));

  char c;
  bool is_required;
  char raw_app_id[64];
  int64_t app_id;
  NAPI_REQUIRE(napi_get_value_string_utf8(env, argv[0], raw_app_id, 0, NULL));
  NAPI_REQUIRE(napi_get_value_bool(env, argv[1], &is_required));

  int app_id_convert_success = sscanf(raw_app_id, "%" SCNi64 "%c", &app_id, &c);
  if (!app_id_convert_success) {
    NAPI_REQUIRE(napi_get_boolean(env, false, &ret));
    return ret;
  }

  state = data;
  memset(&state->app, 0, sizeof(state->app));
  memset(&state->users_events, 0, sizeof(state->users_events));
  memset(&state->activities_events, 0, sizeof(state->activities_events));
  memset(&state->relationships_events, 0, sizeof(state->relationships_events));

  // Register Events
  // TODO
  // state->users_events.on_current_user_update = OnUserUpdated;
  // state->relationships_events.on_refresh = OnRelationshipsRefresh;

  Application *app = &state->app;


  struct DiscordCreateParams params;
  DiscordCreateParamsSetDefault(&params);
  params.client_id = (DiscordClientId)app_id;
  params.flags = is_required ? DiscordCreateFlags_Default : DiscordCreateFlags_NoRequireDiscord;
  params.event_data = state;
  params.activity_events = &state->activities_events;
  params.relationship_events = &state->relationships_events;
  params.user_events = &state->users_events;

  enum EDiscordResult result;
  result = DiscordCreate(DISCORD_VERSION, &params, &app->core);

  if (result != DiscordResult_Ok) {
    NAPI_REQUIRE(napi_get_boolean(env, false, &ret));
    return ret;
  }

  app->users = app->core->get_user_manager(app->core);
  app->achievements = app->core->get_achievement_manager(app->core);
  app->activities = app->core->get_activity_manager(app->core);
  app->application = app->core->get_application_manager(app->core);
  app->lobbies = app->core->get_lobby_manager(app->core);
  app->relationships = app->core->get_relationship_manager(app->core);

  state->initialized = true;

  NAPI_REQUIRE(napi_get_boolean(env, true, &ret));
  return ret;
}

napi_value Init(napi_env env, napi_value exports)
{
  // Setup Module State
  AddonState* state = malloc(sizeof(*state));
  state->initialized = false;
  napi_wrap(env, exports, state, Destroy, NULL, NULL);

  // Setup Objects
  napi_value version,
             application,
             activity;
  NAPI_REQUIRE(napi_create_uint32(env, DISCORD_VERSION, &version));
  application = Application_Init(env, state);
  activity = Activity_Init(env, state);

  napi_property_descriptor desc[] = {
    { "version", NULL, NULL, NULL, NULL, version, napi_default, NULL },
    { "create", NULL, Create, NULL, NULL, NULL, napi_default, state },
    { "runCallback", NULL, RunCallback, NULL, NULL, NULL, napi_default, state },
    { "Application", NULL, NULL, NULL, NULL, application, napi_default, NULL },
    { "Activity", NULL, NULL, NULL, NULL, activity, napi_default, NULL }
  };

  NAPI_REQUIRE(napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
