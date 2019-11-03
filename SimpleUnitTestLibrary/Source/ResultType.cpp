#include <ResultType.h>

// STL
#include <stdexcept>

/// Static Data Member Initialization \\\

const std::array<std::wstring, static_cast<size_t>(ResultType::End)> ResultTypeUtil::ms_ResultTypeStrings
{
    L"Not Run",
    L"Success",
    L"Setup Failure",
    L"Setup Exception",
    L"Run Failure",
    L"Run Exception",
    L"Cleanup Failure",
    L"Cleanup Exception",
    L"Unhandled Exception"
};

/// Static Private Methods \\\

void ResultTypeUtil::ValidateResultType(_In_z_ const char* f, _In_ const ResultType& r)
{
    if (r < ResultType::Begin || r >= ResultType::End)
    {
        static const std::string msg1(": Invalid ResultType argument provided[");
        static const std::string msg2("]");

        const std::string data1(std::to_string(static_cast<size_t>(r)));

        throw std::invalid_argument(f + msg1 + data1 + msg2);
    }
}

/// Static Public Methods \\\


const std::wstring& ResultTypeUtil::ToString(_In_ const ResultType& r)
{
    ValidateResultType(__FUNCTION__, r);
    return ms_ResultTypeStrings[static_cast<size_t>(r)];
}
