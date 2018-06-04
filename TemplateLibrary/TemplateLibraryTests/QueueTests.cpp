#include "QueueTests.h"

#include "Queue.hpp"

#include "MemoryManagementHelper.hpp"

std::list<std::function<UnitTestResult(void)>> TTLTests::Queue::BuildTestList( )
{
    static const std::list<std::function<UnitTestResult(void)>> testList =
    {
        DefaultConstructor,
        CopyConstructor,
        MoveConstructor,
        CopyAssignment,
        MoveAssignment,
        CopyAppend_Data,
        MoveAppend_Data,
        CopyAppend_Queue,
        MoveAppend_Queue,
        Clear,
        CopyEnqueue_Data,
        MoveEnqueue_Data,
        CopyEnqueue_Queue,
        MoveEnqueue_Queue,
        Dequeue
    };

    return testList;
}

// Constructor Tests
UnitTestResult TTLTests::Queue::DefaultConstructor( )
{
    bool threw = false;
    TTL::Queue<size_t> queue;

    UTL_TEST_ASSERT(queue.Empty( ) == true);
    UTL_TEST_ASSERT(queue.Size( ) == 0);
    UTL_TEST_ASSERT(queue.FrontPtr( ) == nullptr);

    try
    {
        queue.Front( );
    }
    catch ( const std::logic_error& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::CopyConstructor( )
{
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* copyPtr = nullptr;
    const size_t QUEUE_SIZE = 3;

    TTL::Queue<size_t> queue;
    TTL::Queue<size_t>* pCopyQueue = nullptr;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    try
    {
        pCopyQueue = new TTL::Queue<size_t>(queue);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pCopyQueue->Empty( ) == false);
    UTL_TEST_ASSERT(pCopyQueue->Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(pCopyQueue->FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    copyPtr = pCopyQueue->FrontPtr( );

    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(copyPtr != nullptr);
        UTL_TEST_ASSERT(queuePtr != copyPtr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == copyPtr->GetData( ));
        queuePtr = queuePtr->GetNext( );
        copyPtr = copyPtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);
    UTL_TEST_ASSERT(copyPtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::MoveConstructor( )
{
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* movePtr = nullptr;
    const size_t QUEUE_SIZE = 3;

    TTL::Queue<size_t> queue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );

    TTL::Queue<size_t> moveQueue(std::move(queue));
    movePtr = moveQueue.FrontPtr( );

    UTL_TEST_ASSERT(queue.Empty( ) == true);
    UTL_TEST_ASSERT(queue.Size( ) == 0);
    UTL_TEST_ASSERT(queue.FrontPtr( ) == nullptr);
    UTL_TEST_ASSERT(moveQueue.Empty( ) == false);
    UTL_TEST_ASSERT(moveQueue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(moveQueue.FrontPtr( ) != nullptr);
    UTL_TEST_ASSERT(queuePtr == movePtr);

    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(movePtr != nullptr);
        UTL_TEST_ASSERT(movePtr->GetData( ) == i);
        movePtr = movePtr->GetNext( );
    }

    UTL_TEST_ASSERT(movePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Assignment Operator Tests
UnitTestResult TTLTests::Queue::CopyAssignment( )
{
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* copyPtr = nullptr;
    const size_t QUEUE_SIZE = 3;

    TTL::Queue<size_t> queue;
    TTL::Queue<size_t> copyQueue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    try
    {
        copyQueue = queue;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(copyQueue.Empty( ) == false);
    UTL_TEST_ASSERT(copyQueue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(copyQueue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    copyPtr = copyQueue.FrontPtr( );

    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(copyPtr != nullptr);
        UTL_TEST_ASSERT(queuePtr != copyPtr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == copyPtr->GetData( ));
        queuePtr = queuePtr->GetNext( );
        copyPtr = copyPtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);
    UTL_TEST_ASSERT(copyPtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::MoveAssignment( )
{
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* movePtr = nullptr;
    const size_t QUEUE_SIZE = 3;

    TTL::Queue<size_t> queue;
    TTL::Queue<size_t> moveQueue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );

    moveQueue = std::move(queue);
    movePtr = moveQueue.FrontPtr( );

    UTL_TEST_ASSERT(queue.Empty( ) == true);
    UTL_TEST_ASSERT(queue.Size( ) == 0);
    UTL_TEST_ASSERT(queue.FrontPtr( ) == nullptr);
    UTL_TEST_ASSERT(moveQueue.Empty( ) == false);
    UTL_TEST_ASSERT(moveQueue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(moveQueue.FrontPtr( ) != nullptr);
    UTL_TEST_ASSERT(queuePtr == movePtr);

    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(movePtr != nullptr);
        UTL_TEST_ASSERT(movePtr->GetData( ) == i);
        movePtr = movePtr->GetNext( );
    }

    UTL_TEST_ASSERT(movePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::CopyAppend_Data( )
{
    const size_t QUEUE_SIZE = 3;

    const TTL::Node<MemoryManagementHelper>* pMemQueuePtr = nullptr;
    TTL::Queue<MemoryManagementHelper>* pMemQueue = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pMemQueue = new TTL::Queue<MemoryManagementHelper>;
        pMemHelper = new MemoryManagementHelper;
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            *pMemQueue += *pMemHelper;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pMemQueue->Empty( ) == false);
    UTL_TEST_ASSERT(pMemQueue->Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(pMemQueue->FrontPtr( ) != nullptr);

    pMemQueuePtr = pMemQueue->FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(pMemQueuePtr != nullptr);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetCopy( ) == true);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetMove( ) == false);
        pMemQueuePtr = pMemQueuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(pMemQueuePtr == nullptr);

    delete pMemQueue;
    delete pMemHelper;
    pMemQueue = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == QUEUE_SIZE + 1);
    UTL_CLEANUP_ASSERT(!pMemQueue);
    UTL_CLEANUP_ASSERT(!pMemHelper);

    const TTL::Node<size_t>* queuePtr = nullptr;
    TTL::Queue<size_t> queue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue += i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);
        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::MoveAppend_Data( )
{
    const size_t QUEUE_SIZE = 3;

    const TTL::Node<MemoryManagementHelper>* pMemQueuePtr = nullptr;
    TTL::Queue<MemoryManagementHelper>* pMemQueue = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pMemQueue = new TTL::Queue<MemoryManagementHelper>;
        pMemHelper = new MemoryManagementHelper;
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            *pMemQueue += std::move(*pMemHelper);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pMemQueue->Empty( ) == false);
    UTL_TEST_ASSERT(pMemQueue->Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(pMemQueue->FrontPtr( ) != nullptr);

    pMemQueuePtr = pMemQueue->FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(pMemQueuePtr != nullptr);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetCopy( ) == false);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetMove( ) == true);
        pMemQueuePtr = pMemQueuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(pMemQueuePtr == nullptr);

    delete pMemQueue;
    delete pMemHelper;
    pMemQueue = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == QUEUE_SIZE + 1);
    UTL_CLEANUP_ASSERT(!pMemQueue);
    UTL_CLEANUP_ASSERT(!pMemHelper);

    const TTL::Node<size_t>* queuePtr = nullptr;
    TTL::Queue<size_t> queue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            size_t val = i;
            queue += std::move(val);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);
        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::CopyAppend_Queue( )
{
    const size_t QUEUE_SIZE = 3;
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* copyPtr = nullptr;

    TTL::Queue<size_t> queue;
    TTL::Queue<size_t> copyQueue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            copyQueue.Enqueue(QUEUE_SIZE + i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(copyQueue.Empty( ) == false);
    UTL_SETUP_ASSERT(copyQueue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(copyQueue.FrontPtr( ) != nullptr);

    try
    {
        queue += copyQueue;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE << 1);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);
    UTL_TEST_ASSERT(copyQueue.Empty( ) == false);
    UTL_TEST_ASSERT(copyQueue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(copyQueue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    copyPtr = copyQueue.FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE << 1; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);

        if ( i >= QUEUE_SIZE )
        {
            UTL_TEST_ASSERT(copyPtr != nullptr);
            UTL_TEST_ASSERT(queuePtr != copyPtr);
            UTL_TEST_ASSERT(copyPtr->GetData( ) == i);
            copyPtr = copyPtr->GetNext( );
        }

        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);
    UTL_TEST_ASSERT(copyPtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::MoveAppend_Queue( )
{
    const size_t QUEUE_SIZE = 3;
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* movePtr = nullptr;

    TTL::Queue<size_t> queue;
    TTL::Queue<size_t> moveQueue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            moveQueue.Enqueue(QUEUE_SIZE + i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(moveQueue.Empty( ) == false);
    UTL_SETUP_ASSERT(moveQueue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(moveQueue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    movePtr = moveQueue.FrontPtr( );

    queue += std::move(moveQueue);

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE << 1);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);
    UTL_TEST_ASSERT(moveQueue.Empty( ) == true);
    UTL_TEST_ASSERT(moveQueue.Size( ) == 0);
    UTL_TEST_ASSERT(moveQueue.FrontPtr( ) == nullptr);

    for ( size_t i = 0; i < QUEUE_SIZE << 1; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);

        if ( i >= QUEUE_SIZE )
        {
            UTL_TEST_ASSERT(movePtr != nullptr);
            UTL_TEST_ASSERT(queuePtr == movePtr);
            movePtr = movePtr->GetNext( );
        }

        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);
    UTL_TEST_ASSERT(movePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

// Public Method Tests
UnitTestResult TTLTests::Queue::Clear( )
{
    const size_t QUEUE_SIZE = 3;

    TTL::Queue<size_t> queue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    queue.Clear( );

    UTL_TEST_ASSERT(queue.Empty( ) == true);
    UTL_TEST_ASSERT(queue.Size( ) == 0);
    UTL_TEST_ASSERT(queue.FrontPtr( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::CopyEnqueue_Data( )
{
    const size_t QUEUE_SIZE = 3;

    const TTL::Node<MemoryManagementHelper>* pMemQueuePtr = nullptr;
    TTL::Queue<MemoryManagementHelper>* pMemQueue = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pMemQueue = new TTL::Queue<MemoryManagementHelper>;
        pMemHelper = new MemoryManagementHelper;
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            pMemQueue->Enqueue(*pMemHelper);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pMemQueue->Empty( ) == false);
    UTL_TEST_ASSERT(pMemQueue->Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(pMemQueue->FrontPtr( ) != nullptr);

    pMemQueuePtr = pMemQueue->FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(pMemQueuePtr != nullptr);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetCopy( ) == true);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetMove( ) == false);
        pMemQueuePtr = pMemQueuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(pMemQueuePtr == nullptr);

    delete pMemQueue;
    delete pMemHelper;
    pMemQueue = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == QUEUE_SIZE + 1);
    UTL_CLEANUP_ASSERT(!pMemQueue);
    UTL_CLEANUP_ASSERT(!pMemHelper);


    const TTL::Node<size_t>* queuePtr = nullptr;
    TTL::Queue<size_t> queue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);
        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::MoveEnqueue_Data( )
{
    const size_t QUEUE_SIZE = 3;

    const TTL::Node<MemoryManagementHelper>* pMemQueuePtr = nullptr;
    TTL::Queue<MemoryManagementHelper>* pMemQueue = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);

    try
    {
        pMemQueue = new TTL::Queue<MemoryManagementHelper>;
        pMemHelper = new MemoryManagementHelper;
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            pMemQueue->Enqueue(std::move(*pMemHelper));
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pMemQueue->Empty( ) == false);
    UTL_TEST_ASSERT(pMemQueue->Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(pMemQueue->FrontPtr( ) != nullptr);

    pMemQueuePtr = pMemQueue->FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(pMemQueuePtr != nullptr);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetCopy( ) == false);
        UTL_TEST_ASSERT(pMemQueuePtr->GetData( ).GetMove( ) == true);
        pMemQueuePtr = pMemQueuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(pMemQueuePtr == nullptr);

    delete pMemQueue;
    delete pMemHelper;
    pMemQueue = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == QUEUE_SIZE + 1);
    UTL_CLEANUP_ASSERT(pMemQueue == nullptr);
    UTL_CLEANUP_ASSERT(pMemHelper == nullptr);


    const TTL::Node<size_t>* queuePtr = nullptr;
    TTL::Queue<size_t> queue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            size_t val = i;
            queue.Enqueue(std::move(val));
        }
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);
        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::CopyEnqueue_Queue( )
{
    const size_t QUEUE_SIZE = 3;
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* copyPtr = nullptr;

    TTL::Queue<size_t> queue;
    TTL::Queue<size_t> copyQueue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            copyQueue.Enqueue(QUEUE_SIZE + i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(copyQueue.Empty( ) == false);
    UTL_SETUP_ASSERT(copyQueue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(copyQueue.FrontPtr( ) != nullptr);

    try
    {
        queue.Enqueue(copyQueue);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE << 1);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);
    UTL_TEST_ASSERT(copyQueue.Empty( ) == false);
    UTL_TEST_ASSERT(copyQueue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(copyQueue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    copyPtr = copyQueue.FrontPtr( );
    for ( size_t i = 0; i < QUEUE_SIZE << 1; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);

        if ( i >= QUEUE_SIZE )
        {
            UTL_TEST_ASSERT(copyPtr != nullptr);
            UTL_TEST_ASSERT(queuePtr != copyPtr);
            UTL_TEST_ASSERT(copyPtr->GetData( ) == i);
            copyPtr = copyPtr->GetNext( );
        }

        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);
    UTL_TEST_ASSERT(copyPtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::MoveEnqueue_Queue( )
{
    const size_t QUEUE_SIZE = 3;
    const TTL::Node<size_t>* queuePtr = nullptr;
    const TTL::Node<size_t>* movePtr = nullptr;

    TTL::Queue<size_t> queue;
    TTL::Queue<size_t> moveQueue;

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(queue.Empty( ) == false);
    UTL_SETUP_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(queue.FrontPtr( ) != nullptr);

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            moveQueue.Enqueue(QUEUE_SIZE + i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(moveQueue.Empty( ) == false);
    UTL_SETUP_ASSERT(moveQueue.Size( ) == QUEUE_SIZE);
    UTL_SETUP_ASSERT(moveQueue.FrontPtr( ) != nullptr);

    queuePtr = queue.FrontPtr( );
    movePtr = moveQueue.FrontPtr( );

    queue.Enqueue(std::move(moveQueue));

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE << 1);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);
    UTL_TEST_ASSERT(moveQueue.Empty( ) == true);
    UTL_TEST_ASSERT(moveQueue.Size( ) == 0);
    UTL_TEST_ASSERT(moveQueue.FrontPtr( ) == nullptr);

    for ( size_t i = 0; i < QUEUE_SIZE << 1; i++ )
    {
        UTL_TEST_ASSERT(queuePtr != nullptr);
        UTL_TEST_ASSERT(queuePtr->GetData( ) == i);

        if ( i >= QUEUE_SIZE )
        {
            UTL_TEST_ASSERT(movePtr != nullptr);
            UTL_TEST_ASSERT(queuePtr == movePtr);
            movePtr = movePtr->GetNext( );
        }

        queuePtr = queuePtr->GetNext( );
    }

    UTL_TEST_ASSERT(queuePtr == nullptr);
    UTL_TEST_ASSERT(movePtr == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Queue::Dequeue( )
{
    const size_t QUEUE_SIZE = 10;

    bool threw = false;
    TTL::Queue<size_t> queue;

    try
    {
        queue.Dequeue( );
    }
    catch ( const std::logic_error& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw == true);

    try
    {
        for ( size_t i = 0; i < QUEUE_SIZE; i++ )
        {
            queue.Enqueue(i);
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_ASSERT(e.what( ));
    }

    UTL_TEST_ASSERT(queue.Empty( ) == false);
    UTL_TEST_ASSERT(queue.Size( ) == QUEUE_SIZE);
    UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);

    {
        size_t i = 0;
        while ( !queue.Empty( ) )
        {
            UTL_TEST_ASSERT(queue.FrontPtr( ) != nullptr);

            try
            {
                UTL_TEST_ASSERT(queue.Front( ) == i++);
            }
            catch ( const std::logic_error& e )
            {
                UTL_TEST_EXCEPTION(e.what( ));
            }

            try
            {
                queue.Dequeue( );
            }
            catch ( const std::logic_error& e )
            {
                UTL_TEST_EXCEPTION(e.what( ));
            }
        }
    }

    UTL_TEST_ASSERT(queue.Empty( ));
    UTL_TEST_ASSERT(queue.Size( ) == 0);
    UTL_TEST_ASSERT(queue.FrontPtr( ) == nullptr);

    threw = false;
    try
    {
        queue.Front( );
    }
    catch ( const std::logic_error& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw == true);

    threw = false;
    try
    {
        queue.Dequeue( );
    }
    catch ( const std::logic_error& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}
