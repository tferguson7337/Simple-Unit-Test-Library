#include <UnitTestLogger.h>

// STL
#include <cstdarg>
#include <ctime>

/// Non-Member Static Variables \\\

static const std::wstring s_HeaderFormat(
    L"================================"
    L"================================\n"
    L"  Test-Set Name: %-25s -- Total Tests: %zu\n"
    L"================================"
    L"================================\n"
);

static const std::wstring s_SummaryNoFailuresFormat(
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
    L"====================\n"
);

static const std::wstring s_SummaryFailureDetailsFormat(
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
    L"====================\n"
);

static const std::wstring s_TimeFormat(
    L"%c"
);

static const std::wstring s_SuccessFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:   %s\n"
    L"    Line:     %u\n"
    L"    Duration: %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n"
);

static const std::wstring s_FailureFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:   %s\n"
    L"    Line:     %u\n"
    L"    Failure:  %s\n"
    L"    Duration: %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n"
);

static const std::wstring s_ExceptionFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:    %s\n"
    L"    Line:      %u\n"
    L"    Exception: %hs\n"
    L"    Duration:  %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n"
);

static const std::wstring s_SkipFormat(
    L"File: %hs\nTest: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Result:   %s\n"
    L"    Line:     %u\n"
    L"    Reason:   %hs\n"
    L"    Duration: %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n"
);

static const std::wstring s_UnhandledExceptionFormat(
    L"Result: %hs\n"
    L"--------------------------------"
    L"--------------------------------\n"
    L"    Unhandled Exception: %s\n"
    L"    Duration:            %llu microseconds\n"
    L"--------------------------------"
    L"--------------------------------\n"
);

/// Ctors \\\

UnitTestLogger::UnitTestLogger(_In_ const std::filesystem::path& file, _In_ const bool& consoleOutput, _In_ const bool& onlyLogFailures) :
    m_PrintToConsole(consoleOutput),
    m_OnlyLogFailures(onlyLogFailures),
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

    if (m_PrintToConsole && m_ConsoleStream)
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
        m_PrintToConsole = src.m_PrintToConsole;
        m_FileStream = std::move(src.m_FileStream);
        m_TargetFile = std::move(src.m_TargetFile);

        src.m_PrintToConsole = false;
        src.m_ConsoleStream.flush();
        src.m_FileStream.close();
        src.m_TargetFile.clear();
    }

    return *this;
}

/// Method Definitions \\\

// Private Helper Methods

std::wstring UnitTestLogger::BuildTestSetHeaderString(_In_ const TestSetData& data)
{
    return stprintf(
        &s_HeaderFormat,
        data.GetTestSetName().c_str(),
        data.GetTotalTestCount()
    );
}

