#include <UnitTest.h>

// STL
#include <stdexcept>

/// Method Definitons \\\

/// Ctors \\\
    
UnitTest::UnitTest(_Inout_ std::function<UnitTestResult(void)>&& func) noexcept :
    m_TestFunc(std::move(func))
{
    func = nullptr;
}

UnitTest::UnitTest(_Inout_ UnitTest&& src) noexcept :
    UnitTest()
{
    *this = std::move(src);
}

// Operator Overloads

UnitTest& UnitTest::operator=(_Inout_ UnitTest&& src) noexcept
{
    if (this != &src)
    {
        m_TestFunc = std::move(src.m_TestFunc);
        m_TestResult = std::move(src.m_TestResult);

        src.Clear();
    }

    return *this;
}

// Getters

const std::function<UnitTestResult(void)>& UnitTest::GetUnitTestFunction() const noexcept
{
    return m_TestFunc;
}

const UnitTestResult& UnitTest::GetUnitTestResult() const noexcept
{
    return m_TestResult;
}

// Setters

void UnitTest::SetUnitTestFunction(_Inout_ std::function<UnitTestResult(void)>&& func) noexcept
{
    m_TestFunc = std::move(func);
    m_TestResult = UnitTestResult();
}

// Public Methods

void UnitTest::Clear() noexcept
{
    m_TestFunc = nullptr;
    m_TestResult = UnitTestResult();
}

const UnitTestResult& UnitTest::RunTest()
{
    if (!m_TestFunc)
    {
        throw std::logic_error(__FUNCTION__": No function is associated with this UnitTest.");
    }

    return m_TestResult = m_TestFunc();
}
