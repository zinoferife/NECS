#pragma once
#include "Enity.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <cassert>


class EnttManager
{
public:
	EnttManager();
	entity_t CreateEntity();
	void DestoryEntity(entity_t entt);
	void SetSignature(entity_t entt, signature_t signature);
	signature_t GetSignature(entity_t entt);
private:
	std::queue<entity_t> mAvailableID;
	std::vector<signature_t> mSignatures;
	entity_t mLivingEntityCount;
};

