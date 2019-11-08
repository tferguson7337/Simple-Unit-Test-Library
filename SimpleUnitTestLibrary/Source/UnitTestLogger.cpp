#include <UnitTestLogger.h>

// STL
#include <cstdarg>
#include <ctime>

/// Non-Member Static Variables \\\

static const wchar_t* s_pHeaderFormat(
    L"================================"
    L"================================\n"
    L"  Test-Set Name: %-25s -- Total Tests: %zu\n"
    L"================================"
    L"================================\n\n\n"
);

static const wchar_t* s_pSummaryNoFailuresFormat(
    L"===================="
    L"====================\n"
    L"    \"%s\" Complete\n\n"
    L"  Total Test Count: %u\n"
    L"  Total Tests Run:  %u\n\n"
    L"   Successful Tests: %u\n\n"
    L"   Failed Tests:     %u\n\n"
    L"   Skipped Tests:    %u\n\n"
    L"  Test Pass Grade:  %2.2f%% (%u / %u)\n"
    L"  Total Duration:   %llu ms\n"
    L"===================="
    L"====================\n\n\n"
);

static const wchar_t* s_pSummaryFailureDetailsFormat(
    L"===================="
    L"====================\n"
    L"    \"%s\" Complete\n\n"
    L"  Total Test Count: %u\n"
    L"  Total Tests Run:  %u\n\n"
    L"   Successful Tests: %u\n\n"
    L"   Failed Tests:     %u\n"
    L"    Setup Failures:       %u\n"
    L"    Setup Exceptions:     %u\n"
    L"    Test Failures:        %u\n"
    L"    Test Exceptions:      %u\n"
    L"    Cleanup Failures:     %u\n"
    L"    Cleanup Exceptions:   %u\n"
    L"    Unhandled Exceptions: %u\n\n"
    L"   Skipped Tests:    %u\n\n"
    L"  Test Pass Grade:  %2.2f%% (%u / %u)\n"
    L"  Total Duration:   %llu ms\n"
    L"===================="
    L"====================\n\n\n"
);

static const wchar_t* s_pSuccessFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:   %s\n"
    L"    Line:     %u\n"
    L"    Duration: %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n\n\n"
);

static const wchar_t* s_pFailureFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:   %s\n"
    L"    Line:     %u\n"
    L"    Failure:  %s\n"
    L"    Duration: %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n\n\n"
);

static const wchar_t* s_pExceptionFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:    %s\n"
    L"    Line:      %u\n"
    L"    Exception: %hs\n"
    L"    Duration:  %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n\n\n"
);

static const wchar_t* s_pSkipFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:   %s\n"
    L"    Line:     %u\n"
    L"    Reason:   %hs\n"
    L"    Duration: %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n\n\n"
);

static const wchar_t* s_pUnhandledExceptionFormat(
    L"Result: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Unhandled Exception: %s\n"
    L"    Duration:            %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n\n\n"
);

/// Ctors \\\

UnitTestLogger::UnitTestLogger(_In_ const std::filesystem::path& file, _In_ const bool bWriteToStdout, _In_ const bool bOnlyLogFailures) :
    m_bLogToStdout(bWriteToStdout),
    m_bOnlyLogFailures(bOnlyLogFailures),
    m_ConsoleStream(std::wcout),
    m_FileStream(file, std::ios_base::binary | std::ios_base::app | std::ios_base::out),
    m_TargetFile(file)
{
    InitializeWorkerThread();
}

/// Dtor \\\

UnitTestLogger::~UnitTestLogger()
{
    TeardownWorkerThread();

    if (m_bLogToStdout && m_ConsoleStream)
    {
        m_ConsoleStream.flush();
    }

    if (m_FileStream)
    {
        m_FileStream.flush();
    }
}

/// Operator Overload \\\

UnitTestLogger& UnitTestLogger::operator=(_Inout_ UnitTestLogger&& src) noexcept
{
    if (this != &src)
    {
        m_bLogToStdout = src.m_bLogToStdout;
        m_FileStream = std::move(src.m_FileStream);
        m_TargetFile = std::move(src.m_TargetFile);

        src.m_bLogToStdout = false;
        src.m_ConsoleStream.flush();
        src.m_FileStream.close();
        src.m_TargetFile.clear();
    }

    return *this;
}

/// Method Definitions \\\

// Private Helper Methods

