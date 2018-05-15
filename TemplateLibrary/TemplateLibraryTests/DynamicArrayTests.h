#pragma once

#include <list>
#include <UnitTest.h>

namespace TTLTests
{
    namespace DynamicArray
    {
        std::list<std::function<UnitTestResult(void)>> BuildTestList( );

        // Constructor Tests
        UnitTestResult DefaultConstructor( );
        UnitTestResult CapacityConstructor( );
        UnitTestResult CopyConstructor( );
        UnitTestResult MoveConstructor( );

        // Assignment Tests
        UnitTestResult CopyAssignment( );
        UnitTestResult MoveAssignment( );

        // Subscript Test
        UnitTestResult Subscript( );

        // Public Method Tests
        UnitTestResult At( );
        UnitTestResult Clear( );
        UnitTestResult CopyAppend( );
        UnitTestResult MoveAppend( );
        UnitTestResult Front( );
        UnitTestResult Back( );
        UnitTestResult Compress( );
        UnitTestResult Reserve( );
    }
}