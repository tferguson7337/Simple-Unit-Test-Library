#include <UnitTestResult.h>

/// Method Definitions \\\

/// Ctors \\\

UnitTestResult::UnitTestResult( ) noexcept :
    mResult(ResultType::NotRun),
    mFuncName(std::string( )),
    mFileName(std::string( )),
    mLineNum(0),
    mResultInfo(std::string( ))
{ }

UnitTestResult::UnitTestResult(
    _In_ ResultType result, 
    _In_ const char* func, _In_ const size_t& funcLen,
    _In_ const char* file, _In_ const size_t& fileLen, _In_ const uint32_t& line, 
    _In_ const std::string& info
) :
    mResult(result),
    mFuncName(ExtractFuncName(func, funcLen)),
    mFileName(ExtractFileName(file, fileLen)),
    mLineNum(line),
    mResultInfo(info)
{ }

// Move Ctor
UnitTestResult::UnitTestResult(_In_ UnitTestResult&& src) noexcept :
    mResult(src.mResult),
    mLineNum(src.mLineNum),
    mFuncName(std::move(src.mFuncName)),
    mFileName(std::move(src.mFileName)),
    mResultInfo(std::move(src.mResultInfo))
{ }

// Operator Overloads \\

UnitTestResult& UnitTestResult::operator=(_In_ UnitTestResult&& src) noexcept
{
    if ( this != &src )
    {
        mResult = src.mResult;
        mLineNum = src.mLineNum;
        mFuncName.assign(std::move(src.mFuncName));
        mFileName.assign(std::move(src.mFileName));
        mResultInfo.assign(std::move(src.mResultInfo));
    }
    
    return *this;
}

UnitTestResult::operator bool( ) const noexcept
{
    return mResult == ResultType::Success;
}

// Private Helpers \\

constexpr const char* UnitTestResult::ExtractNameCommon(_In_ const char* str, _In_ const size_t& len, _In_ ExtractPredicate pred) noexcept
{
    if ( !str || len == 0 )
    {
        return nullptr;
    }

    const char* name = str + len;
    while ( name >= str )
    {
        if ( pred(*(--name), mLTGTCounter) && mLTGTCounter == 0 )
        {
            break;
        }
    }

    return name + sizeof(char);
}

constexpr const char* UnitTestResult::ExtractFileName(_In_ const char* f, _In_ const size_t& n) noexcept
{
    const char* p = ExtractNameCommon(f, n, IsPathSeparator);
    return p ? p : "<BAD_FILE_NAME>";
}

constexpr const char* UnitTestResult::ExtractFuncName(_In_ const char* f, _In_ const size_t& n) noexcept
{
    const char* p = ExtractNameCommon(f, n, IsWhitespace);
    return p ? p : "<BAD_FUNC_NAME>";
}

constexpr bool UnitTestResult::IsPathSeparator(_In_ const char& c, _Inout_ uint32_t&) noexcept
{
    return (c == '/' || c == '\\');
}

constexpr bool UnitTestResult::IsWhitespace(_In_ const char& c, _Inout_ uint32_t& counter) noexcept
{
    if ( c == '>' )
    {
        counter++;
    }
    else if ( c == '<' )
    {
        counter--;
    }

    return (c == ' ');
}


// Getters \\

const ResultType& UnitTestResult::GetResult( ) const noexcept
{
    return mResult;
}

const std::string& UnitTestResult::GetFunctionName( ) const noexcept
{
    return mFuncName;
}

const std::string& UnitTestResult::GetFileName( ) const noexcept
{
    return mFileName;
}

const uint32_t& UnitTestResult::GetLineNumber( ) const noexcept
{
    return mLineNum;
}

const std::string& UnitTestResult::GetResultInfo( ) const noexcept
{
    static const std::string unknownStr("<NULL>");

    return (mResultInfo.empty( )) ? unknownStr : mResultInfo;
}


// Setters \\

void UnitTestResult::SetResult(_In_ const ResultType& res) noexcept
{
    mResult = res;
}

void UnitTestResult::SetFunctionName(_In_ const std::string& func)
{
    mFuncName = func;
}

void UnitTestResult::SetFileName(_In_ const std::string& file)
{
    mFileName = file;
}

void UnitTestResult::SetLineNumber(_In_ const uint32_t& line) noexcept
{
    mLineNum = line;
}

void UnitTestResult::SetResultInfo(_In_ const std::string& info)
{
    mResultInfo = info;
}