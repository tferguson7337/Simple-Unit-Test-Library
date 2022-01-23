#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <array>
#include <cstdint>
#include <string_view>


namespace SUTL
{
    // Unit Test ResultType Enum Flags //

    enum class ResultType : uint32_t
    {
        // Test not run
        NotRun = 0,

        // Test pass
        Success,

        // Test failures - setup
        SetupFailure,
        SetupException,

        // Test failures - actual
        TestFailure,
        TestException,

        // Test failures - cleanup
        CleanupFailure,
        CleanupException,

        // Test failures - unhandled exception.
        UnhandledException,

        // ! Internal use only !
        _Invalid,
        _End,
        _Last = _End - 2, // Last valid value
        _Begin = NotRun
    };


    //
    //
    //  Fully-Static ResultType Helper Class 
    //
    //
    class ResultTypeUtil
    {
    private:
        // Private Data Members //

        static constexpr size_t s_ResultTypeStringArrayLength = static_cast<size_t>(ResultType::_End);

        static constexpr std::array<std::string_view, s_ResultTypeStringArrayLength> s_ResultTypeStrings
        {
            "Not Run",
            "Success",
            "Setup Failure",
            "Setup Exception",
            "Run Failure",
            "Run Exception",
            "Cleanup Failure",
            "Cleanup Exception",
            "Unhandled Exception",
            "InvalidResultType"
        };

    public:
        // Note: purely static class, intended to hide internal static data.
        ResultTypeUtil() = delete;
        ~ResultTypeUtil() = delete;
        ResultTypeUtil(const ResultTypeUtil&) = delete;
        ResultTypeUtil(ResultTypeUtil&&) = delete;
        ResultTypeUtil& operator=(const ResultTypeUtil&) = delete;
        ResultTypeUtil& operator=(ResultTypeUtil&&) = delete;


        template <ResultType RT>
        static consteval bool IsValidResultType() noexcept
        {
            return (RT >= ResultType::_Begin) && (RT <= ResultType::_Last);
        }


        static constexpr bool IsValidResultType(_In_ const ResultType r) noexcept
        {
            return (r >= ResultType::_Begin) && (r <= ResultType::_Last);
        }


        static constexpr std::string_view ToString(_In_range_(ResultType::_Begin, ResultType::_Last) ResultType r) noexcept
        {
            if (!IsValidResultType(r))
            {
                r = ResultType::_Invalid;
            }

            return s_ResultTypeStrings[static_cast<size_t>(r)];
        }
    };
}
