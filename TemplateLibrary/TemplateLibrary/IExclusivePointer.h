#pragma once

namespace TTL
{
    /// Forward Declaration
    template <class T>
    class ExclusivePointer;

    template <class T>
    class IExclusivePointer
    {
    public:
        IExclusivePointer( ) = default;
        virtual ~IExclusivePointer( ) = default;

        /// Operator Overloads \\\

        virtual ExclusivePointer<T>& operator=(const std::nullptr_t) noexcept = 0;
        virtual ExclusivePointer<T>& operator=(T*) noexcept = 0;
        virtual ExclusivePointer<T>& operator=(ExclusivePointer<T>&&) = 0;
        virtual operator bool( ) const noexcept = 0;
        virtual T& operator*( ) const noexcept = 0;
        virtual T* operator->( ) const noexcept = 0;

        /// Public Methods \\\

        virtual void Release( ) noexcept = 0;
        virtual void Set(const std::nullptr_t) noexcept = 0;
        virtual void Set(T*) noexcept = 0;
        virtual void Set(ExclusivePointer<T>&&) = 0;
        virtual T* Get( ) const noexcept = 0;
    };

    /// Forward Declaration
    template <class T>
    class ExclusivePointer<T[ ]>;

    template <class T>
    class IExclusivePointer<T[ ]>
    {
    public:
        IExclusivePointer( ) = default;
        virtual ~IExclusivePointer( ) = default;

        /// Operator Overloads \\\

        virtual ExclusivePointer<T[ ]>& operator=(const std::nullptr_t) noexcept = 0;
        virtual ExclusivePointer<T[ ]>& operator=(T*) noexcept = 0;
        virtual ExclusivePointer<T[ ]>& operator=(ExclusivePointer<T[ ]>&&) = 0;
        virtual operator bool( ) const noexcept = 0;
        virtual T& operator[](size_t) const noexcept = 0;

        /// Public Methods \\\

        virtual void Release( ) noexcept = 0;
        virtual void Set(const std::nullptr_t) noexcept = 0;
        virtual void Set(T*) noexcept = 0;
        virtual void Set(ExclusivePointer<T[ ]>&&) = 0;
        virtual T* Get( ) const noexcept = 0;
    };
}