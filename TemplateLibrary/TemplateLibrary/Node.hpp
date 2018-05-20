#pragma once

#include "Uncopyable.h"

#include "INode.h"

namespace TTL
{
    template <class T>
    class Node : public virtual INode<T>, public Uncopyable
    {
    private:
        T mData{};
        Node* mNext;

    public:
        /// Ctors \\\

        // Default
        explicit Node( ) noexcept :
            mNext(nullptr)
        { }

        // Move
        Node(Node&& src) noexcept :
            Node( )
        {
            *this = std::move(src);
        }

        /// Dtor \\\

        // Node is an "internal" data structure.
        // As such, it assumes no responsiblity for memory management.
        ~Node( ) = default;

        /// Operator Overloads \\\

        inline Node& operator=(Node&& src) noexcept
        {
            mData = std::move(src.mData);
            mNext = src.mNext;

            src.mData = T( );
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

        inline Node<T>* GetNext( ) noexcept
        {
            return mNext;
        }

        inline const Node<T>* GetNext( ) const noexcept
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

        // Raw Pointer
        inline void SetNext(Node<T>* pNode) noexcept
        {
            mNext = pNode;
        }
    };
}
