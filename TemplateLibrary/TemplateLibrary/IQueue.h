#pragma once

namespace TTL
{
    /// Forward Declarations
    template <class T>
    class Node;

    template <class T>
    class Queue;

    template <class T>
    class IQueue
    {
    public:
        IQueue( ) = default;
        virtual ~IQueue( ) = default;

        /// Operator Overloads \\\

        virtual Queue<T>& operator=(const Queue<T>&) = 0;
        virtual Queue<T>& operator=(Queue<T>&&) = 0;

        virtual Queue<T>& operator+=(const T&) = 0;
        virtual Queue<T>& operator+=(T&&) = 0;
        virtual Queue<T>& operator+=(const Queue<T>&) = 0;
        virtual Queue<T>& operator+=(Queue<T>&&) = 0;

        /// Getters \\\

        virtual T& Front( ) = 0;
        virtual const T& Front( ) const = 0;

        virtual const Node<T>* FrontPtr( ) const noexcept = 0;

        /// Public Methods \\\

        virtual bool Empty( ) const noexcept = 0;
        virtual size_t Size( ) const noexcept = 0;

        virtual void Clear( ) noexcept = 0;

        virtual void Enqueue(const T&) = 0;
        virtual void Enqueue(T&&) = 0;

        virtual void Enqueue(const Queue<T>&) = 0;
        virtual void Enqueue(Queue<T>&&) noexcept = 0;

        virtual void Dequeue( ) = 0;
    };
}