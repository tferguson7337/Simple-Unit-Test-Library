#ifndef _UNIT_TEST_LOGGER_INTERFACE_H
#define _UNIT_TEST_LOGGER_INTERFACE_H

#include <string>

#include "UnitTestResult.h"

/// Forward Declaration
template <class T>
class UnitTestLogger;

template <class T>
class UnitTestRunner;

template <class T>
class UnitTestLoggerInterface
{
public:
    UnitTestLoggerInterface( ) = default;
    virtual ~UnitTestLoggerInterface( ) = default;

    virtual UnitTestLogger<T>& operator=(UnitTestLogger<T>&&) noexcept = 0;

    virtual const std::basic_string<T>& GetTargetFile( ) const = 0;

    virtual bool SetTargetFile(const std::basic_string<T>&) = 0;
    virtual bool SetTargetFile(std::basic_string<T>&&) = 0;

    virtual bool LogTestSetHeader(const UnitTestRunner<T>&) = 0;
    virtual bool LogUnitTestResult(const UnitTestResult&) = 0;
    virtual bool LogTestSetSummary(const UnitTestRunner<T>&) = 0;
};

#endif