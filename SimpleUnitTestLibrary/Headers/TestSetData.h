#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <chrono>
#include <cstring>

// SUTL
#include "ResultType.h"


class TestSetData
{
private:

    static constexpr std::pair<const char*, size_t> ms_DefaultName{"Default Name", sizeof("Default Name") - 1};

    // Private Data Members //

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

    const char* m_pTestSetName;
    size_t m_TestSetNameLen;

    // Private Helper Functions //

public:
    // Ctors //
    
    constexpr TestSetData() noexcept :
        m_TotalTests(0),
        m_TestPassCount(0),
        m_TotalFailureCount(0),
        m_SetupFailureCount(0),
        m_TestFailureCount(0),
        m_CleanupFailureCount(0),
        m_SetupExceptionCount(0),
        m_TestExceptionCount(0),
        m_CleanupExceptionCount(0),
        m_UnhandledExceptionCount(0),
        m_TestSkipCount(0),
        m_RunDurationMs(0),
        m_pTestSetName(ms_DefaultName.first),
        m_TestSetNameLen(ms_DefaultName.second)
    { }

    // NOTE: Provided string must be valid for the lifespan of this object.
    explicit TestSetData(_In_z_ const char* testSetName) noexcept :
        TestSetData(testSetName, (!!testSetName) ? strlen(testSetName) : 0)
    { }

    // NOTE: Provided string must be valid for the lifespan of this object.
    constexpr TestSetData(_In_z_count_(testSetNameLen) const char* testSetName, _In_ const size_t testSetNameLen) noexcept :
        TestSetData()
    {
        if (!!testSetName)
        {
            m_pTestSetName = testSetName;
            m_TestSetNameLen = testSetNameLen;
        }
    }

    constexpr TestSetData(_Inout_ TestSetData&& src) noexcept :
        TestSetData()
    {
        *this = std::move(src);
    }


    // Dtor //

    ~TestSetData() noexcept = default;


    // Assignment Overloads //

    constexpr TestSetData& operator=(_Inout_ TestSetData&& src) noexcept
    {
        if (this == &src)
        {
            m_TotalTests = src.m_TotalTests;
            m_TestPassCount = src.m_TestPassCount;
            m_TotalFailureCount = src.m_TotalFailureCount;
            m_SetupFailureCount = src.m_SetupFailureCount;
            m_TestFailureCount = src.m_TestFailureCount;
            m_CleanupFailureCount = src.m_CleanupFailureCount;
            m_SetupExceptionCount = src.m_SetupExceptionCount;
            m_TestExceptionCount = src.m_TestExceptionCount;
            m_CleanupExceptionCount = src.m_CleanupExceptionCount;
            m_UnhandledExceptionCount = src.m_UnhandledExceptionCount;
            m_TestSkipCount = src.m_TestSkipCount;
            m_RunDurationMs = src.m_RunDurationMs;

            m_pTestSetName = src.m_pTestSetName;
            m_TestSetNameLen = src.m_TestSetNameLen;

            src.ClearAll();
        }

        return *this;
    }


    // Getters //

    constexpr uint32_t GetTotalTestCount() const noexcept
    {
        return m_TotalTests;
    }

    constexpr uint32_t GetTestPassCount() const noexcept
    {
        return m_TestPassCount;
    }

    constexpr uint32_t GetTotalFailureCount() const noexcept
    {
        return m_TotalFailureCount;
    }

    constexpr uint32_t GetSetupFailureCount() const noexcept
    {
        return m_SetupFailureCount;
    }

    constexpr uint32_t GetTestFailureCount() const noexcept
    {
        return m_TestFailureCount;
    }

    constexpr uint32_t GetCleanupFailureCount() const noexcept
    {
        return m_CleanupFailureCount;
    }

    constexpr uint32_t GetSetupExceptionCount() const noexcept
    {
        return m_SetupExceptionCount;
    }

    constexpr uint32_t GetTestExceptionCount() const noexcept
    {
        return m_TestExceptionCount;
    }

