#pragma once

namespace TTL
{
    /// Forward Declaration - DNode
    template <class T>
    class DNode;

    /// Forward Declaration - List
    template <class T>
    class List;

    template <class T>
    class IList
    {
    public:
        IList( ) = default;
        virtual ~IList( ) = default;

        /// Assignment Overloads \\\

        virtual List<T>& operator=(const List<T>&) = 0;
        virtual List<T>& operator=(List<T>&&) noexcept = 0;

        virtual List<T>& operator+=(const T&) = 0;
        virtual List<T>& operator+=(T&&) = 0;

        virtual List<T>& operator+=(const List<T>&) = 0;
        virtual List<T>& operator+=(List<T>&&) noexcept = 0;

        /// Getters \\\

        virtual const DNode<T>* GetHead( ) const noexcept = 0;
        virtual const DNode<T>* GetTail( ) const noexcept = 0;
        virtual size_t Size( ) const noexcept = 0;

        /// Public Methods \\\

        virtual bool Empty( ) const noexcept = 0;
        virtual void Clear( ) noexcept = 0;

        virtual T& Front( ) = 0;
        virtual const T& Front( ) const = 0;

        virtual T& Back( ) = 0;
        virtual const T& Back( ) const = 0;

        virtual void PopFront( ) noexcept = 0;
        virtual void PopBack( ) noexcept = 0;
        virtual void Remove(const size_t) = 0;

        virtual void Append(const T&) = 0;
        virtual void Append(T&&) = 0;
        virtual void Append(const List<T>&) = 0;
        virtual void Append(List<T>&&) noexcept = 0;

        virtual void Prepend(const T&) = 0;
        virtual void Prepend(T&&) = 0;
        virtual void Prepend(const List<T>&) = 0;
        virtual void Prepend(List<T>&&) noexcept = 0;

        virtual void Insert(const size_t, const T&) = 0;
        virtual void Insert(const size_t, T&&) = 0;
        virtual void Insert(const size_t, const List<T>&) = 0;
        virtual void Insert(const size_t, List<T>&&) = 0;
    };
}