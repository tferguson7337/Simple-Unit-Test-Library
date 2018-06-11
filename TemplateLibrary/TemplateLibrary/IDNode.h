#pragma once

namespace TTL
{
    /// Forward Declaration
    template <class T>
    class DNode;

    template <class T>
    class IDNode
    {
    public:
        IDNode( ) = default;
        virtual ~IDNode( ) = default;

        /// Operator Overloads \\\

        virtual DNode<T>& operator=(DNode<T>&&) = 0;

        /// Getters \\\

        virtual T& GetData( ) noexcept = 0;
        virtual const T& GetData( ) const noexcept = 0;
        virtual DNode<T>* GetPrev( ) noexcept = 0;
        virtual const DNode<T>* GetPrev( ) const noexcept = 0;
        virtual DNode<T>* GetNext( ) noexcept = 0;
        virtual const DNode<T>* GetNext( ) const noexcept = 0;

        /// Setters \\\

        virtual void SetData(T&&) noexcept = 0;
        virtual void SetPrev(DNode<T>*) noexcept = 0;
        virtual void SetNext(DNode<T>*) noexcept = 0;
    };
}