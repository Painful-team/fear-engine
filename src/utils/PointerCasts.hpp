#ifndef FEARENGINE_UTILS_POINTERCASTS_H__
#define FEARENGINE_UTILS_POINTERCASTS_H__

#include <memory>

namespace FearEngine::utils
{
template< class T, class U >
std::shared_ptr<T> static_pointer_cast(const std::shared_ptr<U>& r) noexcept
{
	auto p = static_cast<typename std::shared_ptr<T>::element_type*>(r.get());
	return std::shared_ptr<T>{r, p};
}

template< class T, class U >
std::shared_ptr<T> dynamic_pointer_cast(const std::shared_ptr<U>& r) noexcept
{
	if (auto p = dynamic_cast<typename std::shared_ptr<T>::element_type*>(r.get()))
	{
		return std::shared_ptr<T>{r, p};
	}
	else
	{
		return std::shared_ptr<T>{};
	}
}

template< class T, class U >
std::shared_ptr<T> const_pointer_cast(const std::shared_ptr<U>& r) noexcept
{
	auto p = const_cast<typename std::shared_ptr<T>::element_type*>(r.get());
	return std::shared_ptr<T>{r, p};
}

template< class T, class U >
std::shared_ptr<T> reinterpret_pointer_cast(const std::shared_ptr<U>& r) noexcept
{
	auto p = reinterpret_cast<typename std::shared_ptr<T>::element_type*>(r.get());
	return std::shared_ptr<T>{r, p};
}
}

#endif
