#include <TestSetData.h>


TestSetData::TestSetData() noexcept :
    m_TotalTests(0),
    m_TestPassCount(0),
    m_TotalFailureCount(0),
    m_SetupFailureCount(0),
    m_TestFailureCount(0),
    m_CleanupFailureCount(0),
    m_SetupExceptionCount(0),
    m_TestExceptionCount(0),
    m_CleanupExceptionCount(0),
    m_UnhandledExceptionCount(0),
    m_TestSkipCount(0),
    m_RunDurationMs(0)
{ }

TestSetData::TestSetData(_Inout_ std::wstring&& testSetName) noexcept :
    TestSetData()
{
    m_TestSetName = std::move(testSetName);
    testSetName.clear();
}

TestSetData::TestSetData(_In_ const TestSetData& src) :
    TestSetData()
{
    *this = src;
}

TestSetData::TestSetData(_Inout_ TestSetData&& src) noexcept :
    TestSetData()
{
    *this = std::move(src);
}

TestSetData& TestSetData::operator=(_In_ const TestSetData& src) noexcept
{
    if (this != &src)
    {
        m_TotalTests = src.m_TotalTests;
        m_TestPassCount = src.m_TestPassCount;
        m_TotalFailureCount = src.m_TotalFailureCount;
        m_SetupFailureCount = src.m_SetupFailureCount;
        m_TestFailureCount = src.m_TestFailureCount;
        m_CleanupFailureCount = src.m_CleanupFailureCount;
        m_SetupExceptionCount = src.m_SetupExceptionCount;
        m_TestExceptionCount = src.m_TestExceptionCount;
        m_CleanupExceptionCount = src.m_CleanupExceptionCount;
        m_UnhandledExceptionCount = src.m_UnhandledExceptionCount;
        m_TestSkipCount = src.m_TestSkipCount;
        m_RunDurationMs = src.m_RunDurationMs;
        m_TestSetName = src.m_TestSetName;
    }

    return *this;
}

TestSetData& TestSetData::operator=(_Inout_ TestSetData&& src) noexcept
{
    if (this == &src)
    {
        m_TotalTests = src.m_TotalTests;
        m_TestPassCount = src.m_TestPassCount;
        m_TotalFailureCount = src.m_TotalFailureCount;
        m_SetupFailureCount = src.m_SetupFailureCount;
        m_TestFailureCount = src.m_TestFailureCount;
        m_CleanupFailureCount = src.m_CleanupFailureCount;
        m_SetupExceptionCount = src.m_SetupExceptionCount;
        m_TestExceptionCount = src.m_TestExceptionCount;
        m_CleanupExceptionCount = src.m_CleanupExceptionCount;
        m_UnhandledExceptionCount = src.m_UnhandledExceptionCount;
        m_TestSkipCount = src.m_TestSkipCount;
        m_RunDurationMs = src.m_RunDurationMs;
        m_TestSetName = std::move(src.m_TestSetName);

        src.ClearAll();
    }

    return *this;
}

uint32_t TestSetData::GetTotalTestCount() const noexcept
{
    return m_TotalTests;
}

uint32_t TestSetData::GetTestPassCount() const noexcept
{
    return m_TestPassCount;
}

uint32_t TestSetData::GetTotalFailureCount() const noexcept
{
    return m_TotalFailureCount;
}

uint32_t TestSetData::GetSetupFailureCount() const noexcept
{
    return m_SetupFailureCount;
}

uint32_t TestSetData::GetTestFailureCount() const noexcept
{
    return m_TestFailureCount;
}

uint32_t TestSetData::GetCleanupFailureCount() const noexcept
{
    return m_CleanupFailureCount;
}

uint32_t TestSetData::GetSetupExceptionCount() const noexcept
{
    return m_SetupExceptionCount;
}

uint32_t TestSetData::GetTestExceptionCount() const noexcept
{
    return m_TestExceptionCount;
}

uint32_t TestSetData::GetCleanupExceptionCount() const noexcept
{
    return m_CleanupExceptionCount;
}

uint32_t TestSetData::GetUnhandledExceptionCount() const noexcept
{
    return m_UnhandledExceptionCount;
}

uint32_t TestSetData::GetTestSkipCount() const noexcept
{
    return m_TestSkipCount;
}

uint64_t TestSetData::GetRunDurationMs() const noexcept
{
    return m_RunDurationMs;
}

const std::wstring& TestSetData::GetTestSetName() const noexcept
{
    return m_TestSetName;
}

void TestSetData::IncrementResultCounter(_In_ const ResultType& r) noexcept
{
    switch (r)
    {
    case ResultType::NotRun:
        m_TestSkipCount++;
        break;

    case ResultType::Success:
        m_TestPassCount++;
        break;

    case ResultType::SetupFailure:
        m_SetupFailureCount++;
        m_TotalFailureCount++;
        break;

    case ResultType::SetupException:
        m_SetupExceptionCount++;
        m_TotalFailureCount++;
        break;

    case ResultType::TestFailure:
        m_TestFailureCount++;
        m_TotalFailureCount++;
        break;

    case ResultType::TestException:
        m_TestExceptionCount++;
        m_TotalFailureCount++;
        break;

    case ResultType::CleanupFailure:
        m_CleanupFailureCount++;
        m_TotalFailureCount++;
        break;

    case ResultType::CleanupException:
        m_CleanupExceptionCount++;
        m_TotalFailureCount++;
        break;

    case ResultType::UnhandledException:
        m_UnhandledExceptionCount++;
        m_TotalFailureCount++;
        break;

    default:
        return;
    }
}

void TestSetData::AddToRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept
{
    if (dur.count() > 0)
    {
        m_RunDurationMs += static_cast<uint64_t>(dur.count());
    }
}

void TestSetData::SetTotalTestCount(_In_ const uint32_t& c) noexcept
{
    m_TotalTests = c;
}

void TestSetData::SetRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept
{
    m_RunDurationMs = (dur.count() > 0) ? static_cast<uint64_t>(dur.count()) : 0;
}

void TestSetData::SetTestSetName(_In_ std::wstring name)
{
    m_TestSetName = std::move(name);
}

void TestSetData::SetTestSetName(_Inout_ std::wstring&& name) noexcept
{
    m_TestSetName = std::move(name);
    name.clear();
}

void TestSetData::ResetCounters() noexcept
{
    m_TotalTests = 0;
    m_TestPassCount = 0;
    m_TotalFailureCount = 0;
    m_SetupFailureCount = 0;
    m_TestFailureCount = 0;
    m_CleanupFailureCount = 0;
    m_SetupExceptionCount = 0;
    m_TestExceptionCount = 0;
    m_CleanupExceptionCount = 0;
    m_UnhandledExceptionCount = 0;
    m_TestSkipCount = 0;
    m_RunDurationMs = 0;
}

void TestSetData::ClearAll() noexcept
{
    ResetCounters();
    m_TestSetName.clear();
}

long double TestSetData::GetTestSetGrade() const noexcept
{
    const uint32_t testRunCount = m_TotalTests - m_TestSkipCount;
    return (testRunCount > 0) ? 100.0L * static_cast<long double>(m_TestPassCount) / static_cast<long double>(testRunCount) : 0.0L;
}
