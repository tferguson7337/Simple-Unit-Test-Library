#include "DNodeTests.h"
#include "MemoryManagementHelper.hpp"

#include <DNode.hpp>


std::list<std::function<UnitTestResult(void)>> TTLTests::DNode::BuildTestList( )
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
    UTL_TEST_ASSERT(iNodeNode.GetData( ).GetData( ) == 0);
    UTL_TEST_ASSERT(iNodeNode.GetData( ).GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(iNodeNode.GetData( ).GetNext( ) == nullptr);

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
    TTL::DNode<MemoryManagementHelper>* pPrev = nullptr;
    TTL::DNode<MemoryManagementHelper>* pNext = nullptr;
    TTL::DNode<MemoryManagementHelper>* pNode = nullptr;
    TTL::DNode<MemoryManagementHelper>* pMoveNode = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pPrev = new TTL::DNode<MemoryManagementHelper>;
        pNext = new TTL::DNode<MemoryManagementHelper>;
        pNode = new TTL::DNode<MemoryManagementHelper>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    pNode->SetPrev(pPrev);
    pNode->SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(pNode->GetPrev( ) != nullptr);
    UTL_SETUP_ASSERT(pNode->GetPrev( ) == pPrev);
    UTL_SETUP_ASSERT(pNode->GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(pNode->GetNext( ) == pNext);

    try
    {
        pMoveNode = new TTL::DNode<MemoryManagementHelper>(std::move(*pNode));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Ensure values were transferred.
    UTL_TEST_ASSERT(pNode->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(pNode->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMoveNode->GetPrev( ) == pPrev);
    UTL_TEST_ASSERT(pMoveNode->GetNext( ) == pNext);

    // Cleanup
    delete pPrev;
    pPrev = nullptr;
    delete pNext;
    pNext = nullptr;
    delete pNode;
    pNode = nullptr;
    delete pMoveNode;
    pMoveNode = nullptr;

    UTL_CLEANUP_ASSERT(pPrev == nullptr);
    UTL_CLEANUP_ASSERT(pNext == nullptr);
    UTL_CLEANUP_ASSERT(pNode == nullptr);
    UTL_CLEANUP_ASSERT(pMoveNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 4);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Move Assignment Test \\\

UnitTestResult TTLTests::DNode::MoveAssignment( )
{
    TTL::DNode<MemoryManagementHelper>* pPrev = nullptr;
    TTL::DNode<MemoryManagementHelper>* pNext = nullptr;
    TTL::DNode<MemoryManagementHelper>* pNode = nullptr;
    TTL::DNode<MemoryManagementHelper>* pMoveNode = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pPrev = new TTL::DNode<MemoryManagementHelper>;
        pNext = new TTL::DNode<MemoryManagementHelper>;
        pNode = new TTL::DNode<MemoryManagementHelper>;
        pMoveNode = new TTL::DNode<MemoryManagementHelper>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    pNode->SetPrev(pPrev);
    pNode->SetNext(pNext);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(pNode->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(pNode->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(pNode->GetPrev( ) != nullptr);
    UTL_SETUP_ASSERT(pNode->GetPrev( ) == pPrev);
    UTL_SETUP_ASSERT(pNode->GetNext( ) != nullptr);
    UTL_SETUP_ASSERT(pNode->GetNext( ) == pNext);

    *pMoveNode = std::move(*pNode);

    // Ensure values were transferred.
    UTL_TEST_ASSERT(pMoveNode->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pMoveNode->GetData( ).GetMove( ) == true);
    UTL_TEST_ASSERT(pNode->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(pNode->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMoveNode->GetPrev( ) == pPrev);
    UTL_TEST_ASSERT(pMoveNode->GetNext( ) == pNext);

    // Cleanup
    delete pPrev;
    pPrev = nullptr;
    delete pNext;
    pNext = nullptr;
    delete pNode;
    pNode = nullptr;
    delete pMoveNode;
    pMoveNode = nullptr;

    UTL_CLEANUP_ASSERT(pPrev == nullptr);
    UTL_CLEANUP_ASSERT(pNext == nullptr);
    UTL_CLEANUP_ASSERT(pNode == nullptr);
    UTL_CLEANUP_ASSERT(pMoveNode == nullptr);
    UTL_CLEANUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 4);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}



UnitTestResult TTLTests::DNode::SetDataCopy( )
{
    TTL::DNode<MemoryManagementHelper>* pNode = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        MemoryManagementHelper copyHelper;
        pNode = new TTL::DNode<MemoryManagementHelper>( );
        pNode->SetData(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

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

UnitTestResult TTLTests::DNode::SetDataMove( )
{
    TTL::DNode<MemoryManagementHelper>* pNode = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pNode = new TTL::DNode<MemoryManagementHelper>( );
        pNode->SetData(MemoryManagementHelper( ));
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

UnitTestResult TTLTests::DNode::SetPrevRaw( )
{
    TTL::DNode<int> node;
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
    UTL_SETUP_ASSERT(node.GetPrev( ) == nullptr);
    UTL_SETUP_ASSERT(pPrev != nullptr);

    node.SetPrev(pPrev);

    // Ensure value was copied.
    UTL_TEST_ASSERT(node.GetPrev( ) != nullptr);
    UTL_TEST_ASSERT(node.GetPrev( ) == pPrev);

    // Cleanup
    delete pPrev;
    pPrev = nullptr;

    UTL_CLEANUP_ASSERT(pPrev == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DNode::SetNextRaw( )
{
    TTL::DNode<int> node;
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