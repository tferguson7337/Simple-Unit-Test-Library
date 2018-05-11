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
    CopyMoveHelper() :
        mCopy(false),
        mMove(false)
    { }

    // Copy Ctor
    CopyMoveHelper(const CopyMoveHelper&) :
        mCopy(true),
        mMove(false)
    { }

    // Move Ctor
    CopyMoveHelper(CopyMoveHelper&&) :
        mCopy(false),
        mMove(true)
    { }

    // Default Dtor
    ~CopyMoveHelper( ) = default;

    // Copy Assignment
    CopyMoveHelper& operator=(const CopyMoveHelper&)
    {
        mCopy = true;
        return *this;
    }

    // Move Assignment
    CopyMoveHelper& operator=(CopyMoveHelper&&)
    {
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