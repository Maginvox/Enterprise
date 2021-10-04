/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_BITSET_H__
#define __ENTERPRISE_BITSET_H__

#include "Core/enTypes.h"

#define ENTERPRISE_BITMASK(b) (1 << ((b) % FCHAR_BIT))
#define ENTERPRISE_BITSLOT(b) ((b) / FCHAR_BIT)
#define ENTERPRISE_BITSET(a, b) ((a)[FBITSLOT(b)] |= FBITMASK(b))
#define ENTERPRISE_BITCLEAR(a, b) ((a)[FBITSLOT(b)] &= ~FBITMASK(b))
#define ENTERPRISE_BITTEST(a, b) ((a)[FBITSLOT(b)] & FBITMASK(b))
#define ENTERPRISE_BITNSLOTS(nb) ((nb + FCHAR_BIT - 1) / FCHAR_BIT)

#endif