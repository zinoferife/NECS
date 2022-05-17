#include "EnttManager.h"

EnttManager::EnttManager()
:mLivingEntityCount(0){
	for (int entt = 0; entt < MAX_ENTITIES; entt++)
	{
		mAvailableID.push(entt);
	}
	mSignatures.resize(MAX_ENTITIES);
}

entity_t EnttManager::CreateEntity()
{
	assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in exisitance");
	entity_t id = mAvailableID.front();
	mAvailableID.pop();
	++mLivingEntityCount;
	return id;
}

void EnttManager::DestoryEntity(entity_t entt)
{
	assert(entt < MAX_ENTITIES && "Entity out of  range.");
	mSignatures[entt].reset();
	mAvailableID.push(entt);
	--mLivingEntityCount;
}

void EnttManager::SetSignature(entity_t entt, signature_t signature)
{
	assert(entt < MAX_ENTITIES && "Entitiy out of range.");
	mSignatures[entt] = signature;
}

signature_t EnttManager::GetSignature(entity_t entt)
{
	assert(entt < MAX_ENTITIES && "Entity is out of range.");
	return mSignatures[entt];
}


