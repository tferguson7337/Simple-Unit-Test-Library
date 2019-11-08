#pragma once

// STL
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

// SUTL
#include "TestSetData.h"
#include "UnitTestResult.h"


class UnitTestLogger
{
    using Buffer = std::vector<wchar_t>;
    using Queue = std::list<Buffer>;

private:

    /// Static Private Data Members \\\

    static const size_t ms_TimeBufferLength = 32;

    /// Private Data Members \\\

    bool m_bLogToStdout;
    bool m_bOnlyLogFailures;
    std::wostream& m_ConsoleStream;
    std::filesystem::path m_TargetFile;
    std::wofstream m_FileStream;

    std::atomic<bool> m_bContinueWork;
    std::atomic<size_t> m_LogQueueSize;

    std::mutex m_LogQueueMutex;
    std::condition_variable m_LogCV;
    Queue m_LogQueue;

    std::thread m_WorkerThread;

    /// Private Logging Worker Thread Methods \\\

    void InitializeWorkerThread();
    void TeardownWorkerThread();

    void WorkerLoop();
    void WaitForWork();
    Queue TransferLogQueueContents();
    void PrintLogs();
    void PrintLog(_In_ const Buffer& logMsg);
    bool WorkerPredicate();
    bool TerminatePredicate();

    /// Private Static Helper Methods \\\

    static Buffer BuildTestSetHeaderString(_In_ const TestSetData&);
    static Buffer BuildTestSetSummaryNoFailuresString(_In_ const TestSetData&);
    static Buffer BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData&);

    static Buffer BuildLogString(_In_ const UnitTestResult&);
    static Buffer BuildSuccessString(_In_ const UnitTestResult&);
    static Buffer BuildFailureString(_In_ const UnitTestResult&);
    static Buffer BuildExceptionString(_In_ const UnitTestResult&);
    static Buffer BuildSkipString(_In_ const UnitTestResult&);
    static Buffer BuildUnhandledExceptionString(_In_ const UnitTestResult&);

    static bool GetTime(_Out_writes_(len) wchar_t* buffer, _In_ const size_t len, _In_ const time_t* t);

    static Buffer stprintf(_In_z_ const wchar_t*, ...);
    static int StringPrintWrapper(_Inout_ Buffer&, _In_z_ const wchar_t*, _In_ va_list);

    void QueueLog(_Inout_ Buffer&&);

public:
    /// Ctors \\\

    UnitTestLogger(_In_ const std::filesystem::path& file = std::filesystem::path(), _In_ const bool bWriteToStdout = true, _In_ const bool bOnlyLogFailures = true);

    /// Dtor \\\

    ~UnitTestLogger();

    /// Operator Overload \\\

    UnitTestLogger& operator=(_Inout_ UnitTestLogger&&) noexcept;

    /// Getters \\\

    const std::filesystem::path& GetTargetFile() const noexcept;
    bool GetPrintToConsole() const noexcept;
    bool GetOnlyLogFailures() const noexcept;

    /// Setters \\\

    bool SetTargetFile(_In_ const std::filesystem::path& filePath);
    void SetLogToStdout(_In_ const bool bLogToStdout) noexcept;
    void SetOnlyLogFailures(_In_ const bool bOnlyLogFailures) noexcept;

    /// Public Methods \\\

    void LogTestSetHeader(_In_ const TestSetData& data);
    void LogUnitTestResult(_In_ const UnitTestResult& result);
    void LogTestSetSummary(_In_ const TestSetData& data);
};
