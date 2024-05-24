#include <tamtypes.h>
#include <stdint.h>
#include <inttypes.h>

#include "include/rac2.h"

// Default memory location in memory where we can store our defined variables. 
// Did it like this so that it can be easier to move around if we ever need to.
#define defaultOffset (0xEF000)

// Variables for keeping track of old values for load remover/autosplitter etc.
#define old_planet_timer_value (*((int*)defaultOffset + 1))
#define old_player_state (*((int*)defaultOffset + 2))
#define old_load_screen (*((int*)defaultOffset + 3))
#define load_screen_count (*((int*)defaultOffset + 4))
#define final_time (*((int*)defaultOffset + 5))
#define old_down_buttons (*((int*)defaultOffset + 6))
// Manual overrride for drawing timer, non-zero means hide
#define drawing_disable (*((int*)defaultOffset + 7))
// Value of frame timer stored from the frame when resetting
#define timer_offset (*((int*)defaultOffset + 8))
// Frames saved via load normalisation
#define load_norm (*((int*)defaultOffset + 9))
// Number of long loads counted, each one is 217 frames
#define long_loads (*((int*)defaultOffset + 10))
// Make sure this one is last
#define formatted_time_string ((char*)defaultOffset + 0x50)

// Every single planet has it's own guiDrawTextEx function, so we need to swap them out every time you load into a new level.
// It's important to not change the address for guiDrawText while the function is being called or else it'll crash.
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

void resetTimer() {
    // Reset timer on Aranos 1 spawn!
    // 98 is like some kinda walking animation idk
    if (current_planet == 0 && player_state == 98 && old_player_state != 98) {
        final_time = 0;
        timer_offset = global_timer;
    }
}

void processLongLoads() {
    // 0 Right-to-left (3.6, 218 frames)
    // 1 Curved (3.7, 226 frames)
    // 2 Left-to-right (3.6, 217 frames)
    // 3 Top-to-bottom (4.0, 238 frames)
    // 4 Planet loading screen
    if (load_screen_type != old_load_screen) {
        load_screen_count = load_screen_count + 1;

        // Load normalisation (ala ng+)
        // Add any extra frames to our timer offset as long the loading screen isn't LTR (fastest screen at 217 frames)
        if (load_screen_type == LOAD_RTL) {
            load_norm += 1;
        }
        if (load_screen_type == LOAD_CURVED) {
            load_norm += 9;
        }
        // if (old_load_screen == LOAD_LTR) // no offset
        if (load_screen_type == LOAD_TTB) { 
            load_norm += 21;
        }

        // Additionally, remove fully any loading screens after the second one.
        // Don't include final load screen in this timing
        if (load_screen_count > 2 && load_screen_type != LOAD_FINAL) {
            long_loads += 1;
        }

    };
}

void drawText(int posX, int posY, char* message) {
    // Additionally, this planet timer only counts up during gameplay.
    // Not displaying when it's frozen prevents drawing during loading screens and pauses.
    // Also check if planet_loading is 0, or else it'll crash on Aranos end, Jamming entry, Insomniac Museum teleporter etc.
    if ((planet_loading == 0 && planet_loading_oozla == 0) && strafe_timer != old_planet_timer_value) {
        GuiDrawTextExFunc guiDrawTextEx = (GuiDrawTextExFunc)guiDrawTextExAddresses[current_planet];
        guiDrawTextEx(posX, posY, 0x80f0f0f0, message, -1);
    }
}

void formatTime(int frames) {
    // Format our time string, can't use / or % operators for some reason so we need to use udivdi3 and umoddi3 functions from the game.
    unsigned long milliseconds, seconds, minutes, hours;

    milliseconds = udivdi3(frames * 1000, 60);

    seconds = udivdi3(milliseconds, 1000);
    milliseconds = umoddi3(milliseconds, 1000);

    minutes = udivdi3(seconds, 60);
    seconds = umoddi3(seconds, 60);

    hours = udivdi3(minutes, 60);
    minutes = umoddi3(minutes, 60);

    sprintf(formatted_time_string, "%u:%02u:%02u.%03u", hours, minutes, seconds, milliseconds);
}


int main(void)
{   
    // Don't run if we haven't loaded a savefile yet. 
    // Probably not needed, but it's safe. 0xFFFC is the default value for loaded_save_slot.
    if (loaded_save_slot == -3) return 0;

    // Process button combos, currently just hides/shows timer
    if (old_down_buttons != down_buttons && down_buttons == 15) {
        drawing_disable = !drawing_disable;
    }

    // Each normalised long load is 217 frames
    int total_offset = timer_offset + load_norm + (217 * long_loads);
    int curr_adjusted_time = global_timer - total_offset;

    // Get our final time when Protopet is killed.
    // Cutscene ID 6 is the protopet death cutscene
    if (current_planet == 20 && final_time == 0 && yeedil_scene == 6) {
        // 9 frames need to be subtracted for the time to be correct with fadeout.
        final_time = curr_adjusted_time - 9;
    }

    // Handle freezing on protopet by drawing final_time if it exists 
    if (final_time != 0) {
        formatTime(final_time);
        drawText(0x8, 0x150, formatted_time_string); 

        char buf[64];
        sprintf(buf, "%d Long Loads", long_loads);
        drawText(0x8, 0x100, buf); 
        sprintf(buf, "%d Load Normalised Frames");
        drawText(0x8, 0x125, formatted_time_string); 
    } else if (drawing_disable == 0) {
        formatTime(curr_adjusted_time);
        // Draw higher on ship missions to avoid blocking timer
        if (current_planet == WUPASH || current_planet == FELTZIN 
        || current_planet == HRUGIS || current_planet == GORN) {
            drawText(0x8, 0x150, formatted_time_string);
        } else {
            drawText(0x8, 0x185, formatted_time_string);
        }
    }
    
    resetTimer();
    processLongLoads();

    // Reset loading screen count if we just landed on a planet
    // Determined by planet-specific timer resetting
    if (strafe_timer < 5) {
        load_screen_count = 0;
    }

    // char funny[128];
    // sprintf(funny, "%d %d %d", player_state, loading_count, load_screen_type); 
    // drawText(0x8, 0x160, funny); 

    // Get previous state of variables for comparison.
    old_planet_timer_value = strafe_timer;
    old_player_state = player_state;
    old_load_screen = load_screen_type;
    old_down_buttons = down_buttons;
	return 0;
}
