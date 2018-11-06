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
    MAX,
    BEGIN = NotRun
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

    static bool ValidateResultType(const ResultType& r, const std::string& f)
    {
        if ( r < ResultType::BEGIN || r > ResultType::MAX )
        {
            throw std::invalid_argument(
                f + ": Invalid ResultType argument provided[" +
                std::to_string(static_cast<ResultTypeUnderlyingType>(r)) +
                "]."
            );
        }

        return true;
    }

public:

    template <typename CharType, typename Enabled = std::enable_if_t<IsSupportedCharType<CharType>( )>>
    static const std::basic_string<CharType>& ToString(const ResultType& r)
    {
        static const std::vector<SupportedStringTuple> vResultTypeStrings
        {
            MAKE_STR_TUPLE("Not Run"),
            MAKE_STR_TUPLE("Success"),
            MAKE_STR_TUPLE("Setup Failure"),
            MAKE_STR_TUPLE("Setup Exception"),
            MAKE_STR_TUPLE("Run Failure"),
            MAKE_STR_TUPLE("Run Exception"),
            MAKE_STR_TUPLE("Cleanup Failure"),
            MAKE_STR_TUPLE("Cleanup Exception"),
            MAKE_STR_TUPLE("Unhandled Exception"),
        };

        ValidateResultType(r, __FUNCTION__);
        
        return std::get<std::basic_string<CharType>>(vResultTypeStrings[static_cast<size_t>(r)]);
    }
};