    constexpr uint32_t GetCleanupExceptionCount() const noexcept
    {
        return m_CleanupExceptionCount;
    }

    constexpr uint32_t GetUnhandledExceptionCount() const noexcept
    {
        return m_UnhandledExceptionCount;
    }

    constexpr uint32_t GetTestSkipCount() const noexcept
    {
        return m_TestSkipCount;
    }

    constexpr uint64_t GetRunDurationMs() const noexcept
    {
        return m_RunDurationMs;
    }

    constexpr const char* GetTestSetName() const noexcept
    {
        return m_pTestSetName;
    }

    constexpr size_t GetTestSetNameLength() const noexcept
    {
        return m_TestSetNameLen;
    }


    // Incrementers //

    constexpr void IncrementResultCounter(_In_range_(ResultType::Begin, ResultType::End - 1) const ResultType r) noexcept
    {
        switch (r)
        {
        case ResultType::NotRun:
            m_TestSkipCount++;
            break;

        case ResultType::Success:
            m_TestPassCount++;
            break;

        case ResultType::SetupFailure:
            m_SetupFailureCount++;
            m_TotalFailureCount++;
            break;

        case ResultType::SetupException:
            m_SetupExceptionCount++;
            m_TotalFailureCount++;
            break;

        case ResultType::TestFailure:
            m_TestFailureCount++;
            m_TotalFailureCount++;
            break;

        case ResultType::TestException:
            m_TestExceptionCount++;
            m_TotalFailureCount++;
            break;

        case ResultType::CleanupFailure:
            m_CleanupFailureCount++;
            m_TotalFailureCount++;
            break;

        case ResultType::CleanupException:
            m_CleanupExceptionCount++;
            m_TotalFailureCount++;
            break;

        case ResultType::UnhandledException:
            m_UnhandledExceptionCount++;
            m_TotalFailureCount++;
            break;

        default:
            return;
        }
    }

    constexpr void AddToRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept
    {
        if (dur.count() > 0)
        {
            m_RunDurationMs += static_cast<uint64_t>(dur.count());
        }
    }


    // Setters //

    constexpr void SetTotalTestCount(_In_ const uint32_t& c) noexcept
    {
        m_TotalTests = c;
    }

    constexpr void SetRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept
    {
        m_RunDurationMs = (dur.count() > 0) ? static_cast<uint64_t>(dur.count()) : 0;
    }

    // NOTE: Provided string must be valid for the lifespan of this object.
    inline void SetTestSetName(_In_z_ const char* name) noexcept
    {
        if (!!name)
        {
            SetTestSetName(name, strlen(name));
        }
        else
        {
            SetTestSetName(ms_DefaultName.first, ms_DefaultName.second);
        }
    }

    // NOTE: Provided string must be valid for the lifespan of this object.
    constexpr void SetTestSetName(_In_z_count_(len) const char* name, _In_ const size_t len) noexcept
    {
        m_pTestSetName = name;
        m_TestSetNameLen = len;
    }


    // Public Methods //

    constexpr void ResetCounters() noexcept
    {
        m_TotalTests = 0;
        m_TestPassCount = 0;
        m_TotalFailureCount = 0;
        m_SetupFailureCount = 0;
        m_TestFailureCount = 0;
        m_CleanupFailureCount = 0;
        m_SetupExceptionCount = 0;
        m_TestExceptionCount = 0;
        m_CleanupExceptionCount = 0;
        m_UnhandledExceptionCount = 0;
        m_TestSkipCount = 0;
        m_RunDurationMs = 0;
    }

    constexpr void ClearAll() noexcept
    {
        ResetCounters();
        m_pTestSetName = nullptr;
        m_TestSetNameLen = 0;
    }

    constexpr long double GetTestSetGrade() const noexcept
    {
        const uint32_t testRunCount = m_TotalTests - m_TestSkipCount;
        return (testRunCount > 0) ? 100.0L * static_cast<long double>(m_TestPassCount) / static_cast<long double>(testRunCount) : 0.0L;
    }
};
