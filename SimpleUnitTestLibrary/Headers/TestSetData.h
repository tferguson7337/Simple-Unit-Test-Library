#pragma once

#include "ResultType.h"

#include <string>

template <class T>
class TestSetData
{
private:
    uint32_t mTotalTests;
    uint32_t mTestPassCount;
    uint32_t mTotalFailureCount;
    uint32_t mSetupFailureCount;
    uint32_t mTestFailureCount;
    uint32_t mCleanupFailureCount;
    uint32_t mSetupExceptionCount;
    uint32_t mTestExceptionCount;
    uint32_t mCleanupExceptionCount;
    uint32_t mUnhandledExceptionCount;
    uint32_t mTestSkipCount;

    std::basic_string<T> mTestSetName;

public:
    /// Ctors \\\

    explicit TestSetData(_In_ const std::basic_string<T>& testSetName = std::basic_string<T>()) noexcept;
    explicit TestSetData(_In_ std::basic_string<T>&& testSetName) noexcept;
    TestSetData(_In_ const TestSetData& src) noexcept;
    TestSetData(_In_ TestSetData&& src) noexcept;

    /// Dtor \\\

    ~TestSetData() = default;

    /// Assignment Overloads \\\

    TestSetData& operator=(_In_ const TestSetData& src) noexcept;
    TestSetData& operator=(_In_ TestSetData&& src) noexcept;

    /// Getters \\\

    uint32_t GetTotalTestCount() const noexcept;
    uint32_t GetTestPassCount() const noexcept;
    uint32_t GetTotalFailureCount() const noexcept;
    uint32_t GetSetupFailureCount() const noexcept;
    uint32_t GetTestFailureCount() const noexcept;
    uint32_t GetCleanupFailureCount() const noexcept;
    uint32_t GetSetupExceptionCount() const noexcept;
    uint32_t GetTestExceptionCount() const noexcept;
    uint32_t GetCleanupExceptionCount() const noexcept;
    uint32_t GetUnhandledExceptionCount() const noexcept;
    uint32_t GetTestSkipCount() const noexcept;

    const std::basic_string<T>& GetTestSetName() const noexcept;

    /// Incrementers \\\

    void IncrementResultCounter(_In_ const ResultType& r);

    /// Setters \\\

    void SetTotalTestCount(_In_ const uint32_t& c) noexcept;

    void SetTestSetName(_In_ const std::basic_string<T>& name);
    void SetTestSetName(_In_ std::basic_string<T>&& name) noexcept;

    /// Public Methods \\\

    void ResetCounters() noexcept;
    void ClearAll() noexcept;
    long double GetTestSetGrade() const noexcept;
};
