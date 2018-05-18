#pragma once

#include <list>

#include <UnitTest.h>

namespace TTLTests
{
    namespace List
    {
        std::list<std::function<UnitTestResult(void)>> BuildTestList( );

        // Constructor Tests
        UnitTestResult DefaultConstructor( );
        UnitTestResult CopyConstructor( );
        UnitTestResult MoveConstructor( );

        // Assignment Tests
        UnitTestResult CopyAssignment( );
        UnitTestResult MoveAssignment( );

        // Append Assignment Tests
        UnitTestResult CopyAppendAssignmentData( );
        UnitTestResult MoveAppendAssignmentData( );
        UnitTestResult CopyAppendAssignmentList( );
        UnitTestResult MoveAppendAssignmentList( );

        // Node Linkage Test
        UnitTestResult AppendDataNodeLinkage( );
        UnitTestResult PrependDataNodeLinkage( );

        // Public Method Tests
        UnitTestResult Clear( );
        UnitTestResult Front( );
        UnitTestResult Back( );
        UnitTestResult CopyAppendData( );
        UnitTestResult MoveAppendData( );
        UnitTestResult CopyAppendList( );
        UnitTestResult MoveAppendList( );
        UnitTestResult CopyPrependData( );
        UnitTestResult MovePrependData( );
        UnitTestResult CopyPrependList( );
        UnitTestResult MovePrependList( );
        UnitTestResult PopFront( );
        UnitTestResult PopBack( );
        UnitTestResult CopyInsertData( );
        UnitTestResult MoveInsertData( );
        UnitTestResult CopyInsertList( );
        UnitTestResult MoveInsertList( );
        UnitTestResult Remove( );
    }
}