#pragma once

// STL
#include <array>
#include <string>


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

    static const std::array<std::wstring, static_cast<size_t>(ResultType::End)> ms_ResultTypeStrings;

    /// Private Helper Methods \\\

    static void ValidateResultType(_In_z_ const char* f, _In_ const ResultType& r);

public:
    /// Public Methods \\\
    

    static const std::wstring& ToString(_In_ const ResultType& r);
};
