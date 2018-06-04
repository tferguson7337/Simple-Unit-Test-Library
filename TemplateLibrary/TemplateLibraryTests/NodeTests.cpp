#include "NodeTests.h"
#include "MemoryManagementHelper.hpp"

#include <Node.hpp>


std::list<std::function<UnitTestResult(void)>> TTLTests::Node::BuildTestList( )
{
    static const std::list<std::function<UnitTestResult(void)>> testList =
    {
        // Constructor Tests
        DefaultConstructor,
        MoveConstructor,

        // Assignment Operator Test
        MoveAssignment,

        // Setter Tests
        SetDataCopy,
        SetDataMove,
        SetNextRaw
    };

    return testList;
}


/// Constructor Tests \\\

UnitTestResult TTLTests::Node::DefaultConstructor( )
{
    TTL::Node<int> iNode;
    TTL::Node<unsigned long long> ullNode;
    TTL::Node<double> dNode;
    TTL::Node<char*> ptrNode;
    TTL::Node<TTL::Node<int>> iNodeNode;

    // Ensure that various data values are default-initialized.
    UTL_TEST_ASSERT(iNode.GetData( ) == 0);
    UTL_TEST_ASSERT(ullNode.GetData( ) == 0ull);
    UTL_TEST_ASSERT(dNode.GetData( ) == 0.0);
    UTL_TEST_ASSERT(ptrNode.GetData( ) == nullptr);
    UTL_TEST_ASSERT(iNodeNode.GetData( ).GetData( ) == 0 && iNodeNode.GetData( ).GetNext( ) == nullptr);

    // Ensure that next ptr values are nullptr-initialized.
    UTL_TEST_ASSERT(iNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(ullNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(dNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(ptrNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(iNodeNode.GetNext( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Node::MoveConstructor( )
{
    const int n = 0x0FEEDBAC;

    TTL::Node<int>* pNext = nullptr;

    TTL::Node<int> iNode;

    try
    {
        pNext = new TTL::Node<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    iNode.SetData(n);
    iNode.SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(iNode.GetData( ) == n);
    UTL_SETUP_ASSERT(iNode.GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(iNode.GetNext( ) == pNext);

    TTL::Node<int> moveNode(std::move(iNode));

    // Ensure values were transferred.
    UTL_TEST_ASSERT(iNode.GetData( ) == 0);
    UTL_TEST_ASSERT(iNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(moveNode.GetData( ) == n);
    UTL_TEST_ASSERT(moveNode.GetNext( ) == pNext);

    // Cleanup
    delete pNext;
    pNext = nullptr;

    UTL_CLEANUP_ASSERT(pNext == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Move Assignment Test \\\

UnitTestResult TTLTests::Node::MoveAssignment( )
{
    const int n = 0x0FEEDBAC;

    TTL::Node<int>* pNext = nullptr;

    TTL::Node<int> iNode;
    TTL::Node<int> moveNode;

    try
    {
        pNext = new TTL::Node<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    iNode.SetData(n);
    iNode.SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(iNode.GetData( ) == n);
    UTL_SETUP_ASSERT(iNode.GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(iNode.GetNext( ) == pNext);

    moveNode = std::move(iNode);

    // Ensure values were transferred.
    UTL_TEST_ASSERT(iNode.GetData( ) == 0);
    UTL_TEST_ASSERT(iNode.GetNext( ) == nullptr);
    UTL_TEST_ASSERT(moveNode.GetData( ) == n);
    UTL_TEST_ASSERT(moveNode.GetNext( ) == pNext);

    // Cleanup
    delete pNext;
    pNext = nullptr;

    UTL_CLEANUP_ASSERT(pNext == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Setter Tests \\\

UnitTestResult TTLTests::Node::SetDataCopy( )
{
    TTL::Node<MemoryManagementHelper>* testNode = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(testNode == nullptr);

    try
    {
        MemoryManagementHelper* ptr = new MemoryManagementHelper( );
        testNode = new TTL::Node<MemoryManagementHelper>( );
        testNode->SetData(*ptr);
        delete ptr;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(testNode != nullptr);

    // Ensure copy was used.
    UTL_TEST_ASSERT(testNode->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(testNode->GetData( ).GetMove( ) == false);

    delete testNode;
    testNode = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    

    UTL_CLEANUP_ASSERT(testNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Node::SetDataMove( )
{
    TTL::Node<MemoryManagementHelper>* testNode = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(testNode == nullptr);

    try
    {
        MemoryManagementHelper* ptr = new MemoryManagementHelper( );
        testNode = new TTL::Node<MemoryManagementHelper>( );
        testNode->SetData(std::move(*ptr));
        delete ptr;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(testNode != nullptr);

    // Ensure copy was used.
    UTL_TEST_ASSERT(testNode->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(testNode->GetData( ).GetMove( ) == true);

    delete testNode;
    testNode = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    

    UTL_CLEANUP_ASSERT(testNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Node::SetNextRaw( )
{
    TTL::Node<int> testNode;
    TTL::Node<int>* pNode = nullptr;

    try
    {
        pNode = new TTL::Node<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(testNode.GetNext( ) == nullptr);
    UTL_SETUP_ASSERT(pNode != nullptr);

    testNode.SetNext(pNode);

    // Ensure value was copied.
    UTL_TEST_ASSERT(testNode.GetNext( ) != nullptr);
    UTL_TEST_ASSERT(testNode.GetNext( ) == pNode);

    // Cleanup
    delete pNode;
    pNode = nullptr;

    UTL_CLEANUP_ASSERT(pNode == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}