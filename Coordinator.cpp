#include "Coordinator.h"

void Coordinator::Init()
{
	mEntityManager = std::make_unique<EnttManager>();
	mComponentManager = std::make_unique<ComponentManager>();
	mSystemManager = std::make_unique<SystemManager>();
}

entity_t Coordinator::CreateEntity()
{
	return mEntityManager->CreateEntity();
}

void Coordinator::DestoryEntity(entity_t entt)
{
	mEntityManager->DestoryEntity(entt);
	mComponentManager->EnityDestoryed(entt);
	mSystemManager->EntityDestoryed(entt);
}

