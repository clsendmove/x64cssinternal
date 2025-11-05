#pragma once
#include "memory.h"
#include "entities.h"
#include "CBaseHandle.h"

class c_client_entity_list {
public:
	virtual i_client_networkable* get_client_networkable(int ent_num) = 0;
	virtual i_client_networkable* get_client_networkable_from_handle(CBaseHandle ent) = 0;
	virtual i_client_unknown* get_client_unknown_from_handle(CBaseHandle ent) = 0;
	virtual i_client_entity* get_client_entity(int ent_num) = 0;
	virtual i_client_entity* get_client_entity_from_handle(CBaseHandle ent) = 0;
	virtual int                   number_of_entities(bool include_non_networked) = 0;
	virtual int                   get_highest_ent_index(void) = 0;
	virtual void                  get_max_entities(int max_ents) = 0;
	virtual int                   get_max_entities() = 0;
};

add_interface(entity_list, c_client_entity_list);
