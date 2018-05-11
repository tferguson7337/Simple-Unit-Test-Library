#include "ArrayTests.h"

#include <Array.hpp>

std::list<std::function<UnitTestResult(void)>> TTLTests::Array::BuildTestList( )
{
    std::list<std::function<UnitTestResult(void)>> testList = 
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
        Clear
    };

    return testList;
}


// Constructor Tests
UnitTestResult TTLTests::Array::DefaultConstructor( )
{
    TTL::Array<uint64> emptyArr;

    UTL_TEST_ASSERT(emptyArr.Capacity( ) == 0ull);
    UTL_TEST_ASSERT(emptyArr.Data( ) == nullptr);
    UTL_TEST_ASSERT(emptyArr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Array::CapacityConstructor( )
{
    const uint64 ARR_SIZE = 16ull;
    TTL::Array<uint64> arr(ARR_SIZE);
    TTL::Array<uint64> emptyArr(0ull);

    UTL_TEST_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(arr.Data( ) != nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == false);

    UTL_TEST_ASSERT(emptyArr.Capacity( ) == 0ull);
    UTL_TEST_ASSERT(emptyArr.Data( ) == nullptr);
    UTL_TEST_ASSERT(emptyArr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Array::CopyConstructor( )
{
    const uint64 ARR_SIZE = 16ull;
    TTL::Array<uint64> arr(ARR_SIZE);

    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        arr.Data( )[i] = i;
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == false);
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Data( )[i] == i);
    }

    TTL::Array<uint64> copyArr(arr);

    // Ensure data was copied.
    UTL_TEST_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(copyArr.Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(arr.Data( ) != nullptr);
    UTL_TEST_ASSERT(copyArr.Data( ) != nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == false);
    UTL_TEST_ASSERT(copyArr.Empty( ) == false);
    UTL_TEST_ASSERT(copyArr.Data( ) != arr.Data( ));
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(copyArr.Data( )[i] == arr.Data( )[i]);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Array::MoveConstructor( )
{
    const uint64 ARR_SIZE = 16ull;
    TTL::Array<uint64> arr(ARR_SIZE);
    uint64* pArr = arr.Data( );

    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        arr.Data( )[i] = i;
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Data( ) == pArr);
    UTL_SETUP_ASSERT(arr.Empty( ) == false);
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Data( )[i] == i);
    }

    TTL::Array<uint64> moveArr(std::move(arr));

    // Ensure data was moved.
    UTL_TEST_ASSERT(arr.Capacity( ) == 0);
    UTL_TEST_ASSERT(moveArr.Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(arr.Data( ) == nullptr);
    UTL_TEST_ASSERT(moveArr.Data( ) != nullptr);
    UTL_TEST_ASSERT(moveArr.Data( ) == pArr);
    UTL_TEST_ASSERT(arr.Empty( ) == true);
    UTL_TEST_ASSERT(moveArr.Empty( ) == false);
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(moveArr.Data( )[i] == i);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Assignment Tests
UnitTestResult TTLTests::Array::CopyAssignment( )
{
    const uint64 ARR_SIZE = 16ull;
    TTL::Array<uint64> arr(ARR_SIZE);
    TTL::Array<uint64> copyArr;

    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        arr.Data( )[i] = i;
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == false);
    UTL_SETUP_ASSERT(copyArr.Capacity( ) == 0ull);
    UTL_SETUP_ASSERT(copyArr.Data( ) == nullptr);
    UTL_SETUP_ASSERT(copyArr.Empty( ) == true);
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Data( )[i] == i);
    }

    copyArr = arr;

    // Ensure data was copied.
    UTL_TEST_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(copyArr.Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(arr.Data( ) != nullptr);
    UTL_TEST_ASSERT(copyArr.Data( ) != nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == false);
    UTL_TEST_ASSERT(copyArr.Empty( ) == false);
    UTL_TEST_ASSERT(copyArr.Data( ) != arr.Data( ));
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(copyArr.Data( )[i] == arr.Data( )[i]);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Array::MoveAssignment( )
{
    const uint64 ARR_SIZE = 16ull;
    TTL::Array<uint64> arr(ARR_SIZE);
    TTL::Array<uint64> moveArr;
    uint64* pArr = arr.Data( );

    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        arr.Data( )[i] = i;
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Data( ) == pArr);
    UTL_SETUP_ASSERT(arr.Empty( ) == false);
    UTL_SETUP_ASSERT(moveArr.Capacity( ) == 0ull);
    UTL_SETUP_ASSERT(moveArr.Data( ) == nullptr);
    UTL_SETUP_ASSERT(moveArr.Empty( ) == true);
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Data( )[i] == i);
    }

    moveArr = std::move(arr);

    // Ensure data was moved.
    UTL_TEST_ASSERT(arr.Capacity( ) == 0);
    UTL_TEST_ASSERT(moveArr.Capacity( ) == ARR_SIZE);
    UTL_TEST_ASSERT(arr.Data( ) == nullptr);
    UTL_TEST_ASSERT(moveArr.Data( ) != nullptr);
    UTL_TEST_ASSERT(moveArr.Data( ) == pArr);
    UTL_TEST_ASSERT(arr.Empty( ) == true);
    UTL_TEST_ASSERT(moveArr.Empty( ) == false);
    for ( uint64 i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(moveArr.Data( )[i] == i);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Subscript Test
UnitTestResult TTLTests::Array::Subscript( )
{
    const uint64 ARR_SIZE = 16ull;
    bool boundaryThrow = false;
    TTL::Array<uint64> emptyArr;
    TTL::Array<uint64> arr;

    try
    {
        arr = std::move(TTL::Array<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == false);
    UTL_SETUP_ASSERT(emptyArr.Capacity( ) == 0ull);
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

    uint64 idx = 0;
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
    UTL_TEST_ASSERT(idx == ARR_SIZE);

    for ( idx = 0; idx < ARR_SIZE; idx++ )
    {
        UTL_TEST_ASSERT(arr[idx] == idx);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


// Public Method Tests
UnitTestResult TTLTests::Array::At( )
{
    const uint64 ARR_SIZE = 16ull;
    bool boundaryThrow = false;
    TTL::Array<uint64> emptyArr;
    TTL::Array<uint64> arr;

    try
    {
        arr = std::move(TTL::Array<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == false);
    UTL_SETUP_ASSERT(emptyArr.Capacity( ) == 0ull);
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

    uint64 idx = 0;
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
    UTL_TEST_ASSERT(idx == ARR_SIZE);

    for ( idx = 0; idx < ARR_SIZE; idx++ )
    {
        UTL_TEST_ASSERT(arr.At(idx) == idx);
    }

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::Array::Clear( )
{
    const uint64 ARR_SIZE = 16ull;
    TTL::Array<uint64> arr;

    try
    {
        arr = std::move(TTL::Array<uint64>(ARR_SIZE));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(arr.Capacity( ) == ARR_SIZE);
    UTL_SETUP_ASSERT(arr.Data( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Empty( ) == false);

    arr.Clear( );

    UTL_TEST_ASSERT(arr.Capacity( ) == 0ull);
    UTL_TEST_ASSERT(arr.Data( ) == nullptr);
    UTL_TEST_ASSERT(arr.Empty( ) == true);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}