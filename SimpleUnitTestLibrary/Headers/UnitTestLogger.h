#pragma once

#include "Interfaces/IUnitTestLogger.h"

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>

#include "TestSetData.h"
#include "UnitTestResult.h"

template <class T>
class UnitTestLogger : public IUnitTestLogger<T>
{
    // Allow other UnitTestLogger specializations to access private static methods.
    template <class U>
    friend class UnitTestLogger;

    // Allow UnitTestRunner to access private methods.
    template <class U>
    friend class UnitTestRunner;

private:

    /// Static Private Data Members \\\

    static const size_t ms_TimeBufferLength = 32;

    /// Private Data Members \\\

    bool m_PrintToConsole;
    bool m_OnlyLogFailures;
    std::basic_ostream<T>& m_ConsoleStream;
    std::filesystem::path m_TargetFile;
    std::basic_ofstream<T> m_FileStream;

    std::atomic<bool> m_ContinueWork;
    std::atomic<size_t> m_LogQueueSize;

    std::mutex m_LogQueueMutex;
    std::condition_variable m_CVSignaler;
    std::queue<std::basic_string<T>> m_LogQueue;

    std::thread m_WorkerThread;

    /// Private Logging Worker Thread Methods \\\

    void InitializeWorkerThread();
    void TeardownWorkerThread();

    void WorkerLoop();
    void WaitForWork();
    void PrintLogs();
    void PrintLog(_In_ const std::basic_string<T>&);
    bool WorkerPredicate();
    bool TerminatePredicate();

    /// Private Static Helper Methods \\\

    static std::basic_ostream<T>& InitConsoleStream();

    static std::basic_string<T> BuildTestSetHeaderString(_In_ const TestSetData<T>&);
    static std::basic_string<T> BuildTestSetSummaryNoFailuresString(_In_ const TestSetData<T>&);
    static std::basic_string<T> BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData<T>&);

    static const std::basic_string<T>& GetTestSetHeaderFormat();
    static const std::basic_string<T>& GetTestSetSummaryNoFailuresFormat();
    static const std::basic_string<T>& GetTestSetSummaryFailureDetailsFormat();

    static std::basic_string<T> BuildLogString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildTimeString();
    static std::basic_string<T> BuildSuccessString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildFailureString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildExceptionString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildSkipString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildUnhandledExceptionString(_In_ const UnitTestResult&);

    static const std::basic_string<T>& GetResultString(_In_ const ResultType&);
    static const std::basic_string<T>& GetTimeFormat();
    static const std::basic_string<T>& GetSuccessFormat();
    static const std::basic_string<T>& GetFailureFormat();
    static const std::basic_string<T>& GetExceptionFormat();
    static const std::basic_string<T>& GetSkipFormat();
    static const std::basic_string<T>& GetUnhandledExceptionFormat();

    static bool GetTime(_Out_writes_(len) T* buffer, _In_ const size_t len, _In_ const time_t* t);

    static std::basic_string<T> stprintf(_In_ const std::basic_string<T>*, ...);
    static int StringPrintWrapper(_Inout_ std::vector<T>&, _In_ const std::basic_string<T>*, _In_ va_list);

    void LogCommon(_Inout_ std::basic_string<T>&&);

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

    void LogTestSetHeader(_In_ const TestSetData<T>&);
    void LogUnitTestResult(_In_ const UnitTestResult&);
    void LogTestSetSummary(_In_ const TestSetData<T>&);
};
