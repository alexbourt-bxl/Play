#ifndef TABLET_H
#define TABLET_H

#include <Events/Message.h>
#include <Containers/Array.h>
#include <Types/Geometric\Rect.h>

#include <windows.h>

#include "Wintab\wintab.h"

#define PACKETDATA	PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_TIME | PK_ORIENTATION | PK_CURSOR
#define PACKETMODE	PK_BUTTONS | PK_TIME

#include "Wintab\pktdef.h"

#ifdef USE_X_LIB
#include <wintabx.h>
#endif

#define TABLET_PACKET_QUEUE_SIZE	64

typedef UINT (API *WTINFO) (UINT, UINT, LPVOID);
typedef HCTX (API *WTOPEN) (HWND, LPLOGCONTEXT, BOOL);
typedef BOOL (API *WTCLOSE) (HCTX);
typedef int  (API *WTPACKETSGET) (HCTX, int, LPVOID);
typedef BOOL (API *WTPACKET) (HCTX, int, LPVOID);
typedef BOOL (API *WTENABLE) (HCTX, BOOL);
typedef BOOL (API *WTOVERLAP) (HCTX, BOOL);
typedef BOOL (API *WTGET) (HCTX, LPLOGCONTEXT);
typedef BOOL (API *WTSET) (HCTX, LPLOGCONTEXT);
typedef BOOL (API *WTQUEUESIZESET) (HCTX, int);
typedef BOOL (API *WTEXTGET) (HCTX, UINT, LPVOID);

class Tablet
{
public:
	class Message;

public:
	UINT	curStylusId;

	LONG	minPressure;
	LONG	maxPressure;

	int		packetQueueSize;

public:
	Tablet(HWND hwnd);
	~Tablet();

	HCTX	GetContext();

	void	Enable(bool enable);
	void	Overlap(bool top);

	__declspec(property(get = GetHasTilt))
	bool	HasTilt;
	bool	GetHasTilt();

	__declspec(property(get = GetInContext, put = SetInContext))
	bool	InContext;
	bool	GetInContext();
	void	SetInContext(bool inContext);

	__declspec(property(get = GetContextRect))
	Rect	ContextRect;
	Rect	GetContextRect();

	void	UpdateInfo();
	void	Flush();

	UINT	GetExtension(UINT extTag);

private:
	HCTX	m_htab;
	bool	m_inContext;
	Rect	m_contextRect;
	
	UINT	m_tiltExt;

public:
	static bool IsPresent();

	static bool LoadWintab();
	static void UnloadWintab();

	static WTINFO			dll_WTInfo;
	static WTOPEN			dll_WTOpen;
	static WTCLOSE			dll_WTClose;
	static WTPACKETSGET		dll_WTPacketsGet;
	static WTPACKET			dll_WTPacket;
	static WTENABLE			dll_WTEnable;
	static WTOVERLAP		dll_WTOverlap;
	static WTGET			dll_WTGet;
	static WTSET			dll_WTSet;
	static WTQUEUESIZESET	dll_WTQueueSizeSet;
	static WTEXTGET			dll_WTExtGet;

private:
	static HMODULE			s_hWintab;
};

class Tablet::Message
	: public ::Message
{
public:
	struct Point
	{
		uint  CursorId;
		double X;
		double Y;
		double Pressure;
		double Azimuth;
		double Elevation;
		double Twist;
		long   dTime; // Time elapsed sice the last packet (in milliseconds)
	};

	__declspec(property(get = GetPoints, put = SetPoints))
	Array<Point>& Points;
	Array<Point>& GetPoints() { return *m_points; }
	void SetPoints(Array<Point>& points) { m_points = &points; }

	Message() 
		: m_points(null) {}

	Message(Array<Point>& points)
		: m_points(&points) {}

private:
	Array<Point>* m_points;
};

#endif