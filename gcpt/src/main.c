#include <tamtypes.h>
#include <stdint.h>
#include <inttypes.h>

#include "include/rac2.h"
#include "include/timer.h"


#define defaultOffset (0xEF000)

// efficient memory usage fr
#define currentTime (*((u64*)0xEFDF4))
#define timerStarted (*((int*)defaultOffset + 0x20))
#define sugmaBalls (*((int*)defaultOffset + 0x30))
#define formatted_time_string ((char*)defaultOffset + 0x40)
#define textDrawingPtr ((char*)defaultOffset + 0x50)


typedef void (*GuiDrawTextExFunc)(int, int, u64, char*, int);



const uint64_t guiDrawTextExAddresses[] = {
    0x2e94c0,   // ARANOS_TUTORIAL
    0x2DEC08,   // OOZLA
    0x2EC950,   // MAKTAR
    0x2E9FC0,   // ENDAKO
    0x309C98,   // BARLOW
    0x301E90,   // FELTZIN
    0x328B20,   // NOTAK
    0x2DF498,   // SIBERIUS
    0x2F5508,   // TABORA
    0x2E5598,   // DOBBO
    0x304390,   // HRUGIS
    0x2FB2A0,   // JOBA
    0x2ECB50,   // TODANO
    0x2F3F28,   // BOLDAN
    0x2F0DC0,   // ARANOS_REVISIT
    0x302490,   // GORN
    0x2E2718,   // SNIVELAK
    0x2EA4F8,   // SMOLG
    0x303D08,   // DAMOSEL
    0x2E7180,   // GRELBIN
    0x2FD460,   // YEEDIL
    0x2F0570,   // MUSEUM
    0x2EF690,   // DOBBO_ORBIT
    0x2F3D70,   // DAMOSEL_ORBIT
    0x2E8458,   // SHIP_SHACK
    0x2FC810,   // WUPASH
    0x2E9AD8    // JAMMING_ARRAY
};




void drawText(int posX, int posY, char* message) {
        GuiDrawTextExFunc guiDrawTextEx = (GuiDrawTextExFunc)guiDrawTextExAddresses[current_planet];
        guiDrawTextEx(posX, posY, 0x80f0f0f0, message, -1);
}

u64 customMillis;

int main(void)
{ // bløah asd asd

    if(timerStarted == 0)
    {
        TimerInit();
        timerStarted = 1;
    }

        sprintf(formatted_time_string, "Current timer ticks: %" PRIu64, customMilliseconds);
        drawText(0x8, 0x185, formatted_time_string);
        
    }
    

	return 0;
}
