#pragma once

///
//
//
//  Class   -   MemoryManagementHelper
//
//  Purpose -   Used in tests to check use of copy or move.
//              Also tracks number of instantiated object destructions.
//
//
///
class MemoryManagementHelper
{
private:
    bool mCopy;
    bool mMove;

    static size_t smDeleteCount;

public:
    // Default Ctor
    MemoryManagementHelper() noexcept :
        mCopy(false),
        mMove(false)
    { }

    // Copy Ctor
    MemoryManagementHelper(const MemoryManagementHelper&) noexcept :
        mCopy(true),
        mMove(false)
    { }

    // Move Ctor
    MemoryManagementHelper(MemoryManagementHelper&&) noexcept :
        mCopy(false),
        mMove(true)
    { }

    // Default Dtor
    ~MemoryManagementHelper( )
    {
        smDeleteCount++;
    }

    // Copy Assignment
    MemoryManagementHelper& operator=(const MemoryManagementHelper&) noexcept
    {
        mCopy = true;
        mMove = false;
        return *this;
    }

    // Move Assignment
    MemoryManagementHelper& operator=(MemoryManagementHelper&&) noexcept
    {
        mCopy = false;
        mMove = true;
        return *this;
    }

    // Getters
    bool GetCopy( ) const noexcept
    {
        return mCopy;
    }

    bool GetMove( ) const noexcept
    {
        return mMove;
    }

    // Setters
    void ResetCopy( ) noexcept
    {
        mCopy = false;
    }

    void ResetMove( ) noexcept
    {
        mMove = false;
    }

    void Reset( ) noexcept
    {
        mCopy = mMove = false;
    }

    static size_t ResetDeleteCount( ) noexcept
    {
        size_t oldCount = smDeleteCount;
        smDeleteCount = 0;
        return oldCount;
    }
};