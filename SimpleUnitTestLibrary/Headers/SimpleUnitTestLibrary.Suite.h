#pragma once

#if !defined(SUTL_USE_MODULES)
#include <algorithm>
#include <concepts>
#include <cstdint>
#include <format>
#include <functional>
#include <iterator>
#include <numeric>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <vector>

#include "APIAnnotations.h"
#include "SimpleUnitTestLibrary.Test.h"
#endif

namespace SimpleUnitTestLibrary
{
    namespace Internal_
    {
        using namespace std::string_view_literals;

        inline constexpr auto g_cResultCountWithoutFailureDetailsFormatSV
        {
R"(
{} Result Counts:
  Total Tests:  {}

    Successful: {}
    Skipped:    {}
    Not Run:    {}
    Failed:     {}
)"sv
        };

        inline constexpr auto g_cResultCountWithFailureDetailsFormatSV
        {
R"(
{} Result Counts:
  Total Tests:  {}

    Successful: {}
    Skipped:    {}
    Not Run:    {}

    Failed:     {}
      Setup:      {}
      Test:       {}
      Cleanup:    {}
      Exceptions: {}
)"sv
        };
    }
}

namespace SimpleUnitTestLibrary
{
    namespace Concepts
    {
        template <typename RangeT>
        concept ValidUnitTestRangeSource = requires
        {
            std::ranges::range<RangeT>;
            std::constructible_from<TestFunction, std::remove_cvref_t<std::ranges::range_value_t<RangeT>>>;
        };
    }

    class Suite;

    namespace Internal_
    {
        constinit std::vector<const Suite*> g_RuntimeSuiteRegistry;
    }

    class Suite
    {
    private:

        std::string m_SuiteName;
        std::vector<Test> m_UnitTests;

        TestFunction m_SuiteSetupFn;
        TestFunction m_SuiteCleanupFn;

    public:

        static_assert(std::is_pointer_v<TestFunction>, "SAL needs updating");
        template <Concepts::ValidUnitTestRangeSource UnitTestRangeT>
        constexpr Suite(
            _In_ const std::string_view suiteNameSV,
            _Inout_ UnitTestRangeT&& unitTests,
            _In_opt_ const TestFunction suiteSetupFn = TestFunction{},
            _In_opt_ const TestFunction suiteCleanupFn = TestFunction{}) :
            m_SuiteName{suiteNameSV},
            m_SuiteSetupFn{suiteSetupFn},
            m_SuiteCleanupFn{suiteCleanupFn}
        {
            m_UnitTests.reserve(!!suiteSetupFn + std::ranges::size(unitTests) + !!suiteCleanupFn);
            if (!!suiteSetupFn)
            {
                m_UnitTests.emplace_back(m_SuiteName + "(Setup)", suiteSetupFn);
            }
            m_UnitTests.append_range(std::forward<UnitTestRangeT>(unitTests));
            if (!!suiteCleanupFn)
            {
                m_UnitTests.emplace_back(m_SuiteName + "(Cleanup)", suiteCleanupFn);
            }

            if not consteval
            {
                Internal_::g_RuntimeSuiteRegistry.push_back(this);
            }
        }

        constexpr ~Suite() noexcept
        {
            if not consteval
            {
                const auto itr{std::ranges::find(Internal_::g_RuntimeSuiteRegistry | std::views::reverse, this)};
                if (itr != Internal_::g_RuntimeSuiteRegistry.rend())
                {
                    Internal_::g_RuntimeSuiteRegistry.erase(std::next(itr).base());
                }
            }
        }

        [[nodiscard]] constexpr std::string_view GetSuiteName() const noexcept
        {
            return m_SuiteName;
        }

        struct RunResults
        {
            std::string m_OriginSuiteNameSV;
            std::vector<Test> m_UnitTests;

            template <Concepts::ValidUnitTestRangeSource RangeT>
            constexpr RunResults(
                _In_ const std::string_view originSuiteNameSV,
                _Inout_ RangeT&& tests) :
                m_OriginSuiteNameSV{originSuiteNameSV},
                m_UnitTests(std::forward<RangeT>(tests))
            { }

            [[nodiscard]] constexpr explicit operator bool() const noexcept
            {
                return std::ranges::all_of(
                    m_UnitTests,
                    [](_In_ const Test& unitTest) static constexpr { return !!unitTest.GetResult(); });
            }

            [[nodiscard]] constexpr auto begin() noexcept { return m_UnitTests.begin(); }
            [[nodiscard]] constexpr auto begin() const noexcept { return m_UnitTests.begin(); }
            [[nodiscard]] constexpr auto cbegin() const noexcept { return m_UnitTests.cbegin(); }

            [[nodiscard]] constexpr auto end() noexcept { return m_UnitTests.end(); }
            [[nodiscard]] constexpr auto end() const noexcept { return m_UnitTests.end(); }
            [[nodiscard]] constexpr auto cend() const noexcept { return m_UnitTests.cend(); }

            [[nodiscard]] constexpr auto rbegin() noexcept { return m_UnitTests.rbegin(); }
            [[nodiscard]] constexpr auto rbegin() const noexcept { return m_UnitTests.rbegin(); }
            [[nodiscard]] constexpr auto crbegin() const noexcept { return m_UnitTests.crbegin(); }

