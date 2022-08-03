#pragma once

#include "memory"

namespace Engine
{
	// templated class meant to encapsulate shareable assets
	// provides means to claim shared or exclusive ownership
	template<typename T>
	class Shared
	{
	public:
		// constructors
		Shared();

		Shared(const std::shared_ptr<T>& shared);
		Shared(const std::shared_ptr<T>&& shared);

		Shared(const T* shared);
		Shared(const T& shared);
		Shared(const T&& shared);

		Shared(Shared&&)  noexcept = default;
		Shared& operator=(Shared&&)  noexcept = default;

		Shared(const Shared&) = default;
		Shared& operator=(const Shared&) = default;

		// assignment operators
		Shared& operator=(const std::shared_ptr<T>*);
		Shared& operator=(const std::shared_ptr<T>&);
		Shared& operator=(const std::shared_ptr<T>&&);

		Shared& operator=(const T*);
		Shared& operator=(const T&);
		Shared& operator=(const T&&);

		~Shared() = default;

		// data access, similar syntax to shared_ptr
		T& operator*();
		T* operator->();
		T* Get();

		// utility
		explicit operator bool() const;
		bool operator!() const;
		bool operator==(const T&) const;
		bool operator==(const Shared<T>&) const;
		bool operator==(const std::shared_ptr<T>&) const;

		// if the held asset is shared,
		// copy and claim exclusive ownership over the copy
		void Claim();

		// reset internal shared_ptr
		void Reset();
		void Reset(const T*);
		void Reset(const T&);
		void Reset(const T&&);
		void Reset(const Shared<T>*);
		void Reset(const Shared<T>&);
		void Reset(const Shared<T>&&);
		void Reset(const std::shared_ptr<T>*);
		void Reset(const std::shared_ptr<T>&);
		void Reset(const std::shared_ptr<T>&&);

		unsigned int GetUseCount() const;

	private:
		std::shared_ptr<T> shared;
	};

	// ===========================================================================
	// templated class ^, member function implementations have to be defined here
	// ===========================================================================

	template<typename T>
	unsigned int Engine::Shared<T>::GetUseCount() const
	{
		return this->shared.use_count();
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const std::shared_ptr<T>&& asset)
	{
		this->shared.reset(std::move(asset.get()));
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const std::shared_ptr<T>& asset)
	{
		this->shared.reset(asset.get());
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const std::shared_ptr<T>* asset)
	{
		this->shared.reset(asset->get());
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const Shared<T>&& asset)
	{
		this->shared.reset(std::move(asset.shared.get()));
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const Shared<T>& asset)
	{
		this->shared.reset(asset.shared.get());
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const Shared<T>* asset)
	{
		this->shared.reset(asset->shared.get());
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const T&& asset)
	{
		this->shared.reset(&std::move(asset));
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const T& asset)
	{
		this->shared.reset(&asset);
	}

	template<typename T>
	void Engine::Shared<T>::Reset(const T* asset)
	{
		this->shared.reset(asset);
	}

	template<typename T>
	Shared<T>& Engine::Shared<T>::operator=(const T&& asset)
	{
		this->shared = std::make_shared<T>(std::move(asset));

		return *this;
	}

	template<typename T>
	Shared<T>& Engine::Shared<T>::operator=(const T& asset)
	{
		this->shared = std::make_shared<T>(asset);

		return *this;
	}

	template<typename T>
	Shared<T>& Engine::Shared<T>::operator=(const T* asset)
	{
		this->shared = std::make_shared<T>(*asset);

		return *this;
	}

	template<typename T>
	Shared<T>& Engine::Shared<T>::operator=(const std::shared_ptr<T>&& asset)
	{
		this->shared = std::move(asset);

		return *this;
	}

	template<typename T>
	Shared<T>& Engine::Shared<T>::operator=(const std::shared_ptr<T>& asset)
	{
		this->shared = asset;

		return *this;
	}

	template<typename T>
	Shared<T>& Engine::Shared<T>::operator=(const std::shared_ptr<T>* asset)
	{
		this->shared = *asset;

		return *this;
	}

	template<typename T>
	T& Engine::Shared<T>::operator*()
	{
		return *this->Get();
	}

	template<typename T>
	Engine::Shared<T>::operator bool() const
	{
		return this->shared.get();
	}

	template<typename T>
	bool Engine::Shared<T>::operator!() const
	{
		return !this->shared.get();
	}

	template<typename T>
	T* Engine::Shared<T>::Get()
	{
		// check if the pointer has any data, create a new object if it doesn't
		return !this->shared.get() ? (this->shared = std::make_shared<T>()).get() : this->shared.get();
	}

	template<typename T>
	void Engine::Shared<T>::Reset()
	{
		if (this->shared.get())
		this->shared.reset();
	}

	template<typename T>
	void Engine::Shared<T>::Claim()
	{
		// check if we're pointing to something and if anyone else is too
		if (this->shared.get() && this->shared.use_count() >= 1)
			this->shared.reset(this->shared.get());
	}

	template<typename T>
	bool Engine::Shared<T>::operator==(const Shared<T>& other) const
	{
		return this->shared.get() == other.shared.get();
	}

	template<typename T>
	bool Engine::Shared<T>::operator==(const T& other) const
	{
		return this->shared.get() == &other;
	}

	template<typename T>
	bool Engine::Shared<T>::operator==(const std::shared_ptr<T>& other) const
	{
		return this->shared.get() == other.get();
	}

	template<typename T>
	T* Engine::Shared<T>::operator->()
	{
		return this->Get();
	}

	// ===========================================================================

	template<typename T>
	Engine::Shared<T>::Shared(const T&& shared)
	{
		this->shared = std::make_shared<T>(std::move(shared));
	}

	template<typename T>
	Engine::Shared<T>::Shared(const T& shared)
	{
		this->shared = std::make_shared<T>(shared);
	}

	template<typename T>
	Engine::Shared<T>::Shared(const T* shared)
	{
		this->shared = std::make_shared<T>(*shared);
	}

	template<typename T>
	Engine::Shared<T>::Shared(const std::shared_ptr<T>&& shared)
	{
		this->shared = std::move(shared);
	}

	template<typename T>
	Engine::Shared<T>::Shared(const std::shared_ptr<T>& shared)
	{
		this->shared = shared;
	}

	template<typename T>
	Engine::Shared<T>::Shared()
	{
		this->shared = nullptr;
	}

}

