#pragma once
#include <new.h>

class Delegate
{
	class DelegateBase
	{
	public:
		virtual ~DelegateBase() {}
		virtual void Call() const = 0;
		virtual DelegateBase* Clone() const = 0;
	};

	class FreeFunctionDelegate : public DelegateBase
	{
		void(*function_)();
	public:
		FreeFunctionDelegate(void(function)()) : function_(function) {}

		virtual void Call() const override
		{
			return function_();
		}

		virtual DelegateBase* Clone() const override
		{
			return new FreeFunctionDelegate(*this);
		}
	};

	template <class C>
	class MemberFunctionDelegate : public DelegateBase
	{
		void(C::*function_)();
		C* instance_;
	public:
		MemberFunctionDelegate(void(C::*function)(), C* instance) : function_(function), instance_(instance) {}

		virtual void Call() const override
		{
			return (instance_->*function_)();
		}

		virtual DelegateBase* Clone() const override
		{
			return new MemberFunctionDelegate(*this);
		}
	};

	template <class C>
	class ConstMemberFunctionDelegate : public DelegateBase
	{
		void(C::*function_)() const;
		const C* instance_;
	public:
		ConstMemberFunctionDelegate(void(C::*function)() const, const C* instance) : function_(function), instance_(instance) {}

		virtual void Call() const override
		{
			return (instance_->*function_)();
		}

		virtual DelegateBase* Clone() const override
		{
			return new ConstMemberFunctionDelegate(*this);
		}
	};

public:
	Delegate(void);
	Delegate(const Delegate& other);
	Delegate& operator= (const Delegate& other);
	~Delegate();

	void Bind(void(*freeFunction)());
	template <class C>
	void Bind(void (C::*memberFunction)(), C* instance);
	template <class C>
	void Bind(void (C::*constMemberFunction)() const, const C* instance);

	void Invoke() const;

private:
	DelegateBase* delegate_;
	char data_[20];
};