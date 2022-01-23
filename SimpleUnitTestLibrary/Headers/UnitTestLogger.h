#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

// SUTL
#include "TestSetData.h"
#include "UnitTestResult.h"


namespace SUTL
{
    class UnitTestLogger
    {
        using Buffer = std::vector<char>;
        using Queue = std::list<Buffer>;

    private:

        // Static Private Data Members //

        // Format Strings //

        static constexpr std::string_view s_HeaderFormat
        {
            "================================"
            "================================\n"
            "  Test-Set Name: %-25.*s -- Total Tests: %u\n"
            "================================"
            "================================\n\n\n"
        };

        static constexpr std::string_view s_SummaryNoFailuresFormat
        {
            "===================="
            "====================\n"
            "    \"%.*s\" Complete\n\n"
            "  Total Test Count: %u\n"
            "  Total Tests Run:  %u\n\n"
            "   Successful Tests: %u\n\n"
            "   Failed Tests:     %u\n\n"
            "   Skipped Tests:    %u\n\n"
            "  Test Pass Grade:  %2.2f%% (%u / %u)\n"
            "  Total Duration:   %llu ms\n"
            "===================="
            "====================\n\n\n"
        };

        static constexpr std::string_view s_SummaryFailureDetailsFormat
        {
            "===================="
            "====================\n"
            "    \"%.*s\" Complete\n\n"
            "  Total Test Count: %u\n"
            "  Total Tests Run:  %u\n\n"
            "   Successful Tests: %u\n\n"
            "   Failed Tests:     %u\n"
            "    Setup Failures:       %u\n"
            "    Setup Exceptions:     %u\n"
            "    Test Failures:        %u\n"
            "    Test Exceptions:      %u\n"
            "    Cleanup Failures:     %u\n"
            "    Cleanup Exceptions:   %u\n"
            "    Unhandled Exceptions: %u\n\n"
            "   Skipped Tests:    %u\n\n"
            "  Test Pass Grade:  %2.2f%% (%u / %u)\n"
            "  Total Duration:   %llu ms\n"
            "===================="
            "====================\n\n\n"
        };

        static constexpr std::string_view s_SuccessFormat
        {
            "File: %.*s\nTest: %.*s\n"
            "--------------------------------"
            "--------------------------------\n"
            "    Result:   %.*s\n"
            "    Line:     %u\n"
            "    Duration: %llu microseconds\n"
            "--------------------------------"
            "--------------------------------\n\n\n"
        };

        static constexpr std::string_view s_FailureFormat
        {
            "File: %.*s\nTest: %.*s\n"
            "--------------------------------"
            "--------------------------------\n"
            "    Result:   %.*s\n"
            "    Line:     %u\n"
            "    Failure:  %.*s\n"
            "    Duration: %llu microseconds\n"
            "--------------------------------"
            "--------------------------------\n\n\n"
        };

        static constexpr std::string_view s_ExceptionFormat
        {
            "File: %.*s\nTest: %.*s\n"
            "--------------------------------"
            "--------------------------------\n"
            "    Result:   %.*s\n"
            "    Line:     %u\n"
            "    Reason:   %.*s\n"
            "    Duration: %llu microseconds\n"
            "--------------------------------"
            "--------------------------------\n\n\n"
        };

        static constexpr std::string_view s_SkipFormat
        {
            "File: %.*s\nTest: %.*s\n"
            "--------------------------------"
            "--------------------------------\n"
            "    Result:   %.*s\n"
            "    Line:     %u\n"
            "    Reason:   %.*s\n"
            "    Duration: %llu microseconds\n"
            "--------------------------------"
            "--------------------------------\n\n\n"
        };

        static constexpr std::string_view s_UnhandledExceptionFormat
        {
            "Result: %.*s\n"
            "--------------------------------"
            "--------------------------------\n"
            "    Unhandled Exception: %.*s\n"
            "    Duration:            %llu microseconds\n"
            "--------------------------------"
            "--------------------------------\n\n\n"
        };

        // Private Data Members //

        // Logging configuration
        bool m_bLogToStdout = true;
        bool m_bOnlyLogFailures = true;
        std::string m_LogFilePath;
        std::ofstream m_FileStream;

        // Logging queue
        Queue m_LogQueue;
        std::atomic<size_t> m_LogQueueSize = 0;

        // Logging thread.
        std::atomic<bool> m_bContinueWork = false;
        std::mutex m_LogQueueMutex;
        std::condition_variable m_LogCV;
        std::thread m_WorkerThread;

        // Private Logging Worker Thread Methods //

