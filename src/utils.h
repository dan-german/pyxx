#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

template <typename T> using vec = std::vector<T>;
template <typename T> using u_set = std::unordered_set<T>;
template <typename K, typename V> using u_map = std::unordered_map<K, V>;
template <typename T> using u_ptr = std::unique_ptr<T>;
template <typename T> inline auto mv(T&& arg) { return std::move(arg); }
template<typename T, typename PtrTy> inline T* dc(std::unique_ptr<PtrTy>& p) { return dynamic_cast<T*>(p.get()); }
template<typename T, typename PtrTy> inline T* dc(PtrTy* p) { return dynamic_cast<T*>(p); }
