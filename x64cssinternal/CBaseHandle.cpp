#include "CBaseHandle.h"
#include "c_entity_list.h"

i_handle_entity* CBaseHandle::Get() const
{
	return reinterpret_cast<i_handle_entity*>(i::entity_list->get_client_entity_from_handle(m_Index));
}