#pragma once
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <print>

template <typename T> using vec = std::vector<T>;
template <typename T> using u_set = std::unordered_set<T>;
template <typename K, typename V> using u_map = std::unordered_map<K, V>;
template <typename T> using u_ptr = std::unique_ptr<T>;
template <typename T> inline auto mv(T&& arg) { return std::move(arg); }
template <typename T, typename Ty> inline T* dc(const std::unique_ptr<Ty>& p) { return dynamic_cast<T*>(p.get()); }
template <typename T, typename Ty> inline T* dc(Ty* p) { return dynamic_cast<T*>(p); }
template <typename T, typename... Args> inline u_ptr<T> mu(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

template <typename Container> typename Container::value_type pop(Container& container) {
  if (container.empty()) throw std::out_of_range("pop on empty container");
  typename Container::value_type top = std::move(container.back());
  container.pop_back();
  return top;
}

template <typename Container> typename Container::value_type pop_front(Container& container) {
  if (container.empty()) throw std::out_of_range("pop on empty container");
  typename Container::value_type top = std::move(container.front());
  container.pop_front();
  return top;
}

extern bool DBG;
template<typename T, typename ...Args>
void DEBUG(T first, Args ...other) {
  if (!DBG) return;
  std::print("{} ", first);
  if constexpr (sizeof...(other) > 0) {
    DEBUG(other...);
  } else {
    std::print("\n");
  }
}