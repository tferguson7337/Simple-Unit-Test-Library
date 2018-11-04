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

    explicit TestSetData(const std::basic_string<T>& = std::basic_string<T>( )) noexcept;
    explicit TestSetData(std::basic_string<T>&&) noexcept;
    TestSetData(const TestSetData&) noexcept;
    TestSetData(TestSetData&&) noexcept;

    /// Dtor \\\

    ~TestSetData( ) = default;

    /// Assignment Overloads \\\

    TestSetData& operator=(const TestSetData&) noexcept;
    TestSetData& operator=(TestSetData&&) noexcept;

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

    void IncrementResultCounter(ResultType);

    /// Setters \\\

    void SetTotalTestCount(uint32) noexcept;

    void SetTestSetName(const std::basic_string<T>&);
    void SetTestSetName(std::basic_string<T>&&) noexcept;

    /// Public Methods \\\

    void ResetCounters( ) noexcept;
    void ClearAll( ) noexcept;
    long double GetTestSetGrade( ) const noexcept;
};
