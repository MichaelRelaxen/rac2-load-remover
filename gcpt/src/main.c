#include <tamtypes.h>
#include <stdint.h>
#include <inttypes.h>

#include "include/rac2.h"

// Default memory location in memory where we can store our defined variables. 
// Did it like this so that it can be easier to move around if we ever need to.
#define defaultOffset (0xEF000)

// Variables for keeping track of old values for load remover/autosplitter etc.
#define old_scene (*((short*)defaultOffset - 4))
#define old_planet_timer_value (*((int*)defaultOffset + 1))
#define old_player_state (*((int*)defaultOffset + 2))
#define old_load_screen (*((int*)defaultOffset + 3))
#define old_global_timer (*((int*)defaultOffset + 4))
#define load_screen_count (*((int*)defaultOffset + 5))
#define final_time (*((int*)defaultOffset + 6))
#define old_down_buttons (*((int*)defaultOffset + 7))
#define old_planet (*((int*)defaultOffset + 8))
// Manual overrride for drawing timer, non-zero means hide
#define drawing_disable (*((int*)defaultOffset + 9))
// Value of frame timer stored from the frame when resetting
#define timer_offset (*((int*)defaultOffset + 10))
// Frames saved via load normalisation
#define load_norm (*((int*)defaultOffset + 11))
// Number of long loads counted, each one is 217 frames
#define long_loads (*((int*)defaultOffset + 12))
// Number of "black screen"(?) frames when our code doesn't run
#define black_frames (*((int*)defaultOffset + 13))
// Variable for how many splits have been saved.
#define saved_splits (*((int*)defaultOffset + 14))


#define formatted_time_string ((char*)defaultOffset - 0x60)

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
    if (current_planet == 0 && old_player_state == 98) {
        timer_offset = global_timer - 33; // 33 frames after player state is set to 98. this sucks actually but idc LOL
        final_time = 0;
        load_norm = 0;
        long_loads = 0;
        black_frames = 0;
        saved_splits = 0;
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

// Count frozen screens where our code isn't running 
void processFrozenScreens() {
    if (global_timer - old_global_timer > 10) {
        // One frame of difference usually, more means it froze for a bit
        black_frames += global_timer - old_global_timer - 1;
    }
}

int getGameState() {
    // value of original game state offset seems to always change to these bytes, so just check if those bytes exist to put oozla game state instead.
    if(main_game_state == 0x10000004) return oozla_game_state;
    else return main_game_state;
}

void drawText(int posX, int posY, char* message) {
    // Additionally, this planet timer only counts up during gameplay.
    // Not displaying when it's frozen prevents drawing during loading screens and pauses.
    // Also check if planet_loading is 0, or else it'll crash on Aranos end, Jamming entry, Insomniac Museum teleporter etc.
    if ((planet_loading == 0 && planet_loading_oozla == 0) && getGameState() != 3) {
        GuiDrawTextExFunc guiDrawTextEx = (GuiDrawTextExFunc)guiDrawTextExAddresses[current_planet]; // -0x878 for guiDrawTextEx, -0x2d20 for some function that gets font?
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

    sprintf(formatted_time_string, "%u:%02u:%02u.%03u\xf!%d", hours, minutes, seconds, milliseconds, black_frames);
}

int curr_adjusted_time, total_offset, timer_height;

struct SavedSplits {
    int saved_split;
    char saved_planet;
    int split_time;
};

    struct SavedSplits *splits = (struct SavedSplits *)0xE0000;

void addSplit() {
    // Formatting in the game TBD.
    // Save our splits at memory address 0xE0000.

    // Split on FeaR_SR's timing method.
    // Probably should clear memory on Aranos need to find memset address.
    if(current_planet_alt_ofs != old_planet) {
        saved_splits += 1;
        splits[saved_splits - 1].saved_split = saved_splits;
        splits[saved_splits - 1].saved_planet = old_planet;
        splits[saved_splits - 1].split_time = curr_adjusted_time;
    }
}

int main(void)
{   
    // Process button combos, currently just hides/shows timer
    if (old_down_buttons != down_buttons && down_buttons == 15) {
        drawing_disable = !drawing_disable;
    }

    // Each normalised long load is 217 frames
    total_offset = timer_offset + load_norm + (217 * long_loads) + black_frames;
    curr_adjusted_time = global_timer - total_offset;

    // Get our final time when Protopet is killed.
    // Cutscene ID 6 is the protopet death cutscene
    if (current_planet == 20 && final_time == 0 && current_scene == 6) {
        // 9 frames need to be subtracted for the time to be correct with fadeout.
        final_time = curr_adjusted_time - 16;
    }
    // Handle freezing on protopet by drawing final_time if it exists 
    if (final_time != 0) {
        formatTime(final_time);
        timer_height = 0x150;

    } else if (drawing_disable == 0) {
        formatTime(curr_adjusted_time);

        timer_height = 0x185;
        // Draw higher on ship missions to avoid blocking timer
        if (current_planet == WUPASH || current_planet == FELTZIN 
        || current_planet == HRUGIS || current_planet == GORN) {
            timer_height = 0x150;
        }
    }
    if (drawing_disable == 0 || final_time != 0)
    {
        drawText(0x8, timer_height, formatted_time_string);
    }
    
    resetTimer();
    processFrozenScreens();
    processLongLoads();
    // addSplit();

    // Reset loading screen count if we just landed on a planet
    // Determined by planet-specific timer resetting
    if (strafe_timer < 5) {
        load_screen_count = 0;
    }
    // Get previous state of variables for comparison.
    old_planet_timer_value = strafe_timer;
    old_global_timer = global_timer;
    old_player_state = player_state;
    old_load_screen = load_screen_type;
    old_down_buttons = down_buttons;
    old_scene = current_scene;
    old_planet = current_planet_alt_ofs;

	return 0;
}
