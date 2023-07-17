#pragma once

// Components only have variables and no methods.
// This could be implemented in the Entity class but 
// files are separated to distinguish components from entities
struct Component {
	virtual ~Component() {};
};





