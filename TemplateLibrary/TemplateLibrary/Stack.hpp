#pragma once

#include "Array.hpp"
#include "Node.hpp"
#include "IStack.h"

namespace TTL
{
    template <class T>
    class Stack : public virtual IStack<T>
    {
    private:
        Node<T>* mTop;

        size_t mSize;

        inline Node<T>* BuildNode(const T& data)
        {
            Node<T>* ptr = new Node<T>;
            ptr->SetData(data);
            return ptr;
        }

        inline Node<T>* BuildNode(T&& data)
        {
            Node<T>* ptr = new Node<T>;
            ptr->SetData(std::move(data));
            return ptr;
        }

        inline Stack BuildCopy( ) const
        {
            const Node<T>* ptr = mTop;
            Array<T> arr(mSize);
            Stack temp;

            if ( !ptr )
            {
                return temp;
            }

            for ( size_t i = 0; i < arr.Capacity( ); i++ )
            {
                arr[arr.Capacity( ) - i - 1] = ptr->GetData( );
                ptr = ptr->GetNext( );
            }

            for ( size_t i = 0; i < arr.Capacity( ); i++ )
            {
                temp.Push(std::move(arr[i]));
            }

            return temp;
        }

        inline void PushCommon(Node<T>* ptr)
        {
            ptr->SetNext(mTop);
            mTop = ptr;
            mSize++;
        }

    public:
        Stack( ) noexcept :
            mTop(nullptr),
            mSize(0)
        { }

        Stack(const Stack& src) :
            Stack( )
        {
            *this = src;
        }

        Stack(Stack&& src) noexcept :
            Stack( )
        {
            *this = std::move(src);
        }

        ~Stack( )
        {
            Clear( );
        }

        inline Stack& operator=(const Stack& src)
        {
            return *this = std::move(src.BuildCopy( ));
        }

        inline Stack& operator=(Stack&& src) noexcept
        {
            Clear( );

            mTop = src.mTop;
            mSize = src.mSize;

            src.mTop = nullptr;
            src.mSize = 0;

            return *this;
        }

        /// Getters \\\

        inline T& Top( )
        {
            if ( mSize == 0 )
            {
                throw std::logic_error("TTL::Stack<T>::Top( ) - Attempted to access empty stack.");
            }

            return mTop->GetData( );
        }

        inline const T& Top( ) const
        {
            if ( mSize == 0 )
            {
                throw std::logic_error("TTL::Stack<T>::Top( ) - Attempted to access empty stack.");
            }

            return mTop->GetData( );
        }

        inline const Node<T>* TopPtr( ) const noexcept
        {
            return mTop;
        }

        /// Public Methods \\\

        inline bool Empty( ) const noexcept
        {
            return (mSize == 0);
        }

        inline size_t Size( ) const noexcept
        {
            return mSize;
        }

        inline void Clear( ) noexcept
        {
            Node<T>* del = mTop;
            while ( mTop )
            {
                mTop = mTop->GetNext( );
                delete del;
                del = mTop;
            }

            mSize = 0;
        }

        inline void Push(const T& data)
        {
            PushCommon(BuildNode(data));
        }

        inline void Push(T&& data)
        {
            PushCommon(BuildNode(std::move(data)));
        }

        inline void Pop( )
        {
            if ( mSize == 0 )
            {
                throw std::logic_error("TTL::Stack<T>::Pop( ) - Attempted to pop on an empty stack.");
            }

            Node<T>* del = mTop;
            mTop = mTop->GetNext( );
            delete del;
            mSize--;
        }

    };
}