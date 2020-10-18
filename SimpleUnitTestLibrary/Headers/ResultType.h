#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <array>
#include <utility>


/// Unit Test ResultType Enum Flags \\\

enum class ResultType : size_t
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

    // Enum Boundaries
    End,
    Begin = NotRun
};


///
//
//  Fully-Static ResultType Helper Class 
//
///
class ResultTypeUtil
{
    ResultTypeUtil() = delete;
    ResultTypeUtil(const ResultTypeUtil&) = delete;
    ResultTypeUtil(ResultTypeUtil&&) = delete;
    ResultTypeUtil& operator=(const ResultTypeUtil&) = delete;
    ResultTypeUtil& operator=(ResultTypeUtil&&) = delete;

private:
    /// Private Data Members \\\


#define STR_LEN_PAIR(str) std::pair<const char*, const size_t>(str, sizeof(str) - 1)

    static constexpr std::array<std::pair<const char*, size_t>, static_cast<size_t>(ResultType::End)> ms_ResultTypeStrings
    {
        STR_LEN_PAIR("Not Run"),
        STR_LEN_PAIR("Success"),
        STR_LEN_PAIR("Setup Failure"),
        STR_LEN_PAIR("Setup Exception"),
        STR_LEN_PAIR("Run Failure"),
        STR_LEN_PAIR("Run Exception"),
        STR_LEN_PAIR("Cleanup Failure"),
        STR_LEN_PAIR("Cleanup Exception"),
        STR_LEN_PAIR("Unhandled Exception")
    };

    static constexpr std::pair<const char*, size_t> ms_UnsupportedResultTypeString = STR_LEN_PAIR("<UNKNOWN_RESULT_TYPE>");

#undef STR_LEN_PAIR

    /// Private Helper Methods \\\

    constexpr static bool ValidateResultType(_In_ const ResultType r)
    {
        return (r >= ResultType::Begin) && (r < ResultType::End);
    }

public:
    /// Public Methods \\\

    constexpr static const std::pair<const char*, size_t>& ToString(_In_range_(ResultType::Begin, ResultType::End - 1) const ResultType r)
    {
        return ValidateResultType(r) ?
            ms_ResultTypeStrings[static_cast<size_t>(r)] :
            ms_UnsupportedResultTypeString;
    }
};
