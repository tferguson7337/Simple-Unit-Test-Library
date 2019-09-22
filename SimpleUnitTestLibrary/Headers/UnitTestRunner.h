#pragma once

#include "UnitTestLogger.h"
#include "Interfaces/IUnitTestRunner.h"

#include "TestSetData.h"

template <class T>
class UnitTestRunner final : public virtual IUnitTestRunner<T>
{
private:
    std::list<UnitTest> m_UnitTests;
    mutable UnitTestLogger<T> m_Logger;
    TestSetData<T> m_TestSetData;

public:
    /// Ctors \\\

    explicit UnitTestRunner(_In_ const std::basic_string<T> & = "No Test Set Name");
    explicit UnitTestRunner(_Inout_ std::basic_string<T>&&) noexcept;

    /// Dtor \\\

    ~UnitTestRunner() = default;

    /// Operator Overloads \\\

    UnitTestRunner& operator=(_Inout_ UnitTestRunner&&) noexcept;

    /// Getters \\\

    const std::list<UnitTest>& GetUnitTests() const noexcept;
    IUnitTestLogger<T>& GetLogger() const noexcept;
    TestSetData<T>& GetTestSetData() noexcept;
    const TestSetData<T>& GetTestSetData() const noexcept;

    /// Public Methods \\\

    bool AddUnitTest(_Inout_ UnitTest&&);
    bool AddUnitTest(_Inout_ std::function<UnitTestResult(void)>&&);

    bool AddUnitTests(_Inout_ std::list<UnitTest>&&);
    bool AddUnitTests(_Inout_ std::list<std::function<UnitTestResult(void)>>&&);

    void ClearUnitTests() noexcept;

    bool RunUnitTests();
};
