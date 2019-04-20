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

    static const size_t mTimeBufferLength = 32;

    /// Private Data Members \\\

    bool mPrintToConsole;
    bool mOnlyLogFailures;
    std::basic_ostream<T>& mConsoleStream;
    std::filesystem::path mTargetFile;
    std::basic_ofstream<T> mFileStream;

    std::atomic<bool> mContinueWork;
    std::atomic<size_t> mLogQueueSize;

    std::mutex mLogQueueMutex;
    std::condition_variable mCVSignaler;
    std::queue<std::basic_string<T>> mLogQueue;

    std::thread mWorkerThread;

    /// Private Logging Worker Thread Methods \\\

    void InitializeWorkerThread( );
    void TeardownWorkerThread( );

    void WorkerLoop( );
    void WaitForWork( );
    void PrintLogs( );
    void PrintLog(_In_ const std::basic_string<T>&);
    bool WorkerPredicate( );
    bool TerminatePredicate( );

    /// Private Static Helper Methods \\\

    static std::basic_ostream<T>& InitConsoleStream( );

    static std::basic_string<T> BuildTestSetHeaderString(_In_ const TestSetData<T>&);
    static std::basic_string<T> BuildTestSetSummaryString(_In_ const TestSetData<T>&);

    static const std::basic_string<T>& GetTestSetHeaderFormat( );
    static const std::basic_string<T>& GetTestSetSummaryFormat( );

    static std::basic_string<T> BuildLogString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildTimeString( );
    static std::basic_string<T> BuildSuccessString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildFailureString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildExceptionString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildSkipString(_In_ const UnitTestResult&);
    static std::basic_string<T> BuildUnhandledExceptionString(_In_ const UnitTestResult&);

    static const std::basic_string<T>& GetResultString(_In_ const ResultType&);
    static const std::basic_string<T>& GetTimeFormat( );
    static const std::basic_string<T>& GetSuccessFormat( );
    static const std::basic_string<T>& GetFailureFormat( );
    static const std::basic_string<T>& GetExceptionFormat( );
    static const std::basic_string<T>& GetSkipFormat( );
    static const std::basic_string<T>& GetUnhandledExceptionFormat( );

    static bool GetTime(_In_ T*, _In_ const time_t*);

    static std::basic_string<T> stprintf(_In_ const std::basic_string<T>*, ...);
    static int StringPrintWrapper(_Inout_ std::vector<T>&, _In_ const std::basic_string<T>*, _In_ va_list);

    void LogCommon(_In_ std::basic_string<T>&&);

public:
    /// Ctors \\\

    UnitTestLogger(_In_ const std::filesystem::path & = std::filesystem::path( ), _In_ const bool& = true, _In_ const bool& = true);

    /// Dtor \\\

    ~UnitTestLogger( );

    /// Operator Overload \\\

    UnitTestLogger& operator=(_In_ UnitTestLogger&&) noexcept;

    /// Getters \\\

    const std::filesystem::path& GetTargetFile( ) const noexcept;
    bool GetPrintToConsole( ) const noexcept;
    bool GetOnlyLogFailures( ) const noexcept;

    /// Setters \\\

    bool SetTargetFile(_In_ const std::filesystem::path&);
    bool SetTargetFile(_In_ std::filesystem::path&&);
    void SetPrintToConsole(_In_ const bool&) noexcept;
    void SetOnlyLogFailures(_In_ const bool&) noexcept;

    /// Public Methods \\\

    void LogTestSetHeader(_In_ const TestSetData<T>&);
    void LogUnitTestResult(_In_ const UnitTestResult&);
    void LogTestSetSummary(_In_ const TestSetData<T>&);
};
