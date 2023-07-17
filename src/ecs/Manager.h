#pragma once

#include <vector>
#include <array>
#include "ecs_def.h"
#include "System.h"
#include "Entity.h"
#include "../checkMemLeaks.h"


class Manager {
private:
	std::array<std::vector<Entity*>, ecs::maxGroupID> entitiesByGrp;	// Array of groups of entities
	std::array<Entity*, ecs::maxHandlerID> handlers;					// Array of handlers (specific entities)

	std::array<System*, ecs::maxSystemId> systems;	// Array of systems
	std::vector<ecs::Message> messages;				// Vector of all the messages
	std::vector<ecs::Message> aux_messages;			// Vector of pending messages

public:
	// Reserves space for 100 groups of entities
	Manager() : handlers(), entitiesByGrp(), systems() {
		for (auto& grpEntities : entitiesByGrp) {
			grpEntities.reserve(100);
		}
	}
	virtual ~Manager() {
		// Deletes every entity from every entity group in the array of groups
		for (auto& ents : entitiesByGrp)
			for (auto e : ents) delete e;

		// Deletes every system from the array of systems
		for (auto& sys : systems) delete sys;
	}



	// Creates an entity, sets it as alive and adds it to the specified group
	Entity* addEntity(ecs::grpId_type grID = ecs::_grp_GENERAL) {
		Entity* e = new Entity();
		e->alive = true;
		entitiesByGrp[grID].push_back(e);

		return e;
	};
	// Returns the group of entities of the specified group 
	const std::vector<Entity*>& getEntities(ecs::grpId_type grID = ecs::_grp_GENERAL) { return entitiesByGrp[grID]; };

	// Sets the specified entity (e) as a handler
	inline void setHandler(ecs::hdlrId_type hdlrID, Entity* e) { handlers[hdlrID] = e; }
	// Returns the entity set as the specified handler (hdlrID)
	inline Entity* getHandler(ecs::hdlrId_type hdlrID) { return handlers[hdlrID]; }


	// Creates a component of the specified class (T) with the 
	// arguments (Ts) and adds it to the specified entity (e)
	template<typename T, typename... Ts>
	inline T* addComponent(Entity* e, Ts&&... args) {
		T* comp = new T(std::forward<Ts>(args)...);

		// Gets the component id from the component's class
		constexpr ecs::compId_type compId = T::compId;
		// Removes the component from the entity just in case it existed before
		removeComponent<T>(e);
		// Adds the component to the entity's vector of components
		e->comps[compId] = comp;

		return comp;
	}
	

	// Removes the component of the specified 
	// class (T) from the specified entity (e)
	template<typename T>
	inline void removeComponent(Entity* e) {
		// Gets the component id from the component's class
		constexpr ecs::compId_type compId = T::compId;

		// If it exists in the entity's vector of components, it gets 
		// deleted and the component in the entity is now a nullptr
		if (e->comps[compId] != nullptr) {
			delete e->comps[compId];
			e->comps[compId] = nullptr;
		}
	}

	// Checks if the specified entity (e) has the component 
	// of the specified class (T) in its components vector
	template<typename T>
	inline bool hasComponent(Entity* e) {
		constexpr ecs::compId_type cId = T::compId;
		return e->comps[cId] != nullptr;
	}

	// Returns the component of the specified class (T)
	// from the specified entity (e)'s vector of components 
	template<typename T>
	inline T* getComponent(Entity* e) {
		constexpr ecs::compId_type cId = T::compId;
		return static_cast<T*>(e->comps[cId]);
	}

	// Changes the alive state of the specified entity (e)
	inline void setAlive(Entity* e, bool alive) { e->alive = alive; }

	// Checks if the specified entity (e) is alive or not
	inline bool isAlive(Entity* e) { return e->alive; }

	// Checks the group the specified entity (e) belongs to
	inline ecs::grpId_type groupId(Entity* e) { return e->gId; }



	// Creates a system of the specified class (T) with the arguments (Ts) 
	// and adds it to the manager's (this) array of systems
	template<typename T, typename... Ts>
	inline T* addSystem(Ts&&... args) {
		System* s = new T(std::forward<Ts>(args)...);

		// Gets the system id from the systems's class
		constexpr ecs::sysId_type sId = T::sysId;

		// Removes the system from the manager just in case it existed before
		removeSystem<T>();

		// Initializes the system
		s->setContext(this);
		s->initSystem();
		
		// Adds the system to the array of systems
		systems[sId] = s;

		return static_cast<T*>(s);
	}

	// Returns the system of the specified class (T) from the array of systems 
	template<typename T>
	inline T* getSystem() {
		constexpr ecs::sysId_type sId = T::compId;
		return static_cast<T*>(systems[sId]);
	}

	// Removes the system of the specified class (T) from the array of systems 
	template<typename T>
	inline void removeSystem() {
		constexpr ecs::sysId_type sId = T::sysId;
		if (systems[sId] != nullptr) {
			delete systems[sId];
			systems[sId] = nullptr;
		}
	}


	// Sends a message (m) to all of the systems with a 
	// specified delay (only some of them will process it)
	inline void send(const ecs::Message& m, bool delay = false) {
		// If there's no delay, the message is send immediately
		if (!delay) {
			for (System* s : systems)
				if (s != nullptr) s->receive(m);
		}
		// Otherwise, it gets added to the pending messages
		else messages.emplace_back(m);
	}

	// Sends the pending messages
	inline void flushMessages() {
		// Swaps the messages vector content for the aux vector (which should be empty at the beginning)
		// this way, the messages vector ends up empty and the new messages are added to it
		swap(messages, aux_messages);

		// Sends every pending message (m) from the aux vector to all of the systems
		for (auto& m : aux_messages)
			for (System* s : systems)
				if (s != nullptr) s->receive(m);

		// Clears the aux vector because all pending messages have been sent
		aux_messages.clear();
	}



	void update(float frameTime) {
		for (auto sys : systems) {
			if (sys != nullptr)
				sys->update(frameTime);
		}
	}


	// Checks each entity from each group in the array of groups 
	// and removes it if the entity's alive is set to false
	void refresh() {
		for (ecs::grpId_type grID = 0; grID < ecs::maxGroupID; grID++) {
			auto& grpEnts = entitiesByGrp[grID];

			grpEnts.erase(
				remove_if(grpEnts.begin(), grpEnts.end(), [](Entity* e) {
					if (e->alive) return false;
					else {
						delete e;
						return true;
					}
				}), grpEnts.end()
			);
		}


	}

};