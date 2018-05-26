#pragma once

#include "Uncopyable.h"
#include "Types.h"

#include "IExclusivePointer.h"

namespace TTL
{
    template <class T>
    class ExclusivePointer : public virtual IExclusivePointer<T>, public Uncopyable
    {
        /// Allow ExclusivePointer<T>-to-CountedPointer<T> moved-based methods to "steal" resource.
        template <class T>
        friend class CountedPointer;

    private:
        T * mPtr;

    public:

        /// Ctors \\\

        // Default
        ExclusivePointer( ) noexcept :
            mPtr(nullptr)
        { }

        // nullptr
        explicit ExclusivePointer(const std::nullptr_t pNull) noexcept :
            mPtr(pNull)
        { }

        // Raw Pointer
        explicit ExclusivePointer(T* pT) noexcept :
            mPtr(pT)
        { }

        // Move
        ExclusivePointer(ExclusivePointer&& src) noexcept :
            mPtr(src.mPtr)
        {
            src.mPtr = nullptr;
        }

        /// Dtor \\\

        // Free owned resource.
        ~ExclusivePointer( ) noexcept
        {
            delete mPtr;
        }

        /// Operator Overloads \\\

        // nullptr Assignment
        inline ExclusivePointer& operator=(const std::nullptr_t pNull) noexcept
        {
            delete mPtr;
            mPtr = pNull;

            return *this;
        }

        // Raw Pointer Assignment
        inline ExclusivePointer& operator=(T* pT) noexcept
        {
            delete mPtr;
            mPtr = pT;

            return *this;
        }

        // Move Assignment
        inline ExclusivePointer& operator=(ExclusivePointer&& src)
        {
            if ( this == &src )
            {
                throw std::invalid_argument("TTL::ExclusivePointer<T>::operator=(ExclusivePointer<T>&&) : self-assignment not allowed.");
            }

            delete mPtr;
            mPtr = src.mPtr;
            src.mPtr = nullptr;

            return *this;
        }

        // Bool Overload for Conditional Statements
        explicit inline operator bool( ) const noexcept
        {
            return (mPtr == nullptr) ? false : true;
        }

        // Dereference Ops
        inline T& operator*( ) const noexcept
        {
            return *mPtr;
        }

        inline T* operator->( ) const noexcept
        {
            return mPtr;
        }

        /// Public Methods \\\

        // Free owned resource, clear pointer.
        inline void Release( ) noexcept
        {
            *this = nullptr;
        }

        // Free owned resource, clear pointer.
        inline void Set(const std::nullptr_t pNull) noexcept
        {
            *this = pNull;
        }
        // Free owned resource, claim passed resource (copy).
        inline void Set(T* pT) noexcept
        {
            *this = pT;
        }

        // Free owned resource, claim passed resource (move).
        inline void Set(ExclusivePointer&& src)
        {
            *this = std::move(src);
        }

        // Return pointer to owned resource.
        inline T* Get( ) const noexcept
        {
            return mPtr;
        }

    };

    template <class T>
    class ExclusivePointer<T[ ]> : public IExclusivePointer<T[ ]>, public Uncopyable
    {
        /// Allow ExclusivePointer<T[ ]>-to-CountedPointer<T[ ]> moved-based methods to "steal" resources.
        template <class T>
        friend class CountedPointer;

    private:
        T * mPtr;

    public:

        /// Ctors \\\

        // Default
        ExclusivePointer( ) noexcept :
            mPtr(nullptr)
        { }

        // nullptr
        explicit ExclusivePointer(const std::nullptr_t pNull) noexcept :
            mPtr(pNull)
        { }

        // Raw Pointer
        explicit ExclusivePointer(T* pT) noexcept :
            mPtr(pT)
        { }

        // Move
        ExclusivePointer(ExclusivePointer&& src) noexcept :
            mPtr(src.mPtr)
        {
            src.mPtr = nullptr;
        }

        /// Dtor \\\

        // Free owned resource.
        ~ExclusivePointer( ) noexcept
        {
            delete[ ] mPtr;
        }

        /// Operator Overloads \\\

        // nullptr Assignment
        inline ExclusivePointer& operator=(const std::nullptr_t pNull) noexcept
        {
            delete[ ] mPtr;
            mPtr = pNull;

            return *this;
        }

