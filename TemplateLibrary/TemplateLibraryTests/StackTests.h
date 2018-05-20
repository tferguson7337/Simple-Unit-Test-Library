#pragma once

#include <list>
#include <UnitTest.h>

namespace TTLTests
{
    namespace Stack
    {
        std::list<std::function<UnitTestResult(void)>> BuildTestList( );

        // Constructor Tests
        UnitTestResult DefaultConstructor( );
        UnitTestResult CopyConstructor( );
        UnitTestResult MoveConstructor( );

        // Assignment Operator Tests
        UnitTestResult CopyAssignment( );
        UnitTestResult MoveAssignment( );

        // Public Method Tests
        UnitTestResult Clear( );
        UnitTestResult CopyPush( );
        UnitTestResult MovePush( );
        UnitTestResult Pop( );
    }
}