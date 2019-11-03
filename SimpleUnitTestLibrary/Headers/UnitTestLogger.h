#pragma once

// STL
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>

// SUTL
#include "TestSetData.h"
#include "UnitTestResult.h"


class UnitTestLogger
{
private:

    /// Static Private Data Members \\\

    static const size_t ms_TimeBufferLength = 32;

    /// Private Data Members \\\

    bool m_PrintToConsole;
    bool m_OnlyLogFailures;
    std::wostream& m_ConsoleStream;
    std::filesystem::path m_TargetFile;
    std::wofstream m_FileStream;

    std::atomic<bool> m_ContinueWork;
    std::atomic<size_t> m_LogQueueSize;

    std::mutex m_LogQueueMutex;
    std::condition_variable m_CVSignaler;
    std::queue<std::wstring> m_LogQueue;

    std::thread m_WorkerThread;

    /// Private Logging Worker Thread Methods \\\

    void InitializeWorkerThread();
    void TeardownWorkerThread();

    void WorkerLoop();
    void WaitForWork();
    void PrintLogs();
    void PrintLog(_In_ const std::wstring&);
    bool WorkerPredicate();
    bool TerminatePredicate();

    /// Private Static Helper Methods \\\

    static std::wstring BuildTestSetHeaderString(_In_ const TestSetData&);
    static std::wstring BuildTestSetSummaryNoFailuresString(_In_ const TestSetData&);
    static std::wstring BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData&);

    static std::wstring BuildLogString(_In_ const UnitTestResult&);
    static std::wstring BuildTimeString();
    static std::wstring BuildSuccessString(_In_ const UnitTestResult&);
    static std::wstring BuildFailureString(_In_ const UnitTestResult&);
    static std::wstring BuildExceptionString(_In_ const UnitTestResult&);
    static std::wstring BuildSkipString(_In_ const UnitTestResult&);
    static std::wstring BuildUnhandledExceptionString(_In_ const UnitTestResult&);

    static bool GetTime(_Out_writes_(len) wchar_t* buffer, _In_ const size_t len, _In_ const time_t* t);

    static std::wstring stprintf(_In_ const std::wstring*, ...);
    static int StringPrintWrapper(_Inout_ std::vector<wchar_t>&, _In_ const std::wstring*, _In_ va_list);

    void LogCommon(_Inout_ std::wstring&&);

public:
    /// Ctors \\\

    UnitTestLogger(_In_ const std::filesystem::path & = std::filesystem::path(), _In_ const bool& = true, _In_ const bool& = true);

    /// Dtor \\\

    ~UnitTestLogger();

    /// Operator Overload \\\

    UnitTestLogger& operator=(_Inout_ UnitTestLogger&&) noexcept;

    /// Getters \\\

    const std::filesystem::path& GetTargetFile() const noexcept;
    bool GetPrintToConsole() const noexcept;
    bool GetOnlyLogFailures() const noexcept;

    /// Setters \\\

    bool SetTargetFile(_In_ const std::filesystem::path&);
    bool SetTargetFile(_Inout_ std::filesystem::path&&);
    void SetPrintToConsole(_In_ const bool&) noexcept;
    void SetOnlyLogFailures(_In_ const bool&) noexcept;

    /// Public Methods \\\

    void LogTestSetHeader(_In_ const TestSetData&);
    void LogUnitTestResult(_In_ const UnitTestResult&);
    void LogTestSetSummary(_In_ const TestSetData&);
};
