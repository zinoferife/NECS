#pragma once
#include <cstdint>
#include <bitset>

typedef std::uint32_t entity_t;
constexpr size_t MAX_ENTITIES = 5000;

typedef std::uint8_t component_t;
constexpr component_t MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> signature_t;