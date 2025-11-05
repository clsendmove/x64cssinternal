#include "dt_common.h"
#include "dt_recv.h"



inline int String(const char* szOrg)
{
	// why
	int iHash = 5381;

	while (int iStr = *szOrg++)
		iHash = iHash * 33 + iStr;

	return iHash;
}


#define HASH_RT(str) String(str)

class CHelpers_NetVarManager
{
public:
	int Get(const char* const szClass, const char* const szNetVar);

private:
	int GetOffset(RecvTable* pTable, const char* const szNetVar);
};

namespace u { inline CHelpers_NetVarManager netvars; }