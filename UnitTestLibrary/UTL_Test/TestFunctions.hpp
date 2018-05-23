#ifndef _TEST_FUNCTIONS_H
#define _TEST_FUNCTIONS_H

#include <UnitTestResult.h>

inline void ThrowAllocationException( )
{
    throw std::bad_alloc( );
}

inline void ThrowRuntimeException( )
{
    throw std::runtime_error("Runtime error encountered.");
}

inline void ThrowInvalidArgument( )
{
    throw std::invalid_argument("Invalid argument provided.");
}

inline UnitTestResult SkippedTest( )
{
    UTL_SKIP_TEST("Testing UTL_SKIP_TEST macro");
}

inline UnitTestResult PositiveTest( )
{
    UTL_TEST_SUCCESS( );
}

inline UnitTestResult SetupFailureTest( )
{
    UTL_SETUP_FAILURE( );
}

inline UnitTestResult SetupExceptionTest( )
{
    try
    {
        ThrowAllocationException( );
    }
    catch ( const std::bad_alloc& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    return UnitTestResult( );
}

inline UnitTestResult RunFailureTest( )
{
    UTL_TEST_FAILURE( );
}

inline UnitTestResult RunExceptionTest( )
{
    try
    {
        ThrowRuntimeException( );
    }
    catch ( const std::runtime_error& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    return UnitTestResult( );
}

inline UnitTestResult CleanupFailureTest( )
{
    UTL_CLEANUP_FAILURE( );
}

inline UnitTestResult CleanupExceptionTest( )
{
    try
    {
        ThrowInvalidArgument( );
    }
    catch ( const std::logic_error& e )
    {
        UTL_CLEANUP_EXCEPTION(e.what( ));
    }

    return UnitTestResult( );
}

inline UnitTestResult UnhandledExceptionTest( )
{
    try
    {
        ThrowInvalidArgument( );
    }
    catch ( const std::runtime_error& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    return UnitTestResult( );
}

#endif