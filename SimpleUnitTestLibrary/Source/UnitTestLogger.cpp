#include <UnitTestLogger.h>

// STL
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

// Static Data Member Initialization //

const char* const UnitTestLogger::ms_pHeaderFormat(
    "================================"
    "================================\n"
    "  Test-Set Name: %-25.*s -- Total Tests: %u\n"
    "================================"
    "================================\n\n\n"
);

const char* const UnitTestLogger::ms_pSummaryNoFailuresFormat(
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
);

const char* const UnitTestLogger::ms_pSummaryFailureDetailsFormat(
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
);

const char* const UnitTestLogger::ms_pSuccessFormat(
    "File: %.*s\nTest: %.*s\n"
    "--------------------------------"
    "--------------------------------\n"
    "    Result:   %.*s\n"
    "    Line:     %u\n"
    "    Duration: %llu microseconds\n"
    "--------------------------------"
    "--------------------------------\n\n\n"
);

const char* const UnitTestLogger::ms_pFailureFormat(
    "File: %.*s\nTest: %.*s\n"
    "--------------------------------"
    "--------------------------------\n"
    "    Result:   %.*s\n"
    "    Line:     %u\n"
    "    Failure:  %.*s\n"
    "    Duration: %llu microseconds\n"
    "--------------------------------"
    "--------------------------------\n\n\n"
);

const char* const UnitTestLogger::ms_pExceptionFormat(
    "File: %s\nTest: %s\n"
    "--------------------------------"
    "--------------------------------\n"
    "    Result:    %.*s\n"
    "    Line:      %u\n"
    "    Exception: %.*s\n"
    "    Duration:  %llu microseconds\n"
    "--------------------------------"
    "--------------------------------\n\n\n"
);

const char* const UnitTestLogger::ms_pSkipFormat(
    "File: %s\nTest: %s\n"
    "--------------------------------"
    "--------------------------------\n"
    "    Result:   %.*s\n"
    "    Line:     %u\n"
    "    Reason:   %.*s\n"
    "    Duration: %llu microseconds\n"
    "--------------------------------"
    "--------------------------------\n\n\n"
);

const char* const UnitTestLogger::ms_pUnhandledExceptionFormat(
    "Result: %.*s\n"
    "--------------------------------"
    "--------------------------------\n"
    "    Unhandled Exception: %.*s\n"
    "    Duration:            %llu microseconds\n"
    "--------------------------------"
    "--------------------------------\n\n\n"
);

// Dtor //

UnitTestLogger::~UnitTestLogger() noexcept
{
    TeardownWorkerThread();

    if (m_bLogToStdout)
    {
        fflush(stdout);
    }

    if (!!m_pFile)
    {
        fflush(m_pFile);
        fclose(m_pFile);
    }

    delete[] m_pLogFilePath;
}

// Method Definitions //

// Private Helper Methods

