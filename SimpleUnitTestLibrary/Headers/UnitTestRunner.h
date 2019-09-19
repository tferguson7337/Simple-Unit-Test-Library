#pragma once

#include "UnitTestLogger.h"
#include "Interfaces/IUnitTestRunner.h"

#include "TestSetData.h"

template <class T>
class UnitTestRunner final : public virtual IUnitTestRunner<T>
{
private:
    std::list<UnitTest> mUnitTests;
    mutable UnitTestLogger<T> mLogger;
    TestSetData<T> mTestSetData;

public:
    /// Ctors \\\

    explicit UnitTestRunner(_In_ const std::basic_string<T> & = "No Test Set Name");
    explicit UnitTestRunner(_In_ std::basic_string<T>&&) noexcept;

    /// Dtor \\\

    ~UnitTestRunner() = default;

    /// Operator Overloads \\\

    UnitTestRunner& operator=(_In_ UnitTestRunner&&) noexcept;

    /// Getters \\\

    const std::list<UnitTest>& GetUnitTests() const noexcept;
    IUnitTestLogger<T>& GetLogger() const noexcept;
    TestSetData<T>& GetTestSetData() noexcept;
    const TestSetData<T>& GetTestSetData() const noexcept;

    /// Public Methods \\\

    bool AddUnitTest(_In_ UnitTest&&);
    bool AddUnitTest(_In_ std::function<UnitTestResult(void)>&&);

    bool AddUnitTests(_In_ std::list<UnitTest>&&);
    bool AddUnitTests(_In_ std::list<std::function<UnitTestResult(void)>>&&);

    void ClearUnitTests() noexcept;

    bool RunUnitTests();
};
