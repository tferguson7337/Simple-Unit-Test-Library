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

        template <typename... ArgTs>
            requires(std::formattable<ArgTs, char> && ...)
        constexpr std::string Format(
            _In_ const std::string_view fmt,
            ArgTs&&... args) const
        {
            if consteval
            {
                return {};
            }
            else
            {
                const auto funcName{Utils::ParseFunctionName(m_SrcLoc.function_name())};
                const auto fileName{Utils::ParseFileName(m_SrcLoc.file_name())};
                const auto line{m_SrcLoc.line()};

                const auto formatArgs{std::make_format_args(std::forward<ArgTs>(args)...)};

                return std::vformat(
                           s_SourceLocationFormatSV,
                           std::make_format_args(funcName, fileName, line)) +
                    std::vformat(fmt, formatArgs) +
                    "\n";
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
            requires(std::formattable<ArgTs, char> && ...)
        constexpr void operator()(
            _In_ const std::string_view fmt,
            ArgTs&&... args) const
        {
            if not consteval
            {
                const std::string logMsg{Format(fmt, args...)};
                std::vprint_unicode(stdout, logMsg, std::make_format_args());
            }
        }
    };
}// namespace SimpleUnitTestLibrary

namespace SUTL = SimpleUnitTestLibrary;
