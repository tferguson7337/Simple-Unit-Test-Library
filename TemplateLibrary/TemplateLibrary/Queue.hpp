#pragma once

#include "Node.hpp"
#include "IQueue.h"

namespace TTL
{
    template <class T>
    class Queue : public virtual IQueue<T>
    {
    private:
        Node<T>* mHead;
        Node<T>* mTail;

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

        inline Queue BuildCopy( ) const
        {
            const Node<T>* ptr = mHead;
            Queue copyQueue;

            while ( ptr )
            {
                copyQueue.Enqueue(ptr->GetData( ));
                ptr = ptr->GetNext( );
            }

            return copyQueue;
        }

        inline void EnqueueDataCommon(Node<T>* ptr)
        {
            if ( Empty( ) )
            {
                mHead = ptr;
            }
            else
            {
                mTail->SetNext(ptr);
            }

            mTail = ptr;
            mSize++;
        }

        inline void EnqueueQueueCommon(Queue&& src)
        {
            if ( Empty( ) )
            {
                *this = std::move(src);
            }
            else
            {
                mTail->SetNext(src.mHead);
                mTail = src.mTail;
                mSize += src.mSize;

                src.mHead = nullptr;
                src.mTail = nullptr;
                src.mSize = 0;
            }
        }

    public:

        /// Ctors/Dtor \\\

        Queue( ) noexcept :
            mHead(nullptr),
            mTail(nullptr),
            mSize(0)
        { }

        Queue(const Queue& src) :
            Queue( )
        {
            *this = src;
        }

        Queue(Queue&& src) noexcept :
            Queue( )
        {
            *this = std::move(src);
        }

        ~Queue( ) noexcept
        {
            Clear( );
        }

        /// Operator Overloads \\\

        inline Queue<T>& operator=(const Queue<T>& src)
        {
            return *this = std::move(src.BuildCopy( ));
        }

        inline Queue<T>& operator=(Queue<T>&& src) noexcept
        {
            Clear( );

            mHead = src.mHead;
            mTail = src.mTail;
            mSize = src.mSize;

            src.mHead = nullptr;
            src.mTail = nullptr;
            src.mSize = 0;

            return *this;
        }

        inline Queue<T>& operator+=(const T& data)
        {
            EnqueueDataCommon(BuildNode(data));
            return *this;
        }

        inline Queue<T>& operator+=(T&& data)
        {
            EnqueueDataCommon(BuildNode(std::move(data)));
            return *this;
        }

        inline Queue<T>& operator+=(const Queue<T>& src)
        {
            EnqueueQueueCommon(std::move(src.BuildCopy( )));
            return *this;
        }

        inline Queue<T>& operator+=(Queue<T>&& src)
        {
            EnqueueQueueCommon(std::move(src));
            return *this;
        }

        /// Getters \\\

        inline T& Front( )
        {
            if ( Empty( ) )
            {
                throw std::logic_error("TTL::Queue<T>::Front( ) - Attempted to access empty queue.");
            }

            return mHead->GetData( );
        }

        inline const T& Front( ) const
        {
            if ( Empty( ) )
            {
                throw std::logic_error("TTL::Queue<T>::Front( ) - Attempted to access empty queue.");
            }

            return mHead->GetData( );
        }

        inline const Node<T>* FrontPtr( ) const noexcept
        {
            return mHead;
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
            Node<T>* del = mHead;
            while ( del )
            {
                mHead = mHead->GetNext( );
                delete del;
                del = mHead;
            }

            mTail = nullptr;
            mSize = 0;
        }

        inline void Enqueue(const T& data)
        {
            EnqueueDataCommon(BuildNode(data));
        }

        inline void Enqueue(T&& data)
        {
            EnqueueDataCommon(BuildNode(std::move(data)));
        }

        inline void Enqueue(const Queue& src)
        {
            EnqueueQueueCommon(std::move(src.BuildCopy( )));
        }

        inline void Enqueue(Queue&& src) noexcept
        {
            EnqueueQueueCommon(std::move(src));
        }

        inline void Dequeue( )
        {
            if ( Empty( ) )
            {
                throw std::logic_error("TTL::Queue<T>::Dequeue( ) - Attempted to dequeue off an empty queue");
            }

            Node<T>* del = mHead;
            mHead = mHead->GetNext( );
            delete del;
            mSize--;

            if ( Empty( ) )
            {
                mTail = nullptr;
            }
        }
    };
}