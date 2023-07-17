#include "Manager.h"

// Constructora que reserva para entities 100 espacios de memoria
Manager::Manager() : handlers(), entitiesByGrp(), systems() {
	for (auto& grpEntities : entitiesByGrp) {
		grpEntities.reserve(100);
	}
}

// Destructora que elimina todas las entidades del vector de entidades
Manager::~Manager() {
	for (auto& ents : entitiesByGrp)
		for (auto e : ents) delete e;
	for (auto& sys : systems)
		delete sys;
}