        void InitializeWorkerThread()
        {
            // Avoid re-initing thread.
            TeardownWorkerThread();
            m_bContinueWork = true;
            m_WorkerThread = std::thread(&UnitTestLogger::WorkerLoop, this);
        }

        void TeardownWorkerThread()
        {
            if (!m_WorkerThread.joinable())
            {
                return;
            }

            // We _must_ change the value under lock in order for the waiting thread(s) to be updated appropriately.
            {
                std::lock_guard<std::mutex> ul(m_LogQueueMutex);
                m_bContinueWork.store(false, std::memory_order_relaxed);
            }

            m_LogCV.notify_all();

            m_WorkerThread.join();
            m_LogQueueSize = 0;
        }

        bool WorkerPredicate() noexcept
        {
            // Note: This function is called within the context of a condition variable.
            // Due to this, the atomics are being accessed under lock, so relax the memory ordering for the loads.
            if (!m_bContinueWork.load(std::memory_order_relaxed))
            {
                // Stop sleeping if we need to be finishing up.
                return true;
            }

            if (m_LogQueueSize.load(std::memory_order_relaxed) >= 64)
            {
                // Stop sleeping if we have a sufficient amount of logs we need to deal with.
                return true;
            }

            // Keep sleeping.
            return false;
        }

        bool TerminatePredicate() noexcept
        {
            return (!m_bContinueWork) && (m_LogQueueSize == 0);
        }

        void WorkerLoop() noexcept
        {
            while (WaitForWork())
            {
                PrintLogs();
            }
        }

        bool WaitForWork() noexcept
        {
            std::unique_lock<std::mutex> ul(m_LogQueueMutex);
            m_LogCV.wait(ul, [this]() -> bool { return this->WorkerPredicate(); });
            return m_bContinueWork && (m_LogQueueSize == 0);
        }

        Queue TransferLogQueueContents() noexcept
        {
            Queue q;
            std::lock_guard<std::mutex> lg(m_LogQueueMutex);
            std::swap(q, m_LogQueue);
            m_LogQueueSize = 0;
            return q;
        }

        void PrintLogs() noexcept
        {
            for (const auto& buf : TransferLogQueueContents())
            {
                PrintLog(buf);
            }
        }

        void PrintLog(_In_ const Buffer& logMsg) noexcept
        {
            if (!logMsg.empty())
            {
                const std::string_view logMsgSV(logMsg.data(), logMsg.size() - 1);
                if (m_bLogToStdout)
                {
                    std::cout << logMsgSV;
                }

                if (!!m_FileStream)
                {
                    m_FileStream << logMsgSV;
                }
            }
        }

        // Private Static Helper Methods //

        static Buffer BuildTestSetHeaderString(_In_ const TestSetData& data) noexcept
        {
            const auto name{data.GetTestSetName()};
            return PrintFormat(
                s_HeaderFormat,
                static_cast<int>(name.length()),
                name.data(),
                data.GetTotalTestCount()
            );
        }

        static Buffer BuildTestSetSummaryNoFailuresString(_In_ const TestSetData& data) noexcept
        {
            const auto name{data.GetTestSetName()};
            return PrintFormat(
                s_SummaryNoFailuresFormat,
                static_cast<int>(name.length()),
                name.data(),
                data.GetTotalTestCount(),
                data.GetTotalTestCount() - data.GetTestSkipCount(),
                data.GetTestPassCount(),
                data.GetTotalFailureCount(),
                data.GetTestSkipCount(),
                data.GetTestSetGrade(),
                data.GetTestPassCount(),
                data.GetTotalTestCount() - data.GetTestSkipCount(),
                data.GetRunDurationMs()
            );
        }

        static Buffer BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData& data) noexcept
        {
            const auto name{data.GetTestSetName()};
            return PrintFormat(
                s_SummaryFailureDetailsFormat,
                static_cast<int>(name.length()),
                name.data(),
                data.GetTotalTestCount(),
                data.GetTotalTestCount() - data.GetTestSkipCount(),
                data.GetTestPassCount(),
                data.GetTotalFailureCount(),
                data.GetSetupFailureCount(),
                data.GetSetupExceptionCount(),
                data.GetTestFailureCount(),
                data.GetTestExceptionCount(),
                data.GetCleanupFailureCount(),
                data.GetCleanupExceptionCount(),
                data.GetUnhandledExceptionCount(),
                data.GetTestSkipCount(),
                data.GetTestSetGrade(),
                data.GetTestPassCount(),
                data.GetTotalTestCount() - data.GetTestSkipCount(),
                data.GetRunDurationMs()
            );
        }

