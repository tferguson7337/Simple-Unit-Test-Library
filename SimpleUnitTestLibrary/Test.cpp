#if defined(SUTL_USE_MODULES)
import SimpleUnitTestLibrary;
import std;

inline constexpr int EXIT_SUCCESS = 0;
inline constexpr int EXIT_FAILURE = 1;

#include "Headers\SimpleUnitTestLibrary.Macros.h"
#else
#include "Headers\SimpleUnitTestLibrary.h"

#include <array>
#include <fstream>
#include <print>
#endif



template <bool bFail>
static constexpr auto MyFreeFunctionTest()
{
    SUTL_LOG("Begin");
    bool bFlag{!bFail};
    SUTL_TEST_ASSERT(bFlag, "Oops, we failed");

    SUTL_TEST_PASS();
}

template <bool bFail>
static constexpr auto MyVeryLonglyNamedAndAwesomeFreeFunctionTestImplementation()
{
    SUTL_LOG("Begin");
    bool bFlag{!bFail};
    SUTL_TEST_ASSERT(bFlag, "Oops, we failed");

    SUTL_TEST_PASS();
}

template <std::uint32_t Id = 0>
static constexpr auto GenerateSuccessfulTestSuite()
{
    auto MyLambdaTest = []() static constexpr
    {
        SUTL_LOG("Begin");
        bool bFlag{true};
        SUTL_TEST_ASSERT(bFlag, "Oops, we failed");

        SUTL_TEST_PASS();
    };

    auto TestSuiteLambdaSetup = []() static constexpr
    {
        SUTL_LOG("Begin");
        bool bFlag{true};
        SUTL_SETUP_ASSERT(bFlag, "Oops, setup is borked");

        SUTL_TEST_PASS();
    };

    auto TestSuiteLambdaCleanup = []() static constexpr
    {
        SUTL_LOG("Begin");
        bool bFlag{true};
        SUTL_CLEANUP_ASSERT(bFlag, "Oops, cleanup is borked");

        SUTL_TEST_PASS();
    };

    constexpr std::string_view cNamePrefix{"SuccessfulTestSuite_"};
    std::array<char, cNamePrefix.size() + std::numeric_limits<std::uint32_t>::digits10> buffer{};
    auto GenerateTestSuiteName = [&buffer, cNamePrefix]() constexpr -> std::string_view
    {
        std::ranges::copy(cNamePrefix, buffer.begin());
        const auto [pEnd, errc] {std::to_chars(buffer.data() + cNamePrefix.size(), buffer.data() + buffer.size(), Id)};
        return (errc != std::errc{})
            ? "SuccessfulTestSuite"
            : std::string_view{buffer.data(), static_cast<std::size_t>(pEnd - buffer.data())};
    };

    return SUTL::Suite
    {
        GenerateTestSuiteName(),
        std::array
        {
            SUTL_CREATE_UNIT_TEST(MyFreeFunctionTest<false>),
            SUTL_CREATE_UNIT_TEST(MyVeryLonglyNamedAndAwesomeFreeFunctionTestImplementation<false>),

            SUTL_CREATE_UNIT_TEST(MyLambdaTest),
            SUTL::Test{
                "MyInlineLambdaTest", 
                []() static constexpr
                {
                    bool bFlag{true};
                    SUTL_TEST_ASSERT(bFlag && "Oops, we failed");
                    SUTL_TEST_PASS();
                }}
        },
        TestSuiteLambdaSetup,
        TestSuiteLambdaCleanup
    };
}

[[nodiscard]] constexpr auto GenerateSuccessfulTestSuites()
{
    return std::array
    {
        GenerateSuccessfulTestSuite<0>(),
        GenerateSuccessfulTestSuite<1>(),
        GenerateSuccessfulTestSuite<2>(),
        GenerateSuccessfulTestSuite<3>()
    };
}

#if !defined(SUTL_USE_MODULES)
static_assert(
    std::ranges::all_of(
        GenerateSuccessfulTestSuites(),
        [](_In_ const SUTL::Suite& suite) static constexpr { return !!suite(); }));
#endif

std::fstream CreateLogFile(_In_ const std::string_view testName)
{
    return std::fstream{
        std::string{".\\"} + std::string{testName} + ".log",
        std::ios_base::out/* | std::ios_base::app */};
}

