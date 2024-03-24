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
