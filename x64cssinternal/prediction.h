#pragma once
#include "memory.h"
#include "entities.h"
#include "c_user_cmd.h"
#include "CBaseHandle.h"

class c_move_helper;
typedef CBaseHandle EntityHandle_t;

class c_move_data {
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;

	EntityHandle_t	m_nPlayerHandle;	// edict index on server, client entity handle on client

	int				m_nImpulseCommand;	// Impulse command issued.
	vec3			m_vecViewAngles;	// Command view angles (local space)
	vec3			m_vecAbsViewAngles;	// Command view angles (world space)
	int				m_nButtons;			// Attack buttons.
	int				m_nOldButtons;		// From host_client->oldbuttons;
	float			m_flForwardMove;
	float			m_flOldForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	vec3			m_vecVelocity;		// edict::velocity		// Current movement direction.
	vec3			m_vecAngles;		// edict::angles
	vec3			m_vecOldAngles;

	// Output only
	float			m_outStepHeight;	// how much you climbed this move
	vec3			m_outWishVel;		// This is where you tried 
	vec3			m_outJumpVel;		// This is your jump velocity

	// Movement constraints	(radius 0 means no constraint)
	vec3			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	vec3			m_vecAbsOrigin;		// edict::origin
};

class c_game_movement {
public:
	void process_movement(c_base_entity* player, c_move_data* move) {
		typedef void(__thiscall* fn)(void*, c_base_entity*, c_move_data*);
		memory::get_vfunc<fn>(this, 1)(this, player, move);
	}
};

/*
class c_prediction {
public:
	void setup_move(c_base_entity* player, c_user_cmd* cmd, c_move_helper* mvhlpr, c_move_data* move) {
		typedef void(__thiscall* fn)(void*, c_base_entity*, c_user_cmd*, c_move_helper*, c_move_data*);
		// (0x1804570D8 - 0x180457048) / 8
		// = 18

		memory::get_vfunc<fn>(this, 18)(this, player, cmd, mvhlpr, move);
	}

	void finish_move(c_base_entity* player, c_user_cmd* cmd, c_move_data* move) {
		typedef void(__thiscall* fn)(void*, c_base_entity*, c_user_cmd*, c_move_data*);
		memory::get_vfunc<fn>(this, 19)(this, player, cmd, move);
	}

	void set_local_view_angles(vec3& angle) {
		typedef void(__thiscall* fn)(void*, vec3&);
		memory::get_vfunc<fn>(this, 16)(this, angle);
	}
};*/

class i_prediction
{
public:
	virtual			~i_prediction(void) {};

	virtual void	Init(void) = 0;
	virtual void	Shutdown(void) = 0;

	// Run prediction
	virtual void
		Update
		(
			int startframe,				// World update ( un-modded ) most recently received
			bool validframe,			// Is frame data valid
			int incoming_acknowledged,	// Last command acknowledged to have been run by server (un-modded)
			int outgoing_command		// Last command (most recent) sent to server (un-modded)
		) = 0;

	// We are about to get a network update from the server.  We know the update #, so we can pull any
	//  data purely predicted on the client side and transfer it to the new from data state.
	virtual void	PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	PostEntityPacketReceived(void) = 0;
	virtual void	PostNetworkDataReceived(int commands_acknowledged) = 0;

	virtual void	OnReceivedUncompressedPacket(void) = 0;

	// The engine needs to be able to access a few predicted values
	virtual void	GetViewOrigin(vec3& org) = 0;
	virtual void	SetViewOrigin(vec3& org) = 0;
	virtual void	GetViewAngles(vec3& ang) = 0;
	virtual void	SetViewAngles(vec3& ang) = 0;
	virtual void	GetLocalViewAngles(vec3& ang) = 0;
	virtual void	SetLocalViewAngles(vec3& ang) = 0;
};


class c_prediction : public i_prediction
{
	// Construction
public:
	virtual			~c_prediction(void) = 0;

	virtual void	Init(void) = 0;
	virtual void	Shutdown(void) = 0;

	// Implement IPrediction
public:

	virtual void	Update
	(
		int startframe,		// World update ( un-modded ) most recently received
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;

	virtual void	OnReceivedUncompressedPacket(void) = 0;

	virtual void	PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	PostEntityPacketReceived(void) = 0;
	virtual void	PostNetworkDataReceived(int commands_acknowledged) = 0;

	virtual bool	InPrediction(void) const = 0;
	virtual bool	IsFirstTimePredicted(void) const = 0;

#if !defined( NO_ENTITY_PREDICTION )
	virtual int		GetIncomingPacketNumber(void) const = 0;
#endif

	// The engine needs to be able to access a few predicted values
	virtual void	GetViewOrigin(vec3& org) = 0;
	virtual void	SetViewOrigin(vec3& org) = 0;
	virtual void	GetViewAngles(vec3& ang) = 0;
	virtual void	SetViewAngles(vec3& ang) = 0;

	virtual void	GetLocalViewAngles(vec3& ang) = 0;
	virtual void	SetLocalViewAngles(vec3& ang) = 0;

	virtual void	RunCommand(c_base_entity* player, c_user_cmd* ucmd, c_move_helper* moveHelper) = 0;

	// Internal
	virtual void	SetupMove(c_base_entity* player, c_user_cmd* ucmd, c_move_helper* pHelper, c_move_data* move) = 0;
	virtual void	FinishMove(c_base_entity* player, c_user_cmd* ucmd, c_move_data* move) = 0;
	virtual void	SetIdealPitch(c_base_entity* player, const vec3& origin, const vec3& angles, const vec3& viewheight) = 0;

	virtual void	_Update
	(
		bool received_new_world_update,
		bool validframe,		// Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command	// Last command (most recent) sent to server (un-modded)
	) = 0;

public:
	// Last object the player was standing on
	CHandle< c_base_entity >		m_hLastGround;

	bool						m_bInPrediction;
	bool						m_bFirstTimePredicted;
	bool						m_bOldCLPredictValue;
	bool						m_bEnginePaused;
};



add_interface(game_movement, c_game_movement);
add_interface(prediction, c_prediction);
add_interface(random_seed, int);