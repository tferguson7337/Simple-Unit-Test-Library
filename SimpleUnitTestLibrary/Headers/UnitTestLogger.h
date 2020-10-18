#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// SUTL
#include "TestSetData.h"
#include "UnitTestResult.h"


class UnitTestLogger
{
    using Buffer = std::vector<char>;
    using Queue = std::list<Buffer>;

private:

    /// Static Private Data Members ///

    // Format Strings
    static const char* const ms_pHeaderFormat;
    static const char* const ms_pSummaryNoFailuresFormat;
    static const char* const ms_pSummaryFailureDetailsFormat;
    static const char* const ms_pSuccessFormat;
    static const char* const ms_pFailureFormat;
    static const char* const ms_pExceptionFormat;
    static const char* const ms_pSkipFormat;
    static const char* const ms_pUnhandledExceptionFormat;

    /// Private Data Members ///

    // Logging configuration
    bool m_bLogToStdout;
    bool m_bOnlyLogFailures;
    const char* m_pLogFilePath;
    FILE* m_pFile;

    // Logging queue
    Queue* m_pLogQueue;
    std::atomic<size_t> m_LogQueueSize;

    // Logging thread.
    std::atomic<bool> m_bContinueWork;
    std::mutex* m_pLogQueueMutex;
    std::condition_variable* m_pLogCV;
    std::thread* m_pWorkerThread;

    /// Private Logging Worker Thread Methods ///

    void InitializeWorkerThread();
    void TeardownWorkerThread();

    constexpr bool IsWorkerThreadInitialized() const noexcept
    {
        return !!m_pLogQueue
            && !!m_pLogQueueMutex
            && !!m_pLogCV
            && !!m_pWorkerThread
            && m_pWorkerThread->joinable();
    }

    bool WorkerPredicate() noexcept;
    bool TerminatePredicate() noexcept;
    void WorkerLoop() noexcept;
    void WaitForWork() noexcept;
    Queue TransferLogQueueContents() noexcept;
    void PrintLogs() noexcept;
    void PrintLog(_In_ const Buffer& logMsg) noexcept;

    /// Private Static Helper Methods ///

    static Buffer BuildTestSetHeaderString(_In_ const TestSetData&) noexcept;
    static Buffer BuildTestSetSummaryNoFailuresString(_In_ const TestSetData&) noexcept;
    static Buffer BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData&) noexcept;

    static Buffer BuildLogString(_In_ const UnitTestResult&) noexcept;
    static Buffer BuildSuccessString(_In_ const UnitTestResult&) noexcept;
    static Buffer BuildFailureString(_In_ const UnitTestResult&) noexcept;
    static Buffer BuildExceptionString(_In_ const UnitTestResult&) noexcept;
    static Buffer BuildSkipString(_In_ const UnitTestResult&) noexcept;
    static Buffer BuildUnhandledExceptionString(_In_ const UnitTestResult&) noexcept;

    static Buffer stprintf(_In_z_ const char*, ...) noexcept;
    static int StringPrintWrapper(_Inout_ Buffer&, _In_z_ const char*, _In_ va_list) noexcept;

    void QueueLog(_Inout_ Buffer&&) noexcept;

public:
    /// Ctors ///

    constexpr UnitTestLogger() noexcept :
        m_bLogToStdout(true),
        m_bOnlyLogFailures(true),
        m_pLogFilePath(nullptr),
        m_pFile(nullptr),
        m_pLogQueue(nullptr),
        m_LogQueueSize(0),
        m_bContinueWork(false),
        m_pLogQueueMutex(nullptr),
        m_pLogCV(nullptr),
        m_pWorkerThread(nullptr)
    { }

    /// Dtor ///

    ~UnitTestLogger() noexcept;

    /// Getters ///

    _Ret_maybenull_z_ constexpr const char* GetLogFilePath() const noexcept
    {
        return m_pLogFilePath;
    }

    constexpr bool GetPrintToConsole() const noexcept
    {
        return m_bLogToStdout;
    }

    constexpr bool GetOnlyLogFailures() const noexcept
    {
        return m_bOnlyLogFailures;
    }

    /// Setters ///

    _Success_(return)
    bool SetLogFilePath(_In_ const std::string& logFilePath) noexcept;

    constexpr void SetLogToStdout(_In_ const bool bLogToStdout) noexcept
    {
        m_bLogToStdout = bLogToStdout;
    }

    constexpr void SetOnlyLogFailures(_In_ const bool bOnlyLogFailures) noexcept
    {
        m_bOnlyLogFailures = bOnlyLogFailures;
    }

    /// Public Methods ///

    void LogTestSetHeader(_In_ const TestSetData& data) noexcept;
    void LogUnitTestResult(_In_ const UnitTestResult& result) noexcept;
    void LogTestSetSummary(_In_ const TestSetData& data) noexcept;
};
