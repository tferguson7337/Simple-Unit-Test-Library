#ifndef _UNIT_TEST_LOGGER_INTERFACE_H
#define _UNIT_TEST_LOGGER_INTERFACE_H

#include <string>

#include "UnitTestResult.h"

template <class T>
class UnitTestLoggerInterface
{
public:
    UnitTestLoggerInterface( ) = default;
    virtual ~UnitTestLoggerInterface( ) = default;

    virtual const std::basic_string<T>& GetTargetFile( ) const = 0;

    virtual bool SetTargetFile(const std::basic_string<T>&) = 0;
    virtual bool SetTargetFile(std::basic_string<T>&&) = 0;

    virtual bool LogUnitTestResult(const UnitTestResult&) = 0;
};

#endif