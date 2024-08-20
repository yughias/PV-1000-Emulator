#include "pv1000.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pv1000_init(pv1000_t* pv1000, const char* filename, int sampleRate){
    memset(pv1000, 0x00, sizeof(pv1000_t));
    
    z80_t* z80 = &pv1000->z80;
    z80->readMemory = pv1000_readMemory;
    z80->writeMemory = pv1000_writeMemory;
    z80->readIO = pv1000_readIO;
    z80->writeIO = pv1000_writeIO;
    z80->master = pv1000;
    pv1000->psg.updateRate = 1.0f / sampleRate;

    z80_init(z80);

    FILE* fptr = fopen(filename, "r");
    if(!fptr){
        printf("can't open rom!\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    fseek(fptr, 0, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    memset(pv1000->memory, 0xFF, 0x1000);
    fread(pv1000->memory, 1, size, fptr);
}

void pv1000_runFrame(pv1000_t* pv1000){
    z80_t* z80 = &pv1000->z80;
    vdp_t* vdp = &pv1000->vdp;

    int next_interrupt = 196;

    while(z80->cycles < CYCLES_PER_FRAME){
        uint32_t old_cycles = z80->cycles;

        z80_step(z80);

        int old_lines = old_cycles / CYCLES_PER_LINE;
        int lines = z80->cycles / CYCLES_PER_LINE;

        if(old_lines != lines && lines == next_interrupt){
            if(next_interrupt == 196)
                pv1000->status |= 0b1;
            if(next_interrupt != 256)
                next_interrupt += 4;
            z80->INTERRUPT_PENDING = true;
        }
    }

    z80->cycles -= CYCLES_PER_FRAME;

    vdp_render(vdp, pv1000->memory);
}