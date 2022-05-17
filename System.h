#pragma once
#include <vector>
#include <set>
#include "Enity.h"
//base class for all systems
class System
{
public:
	friend class SystemManager;
	virtual ~System() = default; 

protected:
	std::set<entity_t> mEntities;
};

