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

    std::function<UnitTestResult(void)> m_TestFunc;
    UnitTestResult m_TestResult;

public:
    /// Ctors \\\

    UnitTest() = default;
    explicit UnitTest(_Inout_ std::function<UnitTestResult(void)>&&) noexcept;
    UnitTest(_Inout_ UnitTest&&) noexcept;

    /// Dtor \\\

    ~UnitTest() = default;

    /// Operator Overloads \\\

    UnitTest& operator=(_Inout_ UnitTest&&) noexcept;

    /// Getters \\\

    const std::function<UnitTestResult(void)>& GetUnitTestFunction() const noexcept;
    const UnitTestResult& GetUnitTestResult() const noexcept;

    /// Setters \\\

    void SetUnitTestFunction(_Inout_ std::function<UnitTestResult(void)>&&) noexcept;

    /// Public Methods \\\

    void Clear() noexcept;
    const UnitTestResult& RunTest();
};
