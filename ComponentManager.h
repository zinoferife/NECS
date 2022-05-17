#pragma once
#include "ComponentArray.h"
#include <memory>
#include <cassert>

class ComponentManager
{
public:
	ComponentManager();
	template<typename T>
	void RegisterComponent()
	{
		const char* type_name = typeid(T).name();
		assert(mComponentTypes.find(type_name) == mComponentTypes.end() && "Component type already registerd");
		mComponentTypes.insert(std::make_pair(type_name, mNextComponentType));
		mComponentArrays.insert(std::make_pair(type_name, std::make_shared<ComponentArray<T>>()));
		++mNextComponentType;
	}


	template<typename T>
	component_t GetComponentType()
	{
		const char* type_name = typeid(T).name();
		auto comp_type_iter = mComponentTypes.find(type_name);
		assert(comp_type_iter != mComponentTypes.end() && "Component is not registered"); 
		return (comp_type_iter->second);
	}

	template<typename T>
	void AddComponent(entity_t entt, T component)
	{
		GetComponentA<T>()->InsertData(entt, component);
	}

	template<typename T>
	void RemoveComponent(entity_t entt)
	{
		GetComponentA<T>()->RemoveData(entt);
	}

	template<typename T>
	T& GetComponent(entity_t entt)
	{
		return GetComponentA<T>()->GetData(entt);
	}

	void EnityDestoryed(entity_t entt);
private:
	std::unordered_map<const char*, component_t> mComponentTypes{};
	std::unordered_map<const char*, std::shared_ptr<component_array_t>> mComponentArrays{};
	component_t mNextComponentType{};
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentA()
	{
		const char* type_name = typeid(T).name();
		auto comp_array_iter = mComponentArrays.find(type_name);
		assert(comp_array_iter != mComponentArrays.end() && "Component type is not registered");
		return std::static_pointer_cast<ComponentArray<T>>(comp_array_iter->second);
	}
};

