#pragma once

#include "ExclusivePointer.hpp"
#include "IArray.h"

namespace TTL
{
    template <class T>
    class Array : public virtual IArray<T>
    {
    protected:
        ExclusivePointer<T[ ]> mArr;
        size_t mCapacity;

    public:
        /// Ctors \\\

        // Default
        explicit Array( ) noexcept :
            mCapacity(0),
            mArr(nullptr)
        { }

        explicit Array(size_t cap) :
            mCapacity(cap),
            mArr((cap > 0) ? new T[cap] : nullptr)
        { }

        // Copy
        Array(const Array& src) :
            Array( )
        {
            *this = src;
        }

        // Move
        Array(Array&& src) noexcept :
            Array( )
        {
            *this = std::move(src);
        }

        /// Dtor \\\

        virtual ~Array( ) = default;

        /// Assignment Overloads \\\
            
        // Copy
        virtual inline Array& operator=(const Array& src)
        {
            ExclusivePointer<T[ ]> newArr(new T[src.mCapacity]);
            for ( size_t i = 0; i < src.mCapacity; i++ )
            {
                newArr[i] = src.mArr[i];
            }

            mArr = std::move(newArr);
            mCapacity = src.mCapacity;

            return *this;
        }

        // Move
        virtual inline Array& operator=(Array&& src) noexcept
        {
            mArr = std::move(src.mArr);
            mCapacity = src.mCapacity;
            src.mCapacity = 0;

            return *this;
        }

        /// Subscript Overload \\\

        virtual inline T& operator[](size_t index)
        {
            if ( index >= mCapacity )
            {
                throw std::out_of_range("TTL::Array<T>::operator[](size_t) - Attempted to access memory beyond array boundaries.");
            }

            return mArr[index];
        }

        virtual inline const T& operator[](size_t index) const
        {
            if ( index >= mCapacity )
            {
                throw std::out_of_range("TTL::Array<T>::operator[](size_t) - Attempted to access memory beyond array boundaries.");
            }

            return mArr[index];
        }

        /// Getters \\\

        virtual inline size_t Capacity( ) const noexcept
        {
            return mCapacity;
        }

        virtual inline bool Empty( ) const noexcept
        {
            return mArr == nullptr;
        }

        virtual inline T* Data( ) noexcept
        {
            return mArr.Get( );
        }

        virtual inline const T* Data( ) const noexcept
        {
            return mArr.Get( );
        }

        /// Public Methods \\\

        // Subscript Wrapper
        virtual inline T& At(size_t index)
        {
            return operator[](index);
        }

        // Subscript Wrapper
        virtual inline const T& At(size_t index) const
        {
            return operator[](index);
        }

        virtual inline void Clear( ) noexcept
        {
            *this = Array( );
        }
    };
};
