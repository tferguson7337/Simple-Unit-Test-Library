#include <UnitTestLogger.h>


// Explicit Template Instantiation
template class UnitTestLogger<char>;
template class UnitTestLogger<wchar_t>;

/// Ctors \\\

template <class T>
UnitTestLogger<T>::UnitTestLogger(const std::basic_string<T>& file, bool consoleOutput) :
    mPrintToConsole(consoleOutput),
    mConsoleStream(InitConsoleStream( )),
    mFileStream(file, std::ios_base::app | std::ios_base::out),
    mTargetFile(file)
{ }

template <class T>
UnitTestLogger<T>::UnitTestLogger(UnitTestLogger&& src) :
    mConsoleStream(InitConsoleStream( ))
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
UnitTestLogger<T>& UnitTestLogger<T>::operator=(UnitTestLogger&& src)
{
    mPrintToConsole = src.mPrintToConsole;
    mFileStream = std::move(src.mFileStream);
    mTargetFile = std::move(src.mTargetFile);

    src.mPrintToConsole = false;
    src.mFileStream.close( );
    src.mTargetFile.clear( );

    return *this;
}


/// Method Definitions \\\

// Operator Overloads

template <class T>
std::basic_ofstream<T>& UnitTestLogger<T>::operator<<(std::basic_string<T>&& logMsg)
{
    if ( mPrintToConsole && mConsoleStream )
    {
        mConsoleStream << logMsg << std::endl;
    }

    if ( mFileStream )
    {
        mFileStream << BuildTimeString( ) << GetDivider( ) << logMsg << std::endl;
    }

    return mFileStream;
}

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
std::basic_string<T> UnitTestLogger<T>::BuildLogString(const UnitTestResult& res)
{
    std::basic_string<T> logStr;

    switch ( res.GetResult( ) )
    {
    case Result::NotRun:
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
    return stprintf(&GetResultFormat( ), GetResultString(res.GetResult( )), res.GetFunctionName( ).c_str( ), res.GetFileName( ).c_str( ), res.GetLineNumber( ));
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildExceptionString(const UnitTestResult& res)
{
    return stprintf(&GetExceptionFormat( ), GetResultString(res.GetResult( )), res.GetFunctionName( ).c_str( ), res.GetFileName( ).c_str( ), res.GetLineNumber( ), res.GetException( ).c_str( ));
}

template <class T>
std::basic_string<T> UnitTestLogger<T>::BuildUnhandledExceptionString(const UnitTestResult& res)
{
    return stprintf(&GetUnhandledExceptionFormat( ), GetResultString(res.GetResult( )), res.GetException( ).c_str( ));
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
        return ("Run exception");
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
        return (L"Run exception");
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
    static const std::basic_string<char> failFormatA("Result: %s" + GetDivider( ) + "Func: %s" + GetDivider( ) + "File: %s" + GetDivider( ) + "Line: %llu");

    return failFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetResultFormat( )
{
    static const std::basic_string<wchar_t> failFormatW(L"Result: %s" + GetDivider( ) + L"Func: %S" + GetDivider( ) + L"File: %S" + GetDivider( ) + L"Line: %llu");

    return failFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetExceptionFormat( )
{
    static const std::basic_string<char> exceptionFormatA("Result: %s" + GetDivider( ) + "Func: %s" + GetDivider( ) + "File: %s" + GetDivider( ) + "Line: %llu" + GetDivider( ) + "Exception: %s");

    return exceptionFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetExceptionFormat( )
{
    static const std::basic_string<wchar_t> exceptionFormatW(L"Result: %s" + GetDivider( ) + L"Func: %S" + GetDivider( ) + L"File: %S" + GetDivider( ) + L"Line: %llu" + GetDivider( ) + L"Exception: %S");

    return exceptionFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetUnhandledExceptionFormat( )
{
    static const std::basic_string<char> unhandledExceptionFormatA("Result: %s" + GetDivider( ) + "Exception: %s");

    return unhandledExceptionFormatA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetUnhandledExceptionFormat( )
{
    static const std::basic_string<wchar_t> unhandledExceptionFormatW(L"Result: %s" + GetDivider( ) + L"Exception: %S");

    return unhandledExceptionFormatW;
}

template <>
const std::basic_string<char>& UnitTestLogger<char>::GetDivider( )
{
    static const std::basic_string<char> dividerA(" || ");

    return dividerA;
}

template <>
const std::basic_string<wchar_t>& UnitTestLogger<wchar_t>::GetDivider( )
{
    static const std::basic_string<wchar_t> dividerW(L" || ");

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

    bufferSize = StringPrintWrapper(buffer, format, args) + 2;

    if ( bufferSize > 0 )
    {
        buffer = std::move(std::vector<T>(bufferSize, T('\0')));
        bufferSize = StringPrintWrapper(buffer, format, args);
    }

    va_end(args);

    return (bufferSize > 0) ? std::basic_string<T>(std::move(buffer.data( ))) : std::basic_string<T>( );
}


template <>
int UnitTestLogger<char>::StringPrintWrapper(std::vector<char>& buffer, const std::basic_string<char>* format, va_list args)
{
    return (buffer.empty( )) ? vsnprintf(nullptr, 0, format->c_str( ), args) : vsnprintf(buffer.data( ), buffer.size( ) - 1, format->c_str( ), args);
}

template <>
int UnitTestLogger<wchar_t>::StringPrintWrapper(std::vector<wchar_t>& buffer, const std::basic_string<wchar_t>* format, va_list args)
{
    return (buffer.empty( )) ? vswprintf(nullptr, 0, format->c_str( ), args) : vswprintf(buffer.data( ), buffer.size( ) - 1, format->c_str( ), args);
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
bool UnitTestLogger<T>::LogUnitTestResult(const UnitTestResult& res)
{
    this->operator<<(std::move(BuildLogString(res)));

    return true;
}
