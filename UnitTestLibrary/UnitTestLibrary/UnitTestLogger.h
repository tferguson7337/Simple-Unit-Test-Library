#ifndef _UNIT_TEST_LOGGER_H
#define _UNIT_TEST_LOGGER_H

#include <Uncopyable.h>
#include <Unmovable.h>

#include <Interface\UnitTestLoggerInterface.h>

#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>


template <class T>
class UnitTestLogger : public UnitTestLoggerInterface<T>, public Uncopyable
{
private:
    /// Private Data Members \\\

    bool mPrintToConsole;
    std::basic_ostream<T>& mConsoleStream;
    std::basic_ofstream<T> mFileStream;
    std::basic_string<T> mTargetFile;

    static const uint64 mTimeBufferSize = 32;

    /// Private Helper Methods \\\

    std::basic_ofstream<T>& operator<<(std::basic_string<T>&&);

    std::basic_ostream<T>& InitConsoleStream( );

    std::basic_string<T> BuildLogString(const UnitTestResult&);
    std::basic_string<T> BuildTimeString( );
    std::basic_string<T> BuildResultString(const UnitTestResult&);
    std::basic_string<T> BuildExceptionString(const UnitTestResult&);
    std::basic_string<T> BuildUnhandledExceptionString(const UnitTestResult&);

    const T* GetResultString(const Result);
    const std::basic_string<T>& GetTimeFormat( );
    const std::basic_string<T>& GetResultFormat( );
    const std::basic_string<T>& GetExceptionFormat( );
    const std::basic_string<T>& GetUnhandledExceptionFormat( );
    const std::basic_string<T>& GetDivider( );

    bool GetTime(T*, const time_t*);

    std::basic_string<T> stprintf(const std::basic_string<T>*, ...);
    int StringPrintWrapper(std::vector<T>&, const std::basic_string<T>*, va_list);

public:
    /// Ctors \\\

    UnitTestLogger(const std::basic_string<T>&, bool);
    UnitTestLogger(UnitTestLogger&&) noexcept;

    /// Dtor \\\

    ~UnitTestLogger( );

    /// Operator Overload \\\

    UnitTestLogger& operator=(UnitTestLogger&&) noexcept;

    /// Getters \\\

    const std::basic_string<T>& GetTargetFile( ) const;
    bool GetPrintToConsole( ) const;

    /// Setters \\\

    bool SetTargetFile(const std::basic_string<T>&);
    bool SetTargetFile(std::basic_string<T>&&);
    void SetPrintToConsole(bool);

    /// Public Methods \\\

    bool LogUnitTestResult(const UnitTestResult&);
};

#endif // _UNIT_TEST_LOGGER_H