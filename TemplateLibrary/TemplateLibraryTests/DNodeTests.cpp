#include "DNodeTests.h"
#include "CopyMoveHelper.hpp"

#include <DNode.hpp>


std::list<std::function<UnitTestResult(void)>> TTLTests::DNode::BuildTestList( )
{
    std::list<std::function<UnitTestResult(void)>> testList =
    {
        // Constructor Tests
        DefaultConstructor,
        MoveConstructor,

        // Assignment Operator Test
        MoveAssignment,

        // Setter Tests
        SetDataCopy,
        SetDataMove,
        SetPrevRaw,
        SetNextRaw
    };

    return testList;
}


/// Constructor Tests \\\

UnitTestResult TTLTests::DNode::DefaultConstructor( )
{
    TTL::DNode<int> iNode;
    TTL::DNode<unsigned long long> ullNode;
    TTL::DNode<double> dNode;
    TTL::DNode<char*> ptrNode;
    TTL::DNode<TTL::DNode<int>> iNodeNode;

    // Ensure that various data values are default-initialized.
    UTL_TEST_ASSERT(iNode.GetData( ) == 0);
    UTL_TEST_ASSERT(ullNode.GetData( ) == 0ull);
    UTL_TEST_ASSERT(dNode.GetData( ) == 0.0);
    UTL_TEST_ASSERT(ptrNode.GetData( ) == nullptr);
    UTL_TEST_ASSERT(iNodeNode.GetData( ).GetData( ) == 0 && iNodeNode.GetData( ).GetNext( ) == nullptr);

    // Ensure that prev ptr values are nullptr-initialized.
    UTL_TEST_ASSERT(iNode.GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(ullNode.GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(dNode.GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(ptrNode.GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(iNodeNode.GetPrev( ) == nullptr);

    // Ensure that next ptr values are nullptr-initialized.
    UTL_TEST_ASSERT(iNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(ullNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(dNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(ptrNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(iNodeNode.GetNext( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DNode::MoveConstructor( )
{
    const int n = 0x0FEEDBAC;

    TTL::DNode<int>* pPrev = nullptr;
    TTL::DNode<int>* pNext = nullptr;

    TTL::DNode<int> iNode;

    try
    {
        pPrev = new TTL::DNode<int>;
        pNext = new TTL::DNode<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    iNode.SetData(n);
    iNode.SetPrev(pPrev);
    iNode.SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(iNode.GetData( ) == n);
    UTL_SETUP_ASSERT(iNode.GetPrev( ) != nullptr);
    UTL_SETUP_ASSERT(iNode.GetPrev( ) == pPrev);
    UTL_SETUP_ASSERT(iNode.GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(iNode.GetNext( ) == pNext);

    TTL::DNode<int> moveNode(std::move(iNode));

    // Ensure values were transferred.
    UTL_TEST_ASSERT(iNode.GetData( ) == 0);
    UTL_TEST_ASSERT(iNode.GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(iNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(moveNode.GetData( ) == n);
    UTL_TEST_ASSERT(moveNode.GetPrev( ) == pPrev);
    UTL_TEST_ASSERT(moveNode.GetNext( ) == pNext);

    // Cleanup
    delete pPrev;
    pPrev = nullptr;
    delete pNext;
    pNext = nullptr;

    UTL_CLEANUP_ASSERT(pPrev == nullptr);
    UTL_CLEANUP_ASSERT(pNext == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Move Assignment Test \\\

UnitTestResult TTLTests::DNode::MoveAssignment( )
{
    const int n = 0x0FEEDBAC;

    TTL::DNode<int>* pPrev = nullptr;
    TTL::DNode<int>* pNext = nullptr;

    TTL::DNode<int> iNode;
    TTL::DNode<int> moveNode;

    try
    {
        pPrev = new TTL::DNode<int>;
        pNext = new TTL::DNode<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    iNode.SetData(n);
    iNode.SetPrev(pPrev);
    iNode.SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(iNode.GetData( ) == n);
    UTL_SETUP_ASSERT(iNode.GetPrev( ) != nullptr);
    UTL_SETUP_ASSERT(iNode.GetPrev( ) == pPrev);
    UTL_SETUP_ASSERT(iNode.GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(iNode.GetNext( ) == pNext);

    moveNode = std::move(iNode);

    // Ensure values were transferred.
    UTL_TEST_ASSERT(iNode.GetData( ) == 0);
    UTL_TEST_ASSERT(iNode.GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(iNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(moveNode.GetData( ) == n);
    UTL_TEST_ASSERT(moveNode.GetPrev( ) == pPrev);
    UTL_TEST_ASSERT(moveNode.GetNext( ) == pNext);

    // Cleanup
    delete pPrev;
    pPrev = nullptr;
    delete pNext;
    pNext = nullptr;

    UTL_CLEANUP_ASSERT(pPrev == nullptr);
    UTL_CLEANUP_ASSERT(pNext == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}



UnitTestResult TTLTests::DNode::SetDataCopy( )
{
    TTL::DNode<CopyMoveHelper> testNode;
    CopyMoveHelper copyHelper;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(testNode.GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(testNode.GetData( ).GetMove( ) == false);

    testNode.SetData(copyHelper);

    // Ensure copy was used.
    UTL_SETUP_ASSERT(testNode.GetData( ).GetCopy( ) == true);
    UTL_SETUP_ASSERT(testNode.GetData( ).GetMove( ) == false);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DNode::SetDataMove( )
{
    TTL::DNode<CopyMoveHelper> testNode;
    CopyMoveHelper moveHelper;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(testNode.GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(testNode.GetData( ).GetMove( ) == false);

    testNode.SetData(std::move(moveHelper));

    // Ensure move was used.
    UTL_SETUP_ASSERT(testNode.GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(testNode.GetData( ).GetMove( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DNode::SetPrevRaw( )
{
    TTL::DNode<int> testNode;
    TTL::DNode<int>* pPrev = nullptr;

    try
    {
        pPrev = new TTL::DNode<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(testNode.GetPrev( ) == nullptr);
    UTL_SETUP_ASSERT(pPrev != nullptr);

    testNode.SetPrev(pPrev);

    // Ensure value was copied.
    UTL_TEST_ASSERT(testNode.GetPrev( ) != nullptr);
    UTL_TEST_ASSERT(testNode.GetPrev( ) == pPrev);

    // Cleanup
    delete pPrev;
    pPrev = nullptr;

    UTL_CLEANUP_ASSERT(pPrev == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DNode::SetNextRaw( )
{
    TTL::DNode<int> testNode;
    TTL::DNode<int>* pNext = nullptr;

    try
    {
        pNext = new TTL::DNode<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(testNode.GetNext( ) == nullptr);
    UTL_SETUP_ASSERT(pNext != nullptr);

    testNode.SetNext(pNext);

    // Ensure value was copied.
    UTL_TEST_ASSERT(testNode.GetNext( ) != nullptr);
    UTL_TEST_ASSERT(testNode.GetNext( ) == pNext);

    // Cleanup
    delete pNext;
    pNext = nullptr;

    UTL_CLEANUP_ASSERT(pNext == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}