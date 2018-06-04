#include "DynamicArrayTests.h"
#include "MemoryManagementHelper.hpp"

#include "DynamicArray.hpp"

std::list<std::function<UnitTestResult(void)>> TTLTests::DynamicArray::BuildTestList( )
{
    static const std::list<std::function<UnitTestResult(void)>> testList =
    {
        // Constructor Tests
        DefaultConstructor,
        CapacityConstructor,
        CopyConstructor,
        MoveConstructor,

        // Assignment Tests
        CopyAssignment,
        MoveAssignment,

        // Subscript Test
        Subscript,

        // Public Method Tests
        At,
        Clear,
        CopyAppend,
        MoveAppend,
        Front,
        Back,
        Compress,
        Reserve
    };

    return testList;
}


// Constructor Tests
UnitTestResult TTLTests::DynamicArray::DefaultConstructor( )
{
    TTL::DynamicArray<uint64> emptyArr;

    UTL_TEST_ASSERT(emptyArr.Size( ) == 0);
    UTL_TEST_ASSERT(emptyArr.Capacity( ) == 0);
    UTL_TEST_ASSERT(emptyArr.Data( ) == nullptr);
    UTL_TEST_ASSERT(emptyArr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::CapacityConstructor( )
{
    const uint64 ARR_CAP = 16;
    TTL::DynamicArray<uint64> emptyArr;
    TTL::DynamicArray<uint64> arr;

    try
    {
        emptyArr = std::move(TTL::DynamicArray<uint64>(0));
        arr = std::move(TTL::DynamicArray<uint64>(16));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(emptyArr.Size( ) == 0);
    UTL_TEST_ASSERT(emptyArr.Capacity( ) == 0);
    UTL_TEST_ASSERT(emptyArr.Data( ) == nullptr);
    UTL_TEST_ASSERT(emptyArr.Empty( ) == true);
    UTL_TEST_ASSERT(arr.Size( ) == 0);
    UTL_TEST_ASSERT(arr.Capacity( ) == ARR_CAP);
    UTL_TEST_ASSERT(arr.Data( ) != nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::CopyConstructor( )
{
    const uint64 ARR_CAP = 16;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(16));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_CAP);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    TTL::DynamicArray<uint64> copy(arr);

    UTL_TEST_ASSERT(copy.Size( ) == 0);
    UTL_TEST_ASSERT(copy.Capacity( ) == ARR_CAP);
    UTL_TEST_ASSERT(copy.Data( ) != nullptr);
    UTL_TEST_ASSERT(copy.Empty( ) == true);
    UTL_TEST_ASSERT(copy.Size( ) == arr.Size( ));
    UTL_TEST_ASSERT(copy.Capacity( ) == arr.Capacity( ));
    UTL_TEST_ASSERT(copy.Data( ) != arr.Data( ));
    UTL_TEST_ASSERT(copy.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::MoveConstructor( )
{
    const uint64 ARR_CAP = 16;
    TTL::DynamicArray<uint64> arr;
    uint64* pArr = nullptr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(16));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    pArr = arr.Data( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_CAP);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Data( ) == pArr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    TTL::DynamicArray<uint64> move(std::move(arr));

    UTL_TEST_ASSERT(move.Size( ) == 0);
    UTL_TEST_ASSERT(move.Capacity( ) == ARR_CAP);
    UTL_TEST_ASSERT(move.Data( ) != nullptr);
    UTL_TEST_ASSERT(move.Data( ) == pArr);
    UTL_TEST_ASSERT(move.Empty( ) == true);
    UTL_TEST_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == 0);
    UTL_SETUP_ASSERT(arr.Data( ) == nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Assignment Tests
UnitTestResult TTLTests::DynamicArray::CopyAssignment( )
{
    const uint64 ARR_CAP = 16;
    TTL::DynamicArray<uint64> arr;
    TTL::DynamicArray<uint64> copy;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_CAP));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_CAP);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);
    UTL_SETUP_ASSERT(copy.Size( ) == 0);
    UTL_SETUP_ASSERT(copy.Capacity( ) == 0);
    UTL_SETUP_ASSERT(copy.Data( ) == nullptr);
    UTL_SETUP_ASSERT(copy.Size( ) == 0);

    copy = arr;

    UTL_TEST_ASSERT(copy.Size( ) == 0);
    UTL_TEST_ASSERT(copy.Capacity( ) == ARR_CAP);
    UTL_TEST_ASSERT(copy.Data( ) != nullptr);
    UTL_TEST_ASSERT(copy.Empty( ) == true);
    UTL_TEST_ASSERT(copy.Size( ) == arr.Size( ));
    UTL_TEST_ASSERT(copy.Capacity( ) == arr.Capacity( ));
    UTL_TEST_ASSERT(copy.Data( ) != arr.Data( ));
    UTL_TEST_ASSERT(copy.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::MoveAssignment( )
{
    const uint64 ARR_CAP = 16;
    TTL::DynamicArray<uint64> arr;
    TTL::DynamicArray<uint64> move;
    uint64* pArr = nullptr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(16));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    pArr = arr.Data( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_CAP);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Data( ) == pArr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);
    UTL_SETUP_ASSERT(move.Size( ) == 0);
    UTL_SETUP_ASSERT(move.Capacity( ) == 0);
    UTL_SETUP_ASSERT(move.Data( ) == nullptr);
    UTL_SETUP_ASSERT(move.Empty( ) == true);

    move = std::move(arr);

    UTL_TEST_ASSERT(move.Size( ) == 0);
    UTL_TEST_ASSERT(move.Capacity( ) == ARR_CAP);
    UTL_TEST_ASSERT(move.Data( ) != nullptr);
    UTL_TEST_ASSERT(move.Data( ) == pArr);
    UTL_TEST_ASSERT(move.Empty( ) == true);
    UTL_TEST_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == 0);
    UTL_SETUP_ASSERT(arr.Data( ) == nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Subscript Test
UnitTestResult TTLTests::DynamicArray::Subscript( )
{
    const size_t ARR_SIZE = 16;
    bool boundaryThrow = false;
    TTL::DynamicArray<uint64> emptyArr;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);
    UTL_SETUP_ASSERT(emptyArr.Size( ) == 0);
    UTL_SETUP_ASSERT(emptyArr.Capacity( ) == 0);
    UTL_SETUP_ASSERT(emptyArr.Data( ) == nullptr);
    UTL_SETUP_ASSERT(emptyArr.Empty( ) == true);

    try
    {
        emptyArr[0] = 0;
    }
    catch ( const std::out_of_range& )
    {
        boundaryThrow = true;
    }

    UTL_TEST_ASSERT(boundaryThrow == true);

    boundaryThrow = false;

    size_t idx = 0;
    try
    {
        for ( ; idx < ARR_SIZE + 1; idx++ )
        {
            arr[idx] = idx;
        }
    }
    catch ( const std::out_of_range& )
    {
        boundaryThrow = true;
    }

    UTL_TEST_ASSERT(boundaryThrow == true);
    UTL_TEST_ASSERT(idx == 0);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Public Method Tests
UnitTestResult TTLTests::DynamicArray::At( )
{
    const size_t ARR_SIZE = 16;
    bool boundaryThrow = false;
    TTL::DynamicArray<uint64> emptyArr;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);
    UTL_SETUP_ASSERT(emptyArr.Size( ) == 0);
    UTL_SETUP_ASSERT(emptyArr.Capacity( ) == 0);
    UTL_SETUP_ASSERT(emptyArr.Data( ) == nullptr);
    UTL_SETUP_ASSERT(emptyArr.Empty( ) == true);

    try
    {
        emptyArr.At(0) = 0;
    }
    catch ( const std::out_of_range& )
    {
        boundaryThrow = true;
    }

    UTL_TEST_ASSERT(boundaryThrow == true);

    boundaryThrow = false;

    size_t idx = 0;
    try
    {
        for ( ; idx < ARR_SIZE + 1; idx++ )
        {
            arr.At(idx) = idx;
        }
    }
    catch ( const std::out_of_range& )
    {
        boundaryThrow = true;
    }

    UTL_TEST_ASSERT(boundaryThrow == true);
    UTL_TEST_ASSERT(idx == 0);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::Clear( )
{
    const size_t ARR_SIZE = 16;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    arr.Clear( );

    UTL_TEST_ASSERT(arr.Size( ) == 0);
    UTL_TEST_ASSERT(arr.Capacity( ) == 0);
    UTL_TEST_ASSERT(arr.Data( ) == nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::CopyAppend( )
{
    const size_t ARR_SIZE = 16;
    TTL::DynamicArray<MemoryManagementHelper>* pEmptyArr = nullptr;
    TTL::DynamicArray<MemoryManagementHelper>* pArr = nullptr;

    MemoryManagementHelper* pMemHelper = nullptr;

    try
    {
        pMemHelper = new MemoryManagementHelper;
        pArr = new TTL::DynamicArray<MemoryManagementHelper>(ARR_SIZE);
        pEmptyArr = new TTL::DynamicArray<MemoryManagementHelper>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);
    UTL_SETUP_ASSERT(pMemHelper != nullptr);
    UTL_SETUP_ASSERT(pArr != nullptr);
    UTL_SETUP_ASSERT(pEmptyArr != nullptr);

    UTL_SETUP_ASSERT(pEmptyArr->Size( ) == 0);
    UTL_SETUP_ASSERT(pEmptyArr->Capacity( ) == 0);
    UTL_SETUP_ASSERT(pEmptyArr->Data( ) == nullptr);
    UTL_SETUP_ASSERT(pEmptyArr->Empty( ) == true);
    UTL_SETUP_ASSERT(pArr->Size( ) == 0);
    UTL_SETUP_ASSERT(pArr->Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(pArr->Data( ) != nullptr);
    UTL_SETUP_ASSERT(pArr->Empty( ) == true);

    try
    {
        pEmptyArr->Append(*pMemHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pEmptyArr->Size( ) == 1);
    UTL_TEST_ASSERT(pEmptyArr->Capacity( ) == TTL::DynamicArray<MemoryManagementHelper>::DefaultCapacity( ));
    UTL_TEST_ASSERT(pEmptyArr->Data( ) != nullptr);
    UTL_TEST_ASSERT(pEmptyArr->Data( )[0].GetCopy( ) == true);
    UTL_TEST_ASSERT(pEmptyArr->Data( )[0].GetMove( ) == false);
    UTL_TEST_ASSERT(pEmptyArr->Empty( ) == false);

    try
    {
        pArr->Append(*pMemHelper);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pArr->Size( ) == 1);
    UTL_TEST_ASSERT(pArr->Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(pArr->Data( )[0].GetCopy( ) == true);
    UTL_TEST_ASSERT(pArr->Data( )[0].GetMove( ) == false);
    UTL_TEST_ASSERT(pArr->Empty( ) == false);

    delete pArr;
    delete pEmptyArr;
    delete pMemHelper;
    pArr = nullptr;
    pEmptyArr = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 
                    ARR_SIZE + 
                    TTL::DynamicArray<MemoryManagementHelper>::DefaultCapacity( ) + 
                    1);
    

    UTL_CLEANUP_ASSERT(pArr == nullptr);
    UTL_CLEANUP_ASSERT(pEmptyArr == nullptr);
    UTL_CLEANUP_ASSERT(pMemHelper == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::MoveAppend( )
{
    const size_t ARR_SIZE = 16;
    TTL::DynamicArray<MemoryManagementHelper>* pEmptyArr = nullptr;
    TTL::DynamicArray<MemoryManagementHelper>* pArr = nullptr;

    MemoryManagementHelper* pMemHelper = nullptr;

    try
    {
        pMemHelper = new MemoryManagementHelper;
        pArr = new TTL::DynamicArray<MemoryManagementHelper>(ARR_SIZE);
        pEmptyArr = new TTL::DynamicArray<MemoryManagementHelper>;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);
    UTL_SETUP_ASSERT(pMemHelper != nullptr);
    UTL_SETUP_ASSERT(pArr != nullptr);
    UTL_SETUP_ASSERT(pEmptyArr != nullptr);

    UTL_SETUP_ASSERT(pEmptyArr->Size( ) == 0);
    UTL_SETUP_ASSERT(pEmptyArr->Capacity( ) == 0);
    UTL_SETUP_ASSERT(pEmptyArr->Data( ) == nullptr);
    UTL_SETUP_ASSERT(pEmptyArr->Empty( ) == true);
    UTL_SETUP_ASSERT(pArr->Size( ) == 0);
    UTL_SETUP_ASSERT(pArr->Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(pArr->Data( ) != nullptr);
    UTL_SETUP_ASSERT(pArr->Empty( ) == true);

    try
    {
        pEmptyArr->Append(std::move(*pMemHelper));
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pEmptyArr->Size( ) == 1);
    UTL_TEST_ASSERT(pEmptyArr->Capacity( ) == TTL::DynamicArray<MemoryManagementHelper>::DefaultCapacity( ));
    UTL_TEST_ASSERT(pEmptyArr->Data( ) != nullptr);
    UTL_TEST_ASSERT(pEmptyArr->Data( )[0].GetCopy( ) == false);
    UTL_TEST_ASSERT(pEmptyArr->Data( )[0].GetMove( ) == true);
    UTL_TEST_ASSERT(pEmptyArr->Empty( ) == false);

    try
    {
        pArr->Append(std::move(*pMemHelper));
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pArr->Size( ) == 1);
    UTL_TEST_ASSERT(pArr->Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(pArr->Data( )[0].GetCopy( ) == false);
    UTL_TEST_ASSERT(pArr->Data( )[0].GetMove( ) == true);
    UTL_TEST_ASSERT(pArr->Empty( ) == false);

    delete pArr;
    delete pEmptyArr;
    delete pMemHelper;
    pArr = nullptr;
    pEmptyArr = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) ==
                    ARR_SIZE +
                    TTL::DynamicArray<MemoryManagementHelper>::DefaultCapacity( ) +
                    1);
    

    UTL_CLEANUP_ASSERT(pArr == nullptr);
    UTL_CLEANUP_ASSERT(pEmptyArr == nullptr);
    UTL_CLEANUP_ASSERT(pMemHelper == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::Front( )
{
    const size_t ARR_SIZE = 16;
    bool boundaryThrow = false;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    try
    {
        arr.Front( );
    }
    catch ( const std::out_of_range& )
    {
        boundaryThrow = true;
    }

    UTL_TEST_ASSERT(boundaryThrow == true);

    boundaryThrow = false;

    arr.Append(0);

    UTL_TEST_ASSERT(arr.Front( ) == 0);

    arr.Append(1);

    UTL_TEST_ASSERT(arr.Front( ) == 0);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::Back( )
{
    const size_t ARR_SIZE = 16;
    bool boundaryThrow = false;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    try
    {
        arr.Back( );
    }
    catch ( const std::out_of_range& )
    {
        boundaryThrow = true;
    }

    UTL_TEST_ASSERT(boundaryThrow == true);

    boundaryThrow = false;

    arr.Append(0);

    UTL_TEST_ASSERT(arr.Back( ) == 0);

    arr.Append(1);

    UTL_TEST_ASSERT(arr.Back( ) == 1);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::Compress( )
{
    const size_t ARR_SIZE = 16;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions Pt. 1
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    arr.Compress( );

    UTL_TEST_ASSERT(arr.Size( ) == 0);
    UTL_TEST_ASSERT(arr.Capacity( ) == 0);
    UTL_TEST_ASSERT(arr.Data( ) == nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == true);

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions Pt. 2
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    arr.Append(0);
    arr.Append(1);
    arr.Append(2);

    arr.Compress( );

    UTL_TEST_ASSERT(arr.Size( ) == 3);
    UTL_TEST_ASSERT(arr.Capacity( ) == arr.Size( ));
    UTL_TEST_ASSERT(arr.Data( ) != nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == false);

    const uint64 oldCapacity = arr.Capacity( );

    try
    {
        arr.Append(3);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(arr.Capacity( ) == ((oldCapacity * 3) >> 1));
    UTL_TEST_ASSERT(arr.Size( ) == 4);

    for ( size_t i = 0; i < arr.Size( ); i++ )
    {
        UTL_TEST_ASSERT(arr[i] == i);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::DynamicArray::Reserve( )
{
    const size_t ARR_SIZE = 16;
    const size_t RESERVE_SIZE = (ARR_SIZE * 3) >> 1;
    TTL::DynamicArray<uint64> arr;

    try
    {
        arr = std::move(TTL::DynamicArray<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions Pt. 1
    UTL_SETUP_ASSERT(arr.Size( ) == 0);
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == true);

    for ( size_t i = 0; i < ARR_SIZE >> 1; i++ )
    {
        arr.Append(i);
    }

    arr.Reserve(RESERVE_SIZE);

    UTL_TEST_ASSERT(arr.Size( ) == ARR_SIZE >> 1);
    UTL_TEST_ASSERT(arr.Capacity( ) == RESERVE_SIZE);
    UTL_TEST_ASSERT(arr.Data( ) != nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == false);

    for ( size_t i = 0; i < ARR_SIZE >> 1; i++ )
    {
        UTL_TEST_ASSERT(arr[i] == i);
    }

    TTL::DynamicArray<MemoryManagementHelper>* pMemHelperArr = nullptr;
    MemoryManagementHelper* pMemHelper = nullptr;

    try
    {
        pMemHelperArr = new TTL::DynamicArray<MemoryManagementHelper>(ARR_SIZE);
        pMemHelper = new MemoryManagementHelper;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions Pt. 2
    UTL_SETUP_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == 0);
    UTL_SETUP_ASSERT(pMemHelperArr->Size( ) == 0);
    UTL_SETUP_ASSERT(pMemHelperArr->Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(pMemHelperArr->Data( ) != nullptr);
    UTL_SETUP_ASSERT(pMemHelperArr->Empty( ) == true);

    for ( size_t i = 0; i < ARR_SIZE >> 1; i++ )
    {
        pMemHelperArr->Append(*pMemHelper);
    }

    for ( size_t i = 0; i < ARR_SIZE >> 1; i++ )
    {
        UTL_SETUP_ASSERT((*pMemHelperArr)[i].GetCopy( ) == true);
        UTL_SETUP_ASSERT((*pMemHelperArr)[i].GetMove( ) == false);
    }

    pMemHelperArr->Reserve(RESERVE_SIZE);

    UTL_TEST_ASSERT(pMemHelperArr->Size( ) == ARR_SIZE >> 1);
    UTL_TEST_ASSERT(pMemHelperArr->Capacity( ) == RESERVE_SIZE);
    UTL_TEST_ASSERT(pMemHelperArr->Data( ) != nullptr);
    UTL_TEST_ASSERT(pMemHelperArr->Empty( ) == false);

    for ( size_t i = 0; i < ARR_SIZE >> 1; i++ )
    {
        UTL_TEST_ASSERT((*pMemHelperArr)[i].GetCopy( ) == false);
        UTL_TEST_ASSERT((*pMemHelperArr)[i].GetMove( ) == true);
    }

    delete pMemHelperArr;
    delete pMemHelper;
    pMemHelperArr = nullptr;
    pMemHelper = nullptr;

    UTL_TEST_ASSERT(MemoryManagementHelper::ResetDeleteCount( ) == ARR_SIZE + RESERVE_SIZE + 1);
    

    UTL_CLEANUP_ASSERT(pMemHelperArr == nullptr);
    UTL_CLEANUP_ASSERT(pMemHelper == nullptr);

    UTL_TEST_SUCCESS( );
}