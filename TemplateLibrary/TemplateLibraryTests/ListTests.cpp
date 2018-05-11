#include "ListTests.h"

#include "List.hpp"

#include "CopyMoveHelper.hpp"

std::list<std::function<UnitTestResult(void)>> TTLTests::List::BuildTestList( )
{
    std::list<std::function<UnitTestResult(void)>> testList =
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
        CopyAppendData,
        MoveAppendData,
        CopyAppendList,
        MoveAppendList,
        CopyPrependData,
        MovePrependData,
        CopyPrependList,
        MovePrependList,
        PopFront,
        PopBack
    };

    return testList;
}

// Constructor Tests

UnitTestResult TTLTests::List::DefaultConstructor( )
{
    TTL::List<uint64> emptyList;

    UTL_TEST_ASSERT(emptyList.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(emptyList.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(emptyList.Size( ) == 0ull);
    UTL_TEST_ASSERT(emptyList.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyConstructor( )
{
    TTL::List<CopyMoveHelper> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list.Append(CopyMoveHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Reset appended CopyMoveHelper() to test later.
    list.Front( ).Reset( );

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(list.Size( ) == 1ull);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    TTL::List<CopyMoveHelper>* pCopyList = nullptr;

    try
    {
        pCopyList = new TTL::List<CopyMoveHelper>(list);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure list wasn't modified after copy.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    // Ensure list was properly copied.
    UTL_TEST_ASSERT(pCopyList != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetHead( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetTail( ) != nullptr);
    UTL_TEST_ASSERT(pCopyList->GetHead( ) == pCopyList->GetTail( ));
    UTL_TEST_ASSERT(pCopyList->GetHead( ) != list.GetHead( ));
    UTL_TEST_ASSERT(pCopyList->GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(pCopyList->GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(pCopyList->Size( ) == 1ull);
    UTL_TEST_ASSERT(pCopyList->Size( ) == list.Size( ));
    UTL_TEST_ASSERT(pCopyList->Empty( ) == false);

    delete pCopyList;
    pCopyList = nullptr;
    UTL_CLEANUP_ASSERT(pCopyList == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveConstructor( )
{
    TTL::List<CopyMoveHelper> list;
    const TTL::DNode<CopyMoveHelper>* pListHead = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list.Append(CopyMoveHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Reset appended CopyMoveHelper() to test later.
    list.Front( ).Reset( );

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(list.Size( ) == 1ull);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    pListHead = list.GetHead( );

    TTL::List<CopyMoveHelper> moveList(std::move(list));

    // Ensure list was cleared out after move.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    // Ensure list was moved.
    UTL_TEST_ASSERT(moveList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(moveList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(moveList.GetHead( ) == moveList.GetTail( ));
    UTL_TEST_ASSERT(moveList.Size( ) == 1ull);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure list move is efficient (just moving pointers).
    UTL_TEST_ASSERT(moveList.GetHead( ) == pListHead);
    UTL_TEST_ASSERT(moveList.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(moveList.GetHead( )->GetData( ).GetMove( ) == false);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Assignment Tests

UnitTestResult TTLTests::List::CopyAssignment( )
{
    TTL::List<CopyMoveHelper> list;
    TTL::List<CopyMoveHelper> copyList;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(copyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(copyList.Empty( ) == true);

    try
    {
        list.Append(CopyMoveHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Reset appended CopyMoveHelper() to test later.
    list.Front( ).Reset( );

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(list.Size( ) == 1ull);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    try
    {
        copyList = list;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure list wasn't modified after copy.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    // Ensure list was properly copied.
    UTL_TEST_ASSERT(copyList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(copyList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(copyList.GetHead( ) == copyList.GetTail( ));
    UTL_TEST_ASSERT(copyList.GetHead( ) != list.GetHead( ));
    UTL_TEST_ASSERT(copyList.GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(copyList.GetHead( )->GetData( ).GetMove( ) == false);
    UTL_TEST_ASSERT(copyList.Size( ) == 1ull);
    UTL_TEST_ASSERT(copyList.Size( ) == list.Size( ));
    UTL_TEST_ASSERT(copyList.Empty( ) == false);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MoveAssignment( )
{
    TTL::List<CopyMoveHelper> list;
    TTL::List<CopyMoveHelper> moveList;
    const TTL::DNode<CopyMoveHelper>* pListHead = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(moveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(moveList.Empty( ) == true);

    try
    {
        list.Append(CopyMoveHelper( ));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Reset appended CopyMoveHelper() to test later.
    list.Front( ).Reset( );

    UTL_SETUP_ASSERT(list.GetHead( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) != nullptr);
    UTL_SETUP_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_SETUP_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);
    UTL_SETUP_ASSERT(list.Size( ) == 1ull);
    UTL_SETUP_ASSERT(list.Empty( ) == false);

    pListHead = list.GetHead( );

    moveList = std::move(list);

    // Ensure list was cleared out after move.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    // Ensure list was moved.
    UTL_TEST_ASSERT(moveList.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(moveList.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(moveList.GetHead( ) == moveList.GetTail( ));
    UTL_TEST_ASSERT(moveList.Size( ) == 1ull);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure list move is efficient (just moving pointers).
    UTL_TEST_ASSERT(moveList.GetHead( ) == pListHead);
    UTL_TEST_ASSERT(moveList.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(moveList.GetHead( )->GetData( ).GetMove( ) == false);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Append Assignment Tests

UnitTestResult TTLTests::List::CopyAppendAssignmentData( )
{
    TTL::List<CopyMoveHelper> list;
    CopyMoveHelper copyHelper;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list += copyHelper;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);

    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( uint64 n = 0; n < LIST_SIZE; n++ )
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

    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = 0ull;
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
    TTL::List<CopyMoveHelper> list;
    CopyMoveHelper moveHelper;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list += std::move(moveHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == true);

    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( uint64 n = 0; n < LIST_SIZE; n++ )
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

    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = 0ull;
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
    const uint64 LIST_SIZE = 16;
    TTL::List<uint64> list;
    TTL::List<uint64> copyList;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(copyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(copyList.Empty( ) == true);

    try
    {
        // Append [0, 15] to copyList
        for ( uint64 i = 0; i < LIST_SIZE; i++ )
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
        for ( uint64 i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
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
    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = LIST_SIZE;
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
    counter = 0ull;
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
    const uint64 LIST_SIZE = 16;
    TTL::List<uint64> list;
    TTL::List<uint64> moveList;

    const TTL::DNode<uint64>* listHead = nullptr;
    const TTL::DNode<uint64>* listTail = nullptr;
    const TTL::DNode<uint64>* moveTail = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(moveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(moveList.Empty( ) == true);

    try
    {
        for ( uint64 i = 0; i < LIST_SIZE; i++ )
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
        for ( uint64 i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
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
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(moveList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == true);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure move cleared out list pointers.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);

    // Ensure the move was performed correctly.
    UTL_TEST_ASSERT(moveTail->GetNext( ) == listHead);
    UTL_TEST_ASSERT(moveList.GetTail( ) == listTail);

    const TTL::DNode<uint64>* ptr = moveList.GetHead( );
    uint64 counter = 0ull;
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
    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    // Create first node.
    try
    {
        list.Append(0ull);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == nullptr);

    // Create second node.
    try
    {
        list.Append(0ull);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 2ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);

    const TTL::DNode<uint64>* pMid = list.GetTail( );

    // Create third node.
    try
    {
        list.Append(0ull);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 3ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMid->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(pMid->GetNext( ) == list.GetTail( ));

    // Test LIST_SIZE linkage.
    const TTL::DNode<uint64>* nodeArr[LIST_SIZE] = { nullptr };
    nodeArr[0] = list.GetHead( );
    nodeArr[1] = pMid;
    nodeArr[2] = list.GetTail( );

    // Fill up the rest of the list, save node addr's as we go.
    try
    {
        while ( list.Size( ) < LIST_SIZE )
        {
            list.Append(0ull);
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
    for ( uint64 i = 1ull; i < LIST_SIZE - 1; i++ )
    {
        UTL_TEST_ASSERT(nodeArr[i]->GetPrev( ) == nodeArr[i - 1]);
        UTL_TEST_ASSERT(nodeArr[i]->GetNext( ) == nodeArr[i + 1]);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::PrependDataNodeLinkage( )
{
    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    // Create first node.
    try
    {
        list.Prepend(0ull);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == nullptr);

    // Create second node.
    try
    {
        list.Prepend(0ull);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 2ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);

    const TTL::DNode<uint64>* pMid = list.GetHead( );

    // Create third node.
    try
    {
        list.Prepend(0ull);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 3ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);

    UTL_TEST_ASSERT(list.GetHead( ) != list.GetTail( ));
    UTL_TEST_ASSERT(list.GetHead( )->GetPrev( ) == nullptr);
    UTL_TEST_ASSERT(list.GetHead( )->GetNext( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetPrev( ) == pMid);
    UTL_TEST_ASSERT(list.GetTail( )->GetNext( ) == nullptr);
    UTL_TEST_ASSERT(pMid->GetPrev( ) == list.GetHead( ));
    UTL_TEST_ASSERT(pMid->GetNext( ) == list.GetTail( ));

    // Test LIST_SIZE linkage.
    const TTL::DNode<uint64>* nodeArr[LIST_SIZE] = { nullptr };
    nodeArr[LIST_SIZE - 1] = list.GetTail( );
    nodeArr[LIST_SIZE - 2] = pMid;
    nodeArr[LIST_SIZE - 3] = list.GetHead( );

    // Fill up the rest of the list, save node addr's as we go.
    try
    {
        while ( list.Size( ) < LIST_SIZE )
        {
            list.Prepend(0ull);
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
    for ( uint64 i = 1ull; i < LIST_SIZE - 1; i++ )
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
    const uint64 LIST_SIZE = 16ull;
    TTL::List<uint64> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        while ( list.Size( ) < LIST_SIZE )
        {
            list.Append(0ull);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    list.Clear( );

    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyAppendData( )
{
    TTL::List<CopyMoveHelper> list;
    CopyMoveHelper copyHelper;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list.Append(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);

    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( uint64 n = 0; n < LIST_SIZE; n++ )
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

    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = 0ull;
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
    TTL::List<CopyMoveHelper> list;
    CopyMoveHelper moveHelper;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list.Append(std::move(moveHelper));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == true);

    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( uint64 n = 0; n < LIST_SIZE; n++ )
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

    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = 0ull;
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
    const uint64 LIST_SIZE = 16;
    TTL::List<uint64> list;
    TTL::List<uint64> copyList;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(copyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(copyList.Empty( ) == true);

    try
    {
        // Append [0, LIST_SIZE - 1] to copyList
        for ( uint64 i = 0; i < LIST_SIZE; i++ )
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
        for ( uint64 i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
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
    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = LIST_SIZE;
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
    counter = 0ull;
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
    const uint64 LIST_SIZE = 16;
    TTL::List<uint64> list;
    TTL::List<uint64> moveList;

    const TTL::DNode<uint64>* listHead = nullptr;
    const TTL::DNode<uint64>* listTail = nullptr;
    const TTL::DNode<uint64>* moveTail = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(moveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(moveList.Empty( ) == true);

    try
    {
        for ( uint64 i = 0; i < LIST_SIZE; i++ )
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
        for ( uint64 i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
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
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(moveList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == true);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure move cleared out list pointers.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);

    // Ensure the move was performed correctly.
    UTL_TEST_ASSERT(moveTail->GetNext( ) == listHead);
    UTL_TEST_ASSERT(moveList.GetTail( ) == listTail);

    const TTL::DNode<uint64>* ptr = moveList.GetHead( );
    uint64 counter = 0ull;
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
    TTL::List<CopyMoveHelper> list;
    CopyMoveHelper copyHelper;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list.Prepend(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == false);

    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( uint64 n = 0ull; n < LIST_SIZE; n++ )
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

    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = LIST_SIZE - 1;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter--);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == 0ull);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::MovePrependData( )
{
    TTL::List<CopyMoveHelper> list;
    CopyMoveHelper moveHelper;

    // Confirm Initial Test Conditions - Part 1
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        list.Prepend(std::move(moveHelper));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Part 1 - Ensure Copy is Performed.
    UTL_TEST_ASSERT(list.GetHead( ) != nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) != nullptr);
    UTL_TEST_ASSERT(list.GetHead( ) == list.GetTail( ));
    UTL_TEST_ASSERT(list.Size( ) == 1ull);
    UTL_TEST_ASSERT(list.Empty( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(list.GetHead( )->GetData( ).GetMove( ) == true);

    const uint64 LIST_SIZE = 64ull;
    TTL::List<uint64> numList;

    // Confirm Initial Test Conditions - Part 2
    UTL_SETUP_ASSERT(numList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(numList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(numList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(numList.Empty( ) == true);

    try
    {
        for ( uint64 n = 0ull; n < LIST_SIZE; n++ )
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

    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = LIST_SIZE - 1;
    for ( ptr = numList.GetHead( ); ptr->GetNext( ); ptr = ptr->GetNext( ) )
    {
        UTL_TEST_ASSERT(ptr->GetData( ) == counter--);
    }

    UTL_TEST_ASSERT(ptr);
    UTL_TEST_ASSERT(ptr == numList.GetTail( ));
    UTL_TEST_ASSERT(counter == 0ull);
    UTL_TEST_ASSERT(ptr->GetData( ) == counter);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::CopyPrependList( )
{
    const uint64 LIST_SIZE = 16;
    TTL::List<uint64> list;
    TTL::List<uint64> copyList;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(copyList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(copyList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(copyList.Empty( ) == true);

    try
    {
        // Append [LIST_SIZE, (LIST_SIZE << 1) - 1] into copyList
        for ( uint64 i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
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
        for ( uint64 i = 0ull; i < LIST_SIZE; i++ )
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
    const TTL::DNode<uint64>* ptr = nullptr;
    uint64 counter = 0ull;
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
    counter = 0ull;
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
    const uint64 LIST_SIZE = 16;
    TTL::List<uint64> list;
    TTL::List<uint64> moveList;

    const TTL::DNode<uint64>* listHead = nullptr;
    const TTL::DNode<uint64>* listTail = nullptr;
    const TTL::DNode<uint64>* moveHead = nullptr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);
    UTL_SETUP_ASSERT(moveList.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(moveList.Size( ) == 0ull);
    UTL_SETUP_ASSERT(moveList.Empty( ) == true);

    try
    {
        for ( uint64 i = LIST_SIZE; i < LIST_SIZE << 1; i++ )
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
        for ( uint64 i = 0ull; i < LIST_SIZE; i++ )
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
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(moveList.Size( ) == LIST_SIZE << 1);
    UTL_TEST_ASSERT(list.Empty( ) == true);
    UTL_TEST_ASSERT(moveList.Empty( ) == false);

    // Ensure move cleared out list pointers.
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);

    // Ensure the move was performed correctly.
    UTL_TEST_ASSERT(listHead == moveList.GetHead( ));
    UTL_TEST_ASSERT(listTail == moveHead->GetPrev( ));

    const TTL::DNode<uint64>* ptr = moveList.GetHead( );
    uint64 counter = 0ull;
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

UnitTestResult TTLTests::List::PopFront( )
{
    const uint64 LIST_SIZE = 16ull;
    TTL::List<uint64> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        for ( uint64 i = 0ull; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    uint64 i = 0ull;
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
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::List::PopBack( )
{
    const uint64 LIST_SIZE = 16ull;
    TTL::List<uint64> list;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(list.GetHead( ) == nullptr);
    UTL_SETUP_ASSERT(list.GetTail( ) == nullptr);
    UTL_SETUP_ASSERT(list.Size( ) == 0ull);
    UTL_SETUP_ASSERT(list.Empty( ) == true);

    try
    {
        for ( uint64 i = 0ull; i < LIST_SIZE; i++ )
        {
            list.Append(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    uint64 i = LIST_SIZE - 1;
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

    UTL_TEST_ASSERT(i == (uint64)-1);
    UTL_TEST_ASSERT(list.GetHead( ) == nullptr);
    UTL_TEST_ASSERT(list.GetTail( ) == nullptr);
    UTL_TEST_ASSERT(list.Size( ) == 0ull);
    UTL_TEST_ASSERT(list.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}