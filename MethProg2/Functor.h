#pragma once

template <typename T>
class functor;


template <typename ResultType, typename ... ArgsType>
class functor <ResultType(ArgsType ...) >
{
public:
	functor() : mInvoker() {};

	template <typename FuncT>
	functor(FuncT fun) : mInvoker(new free_function_holder<FuncT>(fun)) {}

	template <typename MethodType, typename ClassType>
	functor(MethodType ClassType::* fun) : mInvoker(new method_function_holder<MethodType, ArgsType ...>(fun)) {}


	functor(const functor& other) : mInvoker(other.mInvoker->clone()) {};

	functor& operator = (const functor& other)
	{
		mInvoker = other.mInvoker->clone();
		return *this;
	}

	ResultType operator()(ArgsType ... args)
	{
		return mInvoker->invoke(args ...);
	}

private:
	class function_holder
	{
	public:
		function_holder() {};
		virtual ~function_holder() {};
		virtual ResultType invoke(ArgsType ... args) = 0;
		virtual std::shared_ptr<function_holder> clone() = 0;


	private:
		function_holder(const function_holder&);
		function_holder& operator = (const function_holder&);
	};
	using invoker_t = std::shared_ptr<function_holder>;

	template <typename FuncT>
	class free_function_holder : public function_holder
	{
	public:
		free_function_holder(FuncT fun) : function_holder(), mFunction(fun){};

		virtual ResultType invoke(ArgsType ... args)
		{
			return mFunction(args...);
		}

		virtual invoker_t clone()
		{
			return invoker_t(new free_function_holder<FuncT>(mFunction));
		}

	private:
		FuncT mFunction;
	};

	template <typename FuncT, typename ClassT, typename ... RestArgsT>
	class method_function_holder : public function_holder
	{
	public:
		using method_sign_t = FuncT ClassT::*;
		
		method_function_holder(method_sign_t fun) : mFunction(fun) {};

		virtual ResultType invoke(ClassT clobj, RestArgsT... rargs)
		{
			return (clobj.*mFunction)(rargs...);
		}

		virtual invoker_t clone()
		{
			return invoker_t(new method_function_holder(mFunction));
		}

	private:
		method_sign_t mFunction;
	};

	invoker_t mInvoker;
};
