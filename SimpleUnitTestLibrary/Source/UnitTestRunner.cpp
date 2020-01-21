#include <UnitTestRunner.h>

/// Static Data Member Initialization \\\

std::list<UnitTest> UnitTestRunner::ms_DummyList;


/// Public Methods \\\

_Success_(return) bool UnitTestRunner::AddUnitTest(_Inout_ UnitTest&& test) noexcept
{
    try
    {
        AllocateTestListIfNeeded();
        m_pUnitTests->push_back(std::move(test));
        test.Clear();
    }
    catch (...)
    {
        return false;
    }

    return true;
}


_Success_(return) bool UnitTestRunner::AddUnitTest(_In_ const UnitTestFunction pfFunc) noexcept
{
    if (!pfFunc)
    {
        return false;
    }

    try
    {
        AllocateTestListIfNeeded();
        m_pUnitTests->emplace_back(pfFunc);
    }
    catch (...)
    {
        return false;
    }

    return true;
}


_Success_(return) bool UnitTestRunner::AddUnitTest(_In_ const std::function<UnitTestResult(void)>& func) noexcept
{
    if (!func)
    {
        return false;
    }

    try
    {
        auto pF = func.target<UnitTestFunction>();
        if (!!pF)
        {
            AllocateTestListIfNeeded();
            m_pUnitTests->emplace_back(*pF);
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}


_Success_(return) bool UnitTestRunner::AddUnitTests(_Inout_ std::list<UnitTest>&& tests) noexcept
{
    try
    {
        AllocateTestListIfNeeded();
        m_pUnitTests->splice(m_pUnitTests->cend(), std::move(tests));
    }
    catch (...)
    {
        return false;
    }

    tests.clear();

    return true;
}


_Success_(return) bool UnitTestRunner::AddUnitTests(_Inout_ std::list<UnitTestFunction>&& tests) noexcept
{
    try
    {
        for (auto& f : tests)
        {
            if (!!f)
            {
                AllocateTestListIfNeeded();
                m_pUnitTests->emplace_back(std::move(f));
            }
        }
    }
    catch (...)
    {
        return false;
    }

    tests.clear();

    return true;
}


_Success_(return) bool UnitTestRunner::AddUnitTests(_Inout_ std::list<std::function<UnitTestResult(void)>>&& tests) noexcept
{
    try
    {
        for (auto& f : tests)
        {
            auto pF = f.target<UnitTestFunction>();
            if (!!pF)
            {
                AllocateTestListIfNeeded();
                m_pUnitTests->emplace_back(*pF);
            }
        }
    }
    catch (...)
    {
        return false;
    }

    tests.clear();

    return true;
}


bool UnitTestRunner::RunUnitTests() noexcept
{
    using Clock = std::chrono::high_resolution_clock;
    using DurationMs = std::chrono::duration<int64_t, std::milli>;
    using DurationMicroseconds = std::chrono::duration<int64_t, std::micro>;
    using TimePoint = std::chrono::time_point<Clock>;

    bool ret = true;
    std::list<UnitTest>* pTests = (!!m_pUnitTests) ? m_pUnitTests : &ms_DummyList;

    m_TestSetData.ResetCounters();
    m_TestSetData.SetTotalTestCount(static_cast<uint32_t>(pTests->size()));

    m_Logger.LogTestSetHeader(m_TestSetData);

    for (auto& test : *pTests)
    {
        TimePoint t0, t1;
        DurationMs deltaMs;
        DurationMicroseconds deltaMicroseconds;
        ResultType r = ResultType::NotRun;
        std::string eStr;

        t0 = Clock::now();
        try
        {
            test.RunTest();
            t1 = Clock::now();
            r = test.GetUnitTestResult().GetResult();
        }
        catch (const std::exception& e)
        {
            t1 = Clock::now();
            r = ResultType::UnhandledException;
            eStr = e.what();
        }

        m_TestSetData.IncrementResultCounter(r);

        deltaMs = std::chrono::duration_cast<DurationMs, int64_t>(t1 - t0);
        deltaMicroseconds = std::chrono::duration_cast<DurationMicroseconds, int64_t>(t1 - t0);
        m_TestSetData.AddToRunDurationMs(deltaMs);
        test.GetUnitTestResult().SetTestDurationMicroseconds(deltaMicroseconds);

        if (r != ResultType::Success && r != ResultType::NotRun)
        {
            ret = false;
        }
        
        if (r != ResultType::UnhandledException)
        {
            m_Logger.LogUnitTestResult(test.GetUnitTestResult());
        }
        else
        {
            const auto& res = test.GetUnitTestResult();
            res.SetTestDurationMicroseconds(deltaMicroseconds);
            m_Logger.LogUnitTestResult(res);
        }
    }

    m_Logger.LogTestSetSummary(m_TestSetData);

    return ret;
}
