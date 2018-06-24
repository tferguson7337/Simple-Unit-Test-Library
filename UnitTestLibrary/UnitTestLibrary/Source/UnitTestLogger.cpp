#include <UnitTestLogger.h>
#include <UnitTestRunner.h>

#include <cstdarg>
#include <ctime>

// Explicit Template Instantiation
template class UnitTestLogger<char>;
template class UnitTestLogger<wchar_t>;

/// Ctors \\\

template <class T>
UnitTestLogger<T>::UnitTestLogger(const std::basic_string<T>& file, bool consoleOutput) noexcept :
    mPrintToConsole(consoleOutput),
    mConsoleStream(InitConsoleStream( )),
    mFileStream(file, std::ios_base::app | std::ios_base::out),
    mTargetFile(file)
{ }

template <class T>
UnitTestLogger<T>::UnitTestLogger(UnitTestLogger&& src) noexcept :
    mConsoleStream(InitConsoleStream( )),
    mPrintToConsole(true)
{
    *this = std::move(src);
}


/// Dtor \\\

template <class T>
UnitTestLogger<T>::~UnitTestLogger( )
{
    if ( mPrintToConsole && mConsoleStream )
    {
        mConsoleStream.flush( );
    }

    if ( mFileStream )
    {
        mFileStream.flush( );
    }
}

/// Operator Overload \\\

template <class T>
UnitTestLogger<T>& UnitTestLogger<T>::operator=(UnitTestLogger&& src) noexcept
{
    mPrintToConsole = src.mPrintToConsole;
    mFileStream = std::move(src.mFileStream);
    mTargetFile = std::move(src.mTargetFile);

    src.mPrintToConsole = false;
    src.mConsoleStream.flush( );
    src.mFileStream.close( );
    src.mTargetFile.clear( );

    return *this;
}


/// Method Definitions \\\


// Private Helper Methods

template <>
std::basic_ostream<char>& UnitTestLogger<char>::InitConsoleStream( )
{
    return std::cout;
}

template <>
std::basic_ostream<wchar_t>& UnitTestLogger<wchar_t>::InitConsoleStream( )
{
    return std::wcout;
}



