#include "hzpch.h"
#include "UUID.h"

#include <random>

namespace Hazel {
	static std::random_device s_RandomDevice;								// 随机种子
	static std::mt19937_64 s_Engine(s_RandomDevice());						// 随机数生成器
	static std::uniform_int_distribution<uint64_t> s_UniformDistruibution;	// 随机分布

	UUID::UUID() : m_UUID(s_UniformDistruibution(s_Engine)) {}				// 随机生成

	UUID::UUID(uint64_t uuid) : m_UUID(uuid){}
}