#include <TestSetData.h>

// Explicit Template Instantiation
template class TestSetData<char>;
template class TestSetData<wchar_t>;


template <class T>
TestSetData<T>::TestSetData() noexcept :
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

template <class T>
TestSetData<T>::TestSetData(_In_ const std::basic_string<T>& testSetName) :
    TestSetData()
{ 
    m_TestSetName = testSetName;
}

template <class T>
TestSetData<T>::TestSetData(_Inout_ std::basic_string<T>&& testSetName) noexcept :
    TestSetData()
{
    m_TestSetName = std::move(testSetName);
    testSetName.clear();
}

template <class T>
TestSetData<T>::TestSetData(_In_ const TestSetData<T>& src) :
    TestSetData()
{
    *this = src;
}

template <class T>
TestSetData<T>::TestSetData(_Inout_ TestSetData<T>&& src) noexcept :
    TestSetData()
{
    *this = std::move(src);
}

template <class T>
TestSetData<T>& TestSetData<T>::operator=(_In_ const TestSetData<T>& src) noexcept
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

template <class T>
TestSetData<T>& TestSetData<T>::operator=(_Inout_ TestSetData<T>&& src) noexcept
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

template <class T>
uint32_t TestSetData<T>::GetTotalTestCount() const noexcept
{
    return m_TotalTests;
}

template <class T>
uint32_t TestSetData<T>::GetTestPassCount() const noexcept
{
    return m_TestPassCount;
}

template <class T>
uint32_t TestSetData<T>::GetTotalFailureCount() const noexcept
{
    return m_TotalFailureCount;
}

template <class T>
uint32_t TestSetData<T>::GetSetupFailureCount() const noexcept
{
    return m_SetupFailureCount;
}

template <class T>
uint32_t TestSetData<T>::GetTestFailureCount() const noexcept
{
    return m_TestFailureCount;
}

template <class T>
uint32_t TestSetData<T>::GetCleanupFailureCount() const noexcept
{
    return m_CleanupFailureCount;
}

template <class T>
uint32_t TestSetData<T>::GetSetupExceptionCount() const noexcept
{
    return m_SetupExceptionCount;
}

template <class T>
uint32_t TestSetData<T>::GetTestExceptionCount() const noexcept
{
    return m_TestExceptionCount;
}

template <class T>
uint32_t TestSetData<T>::GetCleanupExceptionCount() const noexcept
{
    return m_CleanupExceptionCount;
}

template <class T>
uint32_t TestSetData<T>::GetUnhandledExceptionCount() const noexcept
{
    return m_UnhandledExceptionCount;
}

template <class T>
uint32_t TestSetData<T>::GetTestSkipCount() const noexcept
{
    return m_TestSkipCount;
}

template <class T>
uint64_t TestSetData<T>::GetRunDurationMs() const noexcept
{
    return m_RunDurationMs;
}

template<class T>
const std::basic_string<T>& TestSetData<T>::GetTestSetName() const noexcept
{
    return m_TestSetName;
}

template <class T>
void TestSetData<T>::IncrementResultCounter(_In_ const ResultType& r) noexcept
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

template <class T>
void TestSetData<T>::AddToRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept
{
    if (dur.count() > 0)
    {
        m_RunDurationMs += static_cast<uint64_t>(dur.count());
    }
}

template <class T>
void TestSetData<T>::SetTotalTestCount(_In_ const uint32_t& c) noexcept
{
    m_TotalTests = c;
}

template <class T>
void TestSetData<T>::SetRunDurationMs(_In_ const std::chrono::duration<int64_t, std::milli>& dur) noexcept
{
    m_RunDurationMs = (dur.count() > 0) ? static_cast<uint64_t>(dur.count()) : 0;
}

template<class T>
void TestSetData<T>::SetTestSetName(_In_ const std::basic_string<T>& name)
{
    m_TestSetName = name;
}

template<class T>
void TestSetData<T>::SetTestSetName(_Inout_ std::basic_string<T>&& name) noexcept
{
    m_TestSetName = std::move(name);
    name.clear();
}

template <class T>
void TestSetData<T>::ResetCounters() noexcept
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

template <class T>
void TestSetData<T>::ClearAll() noexcept
{
    ResetCounters();
    m_TestSetName.clear();
}

template <class T>
long double TestSetData<T>::GetTestSetGrade() const noexcept
{
    const uint32_t testRunCount = m_TotalTests - m_TestSkipCount;
    return (testRunCount > 0) ? 100.0L * static_cast<long double>(m_TestPassCount) / static_cast<long double>(testRunCount) : 0.0L;
}
