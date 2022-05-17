#pragma once
#include <memory>
#include <tuple>
#include "EnttManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
template<size_t count>
class Loop;

class Coordinator
{
public:
	void Init();
	entity_t CreateEntity();
	void DestoryEntity(entity_t entt);

	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(entity_t entt, T component)
	{
		mComponentManager->AddComponent(entt, component);
		auto signature = mEntityManager->GetSignature(entt);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entt, signature);
		mSystemManager->EntitySignatureChange(entt, signature);
	}

	template<typename T>
	void RemoveComponent(entity_t entt)
	{
		mComponentManager->RemoveComponent<T>(entt);
		auto signature = mEntityManager->GetSignature(entt);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(signature);
		mSystemManager->EntitySignatureChange(entt, signature);
	}

	template<typename T>
	T& GetComponent(entity_t entt)
	{
		return mComponentManager->GetComponent<T>(entt);
	}

	template<typename T>
	component_t GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	template<typename...T>
	void SetUpSignature(signature_t& signature)
	{
		using tuple = std::tuple<T...>;
		tuple t;
		constexpr size_t size = std::tuple_size<tuple>{};
		Loop<size>::doSetSignature(*this, signature, t);
	}

	template<typename S>
	std::shared_ptr<S> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<S>();
	}

	template<typename S>
	void SetSystemSignature(signature_t signature)
	{
		mSystemManager->SetSignature<S>(signature);
	}

private:
	std::unique_ptr<EnttManager> mEntityManager;
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<SystemManager> mSystemManager;
};

template<size_t count>
class Loop
{
public:
	template<typename T>
	static void doSetSignature(Coordinator& coor, signature_t& signature, T tuple)
	{
		SetSignature<std::tuple_element_t<count - 1,T>>(coor, signature);
		Loop<count - 1>::doSetSignature(coor, signature, tuple);

	}
	template<typename C>
	static void SetSignature(Coordinator& coor, signature_t& signature)
	{
		signature.set(coor.GetComponentType<C>(), true);
	}
};

template<>
class Loop<1>
{
public:
	template<typename T>
	static void doSetSignature(Coordinator& coor, signature_t& signature, T tuple)
	{
		SetSignature<std::tuple_element_t<0, T>>(coor, signature);
	}

	template<typename C>
	static void SetSignature(Coordinator& coor, signature_t& signature)
	{
		signature.set(coor.GetComponentType<C>(), true);
	}
};