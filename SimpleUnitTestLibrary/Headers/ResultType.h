#pragma once

#include "../Common/Headers/Macros.h"

#include <vector>

/// Unit Test ResultType Enum Flags \\\

using ResultTypeUnderlyingType = uint8;

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

    static const std::vector<SupportedStringTuple> m_svResultTypeStrings;

    /// Private Helper Methods \\\

    static void ValidateResultType(_In_ const ResultType&, _In_ const std::string&);

public:
    /// Public Methods \\\
    
    template <typename CharType, typename Enabled = typename std::enable_if_t<IsSupportedCharType<CharType>( )>>
    static const std::basic_string<CharType>& ToString(_In_ const ResultType& r);
};
