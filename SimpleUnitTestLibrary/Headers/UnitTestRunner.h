#pragma once

// STL
#include <list>

// SUTL
#include "UnitTest.h"
#include "UnitTestLogger.h"
#include "TestSetData.h"


class UnitTestRunner
{
private:
    std::list<UnitTest> m_UnitTests;
    mutable UnitTestLogger m_Logger;
    TestSetData m_TestSetData;

public:
    /// Ctors \\\

    explicit UnitTestRunner(_Inout_ std::wstring&&) noexcept;

    /// Dtor \\\

    ~UnitTestRunner() = default;

    /// Operator Overloads \\\

    UnitTestRunner& operator=(_Inout_ UnitTestRunner&&) noexcept;

    /// Getters \\\

    const std::list<UnitTest>& GetUnitTests() const noexcept;
    UnitTestLogger& GetLogger() const noexcept;
    TestSetData& GetTestSetData() noexcept;
    const TestSetData& GetTestSetData() const noexcept;

    /// Public Methods \\\

    bool AddUnitTest(_Inout_ UnitTest&&);
    bool AddUnitTest(_Inout_ std::function<UnitTestResult(void)>&&);

    bool AddUnitTests(_Inout_ std::list<UnitTest>&&);
    bool AddUnitTests(_Inout_ std::list<std::function<UnitTestResult(void)>>&&);

    void ClearUnitTests() noexcept;

    bool RunUnitTests();
};
