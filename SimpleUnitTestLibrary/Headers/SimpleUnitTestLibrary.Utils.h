#pragma once

#if !defined(SUTL_USE_MODULES)
#include <array>
#include <string_view>

#include "APIAnnotations.h"
#endif


namespace SimpleUnitTestLibrary
{
    namespace Internal_
    {
        using namespace std::literals::string_view_literals;

        inline constexpr std::array g_cFunctionNameCallingConventionSVs
        {
            "__cdecl"sv,
            "__stdcall"sv,
            "__fastcall"sv,
            "__thiscall"sv,
            "__vectorcall"sv,
            "__clrcall"sv
        };
    }

    namespace Utils
    {
        [[nodiscard]] inline constexpr std::string_view ParseFileName(
            _In_ const std::string_view file) noexcept
        {
            const auto separatorPos{file.find_last_of("\\/")};
            return (separatorPos == std::string_view::npos)
                ? file
                : file.substr(separatorPos + 1);
        }

        [[nodiscard]] inline constexpr std::string_view ParseFunctionName(
            _In_ std::string_view function) noexcept
        {
            if (const auto parameterStartPos = function.rfind('('); parameterStartPos != std::string_view::npos)
            {
                function.remove_suffix(function.size() - parameterStartPos);
            }

            for (const auto callingConventionSV : Internal_::g_cFunctionNameCallingConventionSVs)
            {
                const auto callingConventionStartPos{function.find(callingConventionSV)};
                if (callingConventionStartPos == std::string_view::npos)
                {
                    continue;
                }

                function = function.substr(callingConventionStartPos + callingConventionSV.size() + 1);
            }

            return function;
        }

        // Mock examples.
        using namespace std::string_view_literals;
        static_assert(ParseFunctionName(""sv) == ""sv);
        static_assert(ParseFunctionName("(paramtype)"sv) == ""sv);
        static_assert(ParseFunctionName("func(paramtype)"sv) == "func"sv);

        static_assert(ParseFunctionName("rettype __cdecl func(paramtype)"sv) == "func"sv);
        static_assert(ParseFunctionName("struct rettype __stdcall func(paramtype)"sv) == "func"sv);
        static_assert(ParseFunctionName("class rettype __fastcall func(paramtype)"sv) == "func"sv);
        static_assert(ParseFunctionName("rettype __thiscall func(paramtype)"sv) == "func"sv);
        static_assert(ParseFunctionName("struct rettype __vectorcall func(paramtype)"sv) == "func"sv);
        static_assert(ParseFunctionName("class rettype __clrcall func(paramtype)"sv) == "func"sv);

        // Real world examples
        static_assert(ParseFunctionName("struct SimpleUnitTestLibrary::UnitTestResult __cdecl Test(void)"sv) == "Test"sv);
        static_assert(ParseFunctionName("struct SimpleUnitTestLibrary::UnitTestResult __cdecl RunTests::<lambda_1>::operator ()(void)"sv) == "RunTests::<lambda_1>::operator ()"sv);
    }
}

namespace SUTL = SimpleUnitTestLibrary;
