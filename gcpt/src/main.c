#include <tamtypes.h>
#include <stdint.h>
#include <inttypes.h>

#include "include/rac2.h"
#include "include/timer.h"
#include "include/memory.h"

typedef void (*GuiDrawTextExFunc)(int, int, u64, char*, int, uint64_t, int*);

typedef u64 (*GetTextSpritesFunc)(u64);

const uint64_t guiDrawTextExAddresses[] = {
    0x2e9230,   // ARANOS_TUTORIAL
    0x2DE978,   // OOZLA
    0x2EC6C0,   // MAKTAR
    0x2E9D30,   // ENDAKO
    0x309A08,   // BARLOW
    0x301C00,   // FELTZIN
    0x328890,   // NOTAK
    0x2DF208,   // SIBERIUS
    0x2F5278,   // TABORA
    0x2E5308,   // DOBBO
    0x304100,   // HRUGIS
    0x2FB010,   // JOBA
    0x2EC8C0,   // TODANO
    0x2F3C98,   // BOLDAN
    0x2F0B30,   // ARANOS_REVISIT
    0x302200,   // GORN
    0x2E2488,   // SNIVELAK
    0x2EA268,   // SMOLG
    0x303A78,   // DAMOSEL
    0x2E6EF0,   // GRELBIN
    0x2FD1D0,   // YEEDIL
    0x2F02E0,   // MUSEUM
    0x2EF400,   // DOBBO_ORBIT
    0x2F3AE0,   // DAMOSEL_ORBIT
    0x2E81C8,   // SHIP_SHACK
    0x2FC580,   // WUPASH
    0x2E9848    // JAMMING_ARRAY
};

const uint64_t guiGetTextSpriteAddresses[] = {
    0x2e67a0,   // ARANOS_TUTORIAL
    0x2DBEE8,   // OOZLA
    0x2E9C30,   // MAKTAR
    0x2E72A0,   // ENDAKO
    0x306F78,   // BARLOW
    0x2FEF40,   // FELTZIN
    0x325E00,   // NOTAK
    0x2DC778,   // SIBERIUS
    0x2F2568,   // TABORA
    0x2E2828,   // DOBBO
    0x301440,   // HRUGIS
    0x2F8580,   // JOBA
    0x2E9E30,   // TODANO
    0x2F1208,   // BOLDAN
    0x2EE0A0,   // ARANOS_REVISIT
    0x2FF540,   // GORN
    0x2DF9F8,   // SNIVELAK
    0x2E77D8,   // SMOLG
    0x300FC8,   // DAMOSEL
    0x2E41C0,   // GRELBIN
    0x2FA720,   // YEEDIL
    0x2ED5D0,   // MUSEUM
    0x2EC970,   // DOBBO_ORBIT
    0x2F1050,   // DAMOSEL_ORBIT
    0x2E5738,   // SHIP_SHACK
    0x2F98C0,   // WUPASH
    0x2E6DB8    // JAMMING_ARRAY
};


void drawText(int posX, int posY, char* message) {
        u64 textSpriteSettings;

        GuiDrawTextExFunc guiDrawTextEx = (GuiDrawTextExFunc)guiDrawTextExAddresses[current_planet];
        GetTextSpritesFunc getTextSprite = (GetTextSpritesFunc)guiGetTextSpriteAddresses[current_planet];

        textSpriteSettings = getTextSprite(1);
        guiDrawTextEx(posX, posY, 0x80f0f0f0, message, -1, textSpriteSettings, 0x28c590);
}

int main(void)
{ // bløah

    if(timerStarted == 0)
    {
        TimerInit();
        timerStarted = 1;
    }

    if(timerStarted == 1) {
        formatTime(BusClkToMs(Timer()));
        drawText(0x8, 0x185, formatted_time_string);
        
    }
    

	return 0;
}
