#pragma once

#include <Uncopyable.h>
#include <StringUtil.hpp>

#include <IUnitTestLogger.h>

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>

#include <TestSetData.h>
#include <UnitTestResult.h>

template <class T>
class UnitTestLogger : public IUnitTestLogger<T>, public Uncopyable
{
    // Allow other UnitTestLogger specializations to access private static methods.
    template <class U>
    friend class UnitTestLogger;

private:
    /// Private Data Members \\\

    bool mPrintToConsole; 
    std::basic_ostream<T>& mConsoleStream;
    std::filesystem::path mTargetFile;
    std::basic_ofstream<T> mFileStream;

    std::atomic<bool> mContinueWork;
    std::atomic<size_t> mLogQueueSize;

    std::mutex mLogQueueMutex;
    std::condition_variable mCVSignaler;
    std::queue<std::basic_string<T>> mLogQueue;
    
    std::thread mWorkerThread;    

    static const size_t mTimeBufferLength = 32;

    /// Private Logging Worker Thread Methods \\\

    void WorkerLoop( );
    void WaitForWork( );
    void PrintLogs( );
    void PrintLog(const std::basic_string<T>&);
    bool WorkerPredicate( );

    /// Private Static Helper Methods \\\

    static std::basic_ostream<T>& InitConsoleStream( );

    static std::basic_string<T> BuildTestSetHeaderString(const TestSetData<T>&);
    static std::basic_string<T> BuildTestSetSummaryString(const TestSetData<T>&);

    static const std::basic_string<T>& GetTestSetHeaderFormat( );
    static const std::basic_string<T>& GetTestSetSummaryFormat( );

    static std::basic_string<T> BuildLogString(const UnitTestResult&);
    static std::basic_string<T> BuildTimeString( );
    static std::basic_string<T> BuildSuccessString(const UnitTestResult&);
    static std::basic_string<T> BuildFailureString(const UnitTestResult&);
    static std::basic_string<T> BuildExceptionString(const UnitTestResult&);
    static std::basic_string<T> BuildSkipString(const UnitTestResult&);
    static std::basic_string<T> BuildUnhandledExceptionString(const UnitTestResult&);

    static const std::basic_string<T>& GetResultString(const ResultType&);
    static const std::basic_string<T>& GetTimeFormat( );
    static const std::basic_string<T>& GetSuccessFormat( );
    static const std::basic_string<T>& GetFailureFormat( );
    static const std::basic_string<T>& GetExceptionFormat( );
    static const std::basic_string<T>& GetSkipFormat( );
    static const std::basic_string<T>& GetUnhandledExceptionFormat( );

    static bool GetTime(T*, const time_t*);

    static std::basic_string<T> stprintf(const std::basic_string<T>*, ...);
    static int StringPrintWrapper(std::vector<T>&, const std::basic_string<T>*, va_list);

public:
    /// Ctors \\\

    UnitTestLogger(const std::filesystem::path& = std::filesystem::path( ), bool = true);

    /// Dtor \\\

    ~UnitTestLogger( );

    /// Operator Overload \\\

    UnitTestLogger& operator=(UnitTestLogger&&) noexcept;

    /// Getters \\\

    const std::filesystem::path& GetTargetFile( ) const noexcept;
    bool GetPrintToConsole( ) const noexcept;

    /// Setters \\\

    bool SetTargetFile(const std::filesystem::path&);
    bool SetTargetFile(std::filesystem::path&&);
    void SetPrintToConsole(bool);

    /// Public Methods \\\

    void LogTestSetHeader(const TestSetData<T>&);
    void LogUnitTestResult(const UnitTestResult&);
    void LogTestSetSummary(const TestSetData<T>&);
};
