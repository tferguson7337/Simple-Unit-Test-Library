#pragma once

#if !defined(SUTL_USE_MODULES)
#include <type_traits>

#include "APIAnnotations.h"
#include "SimpleUnitTestLibrary.Result.h"
#endif

namespace SimpleUnitTestLibrary
{
    using TestFunction = Result(*)();
    static_assert(
        std::is_trivially_constructible_v<TestFunction>&&
        std::is_trivially_copyable_v<TestFunction>);

    class Suite;

    class [[nodiscard]] Test
    {
        friend class Suite;
    private:

        std::string m_TestNameSV;
        TestFunction m_TestFn;

        mutable Result m_Result;

    public:

        constexpr Test(
            _In_ const std::string_view testNameSV,
            _In_ const TestFunction testFn) :
            m_TestNameSV{testNameSV},
            m_TestFn{testFn}
        {
        }

        [[nodiscard]] constexpr std::string_view GetTestName() const noexcept
        {
            return m_TestNameSV;
        }

        [[nodiscard]] constexpr const Result& GetResult() const noexcept
        {
            return m_Result;
        }

        constexpr const Result& operator()() const
        {
            if (m_Result.m_ResultType == ResultType::NotRun)
            {
                m_Result = m_TestFn();
            }

            return m_Result;
        }
    };
}

namespace SUTL = SimpleUnitTestLibrary;
