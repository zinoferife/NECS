#include "ComponentManager.h"

ComponentManager::ComponentManager()
{
}

void ComponentManager::EnityDestoryed(entity_t entt)
{
	for (auto const& pair : mComponentArrays)
	{
		auto const& component = pair.second;
		component->EntityDestoryed(entt);
	}

}
