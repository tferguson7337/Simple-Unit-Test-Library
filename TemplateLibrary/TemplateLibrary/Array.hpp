#pragma once

#include "ExclusivePointer.hpp"

#include <stdexcept>

namespace TTL
{
    template <class T>
    class Array
    {
    protected:
        ExclusivePointer<T[ ]> mArr;
        uint64 mCapacity;

    public:
        /// Ctors \\\

        // Default/Capacity
        explicit Array(uint64 cap = 0ull) :
            mCapacity(cap),
            mArr((cap > 0ull) ? new T[cap] : nullptr)
        { }

        // Copy
        Array(const Array& src)
        {
            *this = src;
        }

        // Move
        Array(Array&& src) noexcept
        {
            *this = std::move(src);
        }

        /// Dtor \\\

        ~Array( ) = default;

        /// Assignment Overloads \\\
        
        // Copy
        inline Array& operator=(const Array& src)
        {
            if ( src.Empty( ) )
            {
                Clear( );
            }
            else
            {
                ExclusivePointer<T[]> newArr(new T[src.mCapacity]);
                for ( uint64 i = 0ull; i < src.mCapacity; i++ )
                {
                    newArr[i] = src.mArr[i];
                }

                mArr = std::move(newArr);
                mCapacity = src.mCapacity;
            }

            return *this;
        }

        // Move
        inline Array& operator=(Array&& src) noexcept
        {
            mArr = std::move(src.mArr);
            mCapacity = src.mCapacity;
            src.mCapacity = 0ull;

            return *this;
        }

        /// Subscript Overload \\\

        inline T& operator[](uint64 index)
        {
            if ( index >= mCapacity )
            {
                throw std::out_of_range("TTL::Array<T>::operator[](uint64) - Attempted to access memory beyond array boundaries.");
            }

            return mArr[index];
        }

        inline const T& operator[](uint64 index) const
        {
            if ( index >= mCapacity )
            {
                throw std::out_of_range("TTL::Array<T>::operator[](uint64) - Attempted to access memory beyond array boundaries.");
            }

            return mArr[index];
        }

        /// Getters \\\

        inline uint64 Capacity( ) const noexcept
        {
            return mCapacity;
        }

        inline bool Empty( ) const noexcept
        {
            return mArr == nullptr;
        }

        inline T* Data( ) noexcept
        {
            return mArr.Get( );
        }

        inline const T* Data( ) const noexcept
        {
            return mArr.Get( );
        }

        /// Public Methods \\\

        // Subscript Wrapper
        inline T& At(uint64 index)
        {
            return operator[](index);
        }

        // Subscript Wrapper
        inline const T& At(uint64 index) const
        {
            return operator[](index);
        }

        inline void Clear( ) noexcept
        {
            *this = Array( );
        }
    };
};
