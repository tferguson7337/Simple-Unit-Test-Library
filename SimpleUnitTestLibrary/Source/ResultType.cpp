#include <ResultType.h>

#include <string>
#include <stdexcept>

/// Static Data Member Initialization \\\

const std::vector<StrTuple> ResultTypeUtil::m_svResultTypeStrings
{
    _SUTL_MAKE_STRING_TUPLE_("Not Run"),
    _SUTL_MAKE_STRING_TUPLE_("Success"),
    _SUTL_MAKE_STRING_TUPLE_("Setup Failure"),
    _SUTL_MAKE_STRING_TUPLE_("Setup Exception"),
    _SUTL_MAKE_STRING_TUPLE_("Run Failure"),
    _SUTL_MAKE_STRING_TUPLE_("Run Exception"),
    _SUTL_MAKE_STRING_TUPLE_("Cleanup Failure"),
    _SUTL_MAKE_STRING_TUPLE_("Cleanup Exception"),
    _SUTL_MAKE_STRING_TUPLE_("Unhandled Exception")
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

template <typename T>
const std::basic_string<T>& ResultTypeUtil::ToString(_In_ const ResultType& r)
{
    ValidateResultType(r, __FUNCTION__);
    return std::get<std::basic_string<T>>(m_svResultTypeStrings[static_cast<size_t>(r)]);
}

/// Explicit Template Instantiation \\\

template const std::basic_string<char>& ResultTypeUtil::ToString(_In_ const ResultType& r);
template const std::basic_string<wchar_t>& ResultTypeUtil::ToString(_In_ const ResultType& r);
