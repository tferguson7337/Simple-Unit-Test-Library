#include <ResultType.h>

/// Static Data Member Initialization \\\

const std::vector<SupportedStringTuple> ResultTypeUtil::m_svResultTypeStrings
{
    MAKE_STR_TUPLE("Not Run"),
    MAKE_STR_TUPLE("Success"),
    MAKE_STR_TUPLE("Setup Failure"),
    MAKE_STR_TUPLE("Setup Exception"),
    MAKE_STR_TUPLE("Run Failure"),
    MAKE_STR_TUPLE("Run Exception"),
    MAKE_STR_TUPLE("Cleanup Failure"),
    MAKE_STR_TUPLE("Cleanup Exception"),
    MAKE_STR_TUPLE("Unhandled Exception")
};

/// Static Private Methods \\\

void ResultTypeUtil::ValidateResultType(_In_ const ResultType& r, _In_ const std::string& f)
{
    if ( r < ResultType::IterationBegin || r >= ResultType::IterationEnd )
    {
        throw std::invalid_argument(
            f + ": Invalid ResultType argument provided[" +
            std::to_string(static_cast<ResultTypeUnderlyingType>(r)) +
            "]."
        );
    }
}

/// Static Public Methods \\\

template <typename CharType, typename Enabled>
const std::basic_string<CharType>& ResultTypeUtil::ToString(_In_ const ResultType& r)
{
    ValidateResultType(r, __FUNCTION__);
    return std::get<std::basic_string<CharType>>(m_svResultTypeStrings[static_cast<size_t>(r)]);
}