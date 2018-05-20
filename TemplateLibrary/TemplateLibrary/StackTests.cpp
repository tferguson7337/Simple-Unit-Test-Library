#include "StackTests.h"

#include "Stack.hpp"

std::list<std::function<UnitTestResult(void)>> TTLTests::Stack::BuildTestList( )
{
    std::list<std::function<UnitTestResult(void)>> testList =
    {
        DefaultConstructor,
        CopyConstructor,
        MoveConstructor,
        CopyAssignment,
        MoveAssignment,
        Clear,
        CopyPush,
        MovePush,
        Pop
    };

    return testList;
}

// Constructor Tests
UnitTestResult TTLTests::Stack::DefaultConstructor( )
{
    bool threw = false;
    TTL::Stack<size_t> stack;

    UTL_TEST_ASSERT(stack.Empty( ));
    UTL_TEST_ASSERT(stack.Size( ) == 0);
    UTL_TEST_ASSERT(stack.TopPtr( ) == nullptr);

    try
    {
        stack.Top( );
    }
    catch ( const std::logic_error& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);

    threw = false;
    try
    {
        stack.Pop( );
    }
    catch ( const std::logic_error& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::Stack::CopyConstructor( )
{
    TTL::Stack<size_t> numStack;
    TTL::Stack<size_t>* pCopyStack = nullptr;

    // Push some data onto numStack.
    try
    {
        numStack.Push(0);
        numStack.Push(1);
        numStack.Push(2);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Construct via copy.
    try
    {
        pCopyStack = new TTL::Stack<size_t>(numStack);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure the top-pointer for both stacks are valid.
    const TTL::Node<size_t>* numStackPtr = numStack.TopPtr( );
    const TTL::Node<size_t>* copyStackPtr = pCopyStack->TopPtr( );
    UTL_TEST_ASSERT(numStackPtr != nullptr);
    UTL_TEST_ASSERT(copyStackPtr != nullptr);

    // Ensure: 
    //  1) Deep Copy
    //  2) Proper Order
    while ( numStackPtr && copyStackPtr )
    {
        UTL_TEST_ASSERT(numStackPtr != copyStackPtr);
        UTL_TEST_ASSERT(numStackPtr->GetData( ) == copyStackPtr->GetData( ));
        numStackPtr = numStackPtr->GetNext( );
        copyStackPtr = copyStackPtr->GetNext( );
    }

    // Ensure we reached the end of both stacks.
    UTL_TEST_ASSERT(numStackPtr == nullptr);
    UTL_TEST_ASSERT(copyStackPtr == nullptr);

    // Ensure that the size and empty values make sense.
    UTL_TEST_ASSERT(numStack.Empty( ) == false);
    UTL_TEST_ASSERT(pCopyStack->Empty( ) == false);
    UTL_TEST_ASSERT(numStack.Size( ) == 3);
    UTL_TEST_ASSERT(numStack.Size( ) == pCopyStack->Size( ));

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::Stack::MoveConstructor( )
{
    const TTL::Node<size_t>* numStackPtr = nullptr;
    const TTL::Node<size_t>* moveStackPtr = nullptr;
    const size_t STACK_SIZE = 3;
    TTL::Stack<size_t> numStack;
    TTL::Stack<size_t>* pMoveStack = nullptr;

    // Push some data onto numStack.
    try
    {
        numStack.Push(0);
        numStack.Push(1);
        numStack.Push(2);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    numStackPtr = numStack.TopPtr( );

    // Construct via move.
    try
    {
        pMoveStack = new TTL::Stack<size_t>(std::move(numStack));
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    // Ensure the move "stole" the internal data from numStack.
    moveStackPtr = pMoveStack->TopPtr( );
    UTL_TEST_ASSERT(numStack.TopPtr( ) == nullptr);
    UTL_TEST_ASSERT(numStackPtr == moveStackPtr);
    UTL_TEST_ASSERT(moveStackPtr != nullptr);


    // Ensure that stack order was not altered in move.
    for ( size_t i = 0; i < STACK_SIZE; i++ )
    {
        UTL_TEST_ASSERT(moveStackPtr->GetData( ) == STACK_SIZE - i - 1);
        moveStackPtr = moveStackPtr->GetNext( );
    }

    // Ensure we reached the end of the move stack in the loop.
    UTL_TEST_ASSERT(moveStackPtr == nullptr);

    // Ensure that the size and empty values make sense.
    UTL_TEST_ASSERT(numStack.Empty( ) == true);
    UTL_TEST_ASSERT(pMoveStack->Empty( ) == false);
    UTL_TEST_ASSERT(numStack.Size( ) == 0);
    UTL_TEST_ASSERT(pMoveStack->Size( ) == STACK_SIZE);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}



// Assignment Operator Test
UnitTestResult TTLTests::Stack::CopyAssignment( )
{


    ///@todo: HARD-CODED FAILURE PENDING IMPLEMENTATION
    UTL_TEST_FAILURE( );
}


UnitTestResult TTLTests::Stack::MoveAssignment( )
{


    ///@todo: HARD-CODED FAILURE PENDING IMPLEMENTATION
    UTL_TEST_FAILURE( );
}



// Public Method Tests
UnitTestResult TTLTests::Stack::Clear( )
{


    ///@todo: HARD-CODED FAILURE PENDING IMPLEMENTATION
    UTL_TEST_FAILURE( );
}


UnitTestResult TTLTests::Stack::CopyPush( )
{


    ///@todo: HARD-CODED FAILURE PENDING IMPLEMENTATION
    UTL_TEST_FAILURE( );
}


UnitTestResult TTLTests::Stack::MovePush( )
{


    ///@todo: HARD-CODED FAILURE PENDING IMPLEMENTATION
    UTL_TEST_FAILURE( );
}


UnitTestResult TTLTests::Stack::Pop( )
{


    ///@todo: HARD-CODED FAILURE PENDING IMPLEMENTATION
    UTL_TEST_FAILURE( );
}

