#pragma once
#include "i_client_entity.h"
#include "netvars.h"
#include "CBaseHandle.h"
#include "enums.h"
#include "c_user_cmd.h"

#define NETVAR(_name, type, table, name) inline type &_name() \
{ \
	static const int nOffset = u::netvars.Get(table, name); \
	return *reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(this) + nOffset); \
}
#define NETVAR_OFFSET(_name, type, table, name, off) inline type &_name() \
{ \
	static const int nOffset = u::netvars.Get(table, name) + off; \
	return *reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(this) + nOffset); \
}

class c_base_entity;
class c_base_combat_weapon;
typedef CHandle<c_base_entity> EHANDLE;

class c_base_entity : public i_client_entity {
public:
	NETVAR(m_flAnimTime, int, "CBaseEntity", "m_flAnimTime");
	NETVAR(m_flSimulationTime, float, "CBaseEntity", "m_flSimulationTime");
	NETVAR(m_ubInterpolationFrame, int, "CBaseEntity", "m_ubInterpolationFrame");
	NETVAR(m_vecOrigin, vec3, "CBaseEntity", "m_vecOrigin");
	NETVAR(m_angRotation, vec3, "CBaseEntity", "m_angRotation");
	NETVAR(m_nModelIndex, int, "CBaseEntity", "m_nModelIndex");
	NETVAR(m_fEffects, int, "CBaseEntity", "m_fEffects");
	NETVAR(m_nRenderMode, int, "CBaseEntity", "m_nRenderMode");
	NETVAR(m_nRenderFX, int, "CBaseEntity", "m_nRenderFX");
	NETVAR(m_clrRender, int, "CBaseEntity", "m_clrRender");
	NETVAR(m_iTeamNum, int, "CBaseEntity", "m_iTeamNum");
	NETVAR(m_CollisionGroup, int, "CBaseEntity", "m_CollisionGroup");
	NETVAR(m_flElasticity, float, "CBaseEntity", "m_flElasticity");
	NETVAR(m_flShadowCastDistance, float, "CBaseEntity", "m_flShadowCastDistance");
	NETVAR(m_hOwnerEntity, EHANDLE, "CBaseEntity", "m_hOwnerEntity");
	NETVAR(m_hEffectEntity, EHANDLE, "CBaseEntity", "m_hEffectEntity");
	NETVAR(moveparent, EHANDLE, "CBaseEntity", "moveparent");
	NETVAR(m_iParentAttachment, int, "CBaseEntity", "m_iParentAttachment");
	NETVAR(m_Collision, void*, "CBaseEntity", "m_Collision");
	NETVAR(m_vecMinsPreScaled, vec3, "CBaseEntity", "m_vecMinsPreScaled");
	NETVAR(m_vecMaxsPreScaled, vec3, "CBaseEntity", "m_vecMaxsPreScaled");
	NETVAR(m_vecMins, vec3, "CBaseEntity", "m_vecMins");
	NETVAR(m_vecMaxs, vec3, "CBaseEntity", "m_vecMaxs");
	NETVAR(m_nSolidType, int, "CBaseEntity", "m_nSolidType");
	NETVAR(m_usSolidFlags, unsigned short, "CBaseEntity", "m_usSolidFlags");
	NETVAR(m_nSurroundType, int, "CBaseEntity", "m_nSurroundType");
	NETVAR(m_triggerBloat, int, "CBaseEntity", "m_triggerBloat");
	NETVAR(m_bUniformTriggerBloat, bool, "CBaseEntity", "m_bUniformTriggerBloat");
	NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMinsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxsPreScaled");
	NETVAR(m_vecSpecifiedSurroundingMins, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMins");
	NETVAR(m_vecSpecifiedSurroundingMaxs, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxs");
	NETVAR(m_iTextureFrameIndex, int, "CBaseEntity", "m_iTextureFrameIndex");
	NETVAR(m_PredictableID, int, "CBaseEntity", "m_PredictableID");
	NETVAR(m_bIsPlayerSimulated, bool, "CBaseEntity", "m_bIsPlayerSimulated");
	NETVAR(m_bSimulatedEveryTick, bool, "CBaseEntity", "m_bSimulatedEveryTick");
	NETVAR(m_bAnimatedEveryTick, bool, "CBaseEntity", "m_bAnimatedEveryTick");
	NETVAR(m_bAlternateSorting, bool, "CBaseEntity", "m_bAlternateSorting");
	NETVAR(m_nModelIndexOverrides, void*, "CBaseEntity", "m_nModelIndexOverrides");
	NETVAR(m_lifeState, unsigned char, "CBasePlayer", "m_lifeState");
	NETVAR(m_nHitboxSet, int, "CBaseAnimating", "m_nHitboxSet");
	NETVAR(m_nTickBase, int, "CBasePlayer", "m_nTickBase");
	NETVAR(m_hActiveWeapon, EHANDLE, "CBaseCombatCharacter", "m_hActiveWeapon");
	// add more netvars if you wanna
public:
	inline c_user_cmd*& m_pCurrentCommand()
	{
		static const int nOffset = (u::netvars.Get("CBasePlayer", "m_hConstraintEntity") - sizeof(uintptr_t));
		return *reinterpret_cast<c_user_cmd**>(reinterpret_cast<DWORD_PTR>(this) + nOffset);
	}

	inline bool is_alive() {
		return (m_lifeState() == LIFE_ALIVE);
	}

	c_base_combat_weapon* get_active_weapon();
};


class c_base_combat_weapon : public c_base_entity {
public:
	float get_spread()
	{
		return memory::get_vfunc<float(__thiscall*)(void*)>(this, 383)(this); 
	}
	float get_cone()
	{
		return memory::get_vfunc<float(__thiscall*)(void*)>(this, 382)(this);
	}

	void update_accuracy_penalty() {
		typedef void(__thiscall* fn)(void*);
		memory::get_vfunc<fn>(this, 384)(this);
	}

	int GetWeaponID() {
		//v22 = (*(__int64(__fastcall**)(__int64*))(*a1 + 2968))(a1);
		// 2968 / 8
		// = 371
		// most likely..
		typedef int(__thiscall* fn)(void*);
		return memory::get_vfunc<fn>(this, 371)(this);
	}
};