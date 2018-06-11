#pragma once

#include <Uncopyable.h>

namespace TTL
{
    /// Forward Declaration
    template <class T>
    class Node;

    template <class T>
    class INode
    {
    public:
        INode( ) = default;
        virtual ~INode( ) = default;

        /// Operator Overloads \\\

        virtual Node<T>& operator=(Node<T>&&) = 0;

        /// Getters \\\

        virtual T& GetData( ) noexcept = 0;
        virtual Node<T>* GetNext( ) noexcept = 0;
        virtual const Node<T>* GetNext( ) const noexcept = 0;

        /// Setters \\\

        virtual void SetData(T&&) noexcept = 0;
        virtual void SetNext(Node<T>*) noexcept = 0;
    };
}