#pragma once

#include <list>
#include <UnitTest.h>

namespace TTLTests
{
    namespace CountedPointer
    {
        std::list<std::function<UnitTestResult(void)>> BuildTestList( );

        // Constructor Tests
        UnitTestResult DefaultConstructor_Single( );
        UnitTestResult DefaultConstructor_Array( );
        UnitTestResult NullConstructor_Single( );
        UnitTestResult NullConstructor_Array( );
        UnitTestResult RawConstructor_Single( );
        UnitTestResult RawConstructor_Array( );
        UnitTestResult CopyConstructor_Single( );
        UnitTestResult CopyConstructor_Array( );
        UnitTestResult MoveConstructor_Single( );
        UnitTestResult MoveConstructor_Array( );

        // Bool and Relational Operator Tests
        UnitTestResult BoolOperator_Single( );
        UnitTestResult BoolOperator_Array( );
        UnitTestResult EquivalenceOperator_Single( );
        UnitTestResult EquivalenceOperator_Array( );
        UnitTestResult NonEquivalenceOperator_Single( );
        UnitTestResult NonEquivalenceOperator_Array( );
        UnitTestResult LessThanOperator_Single( );
        UnitTestResult LessThanOperator_Array( );
        UnitTestResult GreaterThanOperator_Single( );
        UnitTestResult GreaterThanOperator_Array( );
        UnitTestResult LessThanOrEqualOperator_Single( );
        UnitTestResult LessThanOrEqualOperator_Array( );
        UnitTestResult GreaterThanOrEqualOperator_Single( );
        UnitTestResult GreaterThanOrEqualOperator_Array( );

        // Assignment Operator Overload Tests
        UnitTestResult AssignmentOperatorRaw_Single( );
        UnitTestResult AssignmentOperatorRaw_Array( );
        UnitTestResult AssignmentOperatorNull_Single( );
        UnitTestResult AssignmentOperatorNull_Array( );
        UnitTestResult AssignmentOperatorCopy_Single( );
        UnitTestResult AssignmentOperatorCopy_Array( );
        UnitTestResult AssignmentOperatorMove_Single( );
        UnitTestResult AssignmentOperatorMove_Array( );
        UnitTestResult AssignmentOperatorSelf_Single( );
        UnitTestResult AssignmentOperatorSelf_Array( );

        // Public Method Tests
        UnitTestResult Release_Single( );
        UnitTestResult Release_Array( );
        UnitTestResult SetNull_Single( );
        UnitTestResult SetNull_Array( );
        UnitTestResult SetRaw_Single( );
        UnitTestResult SetRaw_Array( );
        UnitTestResult SetCopy_Single( );
        UnitTestResult SetCopy_Array( );
        UnitTestResult SetMove_Single( );
        UnitTestResult SetMove_Array( );
        UnitTestResult SetSelf_Single( );
        UnitTestResult SetSelf_Array( );
    }
}