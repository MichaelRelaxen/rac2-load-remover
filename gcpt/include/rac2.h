// -----
// FUNCTIONS
// -----

// Used for formatting strings.
// Arguments: destination, formatting, ...
#define sprintf ((void (*)(char*, char*, ...))0x0115da8)

// Gets current PS2 clock time.
// Arguments: destination
#define sceCdReadClock ((int (*)(unsigned int*))0x1256d8)

// Division and modulation functions taken from the game, since it appears to not compile as is using the normal methods. IDK LOL
// Arguments: value, value to divide/modulate by
#define udivdi3 ((u64 (*)(u64, u64))0x121b20)
#define umoddi3 ((u32 (*)(u32, s32))0x1220f0)

// Creates a text popup.
#define createPopup ((void(*)(char*, uint64_t))0x30dd70)


// -----
// VARIABLES
// -----

// Appears to be some sort of global frametimer that starts on boot.
#define global_timer (*((int*)0x1A7208))

// If planet is loading or not going from any level that isn't Oozla. Can be set to 1 manually to force a load.
#define planet_loading (*((short*)0x1A8ED4))

// If planet is loading or not, only going from Oozla. Can be set to 1 manually to force a load.
#define planet_loading_oozla (*((short*)0x1A8F14))

// Currently loaded savefile, by default set to 0xFFFC on boot, set to 0xFFFF when quit/memory card pulled out/autosave disabled.
// Usually set between 0-3 to indicate which savefile is loaded.
#define loaded_save_slot (*((short*)0x1393F8))

// Amount frames since last strafe. Pauses on pause menu and during loads.
#define strafe_timer (*((int*)0x18A008))

#define planet_timer_or_something_who_cares (*((int*)0x18B0B0))

// current bolt count.
#define bolt_count (*((int*)0x1A79F8))

// current raritanium count.
#define raritanium_count (*((int*)0x1A79FC))

// current armor equipped.
#define current_armor (*((int*)0x1A7A18))

// current position of ratchet.
#define player_pos (*((vec4*)0x189EA0))

// number of challenge modes you have. 1 byte.
#define challenge_mode (*((char*)0x1A7A0A))

// current planet
#define current_planet (*((char*)0x1A79F0))

// The buttons that are currently pressed.
#define down_buttons (*((unsigned short*)0x138320))

// The buttons that were pressed this frame.
#define pressed_buttons (*((unsigned short*)0x138324))

// The buttons that were released this frame.
#define released_buttons (*((unsigned short*)0x138328))

// Ratchet's movement state
#define player_state (*((unsigned int*)0x18C0B4))

// Loading screen count, first screen if 0, second 1 etc.
#define loading_count (*((unsigned int*)0x152C68))

// Loading screen type.
// 0 Right-to-left (3.6)
// 1 Curved (3.7)
// 2 Left-to-right (3.6)
// 3 Top-to-bottom (4.0)
// 4 Planet loading screen
#define LOAD_RTL 0
#define LOAD_CURVED 1
#define LOAD_LTR 2
#define LOAD_TTB 3
#define LOAD_FINAL 4
#define load_screen_type (*((unsigned int*)0x152C64))

// Address for protopet health untuned. Possible that it may change upon chunk getting reloaded or level reload, but needs more testing.
#define protopet_health (*((float*)0xDB5E60))

// Address for gamestate on Yeedil. 
#define main_game_state (*((unsigned int*)0x1A8F00))
#define oozla_game_state (*((unsigned int*)0x1A8F40))

// Current cutscene ID that's playing. May only be used on Yeedil.
#define yeedil_scene (*((char*)0x1A6414))

// -----
// buttons
// -----

#define PAD_LEFT            0x8000
#define PAD_DOWN            0x4000
#define PAD_RIGHT           0x2000
#define PAD_UP              0x1000
#define PAD_START           0x0800
#define PAD_R3              0x0400
#define PAD_L3              0x0200
#define PAD_SELECT          0x0100
#define PAD_SQUARE          0x0080
#define PAD_CROSS           0x0040
#define PAD_CIRCLE          0x0020
#define PAD_TRIANGLE        0x0010
#define PAD_R1              0x0008
#define PAD_L1              0x0004
#define PAD_R2              0x0002
#define PAD_L2              0x0001

enum Planets {
    ARANOS_TUTORIAL,
    OOZLA,
    MAKTAR,
    ENDAKO,
    BARLOW,
    FELTZIN,
    NOTAK,
    SIBERIUS,
    TABORA,
    DOBBO,
    HRUGIS,
    JOBA,
    TODANO,
    BOLDAN,
    ARANOS_REVISIT,
    GORN,
    SNIVELAK,
    SMOLG,
    DAMOSEL,
    GRELBIN,
    YEEDIL,
    MUSEUM,
    DOBBO_ORBIT,
    DAMOSEL_ORBIT,
    SHIP_SHACK,
    WUPASH,
    JAMMING_ARRAY
};

// -----
// types
// -----

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;