        // Raw Pointer Assignment
        inline ExclusivePointer& operator=(T* pT) noexcept
        {
            delete[ ] mPtr;
            mPtr = pT;

            return *this;
        }

        // Move Assignment
        inline ExclusivePointer& operator=(ExclusivePointer&& src)
        {
            if ( this == &src )
            {
                throw std::invalid_argument("TTL::ExclusivePointer<T>::operator=(ExclusivePointer<T>&&) : self-assignment not allowed.");
            }

            delete[ ] mPtr;
            mPtr = src.mPtr;
            src.mPtr = nullptr;

            return *this;
        }

        // Bool Overload for Conditional Statements
        explicit inline operator bool( ) const noexcept
        {
            return (mPtr == nullptr) ? false : true;
        }

        // Derefence Op
        T& operator[](size_t i) const noexcept
        {
            return mPtr[i];
        }

        /// Public Methods \\\

        // Free owned resource, clear pointer.
        inline void Release( ) noexcept
        {
            *this = nullptr;
        }

        // Free owned resource, clear pointer.
        inline void Set(const std::nullptr_t pNull) noexcept
        {
            *this = pNull;
        }

        // Free owned resource, claim passed resource (copy).
        inline void Set(T* pT) noexcept
        {
            *this = pT;
        }

        // Free owned resource, claim passed resource (move).
        inline void Set(ExclusivePointer&& src)
        {
            *this = std::move(src);
        }

        // Return pointer to owned resource.
        inline T* Get( ) const noexcept
        {
            return mPtr;
        }
    };

    /// Non-member ExclusivePointer Relational Ops \\\

    // ExclusivePointer<T1> vs ExclusivePointer<T2>
    template <class T1, class T2>
    inline bool operator==(const ExclusivePointer<T1>& lhs, const ExclusivePointer<T2>& rhs) noexcept
    {
        return lhs.Get( ) == rhs.Get( );
    }

