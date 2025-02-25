/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_ECS_SYSTEM_MANAGER_H
#define DR_ECS_SYSTEM_MANAGER_H

#include <unordered_map>
#include "System.h"


//####################################################################################
//##    DrSystemManager
//##        Keeps track of Systems within Coordinator (ECS World)
//############################
class DrSystemManager
{
	// #################### VARIABLES ####################
private:
	std::unordered_map<HashID, std::shared_ptr<DrSystem>> 	m_systems 		{ };	// Current Systems in System Manager
	std::unordered_map<HashID, Archetype>     				m_archetypes 	{ };	// Archetypes of Systems


	// #################### INTERNAL FUNCTIONS ####################
public:
	// Adds a System to the System Manager
	template<typename T>
	std::shared_ptr<T> registerSystem() {
		HashID hash = typeid(T).hash_code();
		assert(m_systems.find(hash) == m_systems.end() && "Registering system more than once!");

		// Otherwise, add system to manager
		auto system = std::make_shared<T>();
		m_systems.insert({hash, system});
		return system;
	}

	// Sets Archetype (a bitset based on desired Components) of a System
	template<typename T>
	void setArchetype(Archetype archetype) {
		HashID hash = typeid(T).hash_code();
		assert(m_systems.find(hash) != m_systems.end() && "System used before being registered!");
		m_archetypes.insert({hash, archetype});
	}

	// Entity has been destroyed, remove from all Systems
	void entityDestroyed(EntityID entity) {
		for (auto const& system_pair : m_systems) {
			auto const& system = system_pair.second;
			system->m_entities.erase(entity);
		}
	}

	// Check all Systems for Entity
	//		If Entity has necessay components of System (shares Archetype), make sure that Entity is included in that System
	//		Otherwise remove the Entity from that System
	void entityArchetypeChanged(EntityID entity, Archetype entity_archetype) {
		for (auto const& system_pair : m_systems) {
			auto const& type =   system_pair.first;
			auto const& system = system_pair.second;
			auto const& system_archetype = m_archetypes[type];

			if ((entity_archetype & system_archetype) == system_archetype) {
				system->m_entities.insert(entity);
			} else {
				system->m_entities.erase(entity);
			}
		}
	}

};


#endif	// DR_ECS_SYSTEM_MANAGER_H
