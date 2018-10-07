#include <UnitTestResult.h>

/// Method Definitions \\\

/// Ctors \\\

UnitTestResult::UnitTestResult(ResultType result, const std::string& func, const std::string& file, uint32 line, const std::string& info) noexcept :
    mResult(result),
    mFuncName(func.empty( ) ? std::string( ) : ExtractFuncName(func.c_str( ), func.size( ))),
    mFileName(file.empty( ) ? std::string( ) : ExtractFileName(file.c_str( ), file.size( ))),
    mLineNum(line),
    mResultInfo(info)
{ }

// Move Ctor
UnitTestResult::UnitTestResult(UnitTestResult&& src) noexcept :
    mResult(src.mResult),
    mLineNum(src.mLineNum),
    mFuncName(std::move(src.mFuncName)),
    mFileName(std::move(src.mFileName)),
    mResultInfo(std::move(src.mResultInfo))
{ }

// Operator Overloads \\

UnitTestResult& UnitTestResult::operator=(UnitTestResult&& src) noexcept
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

constexpr const char* UnitTestResult::ExtractNameCommon(const ascii* str, const size_t len, ExtractPredicate pred) noexcept
{
    if ( !str || len == 0 )
    {
        return nullptr;
    }

    const char* name = str + len;
    while ( name >= str )
    {
        if ( pred(*(--name)) )
        {
            break;
        }
    }

    return name + sizeof(char);
}

constexpr const char* UnitTestResult::ExtractFileName(const ascii* f, const size_t n) noexcept
{
    return ExtractNameCommon(f, n, IsPathSeparator);
}

constexpr const char* UnitTestResult::ExtractFuncName(const ascii* f, const size_t n) noexcept
{
    return ExtractNameCommon(f, n, IsWhitespace);
}

constexpr bool UnitTestResult::IsPathSeparator(const ascii c) noexcept
{
    return (c == '/' || c == '\\');
}

constexpr bool UnitTestResult::IsWhitespace(const ascii c) noexcept
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

void UnitTestResult::SetResult(const ResultType& res) noexcept
{
    mResult = res;
}

void UnitTestResult::SetFunctionName(const std::string& func)
{
    mFuncName = func;
}

void UnitTestResult::SetFileName(const std::string& file)
{
    mFileName = file;
}

void UnitTestResult::SetLineNumber(const uint32& line) noexcept
{
    mLineNum = line;
}

void UnitTestResult::SetResultInfo(const std::string& info)
{
    mResultInfo = info;
}