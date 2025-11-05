#pragma once
#include "typedef.h"

class CSaveRestoreData;

#define TICK_INTERVAL		( i::global_vars->interval_per_tick )
#define TIME_TO_TICKS( dt )	( static_cast<int>( (TICK_INTERVAL / 2) + static_cast<float>(dt) / TICK_INTERVAL ) )
#ifndef TICKS_TO_TIME
#define TICKS_TO_TIME( t )	( TICK_INTERVAL * ( t ) )
#endif

class c_globals
{
public:
	float			realtime;
	int				framecount;
	float			absoluteframetime;
	float			curtime;
	float			frametime;
	int				maxClients;
	int				tickcount;
	float			interval_per_tick;
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;
	CSaveRestoreData* pSaveData;
	bool			m_bClient;
	int				nTimestampNetworkingBase;
	int				nTimestampRandomizeWindow;
};

add_interface(global_vars, c_globals);