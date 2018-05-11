#ifndef _UNIT_TEST_RUNNER_INTERFACE_H
#define _UNIT_TEST_RUNNER_INTERFACE_H


#include "UnitTest.h"

#include <list>

///
//
//  Class:      UnitTestRunnerInterface
//  
//  Purpose:    Define public interface for UnitTestRunner class.
//
///
template <class T>
class UnitTestRunnerInterface
{
public:
    UnitTestRunnerInterface( ) = default;
    virtual ~UnitTestRunnerInterface( ) = default;

    virtual const std::list<UnitTest>& GetUnitTests( ) const = 0;
    virtual const std::basic_string<T>& GetLogFile( ) const = 0;
    virtual bool GetConsoleOutput( ) const = 0;

    virtual bool SetLogFile(const std::basic_string<T>&) = 0;
    virtual void SetConsoleOutput(bool) = 0;

    virtual bool AddUnitTest(UnitTest&&) = 0;
    virtual bool RunUnitTests( ) = 0;
};

#endif // _UNIT_TEST_RUNNER_INTERFACE_H
