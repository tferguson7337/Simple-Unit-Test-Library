#pragma once

#include <vector>

using NarrowString = std::basic_string<char>;
using WideString = std::basic_string<wchar_t>;

using StrTuple = std::tuple<NarrowString, WideString>;

#ifndef _MAKE_STRING_TUPLE_
#define _MAKE_STRING_TUPLE_(str) StrTuple(str, L##str)
#endif

/// Unit Test ResultType Enum Flags \\\

using ResultTypeUnderlyingType = size_t;

enum class ResultType : ResultTypeUnderlyingType
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
    IterationEnd,
    IterationBegin = NotRun
};


///
//
//  Fully-Static ResultType Helper Class 
//
///
class ResultTypeUtil
{
    ResultTypeUtil( ) = delete;
    ResultTypeUtil(const ResultTypeUtil&) = delete;
    ResultTypeUtil(ResultTypeUtil&&) = delete;
    ResultTypeUtil& operator=(const ResultTypeUtil&) = delete;
    ResultTypeUtil& operator=(ResultTypeUtil&&) = delete;

private:
    /// Private Data Members \\\

    static const std::vector<StrTuple> m_svResultTypeStrings;

    /// Private Helper Methods \\\

    static void ValidateResultType(_In_ const ResultType&, _In_ const std::string&);

public:
    /// Public Methods \\\
    
    template <typename T>
    static const std::basic_string<T>& ToString(_In_ const ResultType& r);
};
