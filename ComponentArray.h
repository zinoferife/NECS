#pragma once
#include <vector>
#include <array>
#include <unordered_map>
#include <type_traits>
#include  "Enity.h"
#include <cassert>

class component_array_t
{
public:
	virtual ~component_array_t() = default;
	virtual void EntityDestoryed(entity_t entt) = 0;

};

template<typename T>
class ComponentArray : public component_array_t
{
public:
	typedef T component_type;
	ComponentArray();
	virtual ~ComponentArray();
	
	void InsertData(entity_t entt, T component);
	void RemoveData(entity_t entt);
	virtual void EntityDestoryed(entity_t entt) override;
	T& GetData(entity_t entt);
private:
	std::size_t mSize;
	std::array<T, MAX_ENTITIES> mComponentArray;
	std::unordered_map<entity_t, size_t> mEnttToIndex;
	std::unordered_map<size_t, entity_t> mIndexToEntt;
};

template<typename T>
inline ComponentArray<T>::ComponentArray()
: mSize(0){
}

template<typename T>
inline ComponentArray<T>::~ComponentArray()
{
}

template<typename T>
inline void ComponentArray<T>::InsertData(entity_t entt, T component)
{
	assert(mSize < MAX_ENTITIES && "Component array full");
	assert(entt < MAX_ENTITIES && "Entity out of range.");
	assert(mEnttToIndex.find(entt) == mEnttToIndex.end() && "Component already added to entity");
	const std::size_t newIndex = mSize;
	mEnttToIndex[entt] = newIndex;
	mIndexToEntt[newIndex] = entt;
	mComponentArray[newIndex] = component;
	
	++mSize;
}

template<typename T>
inline void ComponentArray<T>::RemoveData(entity_t entt)
{
	auto entt_iter = mEnttToIndex.find(entt);
	assert(entt_iter != mEnttToIndex.end() && "Removing non-existent component");
	const size_t index_of_rm_entt = entt_iter->second;
	const size_t index_of_last_elm = mSize - 1;
	//over engineering lol
	if constexpr (std::is_move_assignable_v<T>)
	{
		mComponentArray[index_of_rm_entt] = std::move(mComponentArray[index_of_last_elm]);
	}
	else
	{
		mComponentArray[index_of_rm_entt] = mComponentArray[index_of_last_elm];
	}
	//update maps
	entity_t entt_of_last_elm = mIndexToEntt[index_of_last_elm];
	mEnttToIndex[entt_of_last_elm] = index_of_rm_entt;
	mIndexToEntt[index_of_rm_entt] = entt_of_last_elm;
	mEnttToIndex.erase(entt);
	mIndexToEntt.erase(index_of_last_elm);
	--mSize;
}

template<typename T>
inline void ComponentArray<T>::EntityDestoryed(entity_t entt)
{
	if (mEnttToIndex.find(entt) != mEnttToIndex.end())
	{
		//entity has an index, therefore has data remove data
		RemoveData(entt);
	}

}

template<typename T>
inline T& ComponentArray<T>::GetData(entity_t entt)
{
	auto entt_iter = mEnttToIndex.find(entt);
	assert(entt_iter != mEnttToIndex.end() && "Entity does not have component data.");
	return mComponentArray[entt_iter->second];
}
