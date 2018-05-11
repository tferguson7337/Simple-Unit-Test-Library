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

inline UnitTestResult ErroneousTest( )
{
    return UnitTestResult(Result::NotRun, __func__, __FILE__, __LINE__, "This should never happen.");
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
    catch ( const std::exception& e )
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
    catch ( const std::exception& e )
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
    catch ( const std::exception& e )
    {
        UTL_CLEANUP_EXCEPTION(e.what( ));
    }

    return UnitTestResult( );
}



#endif