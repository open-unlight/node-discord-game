#ifndef __DISCORD_GAME_CORE_H
#define __DISCORD_GAME_CORE_H

typedef struct {
  struct IDiscordCore* core;
  struct IDiscordUserManager* users;
  struct IDiscordAchievementManager* achievements;
  struct IDiscordActivityManager* activities;
  struct IDiscordRelationshipManager* relationships;
  struct IDiscordApplicationManager* application;
  struct IDiscordLobbyManager* lobbies;
  DiscordUserId user_id;
} Application;

typedef struct {
  Application app;
  struct IDiscordUserEvents users_events;
  struct IDiscordActivityEvents activities_events;
  struct IDiscordRelationshipEvents relationships_events;
  bool initialized;
} AddonState;

#endif
