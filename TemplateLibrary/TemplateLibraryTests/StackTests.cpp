#include "StackTests.h"

#include "Stack.hpp"

#include "CopyMoveHelper.hpp"

std::list<std::function<UnitTestResult(void)>> TTLTests::Stack::BuildTestList( )
{
    static const std::list<std::function<UnitTestResult(void)>> testList =
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
    const size_t STACK_SIZE = 3;
    const TTL::Node<size_t>* numStackPtr = nullptr;
    const TTL::Node<size_t>* copyStackPtr = nullptr;

    TTL::Stack<size_t> numStack;
    TTL::Stack<size_t>* pCopyStack = nullptr;

    try
    {
        numStack.Push(2);
        numStack.Push(1);
        numStack.Push(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    try
    {
        pCopyStack = new TTL::Stack<size_t>(numStack);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    numStackPtr = numStack.TopPtr( );
    copyStackPtr = pCopyStack->TopPtr( );
    
    UTL_TEST_ASSERT(numStackPtr != nullptr);
    UTL_TEST_ASSERT(copyStackPtr != nullptr);

    while ( numStackPtr && copyStackPtr )
    {
        UTL_TEST_ASSERT(numStackPtr != copyStackPtr);
        UTL_TEST_ASSERT(numStackPtr->GetData( ) == copyStackPtr->GetData( ));
        numStackPtr = numStackPtr->GetNext( );
        copyStackPtr = copyStackPtr->GetNext( );
    }

    UTL_TEST_ASSERT(numStackPtr == nullptr);
    UTL_TEST_ASSERT(copyStackPtr == nullptr);

    UTL_TEST_ASSERT(numStack.Empty( ) == false);
    UTL_TEST_ASSERT(pCopyStack->Empty( ) == false);
    UTL_TEST_ASSERT(numStack.Size( ) == STACK_SIZE);
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

    try
    {
        numStack.Push(2);
        numStack.Push(1);
        numStack.Push(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    numStackPtr = numStack.TopPtr( );

    try
    {
        pMoveStack = new TTL::Stack<size_t>(std::move(numStack));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    moveStackPtr = pMoveStack->TopPtr( );
    UTL_TEST_ASSERT(numStack.TopPtr( ) == nullptr);
    UTL_TEST_ASSERT(numStackPtr == moveStackPtr);
    UTL_TEST_ASSERT(moveStackPtr != nullptr);

    for ( size_t i = 0; i < STACK_SIZE; i++ )
    {
        UTL_TEST_ASSERT(moveStackPtr->GetData( ) == i);
        moveStackPtr = moveStackPtr->GetNext( );
    }

    UTL_TEST_ASSERT(moveStackPtr == nullptr);

    UTL_TEST_ASSERT(numStack.Empty( ) == true);
    UTL_TEST_ASSERT(pMoveStack->Empty( ) == false);
    UTL_TEST_ASSERT(numStack.Size( ) == 0);
    UTL_TEST_ASSERT(pMoveStack->Size( ) == STACK_SIZE);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}



// Assignment Operator Tests
UnitTestResult TTLTests::Stack::CopyAssignment( )
{
    const size_t STACK_SIZE = 3;
    const TTL::Node<size_t>* stackPtr = nullptr;
    const TTL::Node<size_t>* copyPtr = nullptr;

    TTL::Stack<size_t> stack;
    TTL::Stack<size_t> copyStack;

    try
    {
        stack.Push(2);
        stack.Push(1);
        stack.Push(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    try
    {
        copyStack = stack;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    stackPtr = stack.TopPtr( );
    copyPtr = copyStack.TopPtr( );

    while ( stackPtr && copyPtr )
    {
        UTL_TEST_ASSERT(stackPtr != copyPtr);
        UTL_TEST_ASSERT(stackPtr->GetData( ) == copyPtr->GetData( ));
        stackPtr = stackPtr->GetNext( );
        copyPtr = copyPtr->GetNext( );
    }

    UTL_TEST_ASSERT(stackPtr == nullptr);
    UTL_TEST_ASSERT(copyPtr == nullptr);

    UTL_TEST_ASSERT(stack.Empty( ) == false);
    UTL_TEST_ASSERT(copyStack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == STACK_SIZE);
    UTL_TEST_ASSERT(copyStack.Size( ) == STACK_SIZE);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::Stack::MoveAssignment( )
{
    const size_t STACK_SIZE = 3;
    const TTL::Node<size_t>* stackPtr = nullptr;
    const TTL::Node<size_t>* movePtr = nullptr;

    TTL::Stack<size_t> stack;
    TTL::Stack<size_t> moveStack;

    try
    {
        stack.Push(2);
        stack.Push(1);
        stack.Push(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    stackPtr = stack.TopPtr( );

    moveStack = std::move(stack);

    movePtr = moveStack.TopPtr( );
    UTL_TEST_ASSERT(stackPtr == movePtr);
    UTL_TEST_ASSERT(stack.TopPtr( ) == nullptr);

    for ( size_t i = 0; i < STACK_SIZE; i++ )
    {
        UTL_TEST_ASSERT(movePtr->GetData( ) == i);
        movePtr = movePtr->GetNext( );
    }

    UTL_TEST_ASSERT(stack.Empty( ) == true);
    UTL_TEST_ASSERT(moveStack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == 0);
    UTL_TEST_ASSERT(moveStack.Size( ) == STACK_SIZE);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}



// Public Method Tests
UnitTestResult TTLTests::Stack::Clear( )
{
    const size_t STACK_SIZE = 3;
    TTL::Stack<size_t> stack;

    try
    {
        stack.Push(2);
        stack.Push(1);
        stack.Push(0);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(stack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == STACK_SIZE);
    UTL_TEST_ASSERT(stack.TopPtr( ) != nullptr);

    stack.Clear( );

    UTL_TEST_ASSERT(stack.Empty( ) == true);
    UTL_TEST_ASSERT(stack.Size( ) == 0);
    UTL_TEST_ASSERT(stack.TopPtr( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::Stack::CopyPush( )
{
    const TTL::Node<CopyMoveHelper>* ptr = nullptr;

    TTL::Stack<CopyMoveHelper> stack;
    CopyMoveHelper copyHelper;

    try
    {
        stack.Push(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(stack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == 1);
    UTL_TEST_ASSERT(stack.TopPtr( ) != nullptr);
    UTL_TEST_ASSERT(stack.Top( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(stack.Top( ).GetMove( ) == false);

    ptr = stack.TopPtr( );

    try
    {
        stack.Push(copyHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(stack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == 2);
    UTL_TEST_ASSERT(stack.TopPtr( ) != nullptr);
    UTL_TEST_ASSERT(stack.Top( ).GetCopy( ) == true);
    UTL_TEST_ASSERT(stack.Top( ).GetMove( ) == false);
    UTL_TEST_ASSERT(ptr == stack.TopPtr( )->GetNext( ));

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::Stack::MovePush( )
{
    const TTL::Node<CopyMoveHelper>* ptr = nullptr;

    TTL::Stack<CopyMoveHelper> stack;
    CopyMoveHelper moveHelper;

    try
    {
        stack.Push(std::move(moveHelper));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(stack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == 1);
    UTL_TEST_ASSERT(stack.TopPtr( ) != nullptr);
    UTL_TEST_ASSERT(stack.Top( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(stack.Top( ).GetMove( ) == true);

    ptr = stack.TopPtr( );

    try
    {
        stack.Push(std::move(moveHelper));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(stack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == 2);
    UTL_TEST_ASSERT(stack.TopPtr( ) != nullptr);
    UTL_TEST_ASSERT(stack.Top( ).GetCopy( ) == false);
    UTL_TEST_ASSERT(stack.Top( ).GetMove( ) == true);
    UTL_TEST_ASSERT(ptr == stack.TopPtr( )->GetNext( ));

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::Stack::Pop( )
{
    const size_t STACK_SIZE = 10;
    bool threw = false;

    TTL::Stack<size_t> stack;

    try
    {
        stack.Pop( );
    }
    catch ( const std::logic_error& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw == true);

    try
    {
        for ( size_t i = 0; i < STACK_SIZE; i++ )
        {
            stack.Push(STACK_SIZE - i - 1);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(stack.Empty( ) == false);
    UTL_TEST_ASSERT(stack.Size( ) == STACK_SIZE);

    size_t i = 0;
    while ( !stack.Empty( ) )
    {
        UTL_TEST_ASSERT(i < STACK_SIZE);
        UTL_TEST_ASSERT(stack.TopPtr( ) != nullptr);
        UTL_TEST_ASSERT(stack.Top( ) == i++);

        try
        {
            stack.Pop( );
        }
        catch ( const std::logic_error& e )
        {
            UTL_TEST_EXCEPTION(e.what( ));
        }
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}
