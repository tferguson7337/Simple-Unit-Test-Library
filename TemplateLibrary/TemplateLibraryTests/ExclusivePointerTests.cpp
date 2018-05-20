#include "ExclusivePointerTests.h"

#include <ExclusivePointer.hpp>


std::list<std::function<UnitTestResult(void)>> TTLTests::ExclusivePointer::BuildTestList( )
{
    static const std::list<std::function<UnitTestResult(void)>> testList =
    {
        // Constructor Tests
        DefaultConstructor,
        NullConstructor,
        RawConstructor,
        MoveConstructor,

        // Bool and Relational Operator Tests
        BoolOperator,
        EquivalenceOperator,
        NonEquivalenceOperator,
        LessThanOperator,
        GreaterThanOperator,
        LessThanOrEqualOperator,
        GreaterThanOrEqualOperator,

        // Assignment Operator Overload Tests
        AssignmentOperatorRaw,
        AssignmentOperatorNull,
        AssignmentOperatorMove,
        AssignmentOperatorSelf,

        // Public Method Tests
        Release,
        SetNull,
        SetRaw,
        SetMove,
        SetSelf
    };

    return testList;
}


/// Constructor Tests \\\

UnitTestResult TTLTests::ExclusivePointer::DefaultConstructor( )
{
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    // Ensure default constructor initializes with nullptr.
    UTL_TEST_ASSERT(numPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::NullConstructor( )
{
    TTL::ExclusivePointer<int> numPtr(nullptr);
    TTL::ExclusivePointer<int[ ]> arrPtr(nullptr);

    // Ensure nullptr_t constructor initializes with nullptr.
    UTL_TEST_ASSERT(numPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::RawConstructor( )
{
    int* n = nullptr;
    int* arr = nullptr;

    try
    {
        n = new int;
        arr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(arr != nullptr);

    TTL::ExclusivePointer<int> numPtr(n);
    TTL::ExclusivePointer<int[ ]> arrPtr(arr);

    // Ensure raw pointer constructor initialized with raw pointer values.
    UTL_TEST_ASSERT(numPtr.Get( ) == n);
    UTL_TEST_ASSERT(arrPtr.Get( ) == arr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::MoveConstructor( )
{
    int* n = nullptr;
    int* arr = nullptr;
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        numPtr.Set(new int);
        arrPtr.Set(new int[16]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    n = numPtr.Get( );
    arr = arrPtr.Get( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) == n);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == arr);

    TTL::ExclusivePointer<int> moveNum(std::move(numPtr));
    TTL::ExclusivePointer<int[ ]> moveArr(std::move(arrPtr));

    // Ensure move constructor transferred resource ownership.
    UTL_TEST_ASSERT(numPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(moveNum.Get( ) == n);
    UTL_TEST_ASSERT(moveArr.Get( ) == arr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Bool/Relational Operator Tests \\\

UnitTestResult TTLTests::ExclusivePointer::BoolOperator( )
{
    TTL::ExclusivePointer<int> numPtr(nullptr);
    TTL::ExclusivePointer<int[ ]> arrPtr(nullptr);

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) == nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == nullptr);

    // Test operator bool() - nullptr
    UTL_TEST_ASSERT(!numPtr);
    UTL_TEST_ASSERT(!arrPtr);

    // Allocate and assign memory
    try
    {
        numPtr.Set(new int);
        arrPtr.Set(new int[16]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_ASSERT(e.what( ));
    }

    // Test operator bool() - not null
    UTL_TEST_ASSERT(numPtr);
    UTL_TEST_ASSERT(arrPtr);

    /// Test pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::EquivalenceOperator( )
{
    int* n = nullptr;

    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n == nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) == nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == nullptr);

    // Test operator==(ExclusivePtr<T1>, ExclusivePtr<T2>)
    UTL_TEST_ASSERT(numPtr == arrPtr);

    // Test operator==(ExclusivePtr<T>, const T* const)
    UTL_TEST_ASSERT(numPtr == n);

    // Test operator==(ExclusivePtr<T[ ]>, const T* const)
    UTL_TEST_ASSERT(arrPtr == n);

    // Test operator==(const T* const, ExclusivePtr<T>)
    UTL_TEST_ASSERT(n == numPtr);

    // Test operator==(const T* const, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(n == arrPtr);

    // Test operator==(ExclusivePtr<T>, std::nullptr_t)
    UTL_TEST_ASSERT(numPtr == nullptr);

    // Test operator==(ExclusivePtr<T[ ]>, std::nullptr_t)
    UTL_TEST_ASSERT(arrPtr == nullptr);

    // Test operator==(std::nullptr_t, ExclusivePtr<T>)
    UTL_TEST_ASSERT(nullptr == numPtr);

    // Test operator==(std::nullptr_t, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(nullptr == arrPtr);


    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::NonEquivalenceOperator( )
{
    int* n = nullptr;

    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        n = new int;
        numPtr = new int;
        arrPtr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);

    // Test operator!=(ExclusivePtr<T1>, ExclusivePtr<T2>)
    UTL_TEST_ASSERT(numPtr != arrPtr);

    // Test operator!=(ExclusivePtr<T>, const T* const)
    UTL_TEST_ASSERT(numPtr != n);

    // Test operator!=(ExclusivePtr<T[ ]>, const T* const)
    UTL_TEST_ASSERT(arrPtr != n);

    // Test operator!=(const T* const, ExclusivePtr<T>)
    UTL_TEST_ASSERT(n != numPtr);

    // Test operator!=(const T* const, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(n != arrPtr);

    // Test operator!=(ExclusivePtr<T>, std::nullptr_t)
    UTL_TEST_ASSERT(numPtr != nullptr);

    // Test operator!=(ExclusivePtr<T[ ]>, std::nullptr_t)
    UTL_TEST_ASSERT(arrPtr != nullptr);

    // Test operator!=(std::nullptr_t, ExclusivePtr<T>)
    UTL_TEST_ASSERT(nullptr != numPtr);

    // Test operator!=(std::nullptr_t, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(nullptr != arrPtr);

    delete n;
    n = nullptr;

    // Confirm Cleanup
    UTL_CLEANUP_ASSERT(n == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::LessThanOperator( )
{
    int* n = nullptr;

    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        n = new int;
        numPtr = new int;
        arrPtr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);

    // Test operator<(ExclusivePtr<T1>, ExclusivePtr<T2>)
    UTL_TEST_ASSERT(numPtr.Get( ) < arrPtr.Get( ) ? numPtr < arrPtr : arrPtr < numPtr);

    // Test operator<(ExclusivePtr<T>, const T* const)
    UTL_TEST_ASSERT(numPtr.Get( ) < n ? numPtr < n : n < numPtr);

    // Test operator<(ExclusivePtr<T[ ]>, const T* const)
    UTL_TEST_ASSERT(arrPtr.Get( ) < n ? arrPtr < n : n < arrPtr);

    // Test operator<(const T* const, ExclusivePtr<T>)
    UTL_TEST_ASSERT(!(numPtr.Get( ) < n ? n < numPtr : numPtr < n));

    // Test operator<(const T* const, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(!(arrPtr.Get( ) < n ? n < arrPtr : arrPtr < n));

    // Test operator<(ExclusivePtr<T>, std::nullptr_t)
    UTL_TEST_ASSERT(!(numPtr < nullptr));

    // Test operator<(ExclusivePtr<T[ ]>, std::nullptr_t)
    UTL_TEST_ASSERT(!(arrPtr < nullptr));

    // Test operator<(std::nullptr_t, ExclusivePtr<T>)
    UTL_TEST_ASSERT(nullptr < numPtr);

    // Test operator<(std::nullptr_t, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(nullptr < arrPtr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::GreaterThanOperator( )
{
    int* n = nullptr;

    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        n = new int;
        numPtr = new int;
        arrPtr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);

    // Test operator>(ExclusivePtr<T1>, ExclusivePtr<T2>)
    UTL_TEST_ASSERT(numPtr.Get( ) > arrPtr.Get( ) ? numPtr > arrPtr : arrPtr > numPtr);

    // Test operator>(ExclusivePtr<T>, const T* const)
    UTL_TEST_ASSERT(numPtr.Get( ) > n ? numPtr > n : n > numPtr);

    // Test operator>(ExclusivePtr<T[ ]>, const T* const)
    UTL_TEST_ASSERT(arrPtr.Get( ) > n ? arrPtr > n : n > arrPtr);

    // Test operator>(const T* const, ExclusivePtr<T>)
    UTL_TEST_ASSERT(!(numPtr.Get( ) > n ? n > numPtr : numPtr > n));

    // Test operator>(const T* const, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(!(arrPtr.Get( ) > n ? n > arrPtr : arrPtr > n));

    // Test operator>(ExclusivePtr<T>, std::nullptr_t)
    UTL_TEST_ASSERT(numPtr > nullptr);

    // Test operator>(ExclusivePtr<T[ ]>, std::nullptr_t)
    UTL_TEST_ASSERT(arrPtr > nullptr);

    // Test operator>(std::nullptr_t, ExclusivePtr<T>)
    UTL_TEST_ASSERT(!(nullptr > numPtr));

    // Test operator>(std::nullptr_t, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(!(nullptr > arrPtr));

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::LessThanOrEqualOperator( )
{
    int* n = nullptr;
    int* arr = nullptr;

    TTL::ExclusivePointer<int> numPtr, numCopy;
    TTL::ExclusivePointer<int[ ]> arrPtr, arrCopy;

    try
    {
        n = new int;
        arr = new int[16];
        numPtr = new int;
        numCopy = n;
        arrPtr = new int[16];
        arrCopy = arr;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(arr != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(numCopy.Get( ) == n);
    UTL_SETUP_ASSERT(arrCopy.Get( ) == arr);

    // Test operator<=(ExclusivePtr<T1>, ExclusivePtr<T2>)
    UTL_TEST_ASSERT(numPtr.Get( ) <= arrPtr.Get( ) ? numPtr <= arrPtr : arrPtr <= numPtr);

    // Test operator<=(ExclusivePtr<T>, const T* const)
    UTL_TEST_ASSERT(numPtr.Get( ) <= n ? numPtr <= n : n <= numPtr);

    // Test operator<=(ExclusivePtr<T[ ]>, const T* const)
    UTL_TEST_ASSERT(arrPtr.Get( ) <= n ? arrPtr <= n : n <= arrPtr);

    // Test operator<=(const T* const, ExclusivePtr<T>)
    UTL_TEST_ASSERT(!(numPtr.Get( ) <= n ? n <= numPtr : numPtr <= n));

    // Test operator<=(const T* const, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(!(arrPtr.Get( ) <= n ? n <= arrPtr : arrPtr <= n));

    // Test operator<=(ExclusivePtr<T>, std::nullptr_t)
    UTL_TEST_ASSERT(!(numPtr <= nullptr));

    // Test operator<=(ExclusivePtr<T[ ]>, std::nullptr_t)
    UTL_TEST_ASSERT(!(arrPtr <= nullptr));

    // Test operator<=(std::nullptr_t, ExclusivePtr<T>)
    UTL_TEST_ASSERT(nullptr <= numPtr);

    // Test operator<=(std::nullptr_t, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(nullptr <= arrPtr);

    // Focus on "equal to" part of "less than or equal to"
    UTL_TEST_ASSERT(numCopy <= n);
    UTL_TEST_ASSERT(n <= numCopy);
    UTL_TEST_ASSERT(arrCopy <= arr);
    UTL_TEST_ASSERT(arr <= arrCopy);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::GreaterThanOrEqualOperator( )
{
    int* n = nullptr;
    int* arr = nullptr;

    TTL::ExclusivePointer<int> numPtr, numCopy;
    TTL::ExclusivePointer<int[ ]> arrPtr, arrCopy;

    try
    {
        n = new int;
        arr = new int[16];
        numPtr = new int;
        numCopy = n;
        arrPtr = new int[16];
        arrCopy = arr;
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(arr != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(numCopy.Get( ) == n);
    UTL_SETUP_ASSERT(arrCopy.Get( ) == arr);

    // Test operator>=(ExclusivePtr<T1>, ExclusivePtr<T2>)
    UTL_TEST_ASSERT(numPtr.Get( ) >= arrPtr.Get( ) ? numPtr >= arrPtr : arrPtr >= numPtr);

    // Test operator>=(ExclusivePtr<T>, const T* const)
    UTL_TEST_ASSERT(numPtr.Get( ) >= n ? numPtr >= n : n >= numPtr);

    // Test operator>=(ExclusivePtr<T[ ]>, const T* const)
    UTL_TEST_ASSERT(arrPtr.Get( ) >= n ? arrPtr >= n : n >= arrPtr);

    // Test operator>=(const T* const, ExclusivePtr<T>)
    UTL_TEST_ASSERT(!(numPtr.Get( ) >= n ? n >= numPtr : numPtr >= n));

    // Test operator>=(const T* const, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(!(arrPtr.Get( ) >= n ? n >= arrPtr : arrPtr >= n));

    // Test operator>=(ExclusivePtr<T>, std::nullptr_t)
    UTL_TEST_ASSERT(numPtr >= nullptr);

    // Test operator>=(ExclusivePtr<T[ ]>, std::nullptr_t)
    UTL_TEST_ASSERT(arrPtr >= nullptr);

    // Test operator>=(std::nullptr_t, ExclusivePtr<T>)
    UTL_TEST_ASSERT(!(nullptr >= numPtr));

    // Test operator>=(std::nullptr_t, ExclusivePtr<T[ ]>)
    UTL_TEST_ASSERT(!(nullptr >= arrPtr));

    // Focus on "equal to" part of "greater than or equal to"
    UTL_TEST_ASSERT(numCopy >= n);
    UTL_TEST_ASSERT(n >= numCopy);
    UTL_TEST_ASSERT(arrCopy >= arr);
    UTL_TEST_ASSERT(arr >= arrCopy);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Assignment Operator Tests \\\

UnitTestResult TTLTests::ExclusivePointer::AssignmentOperatorRaw( )
{
    int* n = nullptr;
    int* arr = nullptr;
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        n = new int;
        arr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) == nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == nullptr);
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(arr != nullptr);

    // Test Raw Pointer Assignment
    numPtr = n;
    UTL_TEST_ASSERT(numPtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(numPtr.Get( ) == n);

    arrPtr = arr;
    UTL_TEST_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == arr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::AssignmentOperatorNull( )
{
    int* n = nullptr;
    int* arr = nullptr;
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        numPtr.Set(new int);
        arrPtr.Set(new int[16]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    n = numPtr.Get( );
    arr = arrPtr.Get( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) == n);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == arr);

    // Test nullptr assignment
    numPtr = nullptr;
    arrPtr = nullptr;

    UTL_TEST_ASSERT(numPtr.Get( ) != n);
    UTL_TEST_ASSERT(numPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) != arr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::AssignmentOperatorMove( )
{
    TTL::ExclusivePointer<int> numPtr, moveNum;
    TTL::ExclusivePointer<int[ ]> arrPtr, moveArr;
    int* copyNum = nullptr;
    int* copyArr = nullptr;

    try
    {
        moveNum.Set(new int);
        moveArr.Set(new int[16]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    copyNum = moveNum.Get( );
    copyArr = moveArr.Get( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) == nullptr);
    UTL_SETUP_ASSERT(moveNum.Get( ) != nullptr);
    UTL_SETUP_ASSERT(copyNum == moveNum.Get( ));

    UTL_SETUP_ASSERT(arrPtr.Get( ) == nullptr);
    UTL_SETUP_ASSERT(moveArr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(copyArr == moveArr.Get( ));

    // Test ExclusivePointer Move Assignment
    numPtr = std::move(moveNum);
    UTL_TEST_ASSERT(moveNum.Get( ) == nullptr);
    UTL_TEST_ASSERT(numPtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(numPtr.Get( ) == copyNum);

    arrPtr = std::move(moveArr);
    UTL_TEST_ASSERT(moveArr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == copyArr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::AssignmentOperatorSelf( )
{
    bool selfAssignmentThrew = false;
    int* n = nullptr;
    int* arr = nullptr;
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        numPtr.Set(new int);
        arrPtr.Set(new int[16]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    n = numPtr.Get( );
    arr = arrPtr.Get( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(selfAssignmentThrew == false);
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) == n);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == arr);

    // Attempt Self Assignment - Non-Array
    try
    {
        numPtr = std::move(numPtr);
    }
    catch ( const std::exception& )
    {
        selfAssignmentThrew = true;
    }

    // Ensure self-assignment threw exception.
    UTL_TEST_ASSERT(selfAssignmentThrew == true);
    // Ensure data was preserved.
    UTL_TEST_ASSERT(numPtr.Get( ) == n);

    // Setup for Array Pointer Self Assignment
    selfAssignmentThrew = false;
    UTL_SETUP_ASSERT(selfAssignmentThrew == false);

    // Attempt Self Assignment - Array
    try
    {
        arrPtr = std::move(arrPtr);
    }
    catch ( const std::exception& )
    {
        selfAssignmentThrew = true;
    }

    // Ensure self-assignment threw exception.
    UTL_TEST_ASSERT(selfAssignmentThrew == true);
    // Ensure data was preserved.
    UTL_TEST_ASSERT(arrPtr.Get( ) == arr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}


/// Public Method Tests \\\

UnitTestResult TTLTests::ExclusivePointer::Release( )
{
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        numPtr = new int;
        arrPtr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);

    numPtr.Release( );
    arrPtr.Release( );

    // Ensure release actually released resources.
    UTL_TEST_ASSERT(numPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::SetNull( )
{
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        numPtr = new int;
        arrPtr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);

    numPtr.Set(nullptr);
    arrPtr.Set(nullptr);

    // Ensure set actually modified the object.
    UTL_TEST_ASSERT(numPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::SetRaw( )
{
    int* n = nullptr;
    int* arr = nullptr;

    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        n = new int;
        arr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(n != nullptr);
    UTL_SETUP_ASSERT(arr != nullptr);

    numPtr.Set(n);
    arrPtr.Set(arr);

    // Ensure set actually modified the object.
    UTL_TEST_ASSERT(numPtr.Get( ) == n);
    UTL_TEST_ASSERT(arrPtr.Get( ) == arr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::SetMove( )
{
    int* n = nullptr;
    int* arr = nullptr;

    TTL::ExclusivePointer<int> numPtr, moveNum;
    TTL::ExclusivePointer<int[ ]> arrPtr, moveArr;

    try
    {
        numPtr = new int;
        arrPtr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    n = numPtr.Get( );
    arr = arrPtr.Get( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) == n);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == arr);

    moveNum.Set(std::move(numPtr));
    moveArr.Set(std::move(arrPtr));

    // Ensure ownership of resources were transferred.
    UTL_TEST_ASSERT(numPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(moveNum.Get( ) != nullptr);
    UTL_TEST_ASSERT(moveNum.Get( ) == n);
    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(moveArr.Get( ) != nullptr);
    UTL_TEST_ASSERT(moveArr.Get( ) == arr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::ExclusivePointer::SetSelf( )
{
    bool selfSetThrew = false;
    int* n = nullptr;
    int* arr = nullptr;
    TTL::ExclusivePointer<int> numPtr;
    TTL::ExclusivePointer<int[ ]> arrPtr;

    try
    {
        numPtr = new int;
        arrPtr = new int[16];
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    n = numPtr.Get( );
    arr = arrPtr.Get( );

    // Confirm Initial Test Conditions
    UTL_SETUP_ASSERT(selfSetThrew == false);
    UTL_SETUP_ASSERT(numPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(numPtr.Get( ) == n);
    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Get( ) == arr);

    // Attempt Self Set - Non-Array
    try
    {
        numPtr.Set(std::move(numPtr));
    }
    catch ( const std::exception& )
    {
        selfSetThrew = true;
    }

    // Ensure self-set threw exception.
    UTL_TEST_ASSERT(selfSetThrew == true);
    // Ensure data was preserved.
    UTL_TEST_ASSERT(numPtr.Get( ) == n);

    // Setup for Array Pointer Self Set
    selfSetThrew = false;
    UTL_SETUP_ASSERT(selfSetThrew == false);

    // Attempt Self Set - Array
    try
    {
        arrPtr.Set(std::move(arrPtr));
    }
    catch ( const std::exception& )
    {
        selfSetThrew = true;
    }

    // Ensure self-set threw exception.
    UTL_TEST_ASSERT(selfSetThrew == true);
    // Ensure data was preserved.
    UTL_TEST_ASSERT(arrPtr.Get( ) == arr);

    /// Test Pass!
    UTL_TEST_SUCCESS( );
}
