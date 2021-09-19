/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FBITSET_H__
#define __FBITSET_H__

#include "FTypes.h"

#define FBITMASK(b) (1 << ((b) % FCHAR_BIT))
#define FBITSLOT(b) ((b) / FCHAR_BIT)
#define FBITSET(a, b) ((a)[FBITSLOT(b)] |= FBITMASK(b))
#define FBITCLEAR(a, b) ((a)[FBITSLOT(b)] &= ~FBITMASK(b))
#define FBITTEST(a, b) ((a)[FBITSLOT(b)] & FBITMASK(b))
#define FBITNSLOTS(nb) ((nb + FCHAR_BIT - 1) / FCHAR_BIT)

#endif