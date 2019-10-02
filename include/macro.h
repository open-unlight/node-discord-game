#ifndef __DISCORD_GAME_MACRO_H
#define __DISCORD_GAME_MACRO_H

// NAPI
#define NAPI_REQUIRE(x) assert(x == napi_ok)
#define NAPI_HAS_PROPERTY_DO(object, property) \
  bool has_##property; \
  NAPI_REQUIRE(napi_has_named_property(env, object, ""#property"", &has_##property)); \
  if(has_##property)

// DISCORD
#define DISCORD_REQUIRE(x) assert(x == DiscordResult_Ok)

#endif
