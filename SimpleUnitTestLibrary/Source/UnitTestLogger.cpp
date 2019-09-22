#include <UnitTestLogger.h>
#include <UnitTestRunner.h>

#include <cstdarg>
#include <ctime>

// Explicit Template Instantiation
template class UnitTestLogger<char>;
template class UnitTestLogger<wchar_t>;

/// Non-Member Static Variables \\\

static const StrTuple s_HeaderFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "================================"
        "================================\n"
        "  Test-Set Name: %-25s -- Total Tests: %zu\n"
        "================================"
        "================================\n"
    )
);

static const StrTuple s_SummaryNoFailuresFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "===================="
        "====================\n"
        "    \"%s\" Complete\n\n"
        "  Total Test Count: %u\n"
        "  Total Tests Run:  %u\n\n"
        "   Successful Tests: %u\n\n"
        "   Failed Tests:     %u\n\n"
        "   Skipped Tests:    %u\n\n"
        "  Test Pass Grade:  %2.2f%% (%u / %u)\n"
        "  Total Duration:   %llu ms\n"
        "===================="
        "====================\n"
    )
);

static const StrTuple s_SummaryFailureDetailsFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "===================="
        "====================\n"
        "    \"%s\" Complete\n\n"
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
        "====================\n"
    )
);

static const StrTuple s_TimeFormats(
    _SUTL_MAKE_STRING_TUPLE_("%c")
);

static const StrTuple s_SuccessFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "File: %hs\nTest: %hs\n"
        "--------------------------------"
        "--------------------------------\n"
        "    Result:   %s\n"
        "    Line:     %u\n"
        "    Duration: %llu microseconds\n"
        "--------------------------------"
        "--------------------------------\n"
    )
);

static const StrTuple s_FailureFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "File: %hs\nTest: %hs\n"
        "--------------------------------"
        "--------------------------------\n"
        "    Result:   %s\n"
        "    Line:     %u\n"
        "    Failure:  %s\n"
        "    Duration: %llu microseconds\n"
        "--------------------------------"
        "--------------------------------\n"
    )
);

static const StrTuple s_ExceptionFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "File: %hs\nTest: %hs\n"
        "--------------------------------"
        "--------------------------------\n"
        "    Result:    %s\n"
        "    Line:      %u\n"
        "    Exception: %hs\n"
        "    Duration:  %llu microseconds\n"
        "--------------------------------"
        "--------------------------------\n"
    )
);

static const StrTuple s_SkipFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "File: %hs\nTest: %hs\n"
        "--------------------------------"
        "--------------------------------\n"
        "    Result:   %s\n"
        "    Line:     %u\n"
        "    Reason:   %hs\n"
        "    Duration: %llu microseconds\n"
        "--------------------------------"
        "--------------------------------\n"
    )
);

static const StrTuple s_UnhandledExceptionFormats(
    _SUTL_MAKE_STRING_TUPLE_(
        "Result: %hs\n"
        "--------------------------------"
        "--------------------------------\n"
        "    Unhandled Exception: %s\n"
        "    Duration:            %llu microseconds\n"
        "--------------------------------"
        "--------------------------------\n"
    )
);

/// Ctors \\\

template <class T>
UnitTestLogger<T>::UnitTestLogger(_In_ const std::filesystem::path& file, _In_ const bool& consoleOutput, _In_ const bool& onlyLogFailures) :
    m_PrintToConsole(consoleOutput),
    m_OnlyLogFailures(onlyLogFailures),
    m_ConsoleStream(InitConsoleStream()),
    m_FileStream(file, std::ios_base::binary | std::ios_base::app | std::ios_base::out),
    m_TargetFile(file)
{ }

/// Dtor \\\

template <class T>
UnitTestLogger<T>::~UnitTestLogger()
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

template <class T>
UnitTestLogger<T>& UnitTestLogger<T>::operator=(_Inout_ UnitTestLogger&& src) noexcept
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

