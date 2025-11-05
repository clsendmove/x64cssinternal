#include "engine_pred.h"
#include "MD5.h"

int c_engine_pred::GetTickbase(c_user_cmd* pCmd, c_base_entity* pLocal)
{
	static int nTick = 0;
	static c_user_cmd* pLastCmd = nullptr;

	if (pCmd)
	{
		if (!pLastCmd || pLastCmd->hasbeenpredicted)
			nTick = pLocal->m_nTickBase();

		else nTick++;

		pLastCmd = pCmd;
	}

	return nTick;
}
#undef max
#undef min


void c_engine_pred::start(c_user_cmd* pCmd)
{
	c_base_entity* pLocal = i::entity_list->get_client_entity(i::engine_client->get_local_player())->as<c_base_entity>();

	memset(&m_MoveData, 0, sizeof(c_move_data));

	if (pLocal && pLocal->is_alive() && i::move_helper) {
		m_fOldCurrentTime = i::global_vars->curtime;
		m_fOldFrameTime = i::global_vars->frametime;
		m_nOldTickCount = i::global_vars->tickcount;

		const int nOldTickBase = pLocal->m_nTickBase();
		const bool bOldIsFirstPrediction = i::prediction->m_bFirstTimePredicted;
		const bool bOldInPrediction = i::prediction->m_bInPrediction;

		i::move_helper->set_host(pLocal);

		pLocal->m_pCurrentCommand() = pCmd;

		*i::random_seed = MD5_PseudoRandom(pCmd->command_number) & INT_MAX;

		i::global_vars->curtime = TICKS_TO_TIME(GetTickbase(pCmd, pLocal));
		i::global_vars->frametime = (i::prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL);
		i::global_vars->tickcount = GetTickbase(pCmd, pLocal);

		i::prediction->m_bFirstTimePredicted = false;
		i::prediction->m_bInPrediction = true;

		i::prediction->SetLocalViewAngles(pCmd->viewangles);

		i::prediction->SetupMove(pLocal, pCmd, i::move_helper, &m_MoveData);
		i::game_movement->process_movement(pLocal, &m_MoveData);
		i::prediction->FinishMove(pLocal, pCmd, &m_MoveData);

		i::move_helper->set_host(nullptr);

		pLocal->m_nTickBase() = nOldTickBase;
		i::prediction->m_bInPrediction = bOldInPrediction;
		i::prediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
	}
}


void c_engine_pred::end(c_user_cmd* pCmd) {
	c_base_entity* pLocal = i::entity_list->get_client_entity(i::engine_client->get_local_player())->as<c_base_entity>();

	if (pLocal && pLocal->is_alive() && i::move_helper)
	{
		pLocal->m_pCurrentCommand() = nullptr;

		i::global_vars->curtime = m_fOldCurrentTime;
		i::global_vars->frametime = m_fOldFrameTime;
		i::global_vars->tickcount = m_nOldTickCount;

		*i::random_seed = -1;
	}
}