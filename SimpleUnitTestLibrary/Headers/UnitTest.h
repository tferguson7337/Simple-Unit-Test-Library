#pragma once

#include <Uncopyable.h>

#include <UnitTestResult.h>

#include <functional>
#include <memory>

///
//
//  Class:      UnitTest
//  
//  Purpose:    Encapsulate unit test information.
//
///
class UnitTest : public Uncopyable
{
private:
    /// Private Data Members \\\

    std::function<UnitTestResult(void)> mTestFunc;
    UnitTestResult mTestResult;

public:
    /// Ctors \\\

    UnitTest( ) = default;
    explicit UnitTest(std::function<UnitTestResult(void)>&&) noexcept;
    UnitTest(UnitTest&&) noexcept;

    /// Dtor \\\

    ~UnitTest( ) = default;

    /// Operator Overloads \\\

    UnitTest& operator=(UnitTest&&) noexcept;

    /// Getters \\\

    const std::function<UnitTestResult(void)>& GetUnitTestFunction( ) const noexcept;
    const UnitTestResult& GetUnitTestResult( ) const noexcept;

    /// Setters \\\

    void SetUnitTestFunction(std::function<UnitTestResult(void)>&&) noexcept;

    /// Public Methods \\\

    const UnitTestResult& RunTest( );
};
