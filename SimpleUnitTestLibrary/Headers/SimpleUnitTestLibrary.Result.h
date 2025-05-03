#pragma once

#if !defined(SUTL_USE_MODULES)
#include <array>
#include <cstdint>
#include <format>
#include <source_location>
#include <string>
#include <string_view>

#include "APIAnnotations.h"
#include "SimpleUnitTestLibrary.Logger.h"
#include "SimpleUnitTestLibrary.Utils.h"
#endif


namespace SimpleUnitTestLibrary
{
    namespace Internal_
    {
        using namespace std::literals::string_view_literals;

        inline constexpr std::array g_cResultTypeStringViewArray
        {
            // Test run was never even attempted
            "NotRun"sv,

            // Test pass
            "Success"sv,

            // Test hit a condition that lead to skip
            "Skipped"sv,

            // Test failure cases
            "SetupFailure"sv,
            "TestFailure"sv,
            "CleanupFailure"sv,

            // Test threw exception
            "UnhandledException"sv
        };
    }
}

namespace SimpleUnitTestLibrary
{
    using namespace std::string_view_literals;

    enum class ResultType : std::uint32_t
    {
        // Test not run
        NotRun = 0,

        // Test non-failure
        Success,
        Skipped,

        // Test stage failures
        SetupFailure,
        TestFailure,
        CleanupFailure,

        // Test failures.
        UnhandledException,

        _End,
        _Last = _End - 1,
        _Begin = 0
    };
    static_assert(Internal_::g_cResultTypeStringViewArray.size() == static_cast<std::size_t>(ResultType::_End));

    [[nodiscard]] constexpr bool IsResultTypeValid(
        _In_ const ResultType resultType) noexcept
    {
        return (ResultType::NotRun <= resultType) && (resultType <= ResultType::UnhandledException);
    }

    [[nodiscard]] constexpr std::string_view ResultTypeToString(
        _In_range_(ResultType::NotRun, ResultType::UnhandledException) const ResultType resultType) noexcept
    {
        if (!IsResultTypeValid(resultType))
        {
            return "InvalidResultType"sv;
        }

        return Internal_::g_cResultTypeStringViewArray[static_cast<std::size_t>(resultType)];
    }


    struct [[nodiscard]] Result
    {
        ResultType m_ResultType{ResultType::NotRun};
        std::source_location m_SourceLocation;
        std::string m_Info;

        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return (m_ResultType == ResultType::NotRun)
                || (m_ResultType == ResultType::Success)
                || (m_ResultType == ResultType::Skipped);
        }

        [[nodiscard]] std::string ToString(_In_ const std::size_t spaces = 0) const
        {
            auto GetInfoFormatStringPrefix = [](_In_ const ResultType resultType) static constexpr
            {
                switch (resultType)
                {
                case ResultType::Skipped:
                    return "Reason:"sv;

                case ResultType::UnhandledException:
                    return "Exception:"sv;
                }

                return "Expression:"sv;
            };

            std::string str;
            str = std::format("{:<{}}Result: {}",
                "", spaces, ResultTypeToString(m_ResultType));
            if ((m_ResultType != ResultType::Success) &&
                (m_ResultType != ResultType::NotRun))
            {
                str += std::format(
                    "\n{:<{}}{} {}\n{:<{}}Test: {} ({} @ {})\n",
                    "", spaces + 2, GetInfoFormatStringPrefix(m_ResultType), m_Info,
                    "", spaces + 2,
                    Utils::ParseFunctionName(m_SourceLocation.function_name()),
                    Utils::ParseFileName(m_SourceLocation.file_name()),
                    m_SourceLocation.line());
            }

            return str;
        }
    };
}

namespace SUTL = ::SimpleUnitTestLibrary;
