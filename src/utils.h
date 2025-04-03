#pragma once
#include <memory>
#include <vector>

template <typename T> using vec = std::vector<T>;
template <typename T> using up = std::unique_ptr<T>;
template <typename T> inline auto mv(T&& arg) { return std::move(arg); }
template<typename T, typename PtrTy> inline T* dc(std::unique_ptr<PtrTy>& p) { return dynamic_cast<T*>(p.get()); }
template<typename T, typename PtrTy> inline T* dc(PtrTy* p) { return dynamic_cast<T*>(p); }