        static Buffer BuildLogString(_In_ const UnitTestResult& res) noexcept
        {
            Buffer logStr;

            switch (res.GetResultType())
            {
            case ResultType::Success:
                logStr = BuildSuccessString(res);
                break;

            case ResultType::SetupFailure:
            case ResultType::TestFailure:
            case ResultType::CleanupFailure:
                logStr = BuildFailureString(res);
                break;

            case ResultType::SetupException:
            case ResultType::TestException:
            case ResultType::CleanupException:
                logStr = BuildExceptionString(res);
                break;

            case ResultType::NotRun:
                logStr = BuildSkipString(res);
                break;

            case ResultType::UnhandledException:
                logStr = BuildUnhandledExceptionString(res);
                break;

            case ResultType::_End:
            default:
                logStr.push_back('\0');
            }

            return logStr;
        }

        static Buffer BuildSuccessString(_In_ const UnitTestResult& res) noexcept
        {
            const std::string_view fileNameSV{res.GetFileName()};
            const std::string_view funcNameSV{res.GetFunctionName()};
            const auto resultTypeName{ResultTypeUtil::ToString(res.GetResultType())};
            return PrintFormat(
                s_SuccessFormat,
                static_cast<int>(fileNameSV.length()),
                fileNameSV.data(),
                static_cast<int>(funcNameSV.length()),
                funcNameSV.data(),
                static_cast<int>(resultTypeName.length()),
                resultTypeName.data(),
                res.GetLineNumber(),
                res.GetTestDurationMicroseconds()
            );
        }

        static Buffer BuildFailureString(_In_ const UnitTestResult& res) noexcept
        {
            const std::string_view fileNameSV{res.GetFileName()};
            const std::string_view funcNameSV{res.GetFunctionName()};
            const auto resultTypeName{ResultTypeUtil::ToString(res.GetResultType())};
            const std::string_view infoSV{res.GetInfoString()};
            return PrintFormat(
                s_FailureFormat,
                static_cast<int>(fileNameSV.length()),
                fileNameSV.data(),
                static_cast<int>(funcNameSV.length()),
                funcNameSV.data(),
                static_cast<int>(resultTypeName.length()),
                resultTypeName.data(),
                res.GetLineNumber(),
                static_cast<int>(infoSV.length()),
                infoSV.data(),
                res.GetTestDurationMicroseconds()
            );
        }

        static Buffer BuildExceptionString(_In_ const UnitTestResult& res) noexcept
        {
            const std::string_view fileNameSV{res.GetFileName()};
            const std::string_view funcNameSV{res.GetFunctionName()};
            const auto resultTypeName{ResultTypeUtil::ToString(res.GetResultType())};
            const std::string_view infoSV{res.GetInfoString()};
            return PrintFormat(
                s_ExceptionFormat,
                static_cast<int>(fileNameSV.length()),
                fileNameSV.data(),
                static_cast<int>(funcNameSV.length()),
                funcNameSV.data(),
                static_cast<int>(resultTypeName.length()),
                resultTypeName.data(),
                res.GetLineNumber(),
                static_cast<int>(infoSV.length()),
                infoSV.data(),
                res.GetTestDurationMicroseconds()
            );
        }

        static Buffer BuildSkipString(_In_ const UnitTestResult& res) noexcept
        {
            const std::string_view fileNameSV{res.GetFileName()};
            const std::string_view funcNameSV{res.GetFunctionName()};
            const auto resultTypeName{ResultTypeUtil::ToString(res.GetResultType())};
            const std::string_view infoSV{res.GetInfoString()};
            return PrintFormat(
                s_SkipFormat,
                static_cast<int>(fileNameSV.length()),
                fileNameSV.data(),
                static_cast<int>(funcNameSV.length()),
                funcNameSV.data(),
                static_cast<int>(resultTypeName.length()),
                resultTypeName.data(),
                res.GetLineNumber(),
                static_cast<int>(infoSV.length()),
                infoSV.data(),
                res.GetTestDurationMicroseconds()
            );
        }

        static Buffer BuildUnhandledExceptionString(_In_ const UnitTestResult& res) noexcept
        {
            const auto resultTypeName{ResultTypeUtil::ToString(res.GetResultType())};
            const std::string_view infoSV{res.GetInfoString()};
            return PrintFormat(
                s_UnhandledExceptionFormat,
                static_cast<int>(resultTypeName.length()),
                resultTypeName.data(),
                static_cast<int>(infoSV.length()),
                infoSV.data(),
                res.GetTestDurationMicroseconds()
            );
        }

