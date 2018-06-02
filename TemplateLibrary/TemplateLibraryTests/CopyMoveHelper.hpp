#pragma once

///
//
//
//  Class   -   CopyMoveHelper
//
//  Purpose -   Used in tests to ensure copy and/or move methods work as intended.
//
//
///
class CopyMoveHelper
{
private:
    bool mCopy;
    bool mMove;

public:
    // Default Ctor
    CopyMoveHelper() noexcept :
        mCopy(false),
        mMove(false)
    { }

    // Copy Ctor
    CopyMoveHelper(const CopyMoveHelper&) :
        mCopy(true),
        mMove(false)
    { }

    // Move Ctor
    CopyMoveHelper(CopyMoveHelper&&) noexcept :
        mCopy(false),
        mMove(true)
    { }

    // Default Dtor
    ~CopyMoveHelper( ) = default;

    // Copy Assignment
    CopyMoveHelper& operator=(const CopyMoveHelper&) noexcept
    {
        mCopy = true;
        mMove = false;
        return *this;
    }

    // Move Assignment
    CopyMoveHelper& operator=(CopyMoveHelper&&) noexcept
    {
        mCopy = false;
        mMove = true;
        return *this;
    }

    // Getters
    bool GetCopy( ) const
    {
        return mCopy;
    }

    bool GetMove( ) const
    {
        return mMove;
    }

    // Setters
    void ResetCopy( )
    {
        mCopy = false;
    }

    void ResetMove( )
    {
        mMove = false;
    }

    void Reset( )
    {
        mCopy = mMove = false;
    }
};