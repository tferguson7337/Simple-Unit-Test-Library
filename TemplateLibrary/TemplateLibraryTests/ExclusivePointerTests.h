#pragma once

#include <list>
#include <UnitTest.h>

namespace TTLTests
{
    namespace ExclusivePointer
    {
        std::list<std::function<UnitTestResult(void)>> BuildTestList( );
        
        // Constructor Tests
        UnitTestResult DefaultConstructor( );
        UnitTestResult NullConstructor( );
        UnitTestResult RawConstructor( );
        UnitTestResult MoveConstructor( );

        // Bool and Relational Operator Tests
        UnitTestResult BoolOperator( );
        UnitTestResult EquivalenceOperator( );
        UnitTestResult NonEquivalenceOperator( );
        UnitTestResult LessThanOperator( );
        UnitTestResult GreaterThanOperator( );
        UnitTestResult LessThanOrEqualOperator( );
        UnitTestResult GreaterThanOrEqualOperator( );

        // Assignment Operator Overload Tests
        UnitTestResult AssignmentOperatorRaw( );
        UnitTestResult AssignmentOperatorNull( );
        UnitTestResult AssignmentOperatorMove( );
        UnitTestResult AssignmentOperatorSelf( );

        // Public Method Tests
        UnitTestResult Release( );
        UnitTestResult SetNull( );
        UnitTestResult SetRaw( );
        UnitTestResult SetMove( );
        UnitTestResult SetSelf( );
    }
}