template <class T>
std::basic_ostream<T>& UnitTestLogger<T>::InitConsoleStream()
{
    if constexpr (std::is_same_v<T, char>)
    {
        return std::cout;
    }
    else if constexpr (std::is_same_v<T, wchar_t>)
    {
        return reinterpret_cast<std::basic_ostream<wchar_t>&>(std::wcout);
    }
    else
    {
        throw std::runtime_error(__FUNCTION__": Unsupported type.");
    }

}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildTestSetHeaderString(_In_ const TestSetData<T>& data)
{
    return stprintf(
        &GetTestSetHeaderFormat(),
        data.GetTestSetName().c_str(),
        data.GetTotalTestCount()
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildTestSetSummaryNoFailuresString(_In_ const TestSetData<T>& data)
{
    return stprintf(
        &GetTestSetSummaryNoFailuresFormat(),
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

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData<T>& data)
{
    return stprintf(
        &GetTestSetSummaryFailureDetailsFormat(),
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

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetTestSetHeaderFormat()
{
    return std::get<std::basic_string<T>>(s_HeaderFormats);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetTestSetSummaryNoFailuresFormat()
{
    return std::get<std::basic_string<T>>(s_SummaryNoFailuresFormats);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetTestSetSummaryFailureDetailsFormat()
{
    return std::get<std::basic_string<T>>(s_SummaryFailureDetailsFormats);
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildLogString(_In_ const UnitTestResult& res)
{
    std::basic_string<T> logStr;

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

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildTimeString()
{
    std::vector<T> buffer(ms_TimeBufferLength, T('\0'));
    const time_t time = std::time(nullptr);

    if (!GetTime(buffer.data(), ms_TimeBufferLength, &time))
    {
        return std::basic_string<T>();
    }

    while (!buffer.empty() && buffer.back() != T('\n'))
    {
        buffer.pop_back();
    }

    if (buffer.size() <= 1)
    {
        return std::basic_string<T>();
    }
    else
    {
        buffer.pop_back();
    }

    buffer.push_back(T('\0'));

    return std::basic_string<T>(buffer.data());
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildSuccessString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &GetSuccessFormat(),
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        GetResultString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetTestDurationMicroseconds()
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildFailureString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &GetFailureFormat(),
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        GetResultString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildExceptionString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &GetExceptionFormat(),
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        GetResultString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildSkipString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &GetSkipFormat(),
        res.GetFileName().c_str(),
        res.GetFunctionName().c_str(),
        GetResultString(res.GetResult()).c_str(),
        res.GetLineNumber(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildUnhandledExceptionString(_In_ const UnitTestResult& res)
{
    return stprintf(
        &GetUnhandledExceptionFormat(),
        GetResultString(res.GetResult()).c_str(),
        res.GetResultInfo().c_str(),
        res.GetTestDurationMicroseconds()
    );
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetResultString(_In_ const ResultType& r)
{
    return ResultTypeUtil::ToString<T>(r);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetTimeFormat()
{
    return std::get<std::basic_string<T>>(s_TimeFormats);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetSuccessFormat()
{
    return std::get<std::basic_string<T>>(s_SuccessFormats);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetFailureFormat()
{
    return std::get<std::basic_string<T>>(s_FailureFormats);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetExceptionFormat()
{
    return std::get<std::basic_string<T>>(s_ExceptionFormats);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetSkipFormat()
{
    return std::get<std::basic_string<T>>(s_SkipFormats);
}

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetUnhandledExceptionFormat()
{
    return std::get<std::basic_string<T>>(s_UnhandledExceptionFormats);
}

template <class T>
bool UnitTestLogger<T>::GetTime(_Out_writes_(len) T* buffer, _In_ const size_t len, _In_ const time_t* t)
{
    if constexpr (std::is_same_v<T, char>)
    {
        return ctime_s(buffer, len, t) == 0;
    }
    else if constexpr (std::is_same_v<T, wchar_t>)
    {
        return _wctime_s(buffer, len, t) == 0;
    }
    else
    {
        throw std::runtime_error(__FUNCTION__" - invalid buffer type.");
    }
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::stprintf(_In_ const std::basic_string<T>* format, ...)
{
    va_list args;
    std::vector<T> buffer;
    int bufferSize = 0;

    if (!format || format->empty())
    {
        return std::basic_string<T>();
    }

    va_start(args, format);

    bufferSize = StringPrintWrapper(buffer, format, args);

    if (bufferSize > 0)
    {
        buffer = std::move(std::vector<T>(static_cast<size_t>(bufferSize) + 2, T('\0')));
        bufferSize = StringPrintWrapper(buffer, format, args);
    }

    va_end(args);

    return (bufferSize > 0) ? std::basic_string<T>(std::move(buffer.data())) : std::basic_string<T>();
}

template <class T>
int UnitTestLogger<T>::StringPrintWrapper(_Inout_ std::vector<T>& buffer, _In_ const std::basic_string<T>* format, _In_ va_list args)
{
    if constexpr (std::is_same_v<T, char>)
    {
        if (buffer.empty())
        {
            return _vscprintf(
                reinterpret_cast<const char*>(format->c_str()),
                args
            );
        }
        else
        {
            return vsnprintf(
                reinterpret_cast<char*>(buffer.data()),
                buffer.size() - 1,
                reinterpret_cast<const char*>(format->c_str()),
                args
            );
        }
    }
    else if constexpr (std::is_same_v<T, wchar_t>)
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
    else
    {
        throw std::runtime_error(__FUNCTION__": Unsupported type.");
    }
}

template <class T>
void UnitTestLogger<T>::LogCommon(_Inout_ std::basic_string<T>&& str)
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

template <class T>
const std::filesystem::path& UnitTestLogger<T>::GetTargetFile() const noexcept
{
    return m_TargetFile;
}

template <class T>
bool UnitTestLogger<T>::GetPrintToConsole() const noexcept
{
    return m_PrintToConsole;
}

template <class T>
bool UnitTestLogger<T>::GetOnlyLogFailures() const noexcept
{
    return m_OnlyLogFailures;
}

// Setters

template <class T>
bool UnitTestLogger<T>::SetTargetFile(_In_ const std::filesystem::path& filePath)
{
    std::basic_ofstream<T> newFileStream(filePath);

    if (!newFileStream)
    {
        return false;
    }

    m_FileStream.flush();
    std::swap(newFileStream, m_FileStream);
    m_TargetFile.assign(filePath);

    return true;
}

template <class T>
bool UnitTestLogger<T>::SetTargetFile(_Inout_ std::filesystem::path&& filePath)
{
    std::basic_ofstream<T> newFileStream(filePath);

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

template <class T>
void UnitTestLogger<T>::SetPrintToConsole(_In_ const bool& print) noexcept
{
    m_PrintToConsole = print;
}

template <class T>
void UnitTestLogger<T>::SetOnlyLogFailures(_In_ const bool& log) noexcept
{
    m_OnlyLogFailures = log;
}

// Public Methods

template <class T>
void UnitTestLogger<T>::LogTestSetHeader(_In_ const TestSetData<T>& data)
{
    std::basic_string<T> buf(BuildTestSetHeaderString(data));
    buf.append(2, static_cast<T>('\n'));
    LogCommon(std::move(buf));
}

template <class T>
void UnitTestLogger<T>::LogUnitTestResult(_In_ const UnitTestResult& res)
{
    if (m_OnlyLogFailures)

    {
        const ResultType& rt = res.GetResult();
        if ((rt == ResultType::Success) || (rt == ResultType::NotRun))
        {
            return;
        }
    }

    std::basic_string<T> buf(BuildLogString(res));
    buf.append(2, static_cast<T>('\n'));
    LogCommon(std::move(buf));
}

template <class T>
void UnitTestLogger<T>::LogTestSetSummary(_In_ const TestSetData<T>& data)
{
    std::basic_string<T> buf = (data.GetTestFailureCount() == 0)
        ? BuildTestSetSummaryNoFailuresString(data)
        : BuildTestSetSummaryFailureDetailsString(data);

    buf.append(2, static_cast<T>('\n'));
    LogCommon(std::move(buf));
}

/// Logging Worker Thread Methods \\\

template <class T>
void UnitTestLogger<T>::InitializeWorkerThread()
{
    TeardownWorkerThread();

    m_ContinueWork = true;
    m_WorkerThread = std::thread(&UnitTestLogger<T>::WorkerLoop, this);
}

template <class T>
void UnitTestLogger<T>::TeardownWorkerThread()
{
    if (m_WorkerThread.joinable())
    {
        m_ContinueWork = false;
        m_CVSignaler.notify_all();
        m_WorkerThread.join();
    }
}

template <class T>
void UnitTestLogger<T>::WorkerLoop()
{
    while (!TerminatePredicate())
    {
        WaitForWork();
        PrintLogs();
    }
}

template <class T>
void UnitTestLogger<T>::WaitForWork()
{
    std::unique_lock<std::mutex> ul(m_LogQueueMutex);
    m_CVSignaler.wait(ul, [this]() -> bool { return this->WorkerPredicate(); });
}

template <class T>
void UnitTestLogger<T>::PrintLogs()
{
    std::queue<std::basic_string<T>> logQueue;
    {
        std::lock_guard<std::mutex> lg(m_LogQueueMutex);
        std::swap(logQueue, m_LogQueue);
        m_LogQueueSize = 0;
    }

    while (!logQueue.empty())
    {
        std::basic_string<T> logStr(logQueue.front());
        logQueue.pop();

        PrintLog(logStr);
    }
}

template <class T>
void UnitTestLogger<T>::PrintLog(_In_ const std::basic_string<T>& str)
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

template <class T>
bool UnitTestLogger<T>::WorkerPredicate()
{
    return (m_LogQueueSize != 0) || (!m_ContinueWork);
}

template <class T>
bool UnitTestLogger<T>::TerminatePredicate()
{
    return (!m_ContinueWork) && (m_LogQueueSize == 0);
}
