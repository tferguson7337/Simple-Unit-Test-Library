#pragma once

namespace TTL
{
    /// Forward Declarations
    template <class T>
    class Stack;

    template <class T>
    class Node;

    template <class T>
    class IStack
    {
    public:
        IStack( ) = default;
        virtual ~IStack( ) = default;

        /// Operator Overloads \\\

        virtual Stack<T>& operator=(const Stack<T>&) = 0;
        virtual Stack<T>& operator=(Stack<T>&&) noexcept = 0;

        /// Getters \\\

        virtual T& Top( ) = 0;
        virtual const T& Top( ) const = 0;

        virtual const Node<T>* TopPtr( ) const noexcept = 0;

        /// Public Methods \\\

        virtual bool Empty( ) const noexcept = 0;
        virtual size_t Size( ) const noexcept = 0;

        virtual void Clear( ) noexcept = 0;

        virtual void Push(const T&) = 0;
        virtual void Push(T&&) = 0;

        virtual void Pop( ) = 0;
    };
}