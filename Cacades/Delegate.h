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
	Delegate(void) : delegate_(nullptr)
	{}

	Delegate(const Delegate& other)
	{
		if (other.delegate_ == nullptr)
		{
			delegate_ = nullptr;
		}
		else
		{
			delegate_ = other.delegate_->Clone();
		}
	}

	Delegate& operator= (const Delegate& other)
	{
		if (this != &other)
		{
			delete delegate_;
			if (other.delegate_ == nullptr)
			{
				delegate_ = nullptr;
			}
			else
			{
				delegate_ = other.delegate_->Clone();
			}
		}
		return *this;
	}

	~Delegate()
	{
		delete delegate_;
	}

	void Bind(void(*freeFunction)())
	{
		delete delegate_;

		if (freeFunction == nullptr)
		{
			delegate_ = nullptr;
		}
		else
		{
			delegate_ = new(data_) FreeFunctionDelegate(freeFunction);
		}
	}

	template <class C>
	void Bind(void (C::*memberFunction)(), C* instance)
	{
		delete delegate_;
		delegate_ = new(data_) MemberFunctionDelegate<C>(memberFunction, instance);
	}

	template <class C>
	void Bind(void (C::*constMemberFunction)() const, const C* instance)
	{
		delete delegate_;
		delegate_ = new(data_) ConstMemberFunctionDelegate<C>(constMemberFunction, instance);
	}

	void Invoke() const
	{
		if (delegate_ == nullptr)
			return;

		return delegate_->Call();
	}

private:
	DelegateBase* delegate_;
	char data_[256];
};