#include "FreeRTOS.h"
#include "System.h"
#include "Graphics.h"
#include "Resources/MetaWatch_Large_16pt.h"
#include "Resources/MetaWatch_Large_8pt.h"
#include "Resources/MetaWatch_Small_8pt.h"
#include "Resources/neko.h"
/*
void* operator new(size_t size)
{
	return pvPortMalloc(size);
}

void* operator new[](size_t size)
{
	return operator new(size);
}

void operator delete(void *p)
{
	vPortFree(p);
}

void operator delete[](void *p)
{
	operator delete (p);
}
*/
void InitialiseSystem(void)
{
	GraphicsInit();

	FrameBuffer->Begin();
	FrameBuffer->Circle(96/2, 96/2, 50, true);
	FrameBuffer->Line(0, 0, 95, 95, true);
	FrameBuffer->Line(0, 95, 95, 0, false);
	int x = MetaWatch_Large_8pt.Print(FrameBuffer, 0, 0, "T", -1);
	MetaWatch_Small_8pt.Print(FrameBuffer, x, MetaWatch_Large_8pt.Baseline() - MetaWatch_Small_8pt.Baseline(), "ESTING", -1);
	MetaWatch_Large_16pt.Print(FrameBuffer, 5, MetaWatch_Large_8pt.Height(), "Hello!", -1);
	FrameBuffer->Blit(&neko, 50, MetaWatch_Large_8pt.Height() + MetaWatch_Large_16pt.Height() + 5, BLIT_INVERT);
	FrameBuffer->End();
}