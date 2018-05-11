#pragma once

#include "Uncopyable.h"

namespace TTL
{
    template <class T>
    class DNode : public Uncopyable
    {
    private:
        T mData{};
        DNode<T>* mPrev;
        DNode<T>* mNext;

    public:
        /// Ctors \\\

        // Default
        explicit DNode( ) noexcept :
            mPrev(nullptr),
            mNext(nullptr)
        { }

        // Move
        DNode(DNode&& src) noexcept
        {
            *this = std::move(src);
        }

        /// Dtor \\\

        // DNode is an "internal" data structure.
        // As such, it assumes no responsiblity for memory management.
        ~DNode( ) = default;

        /// Operator Overloads \\\

        inline DNode& operator=(DNode&& src) noexcept
        {
            mData = std::move(src.mData);
            mPrev = src.mPrev;
            mNext = src.mNext;

            src.mData = T( );
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
            d = T( );
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