#include "interfaces.h"

class c_engine_pred {
private:
	c_move_data m_MoveData = { };
	float m_fOldCurrentTime = 0.0f;
	float m_fOldFrameTime = 0.0f;
	int m_nOldTickCount = 0;
	int GetTickbase(c_user_cmd* pCmd, c_base_entity* pLocal);
public:
	void start(c_user_cmd* pCmd);
	void end(c_user_cmd* pCmd);
};

add_feature_(engine_pred, c_engine_pred);
