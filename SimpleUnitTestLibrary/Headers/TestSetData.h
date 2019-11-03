#pragma once

// STL
#include <chrono>
#include <string>

// SUTL
#include "ResultType.h"


class TestSetData
{
private:
    uint32_t m_TotalTests;
    uint32_t m_TestPassCount;
    uint32_t m_TotalFailureCount;
    uint32_t m_SetupFailureCount;
    uint32_t m_TestFailureCount;
    uint32_t m_CleanupFailureCount;
    uint32_t m_SetupExceptionCount;
    uint32_t m_TestExceptionCount;
    uint32_t m_CleanupExceptionCount;
    uint32_t m_UnhandledExceptionCount;
    uint32_t m_TestSkipCount;
    uint64_t m_RunDurationMs;
    std::wstring m_TestSetName;

    TestSetData() noexcept;

public:
    /// Ctors \\\

    explicit TestSetData(_Inout_ std::wstring&& testSetName) noexcept;
    TestSetData(_In_ const TestSetData& src);
    TestSetData(_Inout_ TestSetData&& src) noexcept;

    /// Dtor \\\

    ~TestSetData() = default;

    /// Assignment Overloads \\\

    TestSetData& operator=(_In_ const TestSetData& src) noexcept;
    TestSetData& operator=(_Inout_ TestSetData&& src) noexcept;

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
    uint64_t GetRunDurationMs() const noexcept;

    const std::wstring& GetTestSetName() const noexcept;

    /// Incrementers \\\

    void IncrementResultCounter(_In_ const ResultType& r) noexcept;
    void AddToRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept;

    /// Setters \\\

    void SetTotalTestCount(_In_ const uint32_t& c) noexcept;
    void SetRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept;

    void SetTestSetName(_In_ std::wstring name);
    void SetTestSetName(_Inout_ std::wstring&& name) noexcept;

    /// Public Methods \\\

    void ResetCounters() noexcept;
    void ClearAll() noexcept;
    long double GetTestSetGrade() const noexcept;
};
