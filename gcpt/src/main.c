#include <tamtypes.h>
#include <stdint.h>
#include <inttypes.h>

#include "include/rac2.h"
#include "include/timer.h"
#include "include/memory.h"


void drawText(int posX, int posY, char* message) {
    u64 textSpriteSettings;
    if(game_state == 3)
        textSpriteSettings = getTextSprite(2);
    if(game_state == 0 || game_state == 4)
        textSpriteSettings = getTextSprite(1);
    
    // textSpriteSettings = getTextSprite(sugmaBalls);

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
        // sprintf(formatted_time_string, "Current timer ticks: %" PRIu64, timerMilliseconds);
        formatTime(BusClkToMs(Timer()));
        
        // draw_text_aranos(0x100, 0x91, 0x70808080, formatted_time_string, -1);

        drawText(0x8, 0x185, formatted_time_string);
        
    }
    

	return 0;
}
