#ifndef __DISCORD_GAME_PROMISE_H
#define __DISCORD_GAME_PROMISE_H

typedef struct {
  napi_env env;
  napi_deferred deferred;
  AddonState* state;
} Promise;

#endif
