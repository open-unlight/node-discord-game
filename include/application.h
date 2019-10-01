#ifndef __DISCORD_GAME_APPLICATION_H
#define __DISCORD_GAME_APPLICATION_H

typedef struct {
  struct IDiscordCore* core;
  struct IDiscordUserManager* users;
  struct IDiscordActivityManager* activities;
  struct IDiscordRelationshipManager* relationships;
  struct IDiscordApplicationManager* application;
  DiscordUserId user_id;
} Application;

typedef struct {
  Application app;
  struct IDiscordUserEvents users_events;
  struct IDiscordActivityEvents activities_events;
  struct IDiscordRelationshipEvents relationships_events;
  bool initialized;
} AddonState;

napi_value Application_Init(napi_env env, AddonState* state);

#endif
