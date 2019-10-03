#ifndef __DISCORD_GAME_H
#define __DISCORD_GAME_H

// Standard Library
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "discord_game_sdk.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>
#endif

// Node NAPI
#include <node_api.h>

// Discord Game
#include "discord_game_sdk.h"

// Discord Game
#include "core.h"
#include "promise.h"
#include "application.h"
#include "activity.h"
#include "macro.h"

#endif
