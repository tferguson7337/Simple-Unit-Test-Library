#include "ListTests.h"

#include "List.hpp"

#include "MemoryManagementHelper.hpp"

std::list<std::function<UnitTestResult(void)>> TTLTests::List::BuildTestList( )
{
    static const std::list<std::function<UnitTestResult(void)>> testList =
    {
        DefaultConstructor,
        CopyConstructor,
        MoveConstructor,
        CopyAssignment,
        MoveAssignment,
        CopyAppendAssignmentData,
        MoveAppendAssignmentData,
        CopyAppendAssignmentList,
        MoveAppendAssignmentList,
        AppendDataNodeLinkage,
        PrependDataNodeLinkage,
        Clear,
        PopFront,
        PopBack,
        Remove,
        CopyAppendData,
        MoveAppendData,
        CopyAppendList,
        MoveAppendList,
        CopyPrependData,
        MovePrependData,
        CopyPrependList,
        MovePrependList,
        CopyInsertData,
        MoveInsertData,
        CopyInsertList,
        MoveInsertList
    };

    return testList;
}

// Constructor Tests

UnitTestResult TTLTests::List::DefaultConstructor( )
{
    TTL::List<size_t> emptyList;

    UTL_TEST_ASSERT(emptyList.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(emptyList.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(emptyList.Size( ) == 0);
    UTL_TEST_ASSERT(emptyList.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyConstructor( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);
    UTL_SETUP_ASSERT(pList == nullptr);

    try
    {
        pList = new TTL::List<MemoryManagementHelper>( );
        pList->Append(MemoryManagementHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Reset appended MemoryManagementHelper() to test later.
    pList->Front( ).Reset( );

    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(pList->Size( ) == 1);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);

    TTL::List<MemoryManagementHelper>* pCopyList = nullptr;

    try
    {
        pCopyList = new TTL::List<MemoryManagementHelper>(*pList);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure pList wasn't modified after copy.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);

    // Ensure pList was properly copied.
    UTL_TEST_ASSERT(pCopyList != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetHead( ) == pCopyList->GetTail( ));
    UTL_TEST_ASSERT(pCopyList->GetHead( ) != pList->GetHead( ));
    UTL_TEST_ASSERT(pCopyList->GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(pCopyList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(pCopyList->Size( ) == 1);
    UTL_TEST_ASSERT(pCopyList->Size( ) == pList->Size( ));
    UTL_TEST_ASSERT(pCopyList->Empty( ) == false);

    delete pList;
    delete pCopyList;
    pList = nullptr;
    pCopyList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 3);

    UTL_CLEANUP_ASSERT(pList == nullptr);
    UTL_CLEANUP_ASSERT(pCopyList == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveConstructor( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;
    const TTL::DNode<MemoryManagementHelper>* pListHead = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);
    UTL_SETUP_ASSERT(pList == nullptr);
    UTL_SETUP_ASSERT(pListHead == nullptr);

    try
    {
        pList = new TTL::List<MemoryManagementHelper>( );
        pList->Append(MemoryManagementHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Reset appended MemoryManagementHelper() to test later.
    pList->Front( ).Reset( );

    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(pList->Size( ) == 1);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);

    pListHead = pList->GetHead( );

    TTL::List<MemoryManagementHelper>* pMoveList = nullptr;

    try
    {
        pMoveList = new TTL::List<MemoryManagementHelper>(std::move(*pList));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Ensure list was cleared out after move.
    UTL_TEST_ASSERT(pList->GetHead( ) == nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) == nullptr);
    UTL_TEST_ASSERT(pList->Size( ) == 0);
    UTL_TEST_ASSERT(pList->Empty( ) == true);

    // Ensure list was moved.
    UTL_TEST_ASSERT(pMoveList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pMoveList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pMoveList->GetHead( ) == pMoveList->GetTail( ));
    UTL_TEST_ASSERT(pMoveList->Size( ) == 1);
    UTL_TEST_ASSERT(pMoveList->Empty( ) == false);

    // Ensure list move is efficient (just moving pointers).
    UTL_TEST_ASSERT(pMoveList->GetHead( ) == pListHead);
    UTL_TEST_ASSERT(pMoveList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pMoveList->GetHead( )->GetData( ).GetMove( ) == false);

    delete pList;
    delete pMoveList;
    pList = nullptr;
    pMoveList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);

    UTL_CLEANUP_ASSERT(pList == nullptr);
    UTL_CLEANUP_ASSERT(pMoveList == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Assignment Tests

UnitTestResult TTLTests::List::CopyAssignment( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;
    TTL::List<MemoryManagementHelper>* pCopyList = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pList->Append(MemoryManagementHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(pList);

    // Reset appended MemoryManagementHelper() to test later.
    pList->Front( ).Reset( );

    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(pList->Size( ) == 1);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);

    try
    {
        pCopyList = new TTL::List<MemoryManagementHelper>;
        *pCopyList = *pList;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure list wasn't modified after copy.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);

    // Ensure list was properly copied.
    UTL_TEST_ASSERT(pCopyList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetHead( ) == pCopyList->GetTail( ));
    UTL_TEST_ASSERT(pCopyList->GetHead( ) != pList->GetHead( ));
    UTL_TEST_ASSERT(pCopyList->GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(pCopyList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(pCopyList->Size( ) == 1);
    UTL_TEST_ASSERT(pCopyList->Size( ) == pList->Size( ));
    UTL_TEST_ASSERT(pCopyList->Empty( ) == false);

    delete pList;
    delete pCopyList;
    pList = nullptr;
    pCopyList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 3);
    UTL_CLEANUP_ASSERT(!pList);
    UTL_CLEANUP_ASSERT(!pCopyList);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveAssignment( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;
    TTL::List<MemoryManagementHelper>* pMoveList = nullptr;
    const TTL::DNode<MemoryManagementHelper>* pListHead = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pList->Append(MemoryManagementHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(pList);

    // Reset appended MemoryManagementHelper() to test later.
    pList->Front( ).Reset( );

    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(pList->Size( ) == 1);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);

    pListHead = pList->GetHead( );

    try
    {
        pMoveList = new TTL::List<MemoryManagementHelper>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(pMoveList);
    *pMoveList = std::move(*pList);

    // Ensure pList->was cleared out after move.
    UTL_TEST_ASSERT(pList->GetHead( ) == nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) == nullptr);
    UTL_TEST_ASSERT(pList->Size( ) == 0);
    UTL_TEST_ASSERT(pList->Empty( ) == true);

    // Ensure list was moved.
    UTL_TEST_ASSERT(pMoveList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pMoveList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pMoveList->GetHead( ) == pMoveList->GetTail( ));
    UTL_TEST_ASSERT(pMoveList->Size( ) == 1);
    UTL_TEST_ASSERT(pMoveList->Empty( ) == false);

    // Ensure list move is efficient (just moving pointers).
    UTL_TEST_ASSERT(pMoveList->GetHead( ) == pListHead);
    UTL_TEST_ASSERT(pMoveList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pMoveList->GetHead( )->GetData( ).GetMove( ) == false);

    delete pList;
    delete pMoveList;
    pList = nullptr;
    pMoveList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    UTL_CLEANUP_ASSERT(!pList);
    UTL_CLEANUP_ASSERT(!pMoveList);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Append Assignment Tests

UnitTestResult TTLTests::List::CopyAppendAssignmentData( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        MemoryManagementHelper copyHelper;
        pList = new TTL::List<MemoryManagementHelper>;
        *pList += copyHelper;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);

    delete pList;
    pList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    UTL_CLEANUP_ASSERT(!pList);

    const size_t LIST_SIZE = 64;
    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( size_t n = 0; n < LIST_SIZE; n++ )
        {
            numList += n;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 2 - Ensure Data Ordering.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetHead( ) != numList.GetTail( ));
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(numList.Empty( ) == false);

    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = 0;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == LIST_SIZE - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveAppendAssignmentData( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        *pList += std::move(MemoryManagementHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == true);

    delete pList;
    pList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    UTL_CLEANUP_ASSERT(pList == nullptr);

    const size_t LIST_SIZE = 64;
    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( size_t n = 0; n < LIST_SIZE; n++ )
        {
            numList += n;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 2 - Ensure Data Ordering.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetHead( ) != numList.GetTail( ));
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(numList.Empty( ) == false);

    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = 0;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == LIST_SIZE - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyAppendAssignmentList( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;
    TTL::List<size_t> copyList;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(copyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.Size( ) == 0);
    UTL_SETUP_ASSERT(copyList.Empty( ) == true);

    try
    {
        // Append [0, 15] to copyList
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            copyList.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(copyList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(copyList.Empty( ) == false);

    try
    {
        // Append [16, 31] to list
        for ( size_t i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    try
    {
        copyList += list;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure size/empty results make sense.
    UTL_TEST_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(copyList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(copyList.Empty( ) == false);

    // Ensure list wasn't modified during copy.
    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = LIST_SIZE;
    for ( ptr = list.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == list.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    // Ensure the copy was performed correctly.
    ptr = copyList.GetHead( );
    counter = 0;
    for ( ptr = copyList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == copyList.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveAppendAssignmentList( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;
    TTL::List<size_t> moveList;

    const TTL::DNode<size_t>* listHead = nullptr;
    const TTL::DNode<size_t>* listTail = nullptr;
    const TTL::DNode<size_t>* moveTail = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(moveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.Size( ) == 0);
    UTL_SETUP_ASSERT(moveList.Empty( ) == true);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            moveList.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(moveList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(moveList.Empty( ) == false);

    try
    {
        for ( size_t i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    listHead = list.GetHead( );
    listTail = list.GetTail( );
    moveTail = moveList.GetTail( );
    moveList += std::move(list);

    // Ensure size/empty results make sense.
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(moveList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == true);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure move cleared out list pointers.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);

    // Ensure the move was performed correctly.
    UTL_TEST_ASSERT(moveTail->GetNext( ) == listHead);
    UTL_TEST_ASSERT(moveList.GetTail( ) == listTail);

    const TTL::DNode<size_t>* ptr = moveList.GetHead( );
    size_t counter = 0;
    for ( ptr = moveList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == moveList.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Node Linkage Test
UnitTestResult TTLTests::List::AppendDataNodeLinkage( )
{
    const size_t LIST_SIZE = 64;
    TTL::List<size_t> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    // Create first node.
    try
    {
        list.Append(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 1);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == nullptr);

    // Create second node.
    try
    {
        list.Append(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 2);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);

    const TTL::DNode<size_t>* pMid = list.GetTail( );

    // Create third node.
    try
    {
        list.Append(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 3);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMid->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(pMid->GetNext( ) == list.GetTail( ));

    // Test LIST_SIZE linkage.
    const TTL::DNode<size_t>* nodeArr[LIST_SIZE] = { nullptr };
    nodeArr[0] = list.GetHead( );
    nodeArr[1] = pMid;
    nodeArr[2] = list.GetTail( );

    // Fill up the rest of the list, save node addr's as we go.
    try
    {
        while ( list.Size( ) < LIST_SIZE )
        {
            list.Append(0);
            nodeArr[list.Size( ) - 1] = list.GetTail( );
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Test edge cases (list head and tail).

    /// Code Analysis incorrectly thinks that nodeArr[0] == nullptr here.
    /// This is based on the above loop (while ( list.Size( ) < LIST_SIZE )
    /// being skipped - this will not happen.
#pragma warning (push)
#pragma warning (disable : 6011)
    UTL_TEST_ASSERT(nodeArr[0]->GetPrev( ) == nullptr);
#pragma warning (pop)

    UTL_TEST_ASSERT(nodeArr[0] == list.GetHead( ));
    UTL_TEST_ASSERT(nodeArr[0]->GetNext( ) == nodeArr[1]);
    UTL_TEST_ASSERT(nodeArr[LIST_SIZE - 1]->GetPrev( ) == nodeArr[LIST_SIZE - 2]);
    UTL_TEST_ASSERT(nodeArr[LIST_SIZE - 1] == list.GetTail( ));
    UTL_TEST_ASSERT(nodeArr[LIST_SIZE - 1]->GetNext( ) == nullptr);

    // Test links in the rest of the list.
    for ( size_t i = 1; i < LIST_SIZE - 1; i++ )
    {
        UTL_TEST_ASSERT(nodeArr[i]->GetPrev( ) == nodeArr[i - 1]);
        UTL_TEST_ASSERT(nodeArr[i]->GetNext( ) == nodeArr[i + 1]);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::PrependDataNodeLinkage( )
{
    const size_t LIST_SIZE = 64;
    TTL::List<size_t> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    // Create first node.
    try
    {
        list.Prepend(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 1);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == nullptr);

    // Create second node.
    try
    {
        list.Prepend(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 2);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);

    const TTL::DNode<size_t>* pMid = list.GetHead( );

    // Create third node.
    try
    {
        list.Prepend(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 3);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMid->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(pMid->GetNext( ) == list.GetTail( ));

    // Test LIST_SIZE linkage.
    const TTL::DNode<size_t>* nodeArr[LIST_SIZE] = { nullptr };
    nodeArr[LIST_SIZE - 1] = list.GetTail( );
    nodeArr[LIST_SIZE - 2] = pMid;
    nodeArr[LIST_SIZE - 3] = list.GetHead( );

    // Fill up the rest of the list, save node addr's as we go.
    try
    {
        while ( list.Size( ) < LIST_SIZE )
        {
            list.Prepend(0);
            nodeArr[LIST_SIZE - list.Size( )] = list.GetHead( );
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Test edge cases (list head and tail).

    /// Code Analysis incorrectly thinks that nodeArr[0] == nullptr here.
    /// This is based on the above loop (while ( list.Size( ) < LIST_SIZE )
    /// being skipped - this will not happen.
#pragma warning (push)
#pragma warning (disable : 6011)
    UTL_TEST_ASSERT(nodeArr[0]->GetPrev( ) == nullptr);
#pragma warning (pop)

    UTL_TEST_ASSERT(nodeArr[0] == list.GetHead( ));
    UTL_TEST_ASSERT(nodeArr[0]->GetNext( ) == nodeArr[1]);
    UTL_TEST_ASSERT(nodeArr[LIST_SIZE - 1]->GetPrev( ) == nodeArr[LIST_SIZE - 2]);
    UTL_TEST_ASSERT(nodeArr[LIST_SIZE - 1] == list.GetTail( ));
    UTL_TEST_ASSERT(nodeArr[LIST_SIZE - 1]->GetNext( ) == nullptr);

    // Test links in the rest of the list.
    for ( size_t i = 1; i < LIST_SIZE - 1; i++ )
    {
        UTL_TEST_ASSERT(nodeArr[i]->GetPrev( ) == nodeArr[i - 1]);
        UTL_TEST_ASSERT(nodeArr[i]->GetNext( ) == nodeArr[i + 1]);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Public Method Tests

UnitTestResult TTLTests::List::Clear( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        while ( list.Size( ) < LIST_SIZE )
        {
            list.Append(0);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    list.Clear( );

    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::PopFront( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    size_t i = 0;
    try
    {
        while ( !list.Empty( ) )
        {
            UTL_TEST_ASSERT(list.Front( ) == i++);
            list.PopFront( );
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(i == LIST_SIZE);
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::PopBack( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    size_t i = LIST_SIZE - 1;
    try
    {
        while ( !list.Empty( ) )
        {
            UTL_TEST_ASSERT(list.Back( ) == i--);
            list.PopBack( );
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(i == (size_t)-1);
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::Remove( )
{
    const size_t LIST_SIZE = 16;
    const size_t REMOVE_POS = LIST_SIZE >> 1;

    bool removeThrew = false;

    size_t removeCount = 0;
    TTL::List<size_t> list;

    // Confirm Initial Test Conditions - Pt 1
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    // Try to remove from an empty list.
    try
    {
        list.Remove(0);
    }
    catch ( const std::out_of_range& )
    {
        removeThrew = true;
    }

    UTL_TEST_ASSERT(removeThrew == true);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    // Try to remove past non-empty list boundaries.
    removeThrew = false;
    try
    {
        list.Remove(list.Size( ));
    }
    catch ( const std::out_of_range& )
    {
        removeThrew = true;
    }

    UTL_TEST_ASSERT(removeThrew == true);

    // Test remove from front (should be same as Pop_Front( ))
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ) == 0);

    try
    {
        list.Remove(0);
        removeCount++;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( )->GetData( ) == 1);
    UTL_TEST_ASSERT(list.Size( ) == LIST_SIZE - removeCount);

    // Test remove from back (should be same as Pop_Back( ))
    UTL_TEST_ASSERT(list.GetTail( )->GetData( ) == LIST_SIZE - 1);

    try
    {
        list.Remove(list.Size( ) - 1);
        removeCount++;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetTail( )->GetData( ) == LIST_SIZE - 2);
    UTL_TEST_ASSERT(list.Size( ) == LIST_SIZE - removeCount);

    // Test remove from middle.
    {
        const TTL::DNode<size_t>* pRemove = list.GetHead( );
        const TTL::DNode<size_t>* pRemovePrev = nullptr;
        const TTL::DNode<size_t>* pRemoveNext = nullptr;
        const size_t expectedVal = REMOVE_POS + 1;

        for ( size_t pos = 0; pos < REMOVE_POS; pos++ )
        {
            pRemove = pRemove->GetNext( );
        }

        UTL_TEST_ASSERT(pRemove != nullptr);
        UTL_TEST_ASSERT(pRemove->GetData( ) == expectedVal);

        pRemovePrev = pRemove->GetPrev( );
        pRemoveNext = pRemove->GetNext( );

        UTL_TEST_ASSERT(pRemovePrev != nullptr);
        UTL_TEST_ASSERT(pRemovePrev->GetData( ) == expectedVal - 1);

        UTL_TEST_ASSERT(pRemoveNext != nullptr);
        UTL_TEST_ASSERT(pRemoveNext->GetData( ) == expectedVal + 1);

        try
        {
            list.Remove(REMOVE_POS);
            removeCount++;
        }
        catch ( const std::exception& e )
        {
            UTL_TEST_EXCEPTION(e.what( ));
        }

        UTL_TEST_ASSERT(list.Size( ) == LIST_SIZE - removeCount);
        UTL_TEST_ASSERT(pRemovePrev->GetNext( ) == pRemoveNext);
        UTL_TEST_ASSERT(pRemoveNext->GetPrev( ) == pRemovePrev);
    }

    list.Clear( );

    // Confirm Initial Test Conditions - Pt 2
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    // Test remove from front until one left.
    try
    {
        while ( list.Size( ) > 1 )
        {
            list.Remove(0);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure single-element list case is valid.
    UTL_TEST_ASSERT(list.Size( ) == 1);
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ) == LIST_SIZE - 1);

    try
    {
        list.Remove(0);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure list is empty after final remove.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    removeThrew = false;
    try
    {
        list.Remove(0);
    }
    catch ( const std::out_of_range& )
    {
        removeThrew = true;
    }

    UTL_TEST_ASSERT(removeThrew == true);

    // Confirm Initial Test Conditions - Pt 3
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    // Test remove from back until one left.
    try
    {
        while ( list.Size( ) > 1 )
        {
            list.Remove(list.Size( ) - 1);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure single-element list case is valid.
    UTL_TEST_ASSERT(list.Size( ) == 1);
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ) == 0);

    try
    {
        list.Remove(list.Size( ) - 1);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure list is empty after final remove.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    removeThrew = false;
    try
    {
        list.Remove(list.Size( ) - 1);
    }
    catch ( const std::out_of_range& )
    {
        removeThrew = true;
    }

    UTL_TEST_ASSERT(removeThrew == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::List::CopyAppendData( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        MemoryManagementHelper copyHelper;
        pList = new TTL::List<MemoryManagementHelper>;
        pList->Append(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);

    delete pList;
    pList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    UTL_CLEANUP_ASSERT(pList == nullptr);

    const size_t LIST_SIZE = 64;
    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( size_t n = 0; n < LIST_SIZE; n++ )
        {
            numList.Append(n);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 2 - Ensure Data Ordering.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetHead( ) != numList.GetTail( ));
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(numList.Empty( ) == false);

    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = 0;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == LIST_SIZE - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveAppendData( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pList->Append(MemoryManagementHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == true);

    delete pList;
    pList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    UTL_CLEANUP_ASSERT(pList == nullptr);

    const size_t LIST_SIZE = 64;
    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( size_t n = 0; n < LIST_SIZE; n++ )
        {
            numList.Append(n);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 2 - Ensure Data Ordering.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetHead( ) != numList.GetTail( ));
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(numList.Empty( ) == false);

    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = 0;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == LIST_SIZE - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::List::CopyAppendList( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;
    TTL::List<size_t> copyList;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(copyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.Size( ) == 0);
    UTL_SETUP_ASSERT(copyList.Empty( ) == true);

    try
    {
        // Append [0, LIST_SIZE - 1] to copyList
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            copyList.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(copyList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(copyList.Empty( ) == false);

    try
    {
        // Append [LIST_SIZE, (LIST_SIZE << 1) - 1] to list
        for ( size_t i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    try
    {
        copyList.Append(list);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure size/empty results make sense.
    UTL_TEST_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(copyList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(copyList.Empty( ) == false);

    // Ensure list wasn't modified during copy.
    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = LIST_SIZE;
    for ( ptr = list.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == list.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    // Ensure the copy was performed correctly.
    ptr = copyList.GetHead( );
    counter = 0;
    for ( ptr = copyList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == copyList.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveAppendList( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;
    TTL::List<size_t> moveList;

    const TTL::DNode<size_t>* listHead = nullptr;
    const TTL::DNode<size_t>* listTail = nullptr;
    const TTL::DNode<size_t>* moveTail = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(moveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.Size( ) == 0);
    UTL_SETUP_ASSERT(moveList.Empty( ) == true);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            moveList.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(moveList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(moveList.Empty( ) == false);

    try
    {
        for ( size_t i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    listHead = list.GetHead( );
    listTail = list.GetTail( );
    moveTail = moveList.GetTail( );
    moveList.Append(std::move(list));

    // Ensure size/empty results make sense.
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(moveList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == true);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure move cleared out list pointers.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);

    // Ensure the move was performed correctly.
    UTL_TEST_ASSERT(moveTail->GetNext( ) == listHead);
    UTL_TEST_ASSERT(moveList.GetTail( ) == listTail);

    const TTL::DNode<size_t>* ptr = moveList.GetHead( );
    size_t counter = 0;
    for ( ptr = moveList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == moveList.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyPrependData( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        MemoryManagementHelper copyHelper;
        pList = new TTL::List<MemoryManagementHelper>;
        pList->Prepend(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == false);

    delete pList;
    pList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    UTL_CLEANUP_ASSERT(pList == nullptr);

    const size_t LIST_SIZE = 64;
    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( size_t n = 0; n < LIST_SIZE; n++ )
        {
            numList.Prepend(n);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 2 - Ensure Data Ordering.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetHead( ) != numList.GetTail( ));
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(numList.Empty( ) == false);

    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = LIST_SIZE - 1;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter--);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == 0);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MovePrependData( )
{
    TTL::List<MemoryManagementHelper>* pList = nullptr;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pList->Prepend(MemoryManagementHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Move is Performed.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetHead( ) == pList->GetTail( ));
    UTL_TEST_ASSERT(pList->Size( ) == 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(pList->GetHead( )->GetData( ).GetMove( ) == true);

    delete pList;
    pList = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 2);
    UTL_CLEANUP_ASSERT(pList == nullptr);

    const size_t LIST_SIZE = 64;
    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( size_t n = 0; n < LIST_SIZE; n++ )
        {
            numList.Prepend(n);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 2 - Ensure Data Ordering.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetHead( ) != numList.GetTail( ));
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(numList.Empty( ) == false);

    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = LIST_SIZE - 1;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter--);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == 0);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyPrependList( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;
    TTL::List<size_t> copyList;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(copyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.Size( ) == 0);
    UTL_SETUP_ASSERT(copyList.Empty( ) == true);

    try
    {
        // Append [LIST_SIZE, (LIST_SIZE << 1) - 1] into copyList
        for ( size_t i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
        {
            copyList.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(copyList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(copyList.Empty( ) == false);

    try
    {
        // Append [0, LIST_SIZE - 1] to list
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    try
    {
        copyList.Prepend(list);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure size/empty results make sense.
    UTL_TEST_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(copyList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(copyList.Empty( ) == false);

    // Ensure list wasn't modified during copy.
    const TTL::DNode<size_t>* ptr = nullptr;
    size_t counter = 0;
    for ( ptr = list.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == list.GetTail( ));
    UTL_TEST_ASSERT(counter == LIST_SIZE - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    // Ensure the copy was performed correctly.
    ptr = copyList.GetHead( );
    counter = 0;
    for ( ptr = copyList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == copyList.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MovePrependList( )
{
    const size_t LIST_SIZE = 16;
    TTL::List<size_t> list;
    TTL::List<size_t> moveList;

    const TTL::DNode<size_t>* listHead = nullptr;
    const TTL::DNode<size_t>* listTail = nullptr;
    const TTL::DNode<size_t>* moveHead = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(moveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.Size( ) == 0);
    UTL_SETUP_ASSERT(moveList.Empty( ) == true);

    try
    {
        for ( size_t i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
        {
            moveList.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(moveList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(moveList.Empty( ) == false);

    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(list.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    listHead = list.GetHead( );
    listTail = list.GetTail( );
    moveHead = moveList.GetHead( );
    moveList.Prepend(std::move(list));

    // Ensure size/empty results make sense.
    UTL_TEST_ASSERT(list.Size( ) == 0);
    UTL_TEST_ASSERT(moveList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == true);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure move cleared out list pointers.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);

    // Ensure the move was performed correctly.
    UTL_TEST_ASSERT(listHead == moveList.GetHead( ));
    UTL_TEST_ASSERT(listTail == moveHead->GetPrev( ));

    const TTL::DNode<size_t>* ptr = moveList.GetHead( );
    size_t counter = 0;
    for ( ptr = moveList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter++);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == moveList.GetTail( ));
    UTL_TEST_ASSERT(counter == (LIST_SIZE << 1) - 1);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyInsertData( )
{
    const size_t LIST_SIZE = 16;
    const size_t INSERT_POS = LIST_SIZE >> 1;
    const size_t INSERT_VAL = 0x1337;

    bool badInsertThrew = false;

    TTL::List<MemoryManagementHelper>* pList = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pMemHelper = new MemoryManagementHelper;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions - Pt 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);
    UTL_SETUP_ASSERT(pList);
    UTL_SETUP_ASSERT(pMemHelper);

    // Test out_of_range throw.
    try
    {
        pList->Insert(1, *pMemHelper);
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    UTL_TEST_ASSERT(badInsertThrew == true);

    // Populate the list.
    for ( size_t i = 0; i < LIST_SIZE; i++ )
    {
        try
        {
            pList->Insert(pList->Size( ), *pMemHelper);
        }
        catch ( const std::exception& e )
        {
            UTL_SETUP_EXCEPTION(e.what( ));
        }
    }

    // Ensure the list was populated.
    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);

    // Test out_of_range throw.
    badInsertThrew = false;
    try
    {
        pList->Insert(pList->Size( ) + 1, *pMemHelper);
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    UTL_TEST_ASSERT(badInsertThrew == true);

    {
        // Linear scan through list, checking Copy was performed for each element.
        const TTL::DNode<MemoryManagementHelper>* ptr = pList->GetHead( );
        size_t i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == true);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == false);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE);
    }

    delete pList;
    delete pMemHelper;
    pList = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == LIST_SIZE + 1);
    UTL_CLEANUP_ASSERT(!pList);
    UTL_CLEANUP_ASSERT(!pMemHelper);

    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Pt 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    // Populate the list.
    for ( size_t i = 0; i < LIST_SIZE; i++ )
    {
        try
        {
            numList.Insert(numList.Size( ), i);
        }
        catch ( const std::exception& e )
        {
            UTL_TEST_EXCEPTION(e.what( ));
        }
    }

    // Ensure the list was populated.
    UTL_SETUP_ASSERT(numList.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(numList.Empty( ) == false);

    // Insert a new value in the middle of the list.
    try
    {
        numList.Insert(INSERT_POS, INSERT_VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    {
        // Ensure data ordering is correct and check for mid-insert value.
        const TTL::DNode<size_t>* ptr = numList.GetHead( );
        size_t i = 0;
        while ( ptr )
        {
            UTL_TEST_ASSERT(ptr->GetData( ) == ((i == INSERT_POS) ? INSERT_VAL : (i >= INSERT_POS) ? i - 1 : i));
            ptr = ptr->GetNext( );
            i++;
        }

        UTL_TEST_ASSERT(i == LIST_SIZE + 1);
    }

    // Try front-insertion (should be equivalent to Prepend).
    try
    {
        numList.Insert(0, INSERT_VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(numList.GetHead( )->GetData( ) == INSERT_VAL);

    // Try back-insertion (should be equivalent to Append).
    try
    {
        numList.Insert(numList.Size( ), INSERT_VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(numList.GetTail( )->GetData( ) == INSERT_VAL);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveInsertData( )
{
    const size_t LIST_SIZE = 16;
    const size_t INSERT_POS = LIST_SIZE >> 1;
    const size_t INSERT_VAL = 0x1337;

    bool badInsertThrew = false;

    TTL::List<MemoryManagementHelper>* pList = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pMemHelper = new MemoryManagementHelper;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions - Pt 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);
    UTL_SETUP_ASSERT(pList);
    UTL_SETUP_ASSERT(pMemHelper);

    // Test out_of_range throw.
    try
    {
        pList->Insert(1, std::move(*pMemHelper));
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    // Populate the list.
    for ( size_t i = 0; i < LIST_SIZE; i++ )
    {
        try
        {
            pList->Insert(pList->Size( ), std::move(*pMemHelper));
        }
        catch ( const std::exception& e )
        {
            UTL_SETUP_EXCEPTION(e.what( ));
        }
    }

    // Ensure the list was populated.
    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);

    // Test out_of_range throw.
    badInsertThrew = false;
    try
    {
        pList->Insert(pList->Size( ) + 1, std::move(*pMemHelper));
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    UTL_TEST_ASSERT(badInsertThrew == true);

    {
        // Linear scan through list, checking Move was performed for each element.
        const TTL::DNode<MemoryManagementHelper>* ptr = pList->GetHead( );
        size_t i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == false);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == true);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE);
    }

    delete pList;
    delete pMemHelper;
    pList = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == LIST_SIZE + 1);
    UTL_CLEANUP_ASSERT(!pList);
    UTL_CLEANUP_ASSERT(!pMemHelper);

    TTL::List<size_t> numList;

    // Confirm Initial Test Conditions - Pt 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    // Populate the list.
    for ( size_t i = 0; i < LIST_SIZE; i++ )
    {
        try
        {
            numList.Insert(numList.Size( ), i);
        }
        catch ( const std::exception& e )
        {
            UTL_TEST_EXCEPTION(e.what( ));
        }
    }

    // Ensure the list was populated.
    UTL_SETUP_ASSERT(numList.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(numList.Empty( ) == false);

    // Insert a new value in the middle of the list.
    try
    {
        numList.Insert(INSERT_POS, INSERT_VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    {
        // Ensure data ordering is correct and check for mid-insert value.
        const TTL::DNode<size_t>* ptr = numList.GetHead( );
        size_t i = 0;
        while ( ptr )
        {
            UTL_TEST_ASSERT(ptr->GetData( ) == ((i == INSERT_POS) ? INSERT_VAL : (i >= INSERT_POS) ? i - 1 : i));
            ptr = ptr->GetNext( );
            i++;
        }

        UTL_TEST_ASSERT(i == LIST_SIZE + 1);
    }

    // Try front-insertion (should be equivalent to Prepend).
    try
    {
        numList.Insert(0, INSERT_VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(numList.GetHead( )->GetData( ) == INSERT_VAL);

    // Try back-insertion (should be equivalent to Append).
    try
    {
        numList.Insert(numList.Size( ), INSERT_VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(numList.GetTail( )->GetData( ) == INSERT_VAL);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyInsertList( )
{
    const size_t LIST_SIZE = 8;
    const size_t INSERT_POS = LIST_SIZE >> 1;

    bool badInsertThrew = false;

    TTL::List<MemoryManagementHelper>* pList = nullptr;
    TTL::List<MemoryManagementHelper>* pCopyList = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    // Confirm Initial Test Conditions - Pt 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    // Test out_of_range throw.
    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pCopyList = new TTL::List<MemoryManagementHelper>;
        pList->Insert(1, *pCopyList);
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    UTL_TEST_ASSERT(badInsertThrew == true);

    // Populate lists.
    try
    {
        pMemHelper = new MemoryManagementHelper;
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            pList->Insert(pList->Size( ), *pMemHelper);
            pCopyList->Insert(pCopyList->Size( ), *pMemHelper);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Ensure lists are populated.
    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);
    UTL_SETUP_ASSERT(pCopyList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pCopyList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pCopyList->Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(pCopyList->Empty( ) == false);

    // Test out_of_range throw.
    badInsertThrew = false;
    try
    {
        pList->Insert(pList->Size( ) + 1, *pCopyList);
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    UTL_TEST_ASSERT(badInsertThrew == true);

    // Ensure list insertions used copy.
    {
        const TTL::DNode<MemoryManagementHelper>* ptr = pList->GetHead( );

        size_t i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == true);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == false);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE);

        ptr = pCopyList->GetHead( );
        i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == true);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == false);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE);
    }

    // Insert copyList into the middle of list.
    try
    {
        pList->Insert(LIST_SIZE >> 1, *pCopyList);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure insertion modified list properly, but did not modify copyList.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pCopyList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(pCopyList->Empty( ) == false);

    // Ensure all values are still marked as copied.
    {
        const TTL::DNode<MemoryManagementHelper>* ptr = pList->GetHead( );

        size_t i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == true);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == false);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE << 1);

        ptr = pCopyList->GetHead( );
        i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == true);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == false);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE);
    }

    delete pList;
    delete pCopyList;
    delete pMemHelper;
    pList = nullptr;
    pCopyList = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 3 * LIST_SIZE + 1);
    UTL_CLEANUP_ASSERT(!pList);
    UTL_CLEANUP_ASSERT(!pCopyList);
    UTL_CLEANUP_ASSERT(!pMemHelper);

    TTL::List<size_t> numList;
    TTL::List<size_t> numCopyList;

    // Confirm Initial Test Conditions - Pt 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);
    UTL_SETUP_ASSERT(numCopyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numCopyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numCopyList.Size( ) == 0);
    UTL_SETUP_ASSERT(numCopyList.Empty( ) == true);

    // Populate lists.
    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            numList.Insert(numList.Size( ), i);
            numCopyList.Insert(numCopyList.Size( ), (LIST_SIZE)-i - 1);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Ensure lists are populated.
    UTL_SETUP_ASSERT(numList.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(numList.Empty( ) == false);
    UTL_SETUP_ASSERT(numCopyList.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(numCopyList.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(numCopyList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(numCopyList.Empty( ) == false);

    // Insert numCopyList into the middle of numList.
    try
    {
        numList.Insert(INSERT_POS, numCopyList);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure insertion modified numList properly, but did not modify numCopyList.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(numList.Empty( ) == false);
    UTL_TEST_ASSERT(numCopyList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numCopyList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numCopyList.Size( ) == LIST_SIZE);
    UTL_TEST_ASSERT(numCopyList.Empty( ) == false);

    // Ensure data ordering.
    {
        const TTL::DNode<size_t>* ptrNum = numList.GetHead( );
        const TTL::DNode<size_t>* ptrCopy = numCopyList.GetHead( );

        size_t numVal = 0;
        size_t pos = 0;
        while ( ptrNum && pos < (LIST_SIZE << 1) )
        {
            if ( pos == INSERT_POS )
            {
                size_t expectedVal = LIST_SIZE - 1;
                while ( ptrCopy )
                {
                    // Check for deep copy and proper data ordering.
                    UTL_TEST_ASSERT(ptrNum != ptrCopy);
                    UTL_TEST_ASSERT(ptrNum->GetData( ) == ptrCopy->GetData( ));
                    UTL_TEST_ASSERT(ptrNum->GetData( ) == expectedVal);

                    ptrNum = ptrNum->GetNext( );
                    ptrCopy = ptrCopy->GetNext( );

                    pos++;
                    expectedVal--;
                }

                UTL_TEST_ASSERT(ptrNum != nullptr);
                UTL_TEST_ASSERT(pos == LIST_SIZE + (LIST_SIZE >> 1));
                UTL_TEST_ASSERT(expectedVal == (size_t)-1);
            }
            else
            {
                UTL_TEST_ASSERT(ptrNum->GetData( ) == numVal);

                ptrNum = ptrNum->GetNext( );
                pos++;
                numVal++;
            }
        }

        UTL_TEST_ASSERT(ptrNum == nullptr);
        UTL_TEST_ASSERT(pos == LIST_SIZE << 1);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveInsertList( )
{
    const size_t LIST_SIZE = 8;
    const size_t INSERT_POS = LIST_SIZE >> 1;

    bool badInsertThrew = false;

    TTL::List<MemoryManagementHelper>* pList = nullptr;
    TTL::List<MemoryManagementHelper>* pMoveList = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    // Confirm Initial Test Conditions - Pt 1
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    // Test out_of_range throw.
    try
    {
        pList = new TTL::List<MemoryManagementHelper>;
        pMoveList = new TTL::List<MemoryManagementHelper>;
        pList->Insert(1, std::move(*pMoveList));
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    UTL_TEST_ASSERT(badInsertThrew == true);

    // Populate lists.
    try
    {
        pMemHelper = new MemoryManagementHelper;
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            pList->Insert(pList->Size( ), std::move(*pMemHelper));
            pMoveList->Insert(pMoveList->Size( ), std::move(*pMemHelper));
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Ensure lists are populated.
    UTL_SETUP_ASSERT(pList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pList->Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(pList->Empty( ) == false);
    UTL_SETUP_ASSERT(pMoveList->GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(pMoveList->GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(pMoveList->Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(pMoveList->Empty( ) == false);

    // Test out_of_range throw.
    badInsertThrew = false;
    try
    {
        pList->Insert(pList->Size( ) + 1, std::move(*pMoveList));
    }
    catch ( const std::out_of_range& )
    {
        badInsertThrew = true;
    }

    UTL_TEST_ASSERT(badInsertThrew == true);

    // Ensure list insertions used move.
    {
        const TTL::DNode<MemoryManagementHelper>* ptr = pList->GetHead( );

        size_t i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == false);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == true);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE);

        ptr = pMoveList->GetHead( );
        i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == false);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == true);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE);
    }

    // Insert copyList into the middle of list.
    try
    {
        pList->Insert(LIST_SIZE >> 1, std::move(*pMoveList));
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure insertion modified list properly and emptied moveList.
    UTL_TEST_ASSERT(pList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pList->Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(pList->Empty( ) == false);
    UTL_TEST_ASSERT(pMoveList->GetHead( ) == nullptr);
    UTL_TEST_ASSERT(pMoveList->GetTail( ) == nullptr);
    UTL_TEST_ASSERT(pMoveList->Size( ) == 0);
    UTL_TEST_ASSERT(pMoveList->Empty( ) == true);

    // Ensure all values are still marked as moved.
    {
        const TTL::DNode<MemoryManagementHelper>* ptr = pList->GetHead( );

        size_t i = 0;
        while ( ptr )
        {
            i++;
            UTL_TEST_ASSERT(ptr->GetData( ).GetCopy( ) == false);
            UTL_TEST_ASSERT(ptr->GetData( ).GetMove( ) == true);
            ptr = ptr->GetNext( );
        }

        UTL_TEST_ASSERT(i == LIST_SIZE << 1);
    }

    delete pList;
    delete pMoveList;
    delete pMemHelper;
    pList = nullptr;
    pMoveList = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == (LIST_SIZE << 1) + 1);
    UTL_CLEANUP_ASSERT(!pList);
    UTL_CLEANUP_ASSERT(!pMoveList);
    UTL_CLEANUP_ASSERT(!pMemHelper);

    TTL::List<size_t> numList;
    TTL::List<size_t> numMoveList;

    // Confirm Initial Test Conditions - Pt 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);
    UTL_SETUP_ASSERT(numMoveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numMoveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numMoveList.Size( ) == 0);
    UTL_SETUP_ASSERT(numMoveList.Empty( ) == true);

    // Populate lists.
    try
    {
        for ( size_t i = 0; i < LIST_SIZE; i++ )
        {
            numList.Insert(numList.Size( ), i);
            numMoveList.Insert(numMoveList.Size( ), LIST_SIZE - i - 1);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Ensure lists are populated.
    UTL_SETUP_ASSERT(numList.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(numList.Empty( ) == false);
    UTL_SETUP_ASSERT(numMoveList.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(numMoveList.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(numMoveList.Size( ) == LIST_SIZE);
    UTL_SETUP_ASSERT(numMoveList.Empty( ) == false);

    // Insert numMoveList into the middle of numList.
    try
    {
        numList.Insert(INSERT_POS, std::move(numMoveList));
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure insertion modified numList properly and emptied numMoveList.
    UTL_TEST_ASSERT(numList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(numList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(numList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(numList.Empty( ) == false);
    UTL_TEST_ASSERT(numMoveList.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(numMoveList.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(numMoveList.Size( ) == 0);
    UTL_TEST_ASSERT(numMoveList.Empty( ) == true);

    // Ensure data ordering.
    {
        const TTL::DNode<size_t>* ptrNum = numList.GetHead( );

        size_t numVal = 0;
        size_t pos = 0;
        while ( ptrNum && pos < (LIST_SIZE << 1) )
        {
            if ( pos == INSERT_POS )
            {
                size_t expectedVal = LIST_SIZE - 1;
                while ( expectedVal != 0 )
                {
                    UTL_TEST_ASSERT(ptrNum->GetData( ) == expectedVal--);

                    ptrNum = ptrNum->GetNext( );
                    pos++;
                }

                UTL_TEST_ASSERT(ptrNum->GetData( ) == expectedVal);

                ptrNum = ptrNum->GetNext( );
                pos++;

                UTL_TEST_ASSERT(ptrNum != nullptr);
                UTL_TEST_ASSERT(pos == LIST_SIZE + (LIST_SIZE >> 1));
            }
            else
            {
                UTL_TEST_ASSERT(ptrNum->GetData( ) == numVal);

                ptrNum = ptrNum->GetNext( );
                pos++;
                numVal++;
            }
        }

        UTL_TEST_ASSERT(ptrNum == nullptr);
        UTL_TEST_ASSERT(pos == LIST_SIZE << 1);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}