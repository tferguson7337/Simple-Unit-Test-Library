#pragma once

#include "UnitTestResult.h"

#include <functional>
#include <memory>

///
//
//  Class:      UnitTest
//  
//  Purpose:    Encapsulate unit test information.
//
///
class UnitTest
{
private:
    /// Private Data Members \\\

    std::function<UnitTestResult(void)> mTestFunc;
    UnitTestResult mTestResult;

public:
    /// Ctors \\\

    UnitTest() = default;
    explicit UnitTest(_In_ std::function<UnitTestResult(void)>&&) noexcept;
    UnitTest(_In_ UnitTest&&) noexcept;

    /// Dtor \\\

    ~UnitTest() = default;

    /// Operator Overloads \\\

    UnitTest& operator=(_In_ UnitTest&&) noexcept;

    /// Getters \\\

    const std::function<UnitTestResult(void)>& GetUnitTestFunction() const noexcept;
    const UnitTestResult& GetUnitTestResult() const noexcept;

    /// Setters \\\

    void SetUnitTestFunction(_In_ std::function<UnitTestResult(void)>&&) noexcept;

    /// Public Methods \\\

    const UnitTestResult& RunTest();
};