UnitTestLogger::Buffer UnitTestLogger::BuildTestSetHeaderString(_In_ const TestSetData& data)
{
    return stprintf(
        s_pHeaderFormat,
        data.GetTestSetName().c_str(),
        data.GetTotalTestCount()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildTestSetSummaryNoFailuresString(_In_ const TestSetData& data)
{
    return stprintf(
        s_pSummaryNoFailuresFormat,
        data.GetTestSetName().c_str(),
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

UnitTestLogger::Buffer UnitTestLogger::BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData& data)
{
    return stprintf(
        s_pSummaryFailureDetailsFormat,
        data.GetTestSetName().c_str(),
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

UnitTestLogger::Buffer UnitTestLogger::BuildLogString(_In_ const UnitTestResult& res)
{
    Buffer logStr;

    switch (res.GetResult())
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
    }

    return logStr;
}

UnitTestLogger::Buffer UnitTestLogger::BuildSuccessString(_In_ const UnitTestResult& res)
{
    return stprintf(
        s_pSuccessFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildFailureString(_In_ const UnitTestResult& res)
{
    return stprintf(
        s_pFailureFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildExceptionString(_In_ const UnitTestResult& res)
{
    return stprintf(
        s_pExceptionFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildSkipString(_In_ const UnitTestResult& res)
{
    return stprintf(
        s_pSkipFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildUnhandledExceptionString(_In_ const UnitTestResult& res)
{
    return stprintf(
        s_pUnhandledExceptionFormat,
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

bool UnitTestLogger::GetTime(_Out_writes_(len) wchar_t* buffer, _In_ const size_t len, _In_ const time_t* t)
{
    return _wctime_s(buffer, len, t) == 0;
}

UnitTestLogger::Buffer UnitTestLogger::stprintf(_In_z_ const wchar_t* format, ...)
{
    va_list args;
    Buffer buffer;
    int bufferSize = 0;

    if (!!format)
    {
        va_start(args, format);

        bufferSize = StringPrintWrapper(buffer, format, args);

        if (bufferSize > 0)
        {
            buffer = std::vector(static_cast<size_t>(bufferSize) + 2, L'\0');
            bufferSize = StringPrintWrapper(buffer, format, args);
        }

        va_end(args);
    }

    return buffer;
}

int UnitTestLogger::StringPrintWrapper(_Inout_ Buffer& buffer, _In_z_ const wchar_t* format, _In_ va_list args)
{
    if (buffer.empty())
    {
        return _vscwprintf(format, args);
    }
    else
    {
        return vswprintf_s(buffer.data(), buffer.size() - 1, format, args);
    }
}

void UnitTestLogger::QueueLog(_Inout_ Buffer&& str)
{
    {
        std::lock_guard<std::mutex> lg(m_LogQueueMutex);
        m_LogQueue.push_back(std::move(str));
        m_LogQueueSize++;
    }

    m_LogCV.notify_one();
    str.clear();
}

/// Public Method Definitions \\\

// Getters

const std::filesystem::path& UnitTestLogger::GetTargetFile() const noexcept
{
    return m_TargetFile;
}

bool UnitTestLogger::GetPrintToConsole() const noexcept
{
    return m_bLogToStdout;
}

bool UnitTestLogger::GetOnlyLogFailures() const noexcept
{
    return m_bOnlyLogFailures;
}

// Setters

bool UnitTestLogger::SetTargetFile(_In_ const std::filesystem::path& filePath)
{
    std::wofstream newFileStream(filePath);

    if (!newFileStream)
    {
        return false;
    }

    m_FileStream.flush();
    std::swap(newFileStream, m_FileStream);
    m_TargetFile.assign(filePath);

    return true;
}

void UnitTestLogger::SetLogToStdout(_In_ const bool bLogToStdout) noexcept
{
    m_bLogToStdout = bLogToStdout;
}

void UnitTestLogger::SetOnlyLogFailures(_In_ const bool bOnlyLogFailures) noexcept
{
    m_bOnlyLogFailures = bOnlyLogFailures;
}

// Public Methods

void UnitTestLogger::LogTestSetHeader(_In_ const TestSetData& data)
{
    QueueLog(BuildTestSetHeaderString(data));
}

void UnitTestLogger::LogUnitTestResult(_In_ const UnitTestResult& result)
{
    if (!m_bOnlyLogFailures || ((result.GetResult() != ResultType::Success) && (result.GetResult() != ResultType::NotRun)))
    {
        QueueLog(BuildLogString(result));
    }
}

void UnitTestLogger::LogTestSetSummary(_In_ const TestSetData& data)
{
    Buffer buf((data.GetTestFailureCount() == 0)
        ? BuildTestSetSummaryNoFailuresString(data)
        : BuildTestSetSummaryFailureDetailsString(data));
    QueueLog(std::move(buf));
}

/// Logging Worker Thread Methods \\\

void UnitTestLogger::InitializeWorkerThread()
{
    TeardownWorkerThread();

    m_bContinueWork = true;
    m_WorkerThread = std::thread(&UnitTestLogger::WorkerLoop, this);
}

void UnitTestLogger::TeardownWorkerThread()
{
    if (m_WorkerThread.joinable())
    {
        m_bContinueWork = false;
        m_LogCV.notify_all();
        m_WorkerThread.join();
    }
}

void UnitTestLogger::WorkerLoop()
{
    while (!TerminatePredicate())
    {
        WaitForWork();
        PrintLogs();
    }
}

void UnitTestLogger::WaitForWork()
{
    std::unique_lock<std::mutex> ul(m_LogQueueMutex);
    m_LogCV.wait(ul, [this]() -> bool { return this->WorkerPredicate(); });
}

UnitTestLogger::Queue UnitTestLogger::TransferLogQueueContents()
{
    Queue q;
    std::lock_guard<std::mutex> lg(m_LogQueueMutex);
    std::swap(q, m_LogQueue);
    m_LogQueueSize = 0;
    return q;
}

void UnitTestLogger::PrintLogs()
{
    for (const auto& buf : TransferLogQueueContents())
    {
        PrintLog(buf);
    }
}

void UnitTestLogger::PrintLog(_In_ const Buffer& buf)
{
    if (m_bLogToStdout && m_ConsoleStream)
    {
        m_ConsoleStream.write(buf.data(), buf.size());
    }

    if (m_FileStream)
    {
        m_FileStream.write(buf.data(), buf.size());
    }
}

bool UnitTestLogger::WorkerPredicate()
{
    return (m_LogQueueSize.load(std::memory_order_relaxed) != 0) || (!m_bContinueWork.load(std::memory_order_relaxed));
}

bool UnitTestLogger::TerminatePredicate()
{
    return (!m_bContinueWork) && (m_LogQueueSize == 0);
}
