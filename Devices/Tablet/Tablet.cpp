#include "Tablet.h"

#include <stdio.h>

Tablet::Tablet(HWND hwnd)
{
	curStylusId = 1;

	m_inContext = false;

	// Get default context
	
	LOGCONTEXT logContext;
	dll_WTInfo(WTI_DEFCONTEXT, 0, &logContext);

	// Modify the digitizing region

	swprintf_s(
		logContext.lcName, 
		sizeof(logContext.lcName) / sizeof(wchar_t), 
		L"Play_Tablet_0x%p", hwnd);
	
	logContext.lcOptions  |= CXO_SYSTEM | CXO_MESSAGES | CXO_CSRMESSAGES;
	logContext.lcPktData   = PACKETDATA;
	logContext.lcPktMode   = PACKETMODE;
	logContext.lcMoveMask  = PACKETDATA;
	logContext.lcBtnUpMask = logContext.lcBtnDnMask;

	// Open the region

	m_htab = dll_WTOpen(hwnd, &logContext, TRUE);

	UpdateInfo();

	// Set the packet queue size

	packetQueueSize = TABLET_PACKET_QUEUE_SIZE;

	while (packetQueueSize >= 32 
		&& dll_WTQueueSizeSet(m_htab, packetQueueSize)) 
		packetQueueSize -= 16;
}

Tablet::~Tablet()
{
	dll_WTClose(m_htab);
}

HCTX Tablet::GetContext()
{
	return m_htab;
}

void Tablet::Enable(bool enable)
{
	dll_WTEnable(m_htab, enable ? TRUE : FALSE);
}

void Tablet::Overlap(bool top)
{
	dll_WTOverlap(m_htab, top ? TRUE : FALSE);
}

bool Tablet::GetHasTilt()
{
	// TODO:
	return true;
}

bool Tablet::GetInContext()
{
	return m_inContext;
}

void Tablet::SetInContext(bool inContext)
{
	m_inContext = inContext;
}

Rect Tablet::GetContextRect()
{
	return m_contextRect;
}

void Tablet::UpdateInfo()
{
	LOGCONTEXT logContext;

	dll_WTGet(m_htab, &logContext);

	// Use the tablet's set resolution

	logContext.lcOutOrgX = 0;
	logContext.lcOutOrgY = 0;
	logContext.lcOutExtX = logContext.lcInExtX;
	logContext.lcOutExtY = logContext.lcInExtY;

	dll_WTSet(m_htab, &logContext);

	m_contextRect = Rect(
		logContext.lcOutOrgX,
		logContext.lcOutOrgY,
		logContext.lcOutExtX,
		logContext.lcOutExtY);

	// Get the tablet's pressure range

	AXIS pressure;

	dll_WTInfo(WTI_DEVICES, DVC_NPRESSURE, &pressure);

	minPressure = pressure.axMin;
	maxPressure = pressure.axMax;
}

void Tablet::Flush()
{
	dll_WTPacket(m_htab, 0, null);
}

UINT Tablet::GetExtension(UINT extTag)
{
	UINT scanTag;

	for (int i = 0; dll_WTInfo(WTI_EXTENSIONS + i, EXT_TAG, &scanTag); i++) 
	{
		if (extTag == scanTag)
			return i;
	}

	return 0xFFFF;
}

bool Tablet::IsPresent()
{
	if (!dll_WTInfo)
		return false;

	UINT oldErrorMode = ::SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);

	BOOL result = dll_WTInfo(0, 0, null);
	::SetErrorMode(oldErrorMode);

	return result != FALSE;
}