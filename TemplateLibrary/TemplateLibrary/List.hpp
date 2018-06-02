#pragma once

#include "DNode.hpp"

#include "IList.h"

namespace TTL
{
    template <class T>
    class List : public virtual IList<T>
    {
    private:
        DNode<T>* mHead;
        DNode<T>* mTail;

        size_t mSize;

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

        inline DNode<T>* GetNodeAtPos(const size_t pos)
        {
            size_t shiftCount = 0;
            DNode<T>* ptr = nullptr;
            std::function<void(void)> fShift;

            if ( pos <= (mSize >> 1) )
            {
                shiftCount = pos;
                ptr = mHead;
                fShift = [&ptr] ( ) -> void
                {
                    ptr = ptr->GetNext( );
                };
            }
            else
            {
                shiftCount = mSize - pos - 1;
                ptr = mTail;
                fShift = [&ptr] ( ) -> void
                {
                    ptr = ptr->GetPrev( );
                };
            }

            for ( size_t i = 0; i < shiftCount; i++ )
            {
                fShift( );
            }

            return ptr;
        }

        friend inline void InsertListCommon(List<T>& dest, DNode<T>* insertionPoint, List<T>&& src) noexcept
        {
            // Merge the source list into the destination list (adjust pointers).
            dest.LinkNodes(src.mHead, insertionPoint->GetPrev( ), src.mHead->GetNext( ));
            dest.LinkNodes(src.mTail, src.mTail->GetPrev( ), insertionPoint);

            // Increase destination list size.
            dest.mSize += src.mSize;

            // Disasscoiate merged resources from source list.
            src.mHead = nullptr;
            src.mTail = nullptr;
            src.mSize = 0;
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
            mSize(0)
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

        inline size_t Size( ) const noexcept
        {
            return mSize;
        }

        /// Public Methods \\\

        inline bool Empty( ) const noexcept
        {
            return mSize == 0;
        }

        inline void Clear( ) noexcept
        {
            for ( DNode<T>* p = mHead; p; p = mHead )
            {
                mHead = mHead->GetNext( );
                delete p;
            }

            mTail = nullptr;
            mSize = 0;
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
            src.mSize = 0;
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

            if ( mSize == 1 )
            {
                mHead = mTail = nullptr;
            }
            else
            {
                mHead = mHead->GetNext( );
                mHead->SetPrev(nullptr);
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

            if ( mSize == 1 )
            {
                mHead = mTail = nullptr;
            }
            else
            {
                mTail = mTail->GetPrev( );
                mTail->SetNext(nullptr);
            }

            mSize--;
            delete p;
        }

        inline void Insert(const size_t pos, const T& data)
        {
            if ( pos > mSize )
            {
                throw std::out_of_range("TTL::List<T>::Insert(const size_t, const T&) - Attempted to insert beyond list boundaries.");
            }

            if ( pos == 0 )
            {
                PrependDataCommon(BuildNode(data, nullptr, mHead));
            }
            else if ( pos == mSize )
            {
                AppendDataCommon(BuildNode(data, mTail, nullptr));
            }
            else
            {
                DNode<T>* ptr = GetNodeAtPos(pos);
                BuildNode(std::move(data), ptr->GetPrev( ), ptr);
                mSize++;
            }
        }

        inline void Insert(const size_t pos, T&& data)
        {
            if ( pos > mSize )
            {
                throw std::out_of_range("TTL::List<T>::Insert(const size_t, const T&) - Attempted to insert beyond list boundaries.");
            }

            if ( pos == 0 )
            {
                PrependDataCommon(BuildNode(std::move(data), nullptr, mHead));
            }
            else if ( pos == mSize )
            {
                AppendDataCommon(BuildNode(std::move(data), mTail, nullptr));
            }
            else
            {
                DNode<T>* ptr = GetNodeAtPos(pos);
                BuildNode(std::move(data), ptr->GetPrev( ), ptr);
                mSize++;
            }
        }

        inline void Insert(const size_t pos, const List<T>& src)
        {
            if ( pos > mSize )
            {
                throw std::out_of_range("TTL::List<T>::Insert(const size_t, const T&) - Attempted to insert beyond list boundaries.");
            }

            if ( pos == 0 )
            {
                Prepend(src);
            }
            else if ( pos == mSize )
            {
                Append(src);
            }
            else
            {
                InsertListCommon(*this, GetNodeAtPos(pos), List<T>(src));
            }
        }

        inline void Insert(const size_t pos, List<T>&& src)
        {
            if ( pos > mSize )
            {
                throw std::out_of_range("TTL::List<T>::Insert(const size_t, const T&) - Attempted to insert beyond list boundaries.");
            }

            if ( pos == 0 )
            {
                Prepend(std::move(src));
            }
            else if ( pos == mSize )
            {
                Append(std::move(src));
            }
            else
            {
                InsertListCommon(*this, GetNodeAtPos(pos), List<T>(std::move(src)));
            }
        }

        inline void Remove(const size_t pos)
        {
            DNode<T>* prev = nullptr;
            DNode<T>* next = nullptr;
            DNode<T>* del = nullptr;

            if ( Empty( ) )
            {
                throw std::out_of_range("TTL::List<T>::Remove(const size_t) - Attempted to remove an element from an empty list.");
            }
            else if ( pos >= mSize )
            {
                throw std::out_of_range("TTL::List<T>::Remove(const size_t) - Attempted to remove beyond list boundaries.");
            }

            if ( pos == 0 )
            {
                PopFront( );
            }
            else if ( pos == mSize - 1 )
            {
                PopBack( );
            }
            else
            {
                del = GetNodeAtPos(pos);
                prev = del->GetPrev( );
                next = del->GetNext( );

                if ( prev )
                {
                    prev->SetNext(next);
                }

                if ( next )
                {
                    next->SetPrev(prev);
                }

                delete del;
                del = nullptr;

                mSize--;
            }
        }
    };

}

