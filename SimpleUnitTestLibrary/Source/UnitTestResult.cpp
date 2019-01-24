#include <UnitTestResult.h>

/// Method Definitions \\\

/// Ctors \\\

UnitTestResult::UnitTestResult(_In_ ResultType result, _In_ const std::string& func, _In_ const std::string& file, _In_ const uint32& line, _In_ const std::string& info) noexcept :
    mResult(result),
    mFuncName(func.empty( ) ? std::string( ) : ExtractFuncName(func.c_str( ), func.size( ))),
    mFileName(file.empty( ) ? std::string( ) : ExtractFileName(file.c_str( ), file.size( ))),
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

constexpr const utf8* UnitTestResult::ExtractNameCommon(_In_ const utf8* str, _In_ const size_t& len, _In_ ExtractPredicate pred) noexcept
{
    if ( !str || len == 0 )
    {
        return nullptr;
    }

    const utf8* name = str + len;
    while ( name >= str )
    {
        if ( pred(*(--name)) )
        {
            break;
        }
    }

    return name + sizeof(utf8);
}

constexpr const utf8* UnitTestResult::ExtractFileName(_In_ const utf8* f, _In_ const size_t& n) noexcept
{
    return ExtractNameCommon(f, n, IsPathSeparator);
}

constexpr const utf8* UnitTestResult::ExtractFuncName(_In_ const utf8* f, _In_ const size_t& n) noexcept
{
    return ExtractNameCommon(f, n, IsWhitespace);
}

constexpr bool UnitTestResult::IsPathSeparator(_In_ const utf8& c) noexcept
{
    return (c == '/' || c == '\\');
}

constexpr bool UnitTestResult::IsWhitespace(_In_ const utf8& c) noexcept
{
    return (c == ' ' || c == '\t');
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

const uint32& UnitTestResult::GetLineNumber( ) const noexcept
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

void UnitTestResult::SetLineNumber(_In_ const uint32& line) noexcept
{
    mLineNum = line;
}

void UnitTestResult::SetResultInfo(_In_ const std::string& info)
{
    mResultInfo = info;
}