    template <class T1, class T2>
    inline bool operator!=(const ExclusivePointer<T1>& lhs, const ExclusivePointer<T2>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T1, class T2>
    inline bool operator<(const ExclusivePointer<T1>& lhs, const ExclusivePointer<T2>& rhs) noexcept
    {
        return lhs.Get( ) < rhs.Get( );
    }

    template <class T1, class T2>
    inline bool operator>(const ExclusivePointer<T1>& lhs, const ExclusivePointer<T2>& rhs) noexcept
    {
        return lhs.Get( ) > rhs.Get( );
    }

    template <class T1, class T2>
    inline bool operator<=(const ExclusivePointer<T1>& lhs, const ExclusivePointer<T2>& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T1, class T2>
    inline bool operator>=(const ExclusivePointer<T1>& lhs, const ExclusivePointer<T2>& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // ExclusivePointer<T> vs Raw Pointer
    template <class T>
    inline bool operator==(const ExclusivePointer<T>& lhs, const T* const rhs) noexcept
    {
        return lhs.Get( ) == rhs;
    }

    template <class T>
    inline bool operator!=(const ExclusivePointer<T>& lhs, const T* const rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const ExclusivePointer<T>& lhs, const T* const rhs) noexcept
    {
        return lhs.Get( ) < rhs;
    }

    template <class T>
    inline bool operator>(const ExclusivePointer<T>& lhs, const T* const rhs) noexcept
    {
        return lhs.Get( ) > rhs;
    }

    template <class T>
    inline bool operator<=(const ExclusivePointer<T>& lhs, const T* const rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const ExclusivePointer<T>& lhs, const T* const rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // ExclusivePointer<T[ ]> vs Raw Pointer
    template <class T>
    inline bool operator==(const ExclusivePointer<T[ ]>& lhs, const T* const rhs) noexcept
    {
        return lhs.Get( ) == rhs;
    }

    template <class T>
    inline bool operator!=(const ExclusivePointer<T[ ]>& lhs, const T* const rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const ExclusivePointer<T[ ]>& lhs, const T* const rhs) noexcept
    {
        return lhs.Get( ) < rhs;
    }

    template <class T>
    inline bool operator>(const ExclusivePointer<T[ ]>& lhs, const T* const rhs) noexcept
    {
        return lhs.Get( ) > rhs;
    }

    template <class T>
    inline bool operator<=(const ExclusivePointer<T[ ]>& lhs, const T* const rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const ExclusivePointer<T[ ]>& lhs, const T* const rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // Raw Pointer vs ExclusivePointer<T>
    template <class T>
    inline bool operator==(const T* const lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return lhs == rhs.Get( );
    }

    template <class T>
    inline bool operator!=(const T* const lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const T* const lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return lhs < rhs.Get( );
    }

    template <class T>
    inline bool operator>(const T* const lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return lhs > rhs.Get( );
    }

    template <class T>
    inline bool operator<=(const T* const lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const T* const lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // Raw Pointer vs ExclusivePointer<T[ ]>
    template <class T>
    inline bool operator==(const T* const lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return lhs == rhs.Get( );
    }

    template <class T>
    inline bool operator!=(const T* const lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const T* const lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return lhs < rhs.Get( );
    }

    template <class T>
    inline bool operator>(const T* const lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return lhs > rhs.Get( );
    }

    template <class T>
    inline bool operator<=(const T* const lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const T* const lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // ExclusivePointer<T> vs nullptr
    template <class T>
    inline bool operator==(const ExclusivePointer<T>& lhs, const std::nullptr_t rhs) noexcept
    {
        return lhs.Get( ) == rhs;
    }

    template <class T>
    inline bool operator!=(const ExclusivePointer<T>& lhs, const std::nullptr_t rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const ExclusivePointer<T>& lhs, const std::nullptr_t rhs) noexcept
    {
        return lhs.Get( ) < rhs;
    }

    template <class T>
    inline bool operator>(const ExclusivePointer<T>& lhs, const std::nullptr_t rhs) noexcept
    {
        return lhs.Get( ) > rhs;
    }

    template <class T>
    inline bool operator<=(const ExclusivePointer<T>& lhs, const std::nullptr_t rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const ExclusivePointer<T>& lhs, const std::nullptr_t rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // ExclusivePointer<T[ ]> vs nullptr
    template <class T>
    inline bool operator==(const ExclusivePointer<T[ ]>& lhs, const std::nullptr_t rhs) noexcept
    {
        return lhs.Get( ) == rhs;
    }

    template <class T>
    inline bool operator!=(const ExclusivePointer<T[ ]>& lhs, const std::nullptr_t rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const ExclusivePointer<T[ ]>& lhs, const std::nullptr_t rhs) noexcept
    {
        return lhs.Get( ) < rhs;
    }

    template <class T>
    inline bool operator>(const ExclusivePointer<T[ ]>& lhs, const std::nullptr_t rhs) noexcept
    {
        return lhs.Get( ) > rhs;
    }

    template <class T>
    inline bool operator<=(const ExclusivePointer<T[ ]>& lhs, const std::nullptr_t rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const ExclusivePointer<T[ ]>& lhs, const std::nullptr_t rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // nullptr vs ExclusivePointer<T>
    template <class T>
    inline bool operator==(const std::nullptr_t lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return lhs == rhs.Get( );
    }

    template <class T>
    inline bool operator!=(const std::nullptr_t lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const std::nullptr_t lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return lhs < rhs.Get( );
    }

    template <class T>
    inline bool operator>(const std::nullptr_t lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return lhs > rhs.Get( );
    }

    template <class T>
    inline bool operator<=(const std::nullptr_t lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const std::nullptr_t lhs, const ExclusivePointer<T>& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    // nullptr vs ExclusivePointer<T[ ]>
    template <class T>
    inline bool operator==(const std::nullptr_t lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return lhs == rhs.Get( );
    }

    template <class T>
    inline bool operator!=(const std::nullptr_t lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline bool operator<(const std::nullptr_t lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return lhs < rhs.Get( );
    }

    template <class T>
    inline bool operator>(const std::nullptr_t lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return lhs > rhs.Get( );
    }

    template <class T>
    inline bool operator<=(const std::nullptr_t lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <class T>
    inline bool operator>=(const std::nullptr_t lhs, const ExclusivePointer<T[ ]>& rhs) noexcept
    {
        return !(lhs < rhs);
    }
};
