#pragma once

#include <list>
#include <UnitTest.h>

namespace TTLTests
{
    namespace DNode
    {
        std::list<std::function<UnitTestResult(void)>> BuildTestList( );

        // Constructor Tests
        UnitTestResult DefaultConstructor( );
        UnitTestResult MoveConstructor( );

        // Assignment Operator Test
        UnitTestResult MoveAssignment( );

        // Setter Tests
        UnitTestResult SetDataCopy( );
        UnitTestResult SetDataMove( );
        UnitTestResult SetPrevRaw( );
        UnitTestResult SetNextRaw( );
    }
}