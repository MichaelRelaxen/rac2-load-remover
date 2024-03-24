/*      
 * The following Code is from uLaunchELF
 */

#include <tamtypes.h>
#include <kernel.h>
#include "include/timer.h"
#include "include/rac2.h"
#include "include/memory.h"

static int TimerInterruptID = -1;
static u64 TimerInterruptCount = 0;

/* Timer Interrupt */
int TimerInterrupt(int a)
{
	TimerInterruptCount++;
	*T0_MODE |= (1 << 11);
	return -1;
}

/* Timer Init */
void TimerInit(void)
{
	*T0_MODE = 0x0000;
	TimerInterruptID = AddIntcHandler(9, TimerInterrupt, 0);
	EnableIntc(9);
	*T0_COUNT = 0;
	*T0_MODE = ( 2 << 0 )+( 1 << 7 )+( 1 << 9 );
	TimerInterruptCount = 0;
}

/* Timer Count */
u64 Timer(void)
{
	u64 ticks = (*T0_COUNT + (TimerInterruptCount << 16));
	return ticks;
}

/* Timer End */
void TimerEnd(void)
{
	*T0_MODE = 0x0000;
	if (TimerInterruptID >= 0){
		DisableIntc(9);
		RemoveIntcHandler(9, TimerInterruptID);
		TimerInterruptID = -1;
	}
	TimerInterruptCount = 0;
}

u64 BusClkToMs(u64 cycles) {
    return udivdi3(cycles, 576);
}

void formatTime(unsigned long milliseconds) {
    unsigned long seconds, minutes, hours;

    seconds = udivdi3(milliseconds, 1000);
    milliseconds = umoddi3(milliseconds, 1000);

    minutes = udivdi3(seconds, 60);
    seconds = umoddi3(seconds, 60);

    hours = udivdi3(minutes, 60);
    minutes = umoddi3(minutes, 60);

    sprintf(formatted_time_string, "%u:%02u:%02u.%03u", hours, minutes, seconds, milliseconds);
}