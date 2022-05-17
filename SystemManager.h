#pragma once
#include <unordered_map>
#include <memory>
#include <type_traits>
#include"System.h"
#include "Enity.h"
#include <cassert>
class SystemManager
{
public:
	SystemManager();
	
	template<typename S>
	std::shared_ptr<S> RegisterSystem()
	{
		static_assert(std::is_base_of_v<System, S>);
		const char* type_name = typeid(S).name();
		assert(mSystems.find(type_name) == mSystems.end() && "System is already registered");
		auto system = std::make_shared<S>();
		mSystems.insert(std::make_pair(type_name, system));
		return system;
	}

	template<typename T>
	void SetSignature(signature_t signature)
	{
		const char* type_name = typeid(T).name();
		assert(mSignatures.find(type_name) == mSignatures.end() && "No registered system for that type");
		mSignatures.insert(std::make_pair(type_name, signature));
	}

	void EntitySignatureChange(entity_t entt, signature_t entt_signature);
	void EntityDestoryed(entity_t entt);


private:
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
	std::unordered_map<const char*, signature_t> mSignatures{};

};

