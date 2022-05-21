#ifndef FEARENGINE_EVENT_DETAIL_DELEGATE_H__
#define FEARENGINE_EVENT_DETAIL_DELEGATE_H__
namespace FearEngine::Events::detail
{
namespace detail
{
	template<class T>
	struct has_call_operator
	{
		template<class U>
		static int8_t func(decltype(&U::operator()))
		{
			return 0;
		}

		template<class U>
		static int16_t func(...)
		{
			return 0;
		}

		static const bool type = (sizeof(func<T>(0)) == sizeof(int8_t));
	};
};

template <typename T>
class Delegate;

template <typename Return, typename ...Params>
class Delegate<Return(Params...)>
{
public:
	using stub_type = Return(*)(void* this_p, Params&&...);

	Delegate() :
		object(nullptr),
		stub(nullptr)
	{}

	Delegate(const Delegate& another) = default;
	Delegate& operator = (const Delegate& another) = default;

	template <typename Lambda, typename = std::enable_if_t<detail::has_call_operator<Lambda>::type>>
	Delegate(const Lambda& lambda)
	{
		*this = lambda;
	}

	template <typename Lambda, typename = std::enable_if_t<detail::has_call_operator<Lambda>::type>>
	Delegate& operator = (const Lambda& instance)
	{
		object = reinterpret_cast<void*>(const_cast<Lambda*>(&instance));
		stub = lambda_stub<Lambda>;

		return *this;
	}

	bool operator == (const Delegate& another) const noexcept
	{
		return this->object == another.object && this->stub == another.stub;
	}

	bool operator != (const Delegate& another) const noexcept
	{
		return !(*this == another);
	}

	template <typename... Args,
		typename = std::enable_if_t<std::is_invocable_v<Return(*)(Params&&...), Args...>>>
	Return operator()(Args&&... arg) const
	{
		return (*stub)(object, std::forward<Args>(arg)...);
	}

	template <typename... Args,
		typename = std::enable_if_t<std::is_invocable_v<Return(*)(Params...), Args...>>>
	Return operator()(Args... arg)
	{
		return (*stub)(object, std::forward<Args>(arg)...);
	}

	Return operator()() const
	{
		return (*stub)(object);
	}

	template <auto Function,
		typename = std::enable_if_t<
		std::is_invocable_r_v<Return, decltype(Function), Params...>
		>>
	static Delegate create(const char* blank = nullptr)
	{
		return { nullptr, function_stub<Function> };
	}

	template <auto Method, typename T,
		typename = std::enable_if_t<(
			std::is_invocable_r_v<Return, decltype(Method), T&, Params...>
		)>>
	static Delegate create(T* instance)
	{
		return { instance, method_stub<Method, T> };
	}

	template <auto Method, typename T,
		typename = std::enable_if_t<(
			std::is_invocable_r_v<Return, decltype(Method), const T&, Params...>
		)>>
	static Delegate create(const T* instance)
	{
		return { const_cast<T*>(instance), method_stub<Method, T> };
	}

	template <typename Lambda, typename = std::enable_if_t<detail::has_call_operator<Lambda>::type>>
	static Delegate create(const Lambda& instance)
	{
		return { reinterpret_cast<void*>(const_cast<Lambda*>(&instance)), lambda_stub<Lambda> };
	}

	bool isNull() const noexcept
	{
		return stub == nullptr;
	}

private:
	Delegate(void* object, stub_type stub) :
		object(object),
		stub(stub)
	{}

	template <Return(*method)(Params...)>
	static Return function_stub(void* this_ptr, Params&&... arg)
	{
		return (method)(std::forward<Params>(arg)...);
	}

	template <auto Method, typename T>
	static Return method_stub(void* this_ptr, Params&&... arg)
	{
		return (static_cast<T*>(this_ptr)->*Method)(std::forward<Params>(arg)...);
	}

	template <typename Lambda>
	static Return lambda_stub(void* this_ptr, Params&&... arg)
	{
		return (static_cast<Lambda*>(this_ptr)->operator())(std::forward<Params>(arg)...);
	}

private:
	void* object;
	stub_type stub;
};
};

#endif