UnitTestLogger::Buffer UnitTestLogger::BuildTestSetHeaderString(_In_ const TestSetData& data) noexcept
{
    return stprintf(
        ms_pHeaderFormat,
        static_cast<int>(data.GetTestSetNameLength()),
        data.GetTestSetName(),
        data.GetTotalTestCount()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildTestSetSummaryNoFailuresString(_In_ const TestSetData& data) noexcept
{
    return stprintf(
        ms_pSummaryNoFailuresFormat,
        static_cast<int>(data.GetTestSetNameLength()),
        data.GetTestSetName(),
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

UnitTestLogger::Buffer UnitTestLogger::BuildTestSetSummaryFailureDetailsString(_In_ const TestSetData& data) noexcept
{
    return stprintf(
        ms_pSummaryFailureDetailsFormat,
        static_cast<int>(data.GetTestSetNameLength()),
        data.GetTestSetName(),
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

UnitTestLogger::Buffer UnitTestLogger::BuildLogString(_In_ const UnitTestResult& res) noexcept
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

    case ResultType::End:
    default:
        logStr.push_back('\0');
    }

    return logStr;
}

UnitTestLogger::Buffer UnitTestLogger::BuildSuccessString(_In_ const UnitTestResult& res) noexcept
{
    return stprintf(
        ms_pSuccessFormat,
        static_cast<int>(res.GetFileNameLength()),
        res.GetFileName(),
        static_cast<int>(res.GetFunctionNameLength()),
        res.GetFunctionName(),
        static_cast<int>(ResultTypeUtil::ToString(res.GetResult()).second),
        ResultTypeUtil::ToString(res.GetResult()).first,
        res.GetLineNumber(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildFailureString(_In_ const UnitTestResult& res) noexcept
{
    return stprintf(
        ms_pFailureFormat,
        static_cast<int>(res.GetFileNameLength()),
        res.GetFileName(),
        static_cast<int>(res.GetFunctionNameLength()),
        res.GetFunctionName(),
        static_cast<int>(ResultTypeUtil::ToString(res.GetResult()).second),
        ResultTypeUtil::ToString(res.GetResult()).first,
        res.GetLineNumber(),
        static_cast<int>(res.GetInfoLength()),
        res.GetInfo(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildExceptionString(_In_ const UnitTestResult& res) noexcept
{
    return stprintf(
        ms_pExceptionFormat,
        static_cast<int>(res.GetFileNameLength()),
        res.GetFileName(),
        static_cast<int>(res.GetFunctionNameLength()),
        res.GetFunctionName(),
        static_cast<int>(ResultTypeUtil::ToString(res.GetResult()).second),
        ResultTypeUtil::ToString(res.GetResult()).first,
        res.GetLineNumber(),
        static_cast<int>(res.GetInfoLength()),
        res.GetInfo(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildSkipString(_In_ const UnitTestResult& res) noexcept
{
    return stprintf(
        ms_pSkipFormat,
        static_cast<int>(res.GetFileNameLength()),
        res.GetFileName(),
        static_cast<int>(res.GetFunctionNameLength()),
        res.GetFunctionName(),
        static_cast<int>(ResultTypeUtil::ToString(res.GetResult()).second),
        ResultTypeUtil::ToString(res.GetResult()).first,
        res.GetLineNumber(),
        static_cast<int>(res.GetInfoLength()),
        res.GetInfo(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::BuildUnhandledExceptionString(_In_ const UnitTestResult& res) noexcept
{
    return stprintf(
        ms_pUnhandledExceptionFormat,
        static_cast<int>(ResultTypeUtil::ToString(res.GetResult()).second),
        ResultTypeUtil::ToString(res.GetResult()).first,
        static_cast<int>(res.GetInfoLength()),
        res.GetInfo(),
        res.GetTestDurationMicroseconds()
    );
}

UnitTestLogger::Buffer UnitTestLogger::stprintf(_In_z_ const char* format, ...) noexcept
{
    va_list args;
    Buffer buffer;
    int bufferSize = 0;

    if (!format)
    {
        return buffer;
    }

    va_start(args, format);

    bufferSize = vsnprintf(nullptr, 0, format, args);

    va_end(args);

    if (bufferSize > 0)
    {
        va_start(args, format);
        try
        {
            buffer = std::vector<char>(static_cast<size_t>(bufferSize) + 1, '\0');
        }
        catch (const std::exception& e)
        {
            fprintf(stderr, "    %s: Failed to allocate buffer for log - exception[%s]",
                __FUNCTION__, e.what());
            va_end(args);
            return buffer;
        }

        if (vsnprintf(buffer.data(), buffer.size(), format, args) < 0)
        {
            fprintf(stderr, "    %s: Failed to write log content to allocated buffer.", __FUNCTION__);
        }

        va_end(args);
    }

    return buffer;
}

void UnitTestLogger::QueueLog(_Inout_ Buffer&& str) noexcept
{
    bool bInitialized = IsWorkerThreadInitialized();

    try
    {
        if (!bInitialized)
        {
            InitializeWorkerThread();
            bInitialized = true;
        }

        std::lock_guard<std::mutex> lg(*m_pLogQueueMutex);
        m_pLogQueue->push_back(std::move(str));
        ++m_LogQueueSize;
    }
    catch (const std::exception & e)
    {
        fprintf(stderr, "    %s: Exception caught when queueing log for worker thread - exception[%s]",
            __FUNCTION__, e.what());
        return;
    }

    if (bInitialized)
    {
        m_pLogCV->notify_one();
    }

    str.clear();
}

// Public Method Definitions //

// Setters

_Success_(return)
bool UnitTestLogger::SetLogFilePath(_In_ const std::string& logFilePath) noexcept
{
    FILE* pNewFile = nullptr;

    if (logFilePath.empty())
    {
        // Interpret empty file path as wanting no file logging.
        if (!!m_pFile)
        {
            fclose(m_pFile);
            m_pFile = nullptr;
        }

        delete[] m_pLogFilePath;
        m_pLogFilePath = nullptr;

        return true;
    }

    pNewFile = fopen(logFilePath.c_str(), "ab");
    if (!pNewFile)
    {
        printf("%s: Failed to open specified log file - errno[%s]\n\n",
            __FUNCTION__, strerror(errno));
        return false;
    }

    try
    {
        const size_t len = logFilePath.length();
        char* pNewPath = new char[len + 1];
        memcpy(pNewPath, logFilePath.c_str(), len + 1);
        delete[] m_pLogFilePath;
        m_pLogFilePath = pNewPath;
    }
    catch (const std::exception& e)
    {
        // Close handle to new file, we'll be keeping the old one for now.
        fprintf(stderr, "    %s: Failed to set new log file path - exception[%s]\n\n",
            __FUNCTION__, e.what());
        fclose(pNewFile);
        return false;
    }

    if (!!m_pFile)
    {
        fclose(m_pFile);
    }

    m_pFile = pNewFile;

    return true;
}

void UnitTestLogger::LogTestSetHeader(_In_ const TestSetData& data) noexcept
{
    Buffer buf;
    try
    {
        buf = BuildTestSetHeaderString(data);
    }
    catch (const std::exception & e)
    {
        fprintf(stderr, "    %s: Exception caught when building log message for worker thread - exception[%s]\n\n",
            __FUNCTION__, e.what());
    }

    if (!buf.empty())
    {
        QueueLog(std::move(buf));
    }
}

void UnitTestLogger::LogUnitTestResult(_In_ const UnitTestResult& result) noexcept
{
    if (!m_bOnlyLogFailures || ((result.GetResult() != ResultType::Success) && (result.GetResult() != ResultType::NotRun)))
    {
        Buffer buf;
        try
        {
            buf = BuildLogString(result);
        }
        catch (const std::exception& e)
        {
            fprintf(stderr, "    %s: Exception caught when building log message for worker thread - exception[%s]\n\n",
                __FUNCTION__, e.what());
        }

        if (!buf.empty())
        {
            QueueLog(std::move(buf));
        }
    }
}

void UnitTestLogger::LogTestSetSummary(_In_ const TestSetData& data) noexcept
{
    Buffer buf;

    try
    {
        buf = ((data.GetTestFailureCount() == 0)
            ? BuildTestSetSummaryNoFailuresString(data)
            : BuildTestSetSummaryFailureDetailsString(data));
    }
    catch (const std::exception & e)
    {
        fprintf(stderr, "    %s: Exception caught when building log message for worker thread - exception[%s]\n\n",
            __FUNCTION__, e.what());
    }

    if (!buf.empty())
    {
        QueueLog(std::move(buf));
    }
}

// Logging Worker Thread Methods //

void UnitTestLogger::InitializeWorkerThread()
{
    // Avoid re-initing thread.
    TeardownWorkerThread();

    m_bContinueWork = true;

    m_pLogQueue = new Queue;
    m_pLogQueueMutex = new std::mutex;
    m_pLogCV = new std::condition_variable;
    m_pWorkerThread = new std::thread(&UnitTestLogger::WorkerLoop, this);
}

void UnitTestLogger::TeardownWorkerThread()
{
    if (!IsWorkerThreadInitialized())
    {
        return;
    }

    // We _must_ change the value under lock in order for the waiting thread(s) to be updated appropriately.
    {
        std::lock_guard<std::mutex> ul(*m_pLogQueueMutex);
        m_bContinueWork.store(false, std::memory_order_relaxed);
    }

    m_pLogCV->notify_all();

    m_pWorkerThread->join();
    m_LogQueueSize = 0;

    // Deallocate resources.
    delete m_pWorkerThread;
    delete m_pLogCV;
    delete m_pLogQueueMutex;
    delete m_pLogQueue;
}

bool UnitTestLogger::WorkerPredicate() noexcept
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

bool UnitTestLogger::TerminatePredicate() noexcept
{
    return (!m_bContinueWork) && (m_LogQueueSize == 0);
}

void UnitTestLogger::WorkerLoop() noexcept
{
    while (!TerminatePredicate())
    {
        WaitForWork();
        PrintLogs();
    }
}

void UnitTestLogger::WaitForWork() noexcept
{
    std::unique_lock<std::mutex> ul(*m_pLogQueueMutex);
    m_pLogCV->wait(ul, [this]() -> bool { return this->WorkerPredicate(); });
}

UnitTestLogger::Queue UnitTestLogger::TransferLogQueueContents() noexcept
{
    Queue q;
    std::lock_guard<std::mutex> lg(*m_pLogQueueMutex);
    std::swap(q, *m_pLogQueue);
    m_LogQueueSize = 0;
    return q;
}

void UnitTestLogger::PrintLogs() noexcept
{
    for (const auto& buf : TransferLogQueueContents())
    {
        PrintLog(buf);
    }
}

void UnitTestLogger::PrintLog(_In_ const Buffer& buf) noexcept
{
    if (buf.empty())
    {
        return;
    }

    if (m_bLogToStdout)
    {
        printf("%.*s", static_cast<unsigned int>(buf.size() - 1), buf.data());
    }

    if (!!m_pFile)
    {
        fwrite(buf.data(), sizeof(Buffer::value_type), buf.size() - 1, m_pFile);
    }
}
