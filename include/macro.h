#ifndef __DISCORD_GAME_MACRO_H
#define __DISCORD_GAME_MACRO_H

// NAPI
#define NAPI_REQUIRE(x) assert(x == napi_ok)

// DISCORD
#define DISCORD_REQUIRE(x) assert(x == DiscordResult_Ok)

#endif