template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildTestSetHeaderString(const TestSetData<T>& data)
{
    return stprintf(
        &GetTestSetHeaderFormat( ),
        data.GetTestSetName( ).c_str( ),
        data.GetTotalTestCount( )
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildTestSetSummaryString(const TestSetData<T>& data)
{
    return stprintf(
        &GetTestSetSummaryFormat( ),
        data.GetTestSetName( ).c_str( ),
        data.GetTotalTestCount( ),
        data.GetTotalTestCount( ) - data.GetTestSkipCount( ),
        data.GetTestPassCount( ),
        data.GetTotalFailureCount( ),
        data.GetSetupFailureCount( ),
        data.GetSetupExceptionCount( ),
        data.GetTestFailureCount( ),
        data.GetTestExceptionCount( ),
        data.GetCleanupFailureCount( ),
        data.GetCleanupExceptionCount( ),
        data.GetUnhandledExceptionCount( ),
        data.GetTestSkipCount( ),
        data.GetTestSetGrade( ),
        data.GetTestPassCount( ),
        data.GetTotalTestCount( ) - data.GetTestSkipCount( )
    );
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetTestSetHeaderFormat( )
{
    static const size_t lineBreakLen = 64;

    static const std::basic_string<char> headerFormatA(
        GetLineBreakHeavy(lineBreakLen)
        +
        "  Test-Set Name: %-25s -- Total Tests: %zu"
        +
        GetLineBreakHeavy(lineBreakLen)
    );

    return headerFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetTestSetHeaderFormat( )
{
    static const size_t lineBreakLen = 64;

    static const std::basic_string<wchar_t> headerFormatW(
        GetLineBreakHeavy(lineBreakLen)
        +
        L"  Test-Set Name: %-25s -- Total Tests: %zu"
        +
        GetLineBreakHeavy(lineBreakLen)
    );

    return headerFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetTestSetSummaryFormat( )
{
    static const size_t lineBreakLen = 40;

    static const std::basic_string<char> lineBreak(
        GetLineBreakHeavy(lineBreakLen)
    );

    static const std::basic_string<char> summaryFormatA(
        lineBreak
        +
        "    \"%s\" Complete\n\n"
        "  Total Test Count: %llu\n"
        "  Total Tests Run:  %llu\n\n"
        "   Successful Tests: %llu\n\n"
        "   Failed Tests:     %llu\n"
        "    Setup Failures:       %llu\n"
        "    Setup Exceptions:     %llu\n"
        "    Test Failures:        %llu\n"
        "    Test Exceptions:      %llu\n"
        "    Cleanup Failures:     %llu\n"
        "    Cleanup Exceptions:   %llu\n"
        "    Unhandled Exceptions: %llu\n\n"
        "   Skipped Tests:    %llu\n\n"
        "  Test Pass Grade:  %2.2f%% (%llu / %llu)"
        +
        lineBreak
    );

    return summaryFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetTestSetSummaryFormat( )
{
    static const size_t lineBreakLen = 40;

    static const std::basic_string<wchar_t> lineBreak(
        GetLineBreakHeavy(lineBreakLen)
    );

    static const std::basic_string<wchar_t> summaryFormatW(
        lineBreak
        +
        L"    \"%s\" Complete\n\n"
        L"  Total Test Count: %llu\n"
        L"  Total Tests Run:  %llu\n\n"
        L"   Successful Tests: %llu\n\n"
        L"   Failed Tests:     %llu\n"
        L"    Setup Failures:       %llu\n"
        L"    Setup Exceptions:     %llu\n"
        L"    Test Failures:        %llu\n"
        L"    Test Exceptions:      %llu\n"
        L"    Cleanup Failures:     %llu\n"
        L"    Cleanup Exceptions:   %llu\n"
        L"    Unhandled Exceptions: %llu\n\n"
        L"   Skipped Tests:    %llu\n\n"
        L"  Test Pass Grade:  %2.2f%% (%llu / %llu)"
        +
        lineBreak
    );

    return summaryFormatW;
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::GetLineBreakLight(size_t len)
{
    std::basic_string<T> lineBreak;

    lineBreak.reserve(len + 1);
    lineBreak.append(len, T('-'));
    lineBreak.push_back(T('\n'));

    return lineBreak;
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::GetLineBreakHeavy(size_t len)
{
    std::basic_string<T> lineBreak;

    lineBreak.reserve(len + 4);
    lineBreak.push_back(T('\n'));
    lineBreak.push_back(T('\n'));
    lineBreak.append(len, T('='));
    lineBreak.push_back(T('\n'));
    lineBreak.push_back(T('\n'));

    return lineBreak;
}


template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildLogString(const UnitTestResult& res)
{
    std::basic_string<T> logStr;

    switch ( res.GetResult( ) )
    {
    case Result::Success:
    case Result::SetupFailure:
    case Result::TestFailure:
    case Result::CleanupFailure:
        logStr.append(BuildResultString(res));
        break;

    case Result::SetupException:
    case Result::TestException:
    case Result::CleanupException:
        logStr.append(BuildExceptionString(res));
        break;

    case Result::NotRun:
        logStr.append(BuildSkipString(res));
        break;

    case Result::UnhandledException:
        logStr.append(BuildUnhandledExceptionString(res));
        break;

    default:
        logStr.clear( );
    }

    return logStr;
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildTimeString( )
{
    std::vector<T> buffer(mTimeBufferSize, T('\0'));
    const time_t time = std::time(nullptr);

    if ( !GetTime(buffer.data( ), &time) )
    {
        return std::basic_string<T>( );
    }

    while ( !buffer.empty( ) && buffer.back( ) != T('\n') )
    {
        buffer.pop_back( );
    }

    if ( buffer.size( ) <= 1 )
    {
        return std::basic_string<T>( );
    }
    else
    {
        buffer.pop_back( );
    }

    buffer.push_back(T('\0'));

    return std::basic_string<T>(std::move(buffer.data( )));
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildResultString(const UnitTestResult& res)
{
    return stprintf(
        &GetResultFormat( ),
        res.GetFileName( ).c_str( ),
        res.GetFunctionName( ).c_str( ),
        GetResultString(res.GetResult( )),
        res.GetLineNumber( )
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildExceptionString(const UnitTestResult& res)
{
    return stprintf(
        &GetExceptionFormat( ),
        res.GetFileName( ).c_str( ),
        res.GetFunctionName( ).c_str( ),
        GetResultString(res.GetResult( )),
        res.GetLineNumber( ),
        res.GetResultInfo( ).c_str( )
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildSkipString(const UnitTestResult& res)
{
    return stprintf(
        &GetSkipFormat( ),
        res.GetFileName( ).c_str( ),
        res.GetFunctionName( ).c_str( ),
        GetResultString(res.GetResult( )),
        res.GetLineNumber( ),
        res.GetResultInfo( ).c_str( )
    );
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildUnhandledExceptionString(const UnitTestResult& res)
{
    return stprintf(
        &GetUnhandledExceptionFormat( ),
        GetResultString(res.GetResult( )),
        res.GetResultInfo( ).c_str( )
    );
}

template <>
const char* UnitTestLogger<char>::GetResultString(const Result r)
{
    switch ( r )
    {
    case Result::NotRun:
        return ("Not Run");
        break;

    case Result::Success:
        return ("Success");
        break;

    case Result::SetupFailure:
        return ("Setup Failure");
        break;

    case Result::SetupException:
        return ("Setup Exception");
        break;

    case Result::TestFailure:
        return ("Run Failure");
        break;

    case Result::TestException:
        return ("Run Exception");
        break;

    case Result::CleanupFailure:
        return ("Cleanup Failure");
        break;

    case Result::CleanupException:
        return ("Cleanup Exception");
        break;

    case Result::UnhandledException:
        return ("Unhandled Exception");
        break;

    default:
        return ("<Unknown Result Code>");
    }
}

template <>
const wchar_t* UnitTestLogger<wchar_t>::GetResultString(const Result r)
{
    switch ( r )
    {
    case Result::NotRun:
        return (L"Not Run");
        break;

    case Result::Success:
        return (L"Success");
        break;

    case Result::SetupFailure:
        return (L"Setup Failure");
        break;

    case Result::SetupException:
        return (L"Setup Exception");
        break;

    case Result::TestFailure:
        return (L"Run Failure");
        break;

    case Result::TestException:
        return (L"Run Exception");
        break;

    case Result::CleanupFailure:
        return (L"Cleanup Failure");
        break;

    case Result::CleanupException:
        return (L"Cleanup Exception");
        break;

    case Result::UnhandledException:
        return (L"Unhandled Exception");
        break;

    default:
        return (L"<Unknown Result Code>");
    }
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetTimeFormat( )
{
    static const std::basic_string<char> timeFormatA("%c");

    return timeFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetTimeFormat( )
{
    static const std::basic_string<wchar_t> timeFormatW(L"%c");

    return timeFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetResultFormat( )
{
    static const std::basic_string<char> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<char> resultFormatA(
        "File: %-24s  Test: %s\n"
        +
        lineBreak
        +
        "    Result: %s\n"
        "    Line: %llu\n"
        +
        lineBreak
    );

    return resultFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetResultFormat( )
{
    static const std::basic_string<wchar_t> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<wchar_t> resultFormatW(
        L"File: %-24S  Test: %S\n"
        +
        lineBreak
        +
        L"    Result: %s\n"
        L"    Line: %llu\n"
        +
        lineBreak
    );

    return resultFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetExceptionFormat( )
{
    static const std::basic_string<char> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<char> exceptionFormatA(
        "File: %-24s  Test: %s\n"
        +
        lineBreak
        +
        "    Result: %s\n"
        "    Line: %llu\n"
        "    Exception: %s\n"
        +
        lineBreak
    );

    return exceptionFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetExceptionFormat( )
{
    static const std::basic_string<wchar_t> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<wchar_t> exceptionFormatW(
        L"File: %-24S  Test: %S\n"
        +
        lineBreak
        +
        L"    Result: %s\n"
        L"    Line: %llu\n"
        L"    Exception: %S\n"
        +
        lineBreak
    );

    return exceptionFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetSkipFormat( )
{
    static const std::basic_string<char> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<char> skipFormatA(
        "File: %-24s  Test: %s\n"
        +
        lineBreak
        +
        "    Result: %s\n"
        "    Line: %llu\n"
        "    Reason: %s\n"
        +
        lineBreak
    );

    return skipFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetSkipFormat( )
{
    static const std::basic_string<wchar_t> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<wchar_t> skipFormatW(
        L"File: %-24S  Test: %S\n"
        +
        lineBreak
        +
        L"    Result: %s\n"
        L"    Line: %llu\n"
        L"    Reason: %S\n"
        +
        lineBreak
    );

    return skipFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetUnhandledExceptionFormat( )
{
    static const std::basic_string<char> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<char> unhandledExceptionFormatA(
        "Result: %s\n"
        +
        lineBreak
        +
        "    Exception: %s\n"
        +
        lineBreak
    );

    return unhandledExceptionFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetUnhandledExceptionFormat( )
{
    static const std::basic_string<wchar_t> lineBreak(
        GetLineBreakLight( )
    );

    static const std::basic_string<wchar_t> unhandledExceptionFormatW(
        L"Result: %s\n"
        +
        lineBreak
        +
        L"    Exception: %S\n"
        +
        lineBreak
    );

    return unhandledExceptionFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetDivider( )
{
    static const std::basic_string<char> dividerA(" -- ");

    return dividerA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetDivider( )
{
    static const std::basic_string<wchar_t> dividerW(L" -- ");

    return dividerW;
}

template <>
bool UnitTestLogger<char>::GetTime(char* buffer, const time_t* t)
{
    return ctime_s(buffer, mTimeBufferSize, t) == 0;
}

template <>
bool UnitTestLogger<wchar_t>::GetTime(wchar_t* buffer, const time_t* t)
{
    return _wctime_s(buffer, mTimeBufferSize, t) == 0;
}


template <class T>
std::basic_string<T> UnitTestLogger<T>::stprintf(const std::basic_string<T>* format, ...)
{
    va_list args;
    std::vector<T> buffer;
    int bufferSize = 0;

    if ( !format || format->empty( ) )
    {
        return std::basic_string<T>( );
    }

    va_start(args, format);

    bufferSize = StringPrintWrapper(buffer, format, args);

    if ( bufferSize > 0 )
    {
        buffer = std::move(std::vector<T>(static_cast<size_t>(bufferSize) + 2, T('\0')));
        bufferSize = StringPrintWrapper(buffer, format, args);
    }

    va_end(args);

    return (bufferSize > 0) ? std::basic_string<T>(std::move(buffer.data( ))) : std::basic_string<T>( );
}


template <>
int UnitTestLogger<char>::StringPrintWrapper(std::vector<char>& buffer, const std::basic_string<char>* format, va_list args)
{
    return (buffer.empty( )) ? _vscprintf(format->c_str( ), args) : vsnprintf(buffer.data( ), buffer.size( ) - 1, format->c_str( ), args);
}

template <>
int UnitTestLogger<wchar_t>::StringPrintWrapper(std::vector<wchar_t>& buffer, const std::basic_string<wchar_t>* format, va_list args)
{
    return (buffer.empty( )) ? _vscwprintf(format->c_str( ), args) : vswprintf(buffer.data( ), buffer.size( ) - 1, format->c_str( ), args);
}


/// Public Method Definitions \\\


// Getters

template <class T>
const std::basic_string<T>& UnitTestLogger<T>::GetTargetFile( ) const
{
    return mTargetFile;
}

template <class T>
bool UnitTestLogger<T>::GetPrintToConsole( ) const
{
    return mPrintToConsole;
}

// Setters

template <class T>
bool UnitTestLogger<T>::SetTargetFile(const std::basic_string<T>& filePath)
{
    std::basic_ofstream<T> newFileStream(filePath);

    if ( !newFileStream )
    {
        return false;
    }

    mFileStream.flush( );
    std::swap(newFileStream, mFileStream);
    mTargetFile.assign(filePath);

    return true;
}

template <class T>
bool UnitTestLogger<T>::SetTargetFile(std::basic_string<T>&& filePath)
{
    std::basic_ofstream<T> newFileStream(filePath);

    if ( !newFileStream )
    {
        return false;
    }

    mFileStream.flush( );
    std::swap(newFileStream, mFileStream);
    mTargetFile.assign(std::move(filePath));

    return true;
}

template <class T>
void UnitTestLogger<T>::SetPrintToConsole(bool print)
{
    mPrintToConsole = print;
}


// Public Methods

template <class T>
bool UnitTestLogger<T>::LogTestSetHeader(const TestSetData<T>& data)
{
    logBuffer.append(std::move(BuildTestSetHeaderString(data)));
    logBuffer.append(2, T('\n'));

    return true;
}

template <class T>
bool UnitTestLogger<T>::LogUnitTestResult(const UnitTestResult& res)
{
    logBuffer.append(std::move(BuildLogString(res)));
    logBuffer.append(2, T('\n'));

    return true;
}

template <class T>
bool UnitTestLogger<T>::LogTestSetSummary(const TestSetData<T>& data)
{
    logBuffer.append(std::move(BuildTestSetSummaryString(data)));
    logBuffer.append(2, T('\n'));

    return true;
}

template <class T>
bool UnitTestLogger<T>::PrintLogs( )
{
    bool ret = true;

    if ( mPrintToConsole && mConsoleStream )
    {
        mConsoleStream << logBuffer;

        if ( !mConsoleStream )
        {
            ret = false;
        }
    }

    if ( mFileStream )
    {
        mFileStream << logBuffer;

        if ( !mFileStream )
        {
            ret = false;
        }
    }

    return ret;
}