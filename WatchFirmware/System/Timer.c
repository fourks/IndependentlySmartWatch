#include <string.h>
#include "hal_rtc.h"
#include "macro.h"
#include "Timer.h"

#define MAX_TIMERS			10

#define TIMER_ALLOCATED		0x01
#define TIMER_ACTIVE		0x02
#define TIMER_REPEATS		0x04

typedef struct {
	unsigned char flags;

	int delay, current;

	TimerCallback callback;
	void *context;
} TIMER_ENTRY;

TIMER_ENTRY Timer_List[MAX_TIMERS];

// Plumbing from the HAL

void Timer_Initialise(void)
{
	memset(Timer_List, 0, sizeof(Timer_List));
}

int Timer_Interrupt(int prescale)
{
	int i, ticks, activate, usePrescale;

	if (GetRTCPrescaleInterruptEnabled())
	{
		if (!prescale)
			return 0;
		ticks = 1;
	}
	else
	{
		if (prescale)
			return 0;
		ticks = RTC_TICKS_PER_SECOND;
	}
	activate = 0;
	usePrescale = 0;
	for (i = 0; i < MAX_TIMERS; i++)
	{
		if (!(Timer_List[i].flags & TIMER_ALLOCATED))
			continue;
		if (!(Timer_List[i].flags & TIMER_ACTIVE))
			continue;
		Timer_List[i].current -= ticks;
		if (Timer_List[i].current <= 0)
		{
			if (Timer_List[i].flags & TIMER_REPEATS)
				Timer_List[i].current = Timer_List[i].delay;
			else
				Timer_List[i].flags &= ~TIMER_ACTIVE;
			TimerCallback toCall = Timer_List[i].callback;
			activate |= toCall((TimerHandle*)(Timer_List + i), Timer_List[i].context);
		}
		else
		{
			if (Timer_List[i].current < 1000)
				usePrescale = 1;
		}
	}
	SetRTCPrescaleInterrupt(usePrescale);
	return activate;
}

// External API

TimerHandle Timer_Create(TimerCallback callback, void *context, int delay, int repeats)
{
	int i;
	TIMER_ENTRY *entry;

	entry = NULL;
	ENTER_CRITICAL_REGION_QUICK();
	for (i = 0; i < MAX_TIMERS; i++)
	{
		if (!(Timer_List[i].flags & TIMER_ALLOCATED))
		{
			entry = Timer_List + i;
			break;
		}
	}
	if (entry != NULL)
	{
		entry->flags |= TIMER_ALLOCATED;
		Timer_Change((TimerHandle*)entry, callback, context);
		Timer_Reset((TimerHandle*)entry, delay, repeats);
	}
	LEAVE_CRITICAL_REGION_QUICK();
	return entry;
}

void Timer_Reset(TimerHandle *timer, int delay, int repeats)
{
	unsigned long muldiv = delay;
	muldiv *= RTC_TICKS_PER_SECOND;
	muldiv /= 1000;
	ENTER_CRITICAL_REGION_QUICK();
	((TIMER_ENTRY*)timer)->delay = muldiv;
	if (repeats)
		((TIMER_ENTRY*)timer)->flags |= TIMER_REPEATS;
	else
		((TIMER_ENTRY*)timer)->flags &= ~TIMER_REPEATS;
	((TIMER_ENTRY*)timer)->current = ((TIMER_ENTRY*)timer)->delay;
	if (delay > 0)
	{
		((TIMER_ENTRY*)timer)->flags |= TIMER_ACTIVE;
		if (delay < 1000)
			SetRTCPrescaleInterrupt(1);
	}
	LEAVE_CRITICAL_REGION_QUICK();
}

void Timer_Change(TimerHandle *timer, TimerCallback callback, void *context)
{
	ENTER_CRITICAL_REGION_QUICK();
	((TIMER_ENTRY*)timer)->callback = callback;
	((TIMER_ENTRY*)timer)->context = context;
	LEAVE_CRITICAL_REGION_QUICK();
}

void Timer_Destroy(TimerHandle *timer)
{
	ENTER_CRITICAL_REGION_QUICK();
	((TIMER_ENTRY*)timer)->flags = 0;
	LEAVE_CRITICAL_REGION_QUICK();
}

TimerCallback Timer_GetCallback(TimerHandle *timer)
{
	return ((TIMER_ENTRY*)timer)->callback;
}

void* Timer_GetContext(TimerHandle *timer)
{
	return ((TIMER_ENTRY*)timer)->context;
}