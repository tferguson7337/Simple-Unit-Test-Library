#pragma once

#include "DNode.hpp"

namespace TTL
{
    template <class T>
    class List
    {
    private:
        DNode<T>* mHead;
        DNode<T>* mTail;

        uint64 mSize;

        /// Private Helper Methods \\\

        inline DNode<T>* BuildNode(const T& d, DNode<T>* p, DNode<T>* n)
        {
            DNode<T>* pNode = new DNode<T>;
            pNode->SetData(d);
            LinkNodes(pNode, p, n);

            return pNode;
        }

        inline DNode<T>* BuildNode(T&& d, DNode<T>* p, DNode<T>* n)
        {
            DNode<T>* pNode = new DNode<T>;
            pNode->SetData(std::move(d));
            LinkNodes(pNode, p, n);

            return pNode;
        }

        inline void LinkNodes(DNode<T>* c, DNode<T>* p, DNode<T>* n) noexcept
        {
            if ( p )
            {
                p->SetNext(c);
                c->SetPrev(p);

                // Check if we're appending.
                if ( p == mTail )
                {
                    mTail = c;
                }
            }

            if ( n )
            {
                n->SetPrev(c);
                c->SetNext(n);

                // Check if we're prepending.
                if ( n == mHead )
                {
                    mHead = c;
                }
            }
        }

        inline void AppendDataCommon(DNode<T>* p) noexcept
        {
            if ( Empty( ) )
            {
                mHead = p;
            }

            mTail = p;
            mSize++;
        }

        inline void PrependDataCommon(DNode<T>* p) noexcept
        {
            if ( Empty( ) )
            {
                mTail = p;
            }

            mHead = p;
            mSize++;
        }

        inline List DeepCopyList(const List& src)
        {
            List copyList;
            const DNode<T>* p = src.GetHead( );
            while ( p )
            {
                copyList.Append(p->GetData( ));
                p = p->GetNext( );
            }

            return copyList;
        }

    public:
        /// Ctors \\\

        // Default
        explicit List( ) noexcept :
            mHead(nullptr),
            mTail(nullptr),
            mSize(0ull)
        {

        }

        // Copy
        List(const List& src) : List( )
        {
            *this = src;
        }

        // Move
        List(List&& src) noexcept : List( )
        {

            *this = std::move(src);
        }

        /// Dtor \\\

        ~List( )
        {
            Clear( );
        }

        /// Assignment Overload \\\

        // Copy
        inline List& operator=(const List& src)
        {
            List copyList;
            copyList.Append(src);
            *this = std::move(copyList);

            return *this;
        }

        // Move
        inline List& operator=(List&& src) noexcept
        {
            Clear( );
            Append(std::move(src));

            return *this;
        }

        // Append Copy Node
        inline List& operator+=(const T& d)
        {
            Append(d);
            return *this;
        }

        // Append Move Node
        inline List& operator+=(T&& d)
        {
            Append(std::move(d));
            return *this;
        }

        // Append Copy List
        inline List& operator+=(const List& src)
        {
            Append(src);
            return *this;
        }

        // Append Move List
        inline List& operator+=(List&& src) noexcept
        {
            Append(std::move(src));
            return *this;
        }

        /// Getters \\\

        inline const DNode<T>* GetHead( ) const noexcept
        {
            return mHead;
        }

        inline const DNode<T>* GetTail( ) const noexcept
        {
            return mTail;
        }

        inline uint64 Size( ) const noexcept
        {
            return mSize;
        }

        /// Public Methods \\\

        inline bool Empty( ) const noexcept
        {
            return mSize == 0ull;
        }

        inline void Clear( ) noexcept
        {
            for ( DNode<T>* p = mHead; p; p = mHead )
            {
                mHead = mHead->GetNext( );
                delete p;
            }

            mTail = nullptr;
            mSize = 0ull;
        }

        inline T& Front( )
        {
            if ( Empty( ) )
            {
                throw std::out_of_range("TTL::List<T>::Front( ) - Attempted to access empty list.");
            }

            return mHead->GetData( );
        }

        inline const T& Front( ) const
        {
            if ( Empty( ) )
            {
                throw std::out_of_range("TTL::List<T>::Front( ) - Attempted to access empty list.");
            }

            return mHead->GetData( );
        }

        inline T& Back( )
        {
            if ( Empty( ) )
            {
                throw std::out_of_range("TTL::List<T>::Back( ) - Attempted to access empty list.");
            }

            return mTail->GetData( );
        }

        inline const T& Back( ) const
        {
            if ( Empty( ) )
            {
                throw std::out_of_range("TTL::List<T>::Back( ) - Attempted to access empty list.");
            }

            return mTail->GetData( );
        }

        inline void Append(const T& d)
        {
            AppendDataCommon(BuildNode(d, mTail, nullptr));
        }

        inline void Append(T&& d)
        {
            AppendDataCommon(BuildNode(std::move(d), mTail, nullptr));
        }

        inline void Append(const List& src)
        {
            // Exit early on empty list.
            if ( src.Empty( ) )
            {
                return;
            }

            Append(std::move(DeepCopyList(src)));
        }

        inline void Append(List&& src) noexcept
        {
            // Check for empty src list - exit early if true.
            if ( src.Empty( ) )
            {
                return;
            }

            if ( Empty( ) )
            {
                mHead = src.mHead;
            }
            else
            {
                src.mHead->SetPrev(mTail);
                mTail->SetNext(src.mHead);
            }

            mTail = src.mTail;
            mSize += src.mSize;

            // Clear out src.
            src.mHead = nullptr;
            src.mTail = nullptr;
            src.mSize = 0ull;
        }

        inline void Prepend(const T& d)
        {
            PrependDataCommon(BuildNode(d, nullptr, mHead));
        }

        inline void Prepend(T&& d)
        {
            PrependDataCommon(BuildNode(std::move(d), nullptr, mHead));
        }

        inline void Prepend(const List& other)
        {
            List copyList(other);
            copyList.Append(std::move(*this));
            *this = std::move(copyList);
        }

        inline void Prepend(List&& other) noexcept
        {
            other.Append(std::move(*this));
            *this = std::move(other);
        }

        inline void PopFront( ) noexcept
        {
            if ( Empty( ) )
            {
                return;
            }

            DNode<T>* ptr = mHead;

            if ( mSize == 1ull )
            {
                mHead = mTail = nullptr;
            }
            else
            {
                mHead = mHead->GetNext( );
            }

            mSize--;
            delete ptr;
        }

        inline void PopBack( ) noexcept
        {
            if ( Empty( ) )
            {
                return;
            }

            DNode<T>* p = mTail;

            if ( mSize == 1ull )
            {
                mHead = mTail = nullptr;
            }
            else
            {
                mTail = mTail->GetPrev( );
            }

            mSize--;
            delete p;
        }
    };
}