        static Buffer PrintFormat(_In_ const std::string_view format, ...) noexcept
        {
            va_list argsGetCount;
            va_list argsPopulateBuffer;
            Buffer buffer;

            va_start(argsGetCount, format);
            va_copy(argsPopulateBuffer, argsGetCount);

            try
            {
                const int bufferSize = vsnprintf(nullptr, 0, format.data(), argsGetCount);
                if (bufferSize > 0)
                {
                    buffer = std::vector<char>(static_cast<size_t>(bufferSize) + 1, '\0');
                    if (vsnprintf(buffer.data(), buffer.size(), format.data(), argsPopulateBuffer) < 0)
                    {
                        std::cerr << __FUNCTION__ << ": Failed to write log content to allocated buffer.\n";
                    }
                }
                else if (bufferSize < 0)
                {
                    std::cerr << __FUNCTION__ << ": Failed to determine buffer size.\n";
                }
                else if (bufferSize == 0)
                {
                    // -1 is technically the error case, but empty strings are also unexpected.
                    std::cerr << __FUNCTION__ << ": Potential 'empty' log message, discarding.";
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << __FUNCTION__ << ": Failed to create log buffer content - Exception[" << e.what() << "]\n";
                buffer.clear(); // Ensure we return empty buffer in this case.
            }

            va_end(argsGetCount);
            va_end(argsPopulateBuffer);

            return buffer;
        }

        void QueueLog(_Inout_ Buffer&& str) noexcept
        {
            if (str.empty())
            {
                return;
            }

            bool bWorkerThreadInitialized = m_WorkerThread.joinable();
            if (!bWorkerThreadInitialized)
            {
                // Lazy initialize the worker thread.
                InitializeWorkerThread();
                bWorkerThreadInitialized = true;
            }

            // Even if worker thread initialization failed, still queue in case a later attempt succeeds.
            {
                std::lock_guard<std::mutex> lg(m_LogQueueMutex);
                m_LogQueue.push_back(std::move(str));
                ++m_LogQueueSize;
            }

            if (bWorkerThreadInitialized)
            {
                m_LogCV.notify_one();
            }
        }

    public:
        // Ctors //

        UnitTestLogger() noexcept = default;

        // Dtor //

        ~UnitTestLogger() noexcept
        {
            TeardownWorkerThread();

            if (m_bLogToStdout)
            {
                fflush(stdout);
            }
        }

        // Getters //

        constexpr std::string_view GetLogFilePath() const noexcept
        {
            return m_LogFilePath;
        }

        constexpr bool GetPrintToConsole() const noexcept
        {
            return m_bLogToStdout;
        }

        constexpr bool GetOnlyLogFailures() const noexcept
        {
            return m_bOnlyLogFailures;
        }

        // Setters //

        _Success_(return) bool SetLogFilePath(_In_ const std::string& logFilePath) noexcept
        {
            if (logFilePath.empty())
            {
                // Interpret empty file path as wanting no file logging.
                m_FileStream = std::ofstream{};
                m_LogFilePath.clear();
                return true;
            }

            std::ofstream tmpFileStream{logFilePath, std::ios_base::out | std::ios_base::app | std::ios_base::binary};
            if (!tmpFileStream)
            {
                std::cout << __FUNCTION__ << ": Failed to open specified log file[" << logFilePath << "]\n\n";
                return false;
            }

            m_LogFilePath = logFilePath;
            m_FileStream = std::move(tmpFileStream);

            return true;
        }

        constexpr void SetLogToStdout(_In_ const bool bLogToStdout) noexcept
        {
            m_bLogToStdout = bLogToStdout;
        }

        constexpr void SetOnlyLogFailures(_In_ const bool bOnlyLogFailures) noexcept
        {
            m_bOnlyLogFailures = bOnlyLogFailures;
        }

        // Public Methods //

        void LogTestSetHeader(_In_ const TestSetData& data) noexcept
        {
            QueueLog(BuildTestSetHeaderString(data));
        }

        void LogUnitTestResult(_In_ const UnitTestResult& result) noexcept
        {
            if (!m_bOnlyLogFailures || ((result.GetResultType() != ResultType::Success) && (result.GetResultType() != ResultType::NotRun)))
            {
                QueueLog(BuildLogString(result));
            }
        }

        void LogTestSetSummary(_In_ const TestSetData& data) noexcept
        {
            QueueLog((data.GetTestFailureCount() == 0)
                ? BuildTestSetSummaryNoFailuresString(data)
                : BuildTestSetSummaryFailureDetailsString(data));
        }
    };
}