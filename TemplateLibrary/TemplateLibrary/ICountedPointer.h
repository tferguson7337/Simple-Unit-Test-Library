#pragma once

namespace TTL
{
    /// Forward Declaration
    template <class T>
    class CountedPointer;

    template <class T>
    class ExclusivePointer;

    template <class T>
    class ICountedPointer
    {
    public:
        ICountedPointer( ) = default;
        virtual ~ICountedPointer( ) = default;

        virtual CountedPointer<T>& operator=(const std::nullptr_t) noexcept = 0;
        virtual CountedPointer<T>& operator=(T*) = 0;
        virtual CountedPointer<T>& operator=(const CountedPointer<T>&) = 0;
        virtual CountedPointer<T>& operator=(CountedPointer<T>&&) = 0;
        virtual CountedPointer<T>& operator=(ExclusivePointer<T>&&) = 0;
        virtual operator bool( ) const noexcept = 0;
        virtual T& operator*( ) const = 0;
        virtual T* operator->( ) const = 0;

        /// Public Methods \\\

        virtual void Release( ) noexcept = 0;
        virtual void Set(const std::nullptr_t) noexcept = 0;
        virtual void Set(T*) = 0;
        virtual void Set(const CountedPointer<T>&) = 0;
        virtual void Set(CountedPointer<T>&&) = 0;
        virtual void Set(ExclusivePointer<T>&&) = 0;
        virtual T* Get( ) const noexcept = 0;
        virtual size_t Count( ) const noexcept = 0;
    };


    /// Forward Declaration
    template <class T>
    class CountedPointer<T[ ]>;

    template <class T>
    class ExclusivePointer<T[ ]>;

    template <class T>
    class ICountedPointer<T[ ]>
    {
    public:
        ICountedPointer( ) = default;
        virtual ~ICountedPointer( ) = default;

        virtual CountedPointer<T[ ]>& operator=(const std::nullptr_t) noexcept = 0;
        virtual CountedPointer<T[ ]>& operator=(T*) = 0;
        virtual CountedPointer<T[ ]>& operator=(const CountedPointer<T[ ]>&) = 0;
        virtual CountedPointer<T[ ]>& operator=(CountedPointer<T[ ]>&&) = 0;
        virtual CountedPointer<T[ ]>& operator=(ExclusivePointer<T[ ]>&&) = 0;
        virtual operator bool( ) const noexcept = 0;
        virtual T& operator*( ) const = 0;
        virtual T* operator->( ) const = 0;

        /// Public Methods \\\

        virtual void Release( ) noexcept = 0;
        virtual void Set(const std::nullptr_t) noexcept = 0;
        virtual void Set(T*) = 0;
        virtual void Set(const CountedPointer<T[ ]>&) = 0;
        virtual void Set(CountedPointer<T[ ]>&&) = 0;
        virtual void Set(ExclusivePointer<T[ ]>&&) = 0;
        virtual T* Get( ) const noexcept = 0;
        virtual size_t Count( ) const noexcept = 0;
    };
}