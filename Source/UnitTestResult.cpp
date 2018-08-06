#include <UnitTestResult.h>

/// Method Definitions \\\

/// Ctors \\\

UnitTestResult::UnitTestResult(Result r, const std::string& func, const std::string& file, uint64 l, const std::string& i) noexcept :
    mResult(r),
    mFuncName(func.empty( ) ? std::string( ) : ExtractFuncName(func.c_str( ), func.size( ))),
    mFileName(file.empty( ) ? std::string( ) : ExtractFileName(file.c_str( ), file.size( ))),
    mLineNum(l),
    mResultInfo(i)
{ }

// Move Ctor
UnitTestResult::UnitTestResult(UnitTestResult&& src) noexcept
{
    *this = std::move(src);
    src = UnitTestResult( );
}

// Operator Overloads \\

UnitTestResult& UnitTestResult::operator=(UnitTestResult&& src) noexcept
{
    mResult = src.mResult;
    mLineNum = src.mLineNum;
    mFuncName.assign(std::move(src.mFuncName));
    mFileName.assign(std::move(src.mFileName));
    mResultInfo.assign(std::move(src.mResultInfo));

    return *this;
}

UnitTestResult::operator bool( ) const noexcept
{
    return mResult == Result::Success;
}

// Private Helpers \\

constexpr const char* UnitTestResult::ExtractFileName(const char* f, const uint64 n) const noexcept
{
    if ( !f || !n )
    {
        return nullptr;
    }

    const char* fn = f + n;
    while ( fn >= f )
    {
        if ( IsPathSeparator(*(--fn)) )
        {
            break;
        }
    }

    return fn + sizeof(*f);
}

constexpr const char* UnitTestResult::ExtractFuncName(const char* f, const uint64 n) const noexcept
{
    if ( !f || !n )
    {
        return nullptr;
    }

    const char* fn = f + n;
    while ( fn >= f )
    {
        if ( IsWhitespace(*(--fn)) )
        {
            break;
        }
    }

    return fn + sizeof(*f);
}

constexpr bool UnitTestResult::IsPathSeparator(const char c) const noexcept
{
    return (c == '/' || c == '\\');
}

constexpr bool UnitTestResult::IsWhitespace(const char c) const noexcept
{
    return (c == ' ' || c == '\t');
}


// Getters \\

Result UnitTestResult::GetResult( ) const noexcept
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

uint64 UnitTestResult::GetLineNumber( ) const noexcept
{
    return mLineNum;
}

const std::string& UnitTestResult::GetResultInfo( ) const noexcept
{
    static const std::string unknownStr("<NULL>");

    return (mResultInfo.empty( )) ? unknownStr : mResultInfo;
}


// Setters \\

void UnitTestResult::SetResult(Result res) noexcept
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

void UnitTestResult::SetLineNumber(uint64 line) noexcept
{
    mLineNum = line;
}

void UnitTestResult::SetResultInfo(const std::string& info)
{
    mResultInfo = info;
}