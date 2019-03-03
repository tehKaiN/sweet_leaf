/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <sweet_leaf/custom.h>

// https://www.amigacoding.com/index.php/Amiga_memory_map

// This is where Amiga address space gets mapped. 4 * 64MB.
#define STM_MEM_BANK_1 0x60000000
#define CUSTOM_OFFSET 0xDF0000

tCustom *g_pCustom = (tCustom*)(STM_MEM_BANK_1 + CUSTOM_OFFSET);
uint8_t *g_pRomExt = (uint8_t*)(STM_MEM_BANK_1 + 0xF00000);
uint8_t *g_pRom512 = (uint8_t*)0xF80000;
uint8_t *g_pRom256 = (uint8_t*)0xFC0000;
