#pragma once

// C++17 Compatibility Shim
// Provides missing C++17 standard library features for older libstdc++ versions

#include <type_traits>
#include <functional>
#include <utility>

namespace std {

// std::invoke - C++17 feature
#if !defined(__cpp_lib_invoke) || __cpp_lib_invoke < 201411L

  namespace detail {
    // Helper for member function pointers
    template<typename T, typename Type, typename T1, typename... Args>
    constexpr auto invoke_impl(Type T::* f, T1&& t1, Args&&... args)
      noexcept(noexcept((std::forward<T1>(t1).*f)(std::forward<Args>(args)...)))
      -> typename std::enable_if<std::is_member_function_pointer<decltype(f)>::value,
                                   decltype((std::forward<T1>(t1).*f)(std::forward<Args>(args)...))>::type {
      return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
    }

    // Helper for member object pointers
    template<typename T, typename Type, typename T1>
    constexpr auto invoke_impl(Type T::* f, T1&& t1)
      noexcept(noexcept(std::forward<T1>(t1).*f))
      -> typename std::enable_if<std::is_member_object_pointer<decltype(f)>::value,
                                   decltype(std::forward<T1>(t1).*f)>::type {
      return std::forward<T1>(t1).*f;
    }

    // Helper for function objects
    template<typename F, typename... Args>
    constexpr auto invoke_impl(F&& f, Args&&... args)
      noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
      -> typename std::enable_if<!std::is_member_pointer<typename std::decay<F>::type>::value,
                                   decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::type {
      return std::forward<F>(f)(std::forward<Args>(args)...);
    }
  }

  template<typename F, typename... Args>
  constexpr auto invoke(F&& f, Args&&... args)
    noexcept(noexcept(detail::invoke_impl(std::forward<F>(f), std::forward<Args>(args)...)))
    -> decltype(detail::invoke_impl(std::forward<F>(f), std::forward<Args>(args)...)) {
    return detail::invoke_impl(std::forward<F>(f), std::forward<Args>(args)...);
  }

#endif

// std::invoke_result_t and related - C++17 feature
#if !defined(__cpp_lib_is_invocable) || __cpp_lib_is_invocable < 201703L

  namespace detail {
    template<typename F, typename... Args>
    struct invoke_result {
      using type = decltype(invoke(std::declval<F>(), std::declval<Args>()...));
    };
  }

  template<typename F, typename... Args>
  using invoke_result = detail::invoke_result<F, Args...>;

  template<typename F, typename... Args>
  using invoke_result_t = typename invoke_result<F, Args...>::type;

  // is_invocable
  namespace detail {
    template<typename F, typename... Args>
    struct is_invocable_impl {
    private:
      template<typename F1, typename... Args1>
      static auto test(int) -> decltype(
        invoke(std::declval<F1>(), std::declval<Args1>()...), std::true_type{}
      );

      template<typename, typename...>
      static auto test(...) -> std::false_type;

    public:
      using type = decltype(test<F, Args...>(0));
    };
  }

  template<typename F, typename... Args>
  struct is_invocable : detail::is_invocable_impl<F, Args...>::type {};

  template<typename F, typename... Args>
  constexpr bool is_invocable_v = is_invocable<F, Args...>::value;

#endif

// Type trait _v helpers - C++17 feature
#if !defined(__cpp_lib_type_trait_variable_templates) || __cpp_lib_type_trait_variable_templates < 201510L

  #ifndef __cpp_lib_is_void_v
  template<typename T>
  constexpr bool is_void_v = is_void<T>::value;
  #endif

  #ifndef __cpp_lib_is_convertible_v
  template<typename From, typename To>
  constexpr bool is_convertible_v = is_convertible<From, To>::value;
  #endif

#endif

// std::clamp - C++17 feature
#if !defined(__cpp_lib_clamp) || __cpp_lib_clamp < 201603L

  template<typename T>
  constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
  }

  template<typename T, typename Compare>
  constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
    return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
  }

#endif

} // namespace std
