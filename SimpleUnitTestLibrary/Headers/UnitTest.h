#pragma once

// SAL
#include "APIAnnotations.h"

// SUTL
#include "TestTypes.h"
#include "UnitTestResult.h"

// STL
#include <stdexcept>


//
//
//  Class:      UnitTest
//  
//  Purpose:    Encapsulate unit test information.
//
//
class UnitTest
{
private:
    // Private Data Members //

    UnitTestFunction m_pfTestFunc;
    UnitTestResult m_TestResult;

public:
    // Ctors //

    constexpr UnitTest() noexcept :
        m_pfTestFunc(nullptr)
    { }

    constexpr explicit UnitTest(_In_opt_ const UnitTestFunction pfFunc) noexcept :
        m_pfTestFunc(pfFunc)
    { }

    constexpr UnitTest(_Inout_ UnitTest&& src) noexcept :
        UnitTest()
    {
        *this = std::move(src);
    }

    // Dtor //

    ~UnitTest() noexcept = default;

    // Operator Overloads //

    constexpr UnitTest& operator=(_Inout_ UnitTest&& src) noexcept
    {
        if (this != &src)
        {
            m_pfTestFunc = src.m_pfTestFunc;
            m_TestResult = std::move(src.m_TestResult);
        }

        return *this;
    }

    // Getters //

    constexpr UnitTestFunction GetUnitTestFunction() const noexcept
    {
        return m_pfTestFunc;
    }

    constexpr const UnitTestResult& GetUnitTestResult() const noexcept
    {
        return m_TestResult;
    }

    // Setters //

    constexpr void SetUnitTestFunction(_In_opt_ const UnitTestFunction pfFunc) noexcept
    {
        m_pfTestFunc = pfFunc;
    }

    // Public Methods //

    constexpr void Clear() noexcept
    {
        m_pfTestFunc = nullptr;
        m_TestResult.Clear();
    }

    const UnitTestResult& RunTest()
    {
        if (!m_pfTestFunc)
        {
            throw std::logic_error("No function is associated with this UnitTest.");
        }

        m_TestResult = m_pfTestFunc();
        return m_TestResult;
    }
};
