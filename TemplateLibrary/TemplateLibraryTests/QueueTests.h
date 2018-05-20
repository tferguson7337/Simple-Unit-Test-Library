#pragma once

#include <list>

#include "UnitTest.h"

namespace TTLTests
{
    namespace Queue
    {
        std::list<std::function<UnitTestResult(void)>> BuildTestList( );

        // Constructor Tests
        UnitTestResult DefaultConstructor( );
        UnitTestResult CopyConstructor( );
        UnitTestResult MoveConstructor( );

        // Operator Overload Tests
        UnitTestResult CopyAssignment( );
        UnitTestResult MoveAssignment( );
        UnitTestResult CopyAppend_Data( );
        UnitTestResult MoveAppend_Data( );
        UnitTestResult CopyAppend_Queue( );
        UnitTestResult MoveAppend_Queue( );

        // Public Method Tests
        UnitTestResult Clear( );
        UnitTestResult CopyEnqueue_Data( );
        UnitTestResult MoveEnqueue_Data( );
        UnitTestResult CopyEnqueue_Queue( );
        UnitTestResult MoveEnqueue_Queue( );
        UnitTestResult Dequeue( );
    }
}