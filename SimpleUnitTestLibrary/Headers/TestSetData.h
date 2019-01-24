#pragma once

#include "../Common/Headers/Types.h"

#include "ResultType.h"

#include <string>

template <class T>
class TestSetData
{
private:
    uint32 mTotalTests;
    uint32 mTestPassCount;
    uint32 mTotalFailureCount;
    uint32 mSetupFailureCount;
    uint32 mTestFailureCount;
    uint32 mCleanupFailureCount;
    uint32 mSetupExceptionCount;
    uint32 mTestExceptionCount;
    uint32 mCleanupExceptionCount;
    uint32 mUnhandledExceptionCount;
    uint32 mTestSkipCount;

    std::basic_string<T> mTestSetName;

public:
    /// Ctors \\\

    explicit TestSetData(_In_ const std::basic_string<T>& testSetName = std::basic_string<T>( )) noexcept;
    explicit TestSetData(_In_ std::basic_string<T>&& testSetName) noexcept;
    TestSetData(_In_ const TestSetData& src) noexcept;
    TestSetData(_In_ TestSetData&& src) noexcept;

    /// Dtor \\\

    ~TestSetData( ) = default;

    /// Assignment Overloads \\\

    TestSetData& operator=(_In_ const TestSetData& src) noexcept;
    TestSetData& operator=(_In_ TestSetData&& src) noexcept;

    /// Getters \\\

    uint32 GetTotalTestCount( ) const noexcept;
    uint32 GetTestPassCount( ) const noexcept;
    uint32 GetTotalFailureCount( ) const noexcept;
    uint32 GetSetupFailureCount( ) const noexcept;
    uint32 GetTestFailureCount( ) const noexcept;
    uint32 GetCleanupFailureCount( ) const noexcept;
    uint32 GetSetupExceptionCount( ) const noexcept;
    uint32 GetTestExceptionCount( ) const noexcept;
    uint32 GetCleanupExceptionCount( ) const noexcept;
    uint32 GetUnhandledExceptionCount( ) const noexcept;
    uint32 GetTestSkipCount( ) const noexcept;

    const std::basic_string<T>& GetTestSetName( ) const noexcept;

    /// Incrementers \\\

    void IncrementResultCounter(_In_ const ResultType& r);

    /// Setters \\\

    void SetTotalTestCount(_In_ const uint32& c) noexcept;

    void SetTestSetName(_In_ const std::basic_string<T>& name);
    void SetTestSetName(_In_ std::basic_string<T>&& name) noexcept;

    /// Public Methods \\\

    void ResetCounters( ) noexcept;
    void ClearAll( ) noexcept;
    long double GetTestSetGrade( ) const noexcept;
};
