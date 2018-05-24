#pragma once

#include "ICountedPointer.h"
#include "ExclusivePointer.hpp"

namespace TTL
{
    template <class T>
    class CountedPointer : public virtual ICountedPointer<T>
    {
    private:
        T * mPtr;
        size_t* mCount;

    public:
        /// Ctors \\\

        // Default
        CountedPointer( ) noexcept :
            mPtr(nullptr),
            mCount(nullptr)
        { }

        // nullptr
        explicit CountedPointer(const std::nullptr_t) noexcept :
            CountedPointer( )
        { }

        // Raw Pointer
        explicit CountedPointer(T* p) :
            mPtr(p),
            mCount(new size_t(1))
        { }

        // Copy
        CountedPointer(const CountedPointer& src) noexcept :
            CountedPointer( )
        {
            *this = src;
        }

        // Move
        CountedPointer(CountedPointer&& src) noexcept :
            CountedPointer( )
        {
            *this = std::move(src);
        }

        // Move - ExclusivePointer<T>
        CountedPointer(ExclusivePointer<T>&& src) :
            CountedPointer( )
        {
            *this = std::move(src);
        }

        /// Dtor \\\

        ~CountedPointer( ) noexcept
        {
            Release( );
        }

        /// Operator Overloads \\\

        // nullptr
        inline CountedPointer& operator=(const std::nullptr_t) noexcept
        {
            Release( );

            return *this;
        }

        // Raw Pointer
        inline CountedPointer& operator=(T* p)
        {
            Release( );
            mPtr = p;
            mCount = new size_t(1);

            return *this;
        }

        // Copy
        inline CountedPointer& operator=(const CountedPointer& src) noexcept
        {
            Release( );
            mPtr = src.mPtr;
            mCount = src.mCount;

            if ( mCount )
            {
                (*mCount)++;
            }

            return *this;
        }

        // Move
        inline CountedPointer& operator=(CountedPointer&& src) noexcept
        {
            Release( );
            mPtr = src.mPtr;
            mCount = src.mCount;
            src.Release( );

            return *this;
        }

        // Move - ExclusivePointer<T>
        inline CountedPointer& operator=(ExclusivePointer<T>&& src)
        {
            Release( );
            mPtr = src.Get( );
            mCount = new size_t(1);
            src.Release( );

            return *this;
        }

        // Bool Conversion
        inline operator bool( ) const noexcept
        {
            return (mPtr != nullptr);
        }

        // Dereference Operator
        inline T& operator*( ) const
        {
            if ( !mPtr )
            {
                throw std::logic_error("TTL::CountedPointer<T>::operator*( ) - Attempted to dereference nullptr.");
            }

            return *mPtr;
        }

        // Class Member Access Operator
        inline T* operator->( ) const
        {
            if ( !mPtr )
            {
                throw std::logic_error("TTL::CountedPointer<T>::operator->( ) - Attempted to dereference a nullptr.");
            }

            return mPtr;
        }

        /// Public Methods \\\

        virtual void Release( ) noexcept
        {
            if ( mCount )
            {
                if ( *mCount == 1 )
                {
                    delete mPtr;
                    delete mCount;
                }
                else
                {
                    (*mCount)--;
                }
            }

            mPtr = mCount = nullptr;
        }

        virtual void Set(const std::nullptr_t pNull) noexcept
        {
            *this = pNull;
        }

        virtual void Set(T* p)
        {
            *this = p;
        }

        virtual void Set(const CountedPointer& src) noexcept
        {
            *this = src;
        }

        virtual void Set(CountedPointer&& src) noexcept
        {
            *this = std::move(src);
        }

        virtual void Set(ExclusivePointer<T>&& src)
        {
            *this = std::move(src);
        }

        virtual T* Get( ) const noexcept
        {
            return mPtr;
        }

        virtual size_t Count( ) const noexcept
        {
            return (mCount) ? *mCount : 0;
        }
    };

