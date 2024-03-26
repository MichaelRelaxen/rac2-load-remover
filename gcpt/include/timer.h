// syscalls from the game to make the timer work
#define AddIntcHandler ((s32 (*)(s32, s32 (*)(s32), s32))0x11a900)
#define RemoveIntcHandler ((void (*)(int, int))0x11a920)
#define EnableIntc ((void (*)(int))0x11a960)
#define DisableIntc ((void (*)(int))0x11a970)

/* fn prototypes */
void TimerInit(void);
u64 Timer(void);
void TimerEnd(void);

u64 BusClkToMs(u64 cycles);
void formatTime(unsigned long milliseconds);

/* Timer Define */
#define T0_COUNT ((volatile unsigned long*)0x10000000)
#define T0_MODE  ((volatile unsigned long*)0x10000010)
#define T0_COMP      ((volatile unsigned long*)0x10000020)
#define T0_HOLD      ((volatile unsigned long*)0x10000030)

#define T1_COUNT   ((volatile unsigned long*)0x10000800)
#define T1_MODE      ((volatile unsigned long*)0x10000810)
#define T1_COMP      ((volatile unsigned long*)0x10000820)
#define T1_HOLD      ((volatile unsigned long*)0x10000830)

// Note! T2 and T3 don't have a Tn_HOLD register!
// ----------------------------------------------
#define T2_COUNT   ((volatile unsigned long*)0x10001000)
#define T2_MODE      ((volatile unsigned long*)0x10001010)
#define T2_COMP      ((volatile unsigned long*)0x10001020)

#define T3_COUNT   ((volatile unsigned long*)0x10001800)
#define T3_MODE      ((volatile unsigned long*)0x10001810)
#define T3_COMP      ((volatile unsigned long*)0x10001820)
