#pragma once

namespace TTL
{
    /// Forward Declaration
    template <class T>
    class DynamicArray;

    template <class T>
    class IDynamicArray
    {
    public:
        IDynamicArray( ) = default;
        virtual ~IDynamicArray( ) = default;

        /// Operator Overloads \\\

        virtual DynamicArray<T>& operator=(const DynamicArray<T>&) = 0;
        virtual DynamicArray<T>& operator=(DynamicArray<T>&&) = 0;

        /// Subscript Overload \\\

        virtual T& operator[](size_t) = 0;
        virtual const T& operator[](size_t) const = 0;

        /// Getters \\\

        virtual size_t Size( ) const noexcept = 0;
        virtual bool Empty( ) const noexcept = 0;

        /// Public Methods \\\

        virtual T& At(size_t) = 0;
        virtual const T& At(size_t) const = 0;
        virtual void Clear( ) = 0;
        virtual void Append(const T&) = 0;
        virtual void Append(T&&) = 0;
        virtual T& Front( ) = 0;
        virtual const T& Front( ) const = 0;
        virtual T& Back( ) = 0;
        virtual const T& Back( ) const = 0;
        virtual void Compress( ) = 0;
        virtual void Reserve(const size_t) = 0;
    };
}