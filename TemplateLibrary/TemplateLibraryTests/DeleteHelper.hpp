#pragma once

class DeleteHelper
{
private:
    static size_t smDeleteCount;

public:
    DeleteHelper( ) noexcept = default;
    DeleteHelper(const DeleteHelper&) noexcept = default;
    DeleteHelper(DeleteHelper&&) noexcept = default;

    ~DeleteHelper( )
    {
        smDeleteCount++;
    }

    DeleteHelper& operator=(const DeleteHelper&) noexcept = default;
    DeleteHelper& operator=(DeleteHelper&&) noexcept = default;

    static void SetDeleteCount(const size_t d) noexcept
    {
        smDeleteCount = d;
    }

    static const size_t& GetDeleteCount( ) noexcept
    {
        return smDeleteCount;
    }
};