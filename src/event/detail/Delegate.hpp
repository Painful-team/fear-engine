#ifndef FEARENGINE_EVENT_DETAIL_DELEGATE_H__
#define FEARENGINE_EVENT_DETAIL_DELEGATE_H__

namespace FearEngine::Events::detail
{

template <typename T>
class delegate;

template<typename Return, typename ...Params>
class delegate<Return(Params...)>
{
public:
	using stub_type = Return(*)(void* this_p, Params...);

	delegate():
		object(nullptr),
		stub(nullptr)
	{}

	delegate(const delegate& another)
	{
		*this = another;
	}

	template <typename Lambda>
	delegate(const Lambda& lambda)
	{
		*this = lambda;
	}

	delegate& operator = (const delegate& another)
	{
		stub = another.stub;
		object = another.object;

		return *this;
	}

	template <typename Lambda>
	delegate& operator = (const Lambda& instance)
	{
		object = reinterpret_cast<void*>(const_cast<Lambda*>(&instance));
		stub = lambda_stub<Lambda>;

		return *this;
	}

	bool operator == (const delegate& another) const
	{
		return this->object == another.object && this->stub == another.stub;
	}

	bool operator != (const delegate& another) const
	{
		return !(*this == another);
	}

	Return operator()(Params... arg) const
	{
		return (*stub)(object, arg...);
	}

	template <class T, Return(T::*method)(Params...)>
	static delegate create(T* instance)
	{
		return { instance, method_stub<T, method> };
	}

	template <class T, Return(T::*method)(Params...) const>
	static delegate create(const T* instance)
	{
		return { const_cast<T*>(instance), const_method_stub<T, method> };
	}

	template <Return(*function)(Params...)>
	static delegate create()
	{
		return { nullptr, function_stub<function> };
	}

	template <typename Lambda>
	static delegate create(const Lambda& instance)
	{
		return { reinterpret_cast<void*>(const_cast<Lambda*>(&instance)), lambda_stub<Lambda> };
	}

	bool isNull() const
	{
		return invocation.stub == nullptr;
	}

private:
	void* object;
	stub_type stub;

	delegate(void* object, stub_type stub):
		object(object),
		stub(stub)
	{}

	template <class T, Return(T::*method)(Params...)>
	static Return method_stub(void* this_ptr, Params... params)
	{
		return (static_cast<T*>(this_ptr)->*method)(params...);
	}

	template <class T, Return(T::*method)(Params...) const>
	static Return const_method_stub(void* this_ptr, Params... params)
	{
		return (static_cast<T*>(this_ptr)->*method)(params...);
	}

	template <Return(*method)(Params...)>
	static Return function_stub(void* this_ptr, Params... params)
	{
		return (method)(params...);
	}

	template <typename Lambda>
	static Return lambda_stub(void* this_ptr, Params... arg)
	{
		return (static_cast<Lambda*>(this_ptr)->operator())(arg...);
	}
};
};

#endif
