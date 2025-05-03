#pragma once

#if !defined(SUTL_USE_MODULES)
#include <concepts>
#include <functional>
#include <format>
#include <print>
#include <source_location>
#include <string>
#include <string_view>
#include <type_traits>

#include "APIAnnotations.h"
#include "SimpleUnitTestLibrary.Utils.h"
#endif

namespace SimpleUnitTestLibrary
{
    struct [[nodiscard]] Logger
    {
    private:

        static constexpr std::string_view s_SourceLocationFormatSV{"{} ({} @ {}): "};

        /*
            The following is a workaround for a Visual Studio 2022 bug, where operator() function cannot call std::println.
            This should normally be allowed, since we're explicitly calling it outside of constexpr context.

            Error:
            constexpr function 'SimpleUnitTestLibrary::UnitTestLogger::operator ()' cannot result in a constant expression
            failure was caused by call of undefined function or one not declared 'constexpr'
            see usage of 'std::println'

            Related dev-comm post: https://developercommunity.visualstudio.com/t/error-C3615-constexpr-function-f-cann/10857535

            We can work around it by adding an extra layer of indirection (operator() --> LogImpl --> std::println).
        */
        template <typename... ArgTs>
            requires (std::formattable<ArgTs, char> && ...)
        constexpr std::string Format(
            _In_ const std::format_string<ArgTs...> fmt,
            ArgTs&&... args) const
        {
            if consteval
            {
                return {};
            }
            else
            {
                const std::size_t requiredLen{
                    std::formatted_size(
                        s_SourceLocationFormatSV,
                        Utils::ParseFunctionName(m_SrcLoc.function_name()),
                        Utils::ParseFileName(m_SrcLoc.file_name()),
                        m_SrcLoc.line()) +
                    std::formatted_size(
                        fmt,
                        std::forward<ArgTs>(args)...)};
                std::string str(requiredLen, '\0');
                std::format_to(
                    std::format_to(
                        str.begin(),
                        s_SourceLocationFormatSV,
                        Utils::ParseFunctionName(m_SrcLoc.function_name()),
                        Utils::ParseFileName(m_SrcLoc.file_name()),
                        m_SrcLoc.line()),
                    fmt,
                    std::forward<ArgTs>(args)...);
                return str;
            }
        }

        template <typename... ArgTs>
            requires (std::formattable<ArgTs, char> && ...)
        constexpr void LogImpl(
            _In_ const std::format_string<ArgTs...> fmt,
            ArgTs&&... args) const
        {
            if not consteval
            {
                std::println("{}", Format(fmt, std::forward<ArgTs>(args)...));
            }
        }

    public:

        std::source_location m_SrcLoc;

        consteval Logger(
            _In_ const std::source_location srcLoc = std::source_location::current()) noexcept :
            m_SrcLoc{srcLoc}
        {
        }

        template <typename... ArgTs>
            requires (std::formattable<ArgTs, char> && ...)
        constexpr void operator()(
            _In_ const std::format_string<ArgTs...> fmt,
            ArgTs&&... args) const
        {
            if not consteval
            {
                return LogImpl(fmt, std::forward<ArgTs>(args)...);
            }
        }
    };
}

namespace SUTL = SimpleUnitTestLibrary;
