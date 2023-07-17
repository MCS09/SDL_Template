#pragma once

#include "../checkMemLeaks.h"
#include <unordered_map>
#include "ecs_def.h"
#include "Component.h"
class Manager;

class Entity {
private:
	friend Manager;
	ecs::grpId_type gId;	// Group the entity belongs to

	bool alive;	// To check if entity is alive or not
	std::unordered_map<ecs::compId_type, Component*> comps;	// Components the entity has

public:
	Entity(ecs::grpId_type gId = ecs::_grp_GENERAL) : alive(), comps(), gId(gId) { }

	// To avoid copying entities
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	virtual ~Entity() {  };

};