#include "SystemManager.h"

SystemManager::SystemManager()
{
}

void SystemManager::EntitySignatureChange(entity_t entt, signature_t entt_signature)
{
	assert(entt < MAX_ENTITIES && "Not a valid entity");
	for (auto& sys : mSystems)
	{
		auto& type_name = sys.first;
		auto& system = sys.second;
		auto& signature = mSignatures[type_name];
		if ((entt_signature & signature) == signature)
		{
			system->mEntities.insert(entt);
		}
		else
		{
			system->mEntities.erase(entt);
		}
	}
}

void SystemManager::EntityDestoryed(entity_t entt)
{
	for (auto const& pair : mSystems)
	{
		auto const& system = pair.second;
		system->mEntities.erase(entt);
	}
}
