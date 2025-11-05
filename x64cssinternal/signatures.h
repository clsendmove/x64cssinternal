#include "memory.h"
#include "typedef.h"

namespace signatures {
	void init();

	inline QWORD global_vars; 
	inline QWORD validate_user_cmd;
	inline QWORD input;
	inline QWORD move_helper;
	inline QWORD random_seed;
}