            [[nodiscard]] constexpr auto rend() noexcept { return m_UnitTests.rend(); }
            [[nodiscard]] constexpr auto rend() const noexcept { return m_UnitTests.rend(); }
            [[nodiscard]] constexpr auto crend() const noexcept { return m_UnitTests.crend(); }

            [[nodiscard]] std::string ToString(_In_ const std::size_t spaces = 0) const
            {
                std::string ret;
                ret += std::format("\n{:<{}}{} Run Results:\n", ""sv, spaces, m_OriginSuiteNameSV);

                const std::size_t maxTestNameLength{
                    std::ranges::max_element(
                        m_UnitTests,
                        [](_In_ const Test& lhs, _In_ const Test& rhs) static constexpr
                        {
                            return lhs.GetTestName().length() < rhs.GetTestName().length();
                        })->GetTestName().length()};

                std::array<std::uint32_t, static_cast<std::size_t>(ResultType::_End)> resultTypeCounts{0};
                for (const Test& test : m_UnitTests)
                {
                    const Result& result{test.GetResult()};

                    ++resultTypeCounts[static_cast<std::size_t>(result.m_ResultType)];
                    if (result.m_ResultType != ResultType::Success)
                    {
                        ret += '\n';
                    }

                    ret += std::format("{:<{}}{:<{}}{}\n",
                        "", spaces + 2, test.GetTestName(), std::min(std::size_t{72}, maxTestNameLength + 1),
                        result.ToString(spaces + 2));
                }

                auto GetTotalFailureCount = [&resultTypeCounts]() constexpr -> std::uint64_t
                {
                    return resultTypeCounts[static_cast<std::size_t>(ResultType::SetupFailure)]
                        + resultTypeCounts[static_cast<std::size_t>(ResultType::TestFailure)]
                        + resultTypeCounts[static_cast<std::size_t>(ResultType::CleanupFailure)]
                        + resultTypeCounts[static_cast<std::size_t>(ResultType::UnhandledException)];
                };

                auto GetResultTypeCount = [&resultTypeCounts](_In_ const ResultType resultType) constexpr -> std::uint64_t
                {
                    return (resultType < ResultType::_End)
                        ? resultTypeCounts[static_cast<std::size_t>(resultType)]
                        : std::accumulate(resultTypeCounts.cbegin(), resultTypeCounts.cend(), 0ull, std::plus<>{});
                };

                const auto totalFailureCount{GetTotalFailureCount()};
                if (totalFailureCount == 0)
                {
                    ret += std::format(
                        Internal_::g_cResultCountWithoutFailureDetailsFormatSV,
                        m_OriginSuiteNameSV,
                        GetResultTypeCount(ResultType::_End),
                        GetResultTypeCount(ResultType::Success),
                        GetResultTypeCount(ResultType::Skipped),
                        GetResultTypeCount(ResultType::NotRun),
                        totalFailureCount);
                }
                else
                {
                    ret += std::format(
                        Internal_::g_cResultCountWithFailureDetailsFormatSV,
                        m_OriginSuiteNameSV,
                        GetResultTypeCount(ResultType::_End),
                        GetResultTypeCount(ResultType::Success),
                        GetResultTypeCount(ResultType::Skipped),
                        GetResultTypeCount(ResultType::NotRun),
                        totalFailureCount,
                        GetResultTypeCount(ResultType::SetupFailure),
                        GetResultTypeCount(ResultType::TestFailure),
                        GetResultTypeCount(ResultType::CleanupFailure),
                        GetResultTypeCount(ResultType::UnhandledException));
                }

                return ret;
            }
        };

        [[nodiscard]] constexpr RunResults operator()() const noexcept
        {
            auto InvokeTest = [](_In_ const Test& test) static constexpr
            {
                test();
            };

            if (!m_UnitTests.empty())
            {
                if (!!m_SuiteSetupFn)
                {
                    const auto& suiteSetup{m_UnitTests.front()};
                    if (!!suiteSetup())
                    {
                        // Only run tests if suite setup was successful.
                        const auto subRange{std::ranges::subrange{m_UnitTests.cbegin() + 1, m_UnitTests.cend()}};
                        std::ranges::for_each(subRange, InvokeTest);
                    }
                    if (!!m_SuiteCleanupFn)
                    {
                        // Even if we're skipping the main body of tests due to setup failure
                        // be sure to run suite cleanup so it can handle any needed teardown to avoid leaks, etc.
                        const auto& suiteCleanup{m_UnitTests.back()};
                        suiteCleanup();
                    }
                }
                else
                {
                    std::ranges::for_each(m_UnitTests, InvokeTest);
                }
            }

            return RunResults{m_SuiteName, m_UnitTests};
        }
    };
}

template<> struct std::formatter<SimpleUnitTestLibrary::Suite::RunResults>
{
    constexpr auto parse(_In_ const std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(
        _In_ const SimpleUnitTestLibrary::Suite::RunResults& suiteRunResults,
        _Inout_ std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "{}", suiteRunResults.ToString());
    }
};

namespace SUTL = ::SimpleUnitTestLibrary;