    template <class T>
    class CountedPointer<T[ ]> : public virtual ICountedPointer<T[ ]>
    {
    private:
        T * mPtr;
        size_t* mCount;

    public:
        /// Ctors \\\

        // Default
        CountedPointer( ) noexcept :
            mPtr(nullptr),
            mCount(nullptr)
        { }

        // nullptr
        explicit CountedPointer(const std::nullptr_t) noexcept :
            CountedPointer( )
        { }

        // Raw Pointer
        explicit CountedPointer(T* p) :
            mPtr(p),
            mCount(new size_t(1))
        { }

        // Copy
        CountedPointer(const CountedPointer<T[ ]>& src) noexcept :
            CountedPointer( )
        {
            *this = src;
        }

        // Move
        CountedPointer(CountedPointer<T[ ]>&& src) noexcept :
            CountedPointer( )
        {
            *this = std::move(src);
        }

        // Move - ExclusivePointer<T[ ]>
        CountedPointer(ExclusivePointer<T[ ]>&& src) :
            CountedPointer( )
        {
            *this = std::move(src);
        }

        /// Dtor \\\

        ~CountedPointer( ) noexcept
        {
            Release( );
        }

        /// Operator Overloads \\\

        // nullptr
        inline CountedPointer& operator=(const std::nullptr_t) noexcept
        {
            Release( );

            return *this;
        }

        // Raw Pointer
        inline CountedPointer& operator=(T* p)
        {
            Release( );
            mPtr = p;
            mCount = new size_t(1);

            return *this;
        }

        // Copy
        inline CountedPointer<T[ ]>& operator=(const CountedPointer<T[ ]>& src) noexcept
        {
            Release( );
            mPtr = src.mPtr;
            mCount = src.mCount;

            if ( mCount )
            {
                (*mCount)++;
            }

            return *this;
        }

        // Move
        inline CountedPointer<T[ ]>& operator=(CountedPointer<T[ ]>&& src) noexcept
        {
            Release( );
            mPtr = src.mPtr;
            mCount = src.mCount;
            src.Release( );

            return *this;
        }

        // Move - ExclusivePointer<T>
        inline CountedPointer<T[ ]>& operator=(ExclusivePointer<T[ ] >&& src)
        {
            Release( );
            mPtr = src.Get( );
            mCount = new size_t(1);
            src.Release( );

            return *this;
        }

        // Bool Conversion
        inline operator bool( ) const noexcept
        {
            return (mPtr != nullptr);
        }

        // Dereference Operator
        inline T& operator*( ) const
        {
            if ( !mPtr )
            {
                throw std::logic_error("TTL::CountedPointer<T[ ]>::operator*( ) - Attempted to dereference nullptr.");
            }

            return *mPtr;
        }

        // Class Member Access Operator
        inline T* operator->( ) const
        {
            if ( !mPtr )
            {
                throw std::logic_error("TTL::CountedPointer<T[ ]>::operator->( ) - Attempted to dereference a nullptr.");
            }

            return mPtr;
        }

        /// Public Methods \\\

        virtual void Release( ) noexcept
        {
            if ( mCount )
            {
                if ( *mCount == 1 )
                {
                    delete[ ] mPtr;
                    delete mCount;
                }
                else
                {
                    (*mCount)--;
                }
            }

            mPtr = mCount = nullptr;
        }

        virtual void Set(const std::nullptr_t pNull) noexcept
        {
            *this = pNull;
        }

        virtual void Set(T* p)
        {
            *this = p;
        }

        virtual void Set(const CountedPointer<T[ ]>& src) noexcept
        {
            *this = src;
        }

        virtual void Set(CountedPointer<T[ ]>&& src) noexcept
        {
            *this = std::move(src);
        }

        virtual void Set(ExclusivePointer<T[ ]>&& src)
        {
            *this = std::move(src);
        }

        virtual T* Get( ) const noexcept
        {
            return mPtr;
        }

        virtual size_t Count( ) const noexcept
        {
            return (mCount) ? *mCount : 0;
        }
    };
}