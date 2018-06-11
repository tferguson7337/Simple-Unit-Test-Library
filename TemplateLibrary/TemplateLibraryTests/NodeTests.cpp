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
    TTL::Node<MemoryManagementHelper>* pNext = nullptr;
    TTL::Node<MemoryManagementHelper>* pNode = nullptr;
    TTL::Node<MemoryManagementHelper>* pMoveNode = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pNode = new TTL::Node<MemoryManagementHelper>;
        pNext = new TTL::Node<MemoryManagementHelper>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    pNode->SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(pNode->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(pNode->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(pNode->GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(pNode->GetNext( ) == pNext);

    try
    {
        pMoveNode = new TTL::Node<MemoryManagementHelper>(std::move(*pNode));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Ensure values were transferred.
    UTL_TEST_ASSERT(pNode->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMoveNode->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pMoveNode->GetData( ).GetMove( ) == true);
    UTL_TEST_ASSERT(pMoveNode->GetNext( ) == pNext);

    // Cleanup
    delete pNode;
    pNode = nullptr;
    delete pNext;
    pNext = nullptr;
    delete pMoveNode;
    pMoveNode = nullptr;

    UTL_CLEANUP_ASSERT(pNode == nullptr);
    UTL_CLEANUP_ASSERT(pNext == nullptr);
    UTL_CLEANUP_ASSERT(pMoveNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 3);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Move Assignment Test \\\

UnitTestResult TTLTests::Node::MoveAssignment( )
{
    TTL::Node<MemoryManagementHelper>* pNext = nullptr;
    TTL::Node<MemoryManagementHelper>* pNode = nullptr;
    TTL::Node<MemoryManagementHelper>* pMoveNode = nullptr;

    bool threw = false;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pNext = new TTL::Node<MemoryManagementHelper>;
        pNode = new TTL::Node<MemoryManagementHelper>;
        pMoveNode = new TTL::Node<MemoryManagementHelper>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    pNode->SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(pNode->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(pNode->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(pNode->GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(pNode->GetNext( ) == pNext);

    try
    {
        *pNode = std::move(*pNode);
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw == true);

    *pMoveNode = std::move(*pNode);

    // Ensure values were transferred.
    UTL_TEST_ASSERT(pNode->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMoveNode->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pMoveNode->GetData( ).GetMove( ) == true);
    UTL_TEST_ASSERT(pMoveNode->GetNext( ) == pNext);

    // Cleanup
    delete pNode;
    pNode = nullptr;
    delete pNext;
    pNext = nullptr;
    delete pMoveNode;
    pMoveNode = nullptr;

    UTL_CLEANUP_ASSERT(pNode == nullptr);
    UTL_CLEANUP_ASSERT(pNext == nullptr);
    UTL_CLEANUP_ASSERT(pMoveNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 3);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Setter Tests \\\

UnitTestResult TTLTests::Node::SetDataCopy( )
{
    TTL::Node<MemoryManagementHelper>* pNode = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        MemoryManagementHelper copyHelper;
        pNode = new TTL::Node<MemoryManagementHelper>( );
        pNode->SetData(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(pNode != nullptr);

    // Ensure copy was used.
    UTL_TEST_ASSERT(pNode->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(pNode->GetData( ).GetMove( ) == false);

    delete pNode;
    pNode = nullptr;

    UTL_CLEANUP_ASSERT(pNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Node::SetDataMove( )
{
    TTL::Node<MemoryManagementHelper>* pNode = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        MemoryManagementHelper copyHelper;
        pNode = new TTL::Node<MemoryManagementHelper>( );
        pNode->SetData(std::move(copyHelper));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(pNode != nullptr);

    // Ensure copy was used.
    UTL_TEST_ASSERT(pNode->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pNode->GetData( ).GetMove( ) == true);

    delete pNode;
    pNode = nullptr;

    UTL_CLEANUP_ASSERT(pNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Node::SetNextRaw( )
{
    TTL::Node<int> node;
    TTL::Node<int>* pNext = nullptr;

    try
    {
        pNext = new TTL::Node<int>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(node.GetNext( ) == nullptr);
    UTL_SETUP_ASSERT(pNext != nullptr);

    node.SetNext(pNext);

    // Ensure value was copied.
    UTL_TEST_ASSERT(node.GetNext( ) != nullptr);
    UTL_TEST_ASSERT(node.GetNext( ) == pNext);

    // Cleanup
    delete pNext;
    pNext = nullptr;

    UTL_CLEANUP_ASSERT(pNext == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}