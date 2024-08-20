#ifndef __PV_1000_H__
#define __PV_1000_H__

#include "z80.h"
#include "vdp.h"
#include "psg.h"
#include "controller.h"

#include <stdint.h>
#include <stdbool.h>

#define REFRESH_RATE 59.9227434033
#define CLOCK_RATE 3579545
#define CYCLES_PER_FRAME 59736
#define CYCLES_PER_LINE 228
#define VSYNC_CYCLE (CYCLES_PER_LINE*256)

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 192

typedef struct pv1000_t
{
    z80_t z80;
    vdp_t vdp;
    psg_t psg;
    controller_t controller;
    uint8_t memory[0x10000];

    uint8_t status;
} pv1000_t;

void pv1000_init(pv1000_t* pv1000, const char* filename, int sampleRate);
void pv1000_runFrame(pv1000_t* pv1000);

#endif