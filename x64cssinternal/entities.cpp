#include "entities.h"

c_base_combat_weapon* c_base_entity::get_active_weapon() {
	return reinterpret_cast<c_base_combat_weapon*>(m_hActiveWeapon().Get());
}