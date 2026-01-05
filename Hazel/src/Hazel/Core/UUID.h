#pragma once

namespace Hazel {
	class UUID {
	public:
		UUID();	// 生成随机UUID
		UUID(uint64_t uuid);	// 从已有值进行构造
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }	// 隐式转换

	private:
		uint64_t m_UUID;
	};
}


/* 特化hash让UUID可以成为key用于unordered_map和unordered_set */
namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<Hazel::UUID> {
		std::size_t operator() (const Hazel::UUID& uuid) const {
			return (uint64_t)uuid;
		}
	};
}