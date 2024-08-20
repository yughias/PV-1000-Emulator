#include "SDL_MAINLOOP.h"
#include "pv1000.h"

#include <string.h>

SDL_AudioDeviceID audioDev;

pv1000_t pv1000;

void closeEmulator(){
    SDL_CloseAudioDevice(audioDev);
}

void setup(){
    char* path = SDL_GetBasePath();

    size(SCREEN_WIDTH, SCREEN_HEIGHT);
    setScaleMode(NEAREST);
    setTitle("PV-1000");

    char logo_path[FILENAME_MAX];
    strncpy(logo_path, path, FILENAME_MAX - 1);
    strncat(logo_path, "data/logo.bmp", FILENAME_MAX - 1);
    setWindowIcon(logo_path);

    SDL_AudioSpec audioSpec;
    audioSpec.channels = 1;
    audioSpec.format = AUDIO_S8;
    audioSpec.freq = 44100;
    audioSpec.samples = 512;
    audioSpec.userdata = &pv1000.psg;
    audioSpec.callback = psg_callback;
    SDL_AudioDeviceID audioDev = SDL_OpenAudioDevice(NULL, 0, &audioSpec, &audioSpec, 0);

    char rom_path[FILENAME_MAX];
    strncpy(rom_path, getArgv(1), FILENAME_MAX - 1);

    pv1000_init(&pv1000, getArgv(1), audioSpec.freq);
    frameRate(REFRESH_RATE);

    onExit = closeEmulator;

    SDL_PauseAudioDevice(audioDev, 0);
}


void loop(){
    pv1000_runFrame(&pv1000);
}