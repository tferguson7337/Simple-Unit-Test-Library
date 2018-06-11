#pragma once

#include "Uncopyable.h"

#include "IDNode.h"

namespace TTL
{
    template <class T>
    class DNode : public virtual IDNode<T>, public Uncopyable
    {
    private:
        T mData{};
        DNode<T>* mPrev;
        DNode<T>* mNext;

    public:
        /// Ctors \\\

        // Default
        DNode( ) noexcept :
            mPrev(nullptr),
            mNext(nullptr)
        { }

        // Move
        DNode(DNode&& src) noexcept :
            DNode( )
        {
            *this = std::move(src);
        }

        /// Dtor \\\

        // DNode is an "internal" data structure.
        // As such, it assumes no responsiblity for memory management.
        ~DNode( ) = default;

        /// Operator Overloads \\\

        inline DNode& operator=(DNode&& src)
        {
            if ( this == &src )
            {
                throw std::invalid_argument("TTL::DNode<T>::operator(DNode<T>&&) - Attempted self-assignment.");
            }

            mData = std::move(src.mData);
            mPrev = src.mPrev;
            mNext = src.mNext;

            src.mPrev = nullptr;
            src.mNext = nullptr;

            return *this;
        }

        /// Getters \\\

        inline T& GetData( ) noexcept
        {
            return mData;
        }

        inline const T& GetData( ) const noexcept
        {
            return mData;
        }

        inline DNode<T>* GetPrev( ) noexcept
        {
            return mPrev;
        }

        inline const DNode<T>* GetPrev( ) const noexcept
        {
            return mPrev;
        }

        inline DNode<T>* GetNext( ) noexcept
        {
            return mNext;
        }

        inline const DNode<T>* GetNext( ) const noexcept
        {
            return mNext;
        }

        /// Setters \\\

        // Copy
        inline void SetData(const T& d) noexcept
        {
            mData = d;
        }

        // Move
        inline void SetData(T&& d) noexcept
        {
            mData = std::move(d);
        }

        inline void SetPrev(DNode<T>* pNode) noexcept
        {
            mPrev = pNode;
        }

        // Raw Pointer
        inline void SetNext(DNode<T>* pNode) noexcept
        {
            mNext = pNode;
        }
    };
}