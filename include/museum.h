#pragma once

#include <iostream>
#include <vector>
#include <fmon.h>
#include "zlib.h"
#include "estream.h"
#include "types.h"
#include "g03.h"

namespace museum {
/* g01: galaga
 * g02: rally-x
 * g03: pac-n-roll
 * g04: gator-panic
 * g07: pac motos
 * g10: title
 * g11: grobda
 */

/* save stuff */
constexpr int SAVE_SIZE = 49156;

constexpr int GALAGA_OFF = 0x30;
constexpr int GALAGA_COMPLETE_OFF = 0x450;

constexpr int RALLYX_OFF = 0x2028;

constexpr int PAC_OFF = 0x401c;
constexpr int PAC_HP_OFF = 0x4004;
constexpr int PAC_LIVES_OFF = 0x4008;
constexpr int PAC_COMPLETE_OFF = 0x400c;

constexpr int GATOR_OFF = 0x6004;

constexpr int MOTOS_OFF = 0x8020;
constexpr int MOTOS_LIVES_OFF = 0x8004;
constexpr int MOTOS_POWER_OFF = 0x845c;
constexpr int MOTOS_JUMP_OFF = 0x8460;
constexpr int MOTOS_CHARGE_OFF = 0x8464;

struct GalagaSave {
    u32 shot;
    u32 unused1;
    u32 accuracy; // (float)accuracy / 10
    u32 hp;
    u32 score;
    u32 shotBadge;
    u32 accuracyBadge;
    u32 scoreBadge;
    u32 hpBadge;
    u32 flag;
    u32 unused2;
};

struct RallyXSave {
    u32 time;
    u32 score; // 0xffffffff marks redmode
    u32 scoreBadge;
    u32 timeBadge;
    u32 flag;
};

struct PacSave {
    u32 time;
    u32 unused1;
    u32 collected;
    u32 max;
    u32 unused2;
    u32 timeBadge;
    u32 ghostBadge;
    u32 collectedBadge;
    u32 flag;
};

struct GatorSave {
    u32 flag;
    u32 unused;
    u32 score;
};

struct MotosSave {
    u32 score;
    u32 time;
    u32 scoreBadge;
    u32 timeBadge;
    u32 unused;
    u32 flag;
};
}
