#pragma once

namespace TTL
{
    /// Forward Declaration
    template <class T>
    class Array;

    template <class T>
    class IArray
    {
    public:
        IArray( ) = default;
        virtual ~IArray( ) = default;

        /// Operator Overloads \\\

        virtual Array<T>& operator=(const Array<T>&) = 0;
        virtual Array<T>& operator=(Array<T>&&) = 0;
        virtual T& operator[](size_t) = 0;
        virtual const T& operator[](size_t) const = 0;

        /// Getters \\\

        virtual size_t Capacity( ) const noexcept = 0;
        virtual bool Empty( ) const noexcept = 0;
        virtual T* Data( ) noexcept = 0;
        virtual const T* Data( ) const noexcept = 0;

        /// Public Methods \\\

        virtual T& At(size_t) = 0;
        virtual const T& At(size_t) const = 0;
        virtual void Clear( ) noexcept = 0;
    };
}