#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <chrono>
#include <cstring>
#include <string_view>

// SUTL
#include "ResultType.h"


namespace SUTL
{
    class TestSetData
    {
    private:

        static constexpr std::string_view s_DefaultName{"Default Name"};

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

        std::string_view m_TestSetName;

    public:
        // Ctors //

        // NOTE: Provided string must be valid for the lifespan of this object.
        constexpr TestSetData(_In_ std::string_view testSetName = std::string_view{}) noexcept :
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
            m_TestSetName(testSetName.empty() ? s_DefaultName : testSetName)
        { }

        // Dtor //

        constexpr ~TestSetData() noexcept = default;


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

        constexpr std::string_view GetTestSetName() const noexcept
        {
            return m_TestSetName;
        }


        // Incrementers //

        constexpr void IncrementResultCounter(_In_range_(ResultType::_Begin, ResultType::_Last) const ResultType r) noexcept
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

        constexpr void AddToRunDurationMs(_In_ const uint64_t ms) noexcept
        {
            m_RunDurationMs += ms;
        }


        // Setters //

        constexpr void SetTotalTestCount(_In_ const uint32_t c) noexcept
        {
            m_TotalTests = c;
        }

        constexpr void SetTestSetName(_In_ std::string_view name) noexcept
        {
            m_TestSetName = name;
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

        constexpr double GetTestSetGrade() const noexcept
        {
            const uint32_t testRunCount = m_TotalTests - m_TestSkipCount;
            return (testRunCount > 0) ? 100.0 * static_cast<double>(m_TestPassCount) / static_cast<double>(testRunCount) : 0.0;
        }
    };
}