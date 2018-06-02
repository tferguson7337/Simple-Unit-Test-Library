#pragma once

#include <UnitTestRunner.h>

// For timing
#include <chrono>

// Template Library Tests
#include "NodeTests.h"
#include "DNodeTests.h"
#include "ArrayTests.h"
#include "DynamicArrayTests.h"
#include "ListTests.h"
#include "StackTests.h"
#include "QueueTests.h"
#include "ExclusivePointerTests.h"
#include "CountedPointerTests.h"


template <class T>
void BuildMasterTestList(UnitTestRunner<T>& runner)
{
    runner.AddUnitTests(TTLTests::Node::BuildTestList( ));
    runner.AddUnitTests(TTLTests::DNode::BuildTestList( ));
    runner.AddUnitTests(TTLTests::Array::BuildTestList( ));
    runner.AddUnitTests(TTLTests::DynamicArray::BuildTestList( ));
    runner.AddUnitTests(TTLTests::List::BuildTestList( ));
    runner.AddUnitTests(TTLTests::Stack::BuildTestList( ));
    runner.AddUnitTests(TTLTests::Queue::BuildTestList( ));
    runner.AddUnitTests(TTLTests::ExclusivePointer::BuildTestList( ));
    runner.AddUnitTests(TTLTests::CountedPointer::BuildTestList( ));
}

template <class R, class T>
R GetTimeDiff(
    const std::chrono::time_point<std::chrono::high_resolution_clock>& t0,
    const std::chrono::time_point<std::chrono::high_resolution_clock>& t1
)
{
    if ( t1 > t0 )
    {
        return std::chrono::duration_cast<std::chrono::duration<R, T>>(t1 - t0).count( );
    }
    else
    {
        return std::chrono::duration_cast<std::chrono::duration<R, T>>(t0 - t1).count( );
    }
}

template <class T>
void RunTemplateLibraryTests( )
{
    std::chrono::time_point<std::chrono::high_resolution_clock> buildT0, buildT1;
    std::chrono::time_point<std::chrono::high_resolution_clock> testT0, testT1;

    UnitTestRunner<T> testRunner("Template Library Tests");

    testRunner.SetLogFile("C:\\TTL_Log.txt");

    buildT0 = std::chrono::high_resolution_clock::now( );
    BuildMasterTestList(testRunner);
    buildT1 = std::chrono::high_resolution_clock::now( );

    testT0 = std::chrono::high_resolution_clock::now( );
    testRunner.RunUnitTests( );
    testT1 = std::chrono::high_resolution_clock::now( );

    testRunner.PrintTestLogs( );

    printf("\tTest Run Time [%3.2f ms]\n", GetTimeDiff<double, std::milli>(testT0, testT1));
    fflush(stdout);
}