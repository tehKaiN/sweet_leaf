/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SWEET_LEAF_CUSTOM_H_
#define _SWEET_LEAF_CUSTOM_H_

#include <stdint.h>

typedef struct _tAudioChannel {
	volatile uint16_t *ac_ptr;   ///< ptr to start of waveform data
	volatile uint16_t ac_len;	   ///< length of waveform in words
	volatile uint16_t ac_per;	   ///< sample period
	volatile uint16_t ac_vol;	   ///< volume
	volatile uint16_t ac_dat;	   ///< sample pair
	volatile uint16_t ac_pad[2]; ///< unused
} tAudioChannel;

typedef struct _tSpriteDef {
	volatile uint16_t pos;
	volatile uint16_t ctl;
	volatile uint16_t dataa;
	volatile uint16_t datab;
} tSpriteDef;

typedef struct _tCustom {
    volatile uint16_t bltddat;
    volatile uint16_t dmaconr;
    volatile uint16_t vposr;
    volatile uint16_t vhposr;
    volatile uint16_t dskdatr;
    volatile uint16_t joy0dat;
    volatile uint16_t joy1dat;
    volatile uint16_t clxdat;
    volatile uint16_t adkconr;
    volatile uint16_t pot0dat;
    volatile uint16_t pot1dat;
    volatile uint16_t potinp;
    volatile uint16_t serdatr;
    volatile uint16_t dskbytr;
    volatile uint16_t intenar;
    volatile uint16_t intreqr;
    volatile void *kpt;
    volatile uint16_t dsklen;
    volatile uint16_t dskdat;
    volatile uint16_t refptr;
    volatile uint16_t vposw;
    volatile uint16_t vhposw;
    volatile uint16_t copcon;
    volatile uint16_t serdat;
    volatile uint16_t serper;
    volatile uint16_t potgo;
    volatile uint16_t joytest;
    volatile uint16_t strequ;
    volatile uint16_t strvbl;
    volatile uint16_t strhor;
    volatile uint16_t strlong;
    volatile uint16_t bltcon0;
    volatile uint16_t bltcon1;
    volatile uint16_t bltafwm;
    volatile uint16_t bltalwm;
    volatile void *bltcpt;
    volatile void *bltbpt;
    volatile void *bltapt;
    volatile void *bltdpt;
    volatile uint16_t bltsize;
    volatile uint8_t pad2d;
    volatile uint8_t bltcon0l;	/* low 8 bits of bltcon0, write only */
    volatile uint16_t bltsizv;
    volatile uint16_t bltsizh;	/* 5e */
    volatile uint16_t bltcmod;
    volatile uint16_t bltbmod;
    volatile uint16_t bltamod;
    volatile uint16_t bltdmod;
    volatile uint16_t pad34[4];
    volatile uint16_t bltcdat;
    volatile uint16_t bltbdat;
    volatile uint16_t bltadat;
    volatile uint16_t pad3b[3];
    volatile uint16_t deniseid;	/* 7c */
    volatile uint16_t dsksync;
    volatile uint32_t cop1lc;
    volatile uint32_t cop2lc;
    volatile uint16_t copjmp1;
    volatile uint16_t copjmp2;
    volatile uint16_t copins;
    volatile uint16_t diwstrt;
    volatile uint16_t diwstop;
    volatile uint16_t ddfstrt;
    volatile uint16_t ddfstop;
    volatile uint16_t dmacon;
    volatile uint16_t clxcon;
    volatile uint16_t intena;
    volatile uint16_t intreq;
    volatile uint16_t adkcon;
    tAudioChannel aud[4];
    volatile void *bplpt[8];
    volatile uint16_t bplcon0;
    volatile uint16_t bplcon1;
    volatile uint16_t bplcon2;
    volatile uint16_t bplcon3;
    volatile uint16_t bpl1mod;
    volatile uint16_t bpl2mod;
    volatile uint16_t bplcon4;
    volatile uint16_t clxcon2;
    volatile uint16_t bpldat[8];
    volatile void *sprpt[8];
    tSpriteDef spr[8];
    volatile uint16_t   color[32];
    volatile uint16_t htotal;
    volatile uint16_t hsstop;
    volatile uint16_t hbstrt;
    volatile uint16_t hbstop;
    volatile uint16_t vtotal;
    volatile uint16_t vsstop;
    volatile uint16_t vbstrt;
    volatile uint16_t vbstop;
    volatile uint16_t sprhstrt;
    volatile uint16_t sprhstop;
    volatile uint16_t bplhstrt;
    volatile uint16_t bplhstop;
    volatile uint16_t hhposw;
    volatile uint16_t hhposr;
    volatile uint16_t beamcon0;
    volatile uint16_t hsstrt;
    volatile uint16_t vsstrt;
    volatile uint16_t hcenter;
    volatile uint16_t diwhigh;	/* 1e4 */
    volatile uint16_t padf3[11];
    volatile uint16_t fmode;
} tCustom;

extern tCustom *g_pCustom;
extern uint8_t *g_pRomExt;
extern uint8_t *g_pRom512;
extern uint8_t *g_pRom256;

#endif // _SWEET_LEAF_CUSTOM_H_
