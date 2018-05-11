#include <UnitTestResult.h>

/// Method Definitions \\\

/// Ctors \\\

UnitTestResult::UnitTestResult(Result r, const std::string& func, const std::string& file, uint64 l, const std::string& e) :
    mResult(r),
    mFuncName(func),
    mFileName(file),
    mLineNum(l),
    mException(e)
{
    if ( !mFileName.empty( ) )
    {
        mFileName = ExtractFileName(mFileName.c_str( ), mFileName.size( ));
    }
}

// Move Ctor
UnitTestResult::UnitTestResult(UnitTestResult&& src)
{
    *this = std::move(src);
    src = std::move(UnitTestResult( ));
}

// Operator Overloads \\

UnitTestResult& UnitTestResult::operator=(UnitTestResult&& src)
{
    mResult = src.mResult;
    mLineNum = src.mLineNum;
    mFuncName.assign(std::move(src.mFuncName));
    mFileName.assign(std::move(src.mFileName));
    mException.assign(std::move(src.mException));

    return *this;
}

UnitTestResult::operator bool( ) const
{
    return mResult == Result::Success;
}

// Private Helpers \\

constexpr const char* UnitTestResult::ExtractFileName(const char* f, const uint64 n)
{
    if ( !f || !n)
    {
        return nullptr;
    }

    const char* fn = f + n;
    while (fn >= f)
    {
        if ( IsPathSeparator(*(--fn)) )
        {
            break;
        }
    }

    return fn + 1;
}

constexpr bool UnitTestResult::IsPathSeparator(const char c)
{
    return (c == '/' || c == '\\');
}


// Getters \\

Result UnitTestResult::GetResult( ) const
{
    return mResult;
}

const std::string& UnitTestResult::GetFunctionName( ) const
{
    return mFuncName;
}

const std::string& UnitTestResult::GetFileName( ) const
{
    return mFileName;
}

uint64 UnitTestResult::GetLineNumber( ) const
{
    return mLineNum;
}

const std::string& UnitTestResult::GetException( ) const
{
    static const std::string unknownStr("<Unknown Exception>");

    return (mException.empty( )) ? unknownStr : mException;
}


// Setters \\

void UnitTestResult::SetResult(Result res)
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

void UnitTestResult::SetLineNumber(uint64 line)
{
    mLineNum = line;
}

void UnitTestResult::SetException(std::string&& exception)
{
    mException = std::move(exception);
}