std::wstring UnitTestLogger::BuildTestSetSummaryNoFailuresString(_In_ const TestSetData& data)
{
    return stprintf(
        &s_SummaryNoFailuresFormat,
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

std::wstring UnitTestLogger::BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData& data)
{
    return stprintf(
        &s_SummaryFailureDetailsFormat,
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

std::wstring UnitTestLogger::BuildLogString(_In_ const UnitTestResult& res)
{
    std::wstring logStr;

    switch (res.GetResult())
    {
    case ResultType::Success:
        logStr.append(BuildSuccessString(res));
        break;

    case ResultType::SetupFailure:
    case ResultType::TestFailure:
    case ResultType::CleanupFailure:
        logStr.append(BuildFailureString(res));
        break;

    case ResultType::SetupException:
    case ResultType::TestException:
    case ResultType::CleanupException:
        logStr.append(BuildExceptionString(res));
        break;

    case ResultType::NotRun:
        logStr.append(BuildSkipString(res));
        break;

    case ResultType::UnhandledException:
        logStr.append(BuildUnhandledExceptionString(res));
        break;

    default:
        logStr.clear();
    }

    return logStr;
}

std::wstring UnitTestLogger::BuildTimeString()
{
    std::vector buffer(ms_TimeBufferLength, L'\0');
    const time_t time = std::time(nullptr);

    if (!GetTime(buffer.data(), ms_TimeBufferLength, &time))
    {
        return std::wstring();
    }

    while (!buffer.empty() && buffer.back() != L'\n')
    {
        buffer.pop_back();
    }

    if (buffer.size() <= 1)
    {
        return std::wstring();
    }
    else
    {
        buffer.pop_back();
    }

    buffer.push_back(L'\0');

    return std::wstring(buffer.data());
}

std::wstring UnitTestLogger::BuildSuccessString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &s_SuccessFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetTestDurationMicroseconds()
    );
}

std::wstring UnitTestLogger::BuildFailureString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &s_FailureFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

std::wstring UnitTestLogger::BuildExceptionString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &s_ExceptionFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

std::wstring UnitTestLogger::BuildSkipString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &s_SkipFormat,
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

std::wstring UnitTestLogger::BuildUnhandledExceptionString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &s_UnhandledExceptionFormat,
        ResultTypeUtil::ToString(res.GetResult()).c_str(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

bool UnitTestLogger::GetTime(_Out_writes_(len) wchar_t* buffer, _In_ const size_t len, _In_ const time_t* t)
{
    return _wctime_s(buffer, len, t) == 0;
}

std::wstring UnitTestLogger::stprintf(_In_ const std::wstring* format, ...)
{
    va_list args;
    std::vector<wchar_t> buffer;
    int bufferSize = 0;

    if (!format || format->empty())
    {
        return std::wstring();
    }

    va_start(args, format);

    bufferSize = StringPrintWrapper(buffer, format, args);

    if (bufferSize > 0)
    {
        buffer = std::move(std::vector(static_cast<size_t>(bufferSize) + 2, L'\0'));
        bufferSize = StringPrintWrapper(buffer, format, args);
    }

    va_end(args);

    return (bufferSize > 0) ? std::wstring(std::move(buffer.data())) : std::wstring();
}

int UnitTestLogger::StringPrintWrapper(_Inout_ std::vector<wchar_t>& buffer, _In_ const std::wstring* format, _In_ va_list args)
{
    if (buffer.empty())
    {
        return _vscwprintf(
            reinterpret_cast<const wchar_t*>(format->c_str()),
            args
        );
    }
    else
    {
        return vswprintf(
            reinterpret_cast<wchar_t*>(buffer.data()),
            buffer.size() - 1,
            reinterpret_cast<const wchar_t*>(format->c_str()),
            args
        );
    }
}

void UnitTestLogger::LogCommon(_Inout_ std::wstring&& str)
{
    {
        std::lock_guard<std::mutex> lg(m_LogQueueMutex);
        m_LogQueue.push(std::move(str));
        str.clear();
        m_LogQueueSize++;
    }

    m_CVSignaler.notify_one();
}

/// Public Method Definitions \\\

// Getters

const std::filesystem::path& UnitTestLogger::GetTargetFile() const noexcept
{
    return m_TargetFile;
}

bool UnitTestLogger::GetPrintToConsole() const noexcept
{
    return m_PrintToConsole;
}

bool UnitTestLogger::GetOnlyLogFailures() const noexcept
{
    return m_OnlyLogFailures;
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

bool UnitTestLogger::SetTargetFile(_Inout_ std::filesystem::path&& filePath)
{
    std::wofstream newFileStream(filePath);

    if (!newFileStream)
    {
        return false;
    }

    m_FileStream.flush();
    std::swap(newFileStream, m_FileStream);
    m_TargetFile.assign(std::move(filePath));
    filePath.clear();

    return true;
}

void UnitTestLogger::SetPrintToConsole(_In_ const bool& print) noexcept
{
    m_PrintToConsole = print;
}

void UnitTestLogger::SetOnlyLogFailures(_In_ const bool& log) noexcept
{
    m_OnlyLogFailures = log;
}

// Public Methods

void UnitTestLogger::LogTestSetHeader(_In_ const TestSetData& data)
{
    std::wstring buf(BuildTestSetHeaderString(data));
    buf.append(2, L'\n');
    LogCommon(std::move(buf));
}

void UnitTestLogger::LogUnitTestResult(_In_ const UnitTestResult& res)
{
    if (m_OnlyLogFailures)

    {
        const ResultType& rt = res.GetResult();
        if ((rt == ResultType::Success) || (rt == ResultType::NotRun))
        {
            return;
        }
    }

    std::wstring buf(BuildLogString(res));
    buf.append(2, L'\n');
    LogCommon(std::move(buf));
}

void UnitTestLogger::LogTestSetSummary(_In_ const TestSetData& data)
{
    std::wstring buf = (data.GetTestFailureCount() == 0)
        ? BuildTestSetSummaryNoFailuresString(data)
        : BuildTestSetSummaryFailureDetailsString(data);

    buf.append(2, L'\n');
    LogCommon(std::move(buf));
}

/// Logging Worker Thread Methods \\\

void UnitTestLogger::InitializeWorkerThread()
{
    TeardownWorkerThread();

    m_ContinueWork = true;
    m_WorkerThread = std::thread(&UnitTestLogger::WorkerLoop, this);
}

void UnitTestLogger::TeardownWorkerThread()
{
    if (m_WorkerThread.joinable())
    {
        m_ContinueWork = false;
        m_CVSignaler.notify_all();
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
    m_CVSignaler.wait(ul, [this]() -> bool { return this->WorkerPredicate(); });
}

void UnitTestLogger::PrintLogs()
{
    std::queue<std::wstring> logQueue;
    {
        std::lock_guard<std::mutex> lg(m_LogQueueMutex);
        std::swap(logQueue, m_LogQueue);
        m_LogQueueSize = 0;
    }

    while (!logQueue.empty())
    {
        std::wstring logStr(logQueue.front());
        logQueue.pop();

        PrintLog(logStr);
    }
}

void UnitTestLogger::PrintLog(_In_ const std::wstring& str)
{
    if (m_PrintToConsole && m_ConsoleStream)
    {
        m_ConsoleStream << str;
    }

    if (m_FileStream)
    {
        m_FileStream << str;
    }
}

bool UnitTestLogger::WorkerPredicate()
{
    return (m_LogQueueSize != 0) || (!m_ContinueWork);
}

bool UnitTestLogger::TerminatePredicate()
{
    return (!m_ContinueWork) && (m_LogQueueSize == 0);
}