template <std::uint32_t Id = 0>
constexpr SUTL::Result FailedTestSuiteSetup()
{
    SUTL_LOG("Begin");

    bool bFlag{true};
    SUTL_SETUP_ASSERT(bFlag, "Oops, setup is borked");

    SUTL_TEST_PASS();
}

template <std::uint32_t Id = 0>
constexpr SUTL::Result FailedTestSuiteCleanup()
{
    SUTL_LOG("End");

    bool bFlag{false};
    SUTL_CLEANUP_ASSERT(bFlag, "Oops, cleanup is borked");

    SUTL_TEST_PASS();
}

template <std::uint32_t Id = 0>
static constexpr auto GenerateFailedTestSuite()
{
    auto MyLambdaTest = []() static constexpr
    {
        SUTL_LOG("Begin");
        bool bFlag{false};
        SUTL_TEST_ASSERT(bFlag, "Oops, we failed");

        SUTL_TEST_PASS();
    };

    constexpr std::string_view cNamePrefix{"FailureTestSuite_"};
    std::array<char, cNamePrefix.size() + std::numeric_limits<std::uint32_t>::digits10> buffer{};
    auto GenerateTestSuiteName = [&buffer, cNamePrefix]() constexpr -> std::string_view
    {
        std::ranges::copy(cNamePrefix, buffer.begin());
        const auto [pEnd, errc] {std::to_chars(buffer.data() + cNamePrefix.size(), buffer.data() + buffer.size(), Id)};
        return (errc != std::errc{})
            ? "FailureTestSuite"
            : std::string_view{buffer.data(), static_cast<std::size_t>(pEnd - buffer.data())};
    };

    return SUTL::Suite
    {
        GenerateTestSuiteName(),
        std::array
        {
            SUTL_CREATE_UNIT_TEST(MyFreeFunctionTest<true>),
            SUTL_CREATE_UNIT_TEST(MyVeryLonglyNamedAndAwesomeFreeFunctionTestImplementation<true>),

            SUTL_CREATE_UNIT_TEST(MyLambdaTest),
            SUTL::Test{
                "MyInlineLambdaTest",
                []() static constexpr
                {
                    bool bFlag{false};
                    SUTL_TEST_ASSERT(bFlag && "Oops, we failed");
                    SUTL_TEST_PASS();
                }}
        },
        FailedTestSuiteSetup<Id>, // No setup, since its failure would prevent tests from running.
        FailedTestSuiteCleanup<Id>
    };
}

[[nodiscard]] constexpr auto GenerateFailedTestSuites()
{
    return std::array
    {
        GenerateFailedTestSuite<0>(),
        GenerateFailedTestSuite<1>(),
        GenerateFailedTestSuite<2>(),
        GenerateFailedTestSuite<3>()
    };
}

#if !defined(SUTL_USE_MODULES)
static_assert(
    std::ranges::none_of(
        GenerateFailedTestSuites(),
        [](const SUTL::Suite& suite) static constexpr { return !!suite(); }));
#endif



int main(
    const int argc,
    const char* argv[])
{
    const std::string_view suiteFilterSV{argc <= 1 ? std::string_view{} : argv[1]};
    {
        std::array runtimeSuccessfulTestSuites{GenerateSuccessfulTestSuites()};
        const auto runResults{SUTL::Runner{suiteFilterSV}()};
        if (runResults.empty())
        {
            return EXIT_FAILURE;
        }

        for (const auto& suiteResult : runResults)
        {
            if (!suiteResult)
            {
                return EXIT_FAILURE;
            }

            std::println("{}", suiteResult);
        }
    }
    {
        const auto runResults{SUTL::Runner{suiteFilterSV}()};
        if (!runResults.empty())
        {
            return EXIT_FAILURE;
        }
    }
    {
        std::array runtimeFailedTestSuites{GenerateFailedTestSuites()};
        const auto runResults{SUTL::Runner{suiteFilterSV}()};
        if (runResults.empty())
        {
            return EXIT_FAILURE;
        }

        for (const auto& suiteResult : SUTL::Runner{suiteFilterSV}())
        {
            if (!!suiteResult)
            {
                return EXIT_FAILURE;
            }

            std::println("{}", suiteResult);
        }
    }
    {
        const auto runResults{SUTL::Runner{suiteFilterSV}()};
        if (!runResults.empty())
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
