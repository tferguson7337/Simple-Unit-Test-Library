#include "CountedPointer.hpp"

#include "CountedPointerTests.h"

std::list<std::function<UnitTestResult(void)>> TTLTests::CountedPointer::BuildTestList( )
{
    static const std::list<std::function<UnitTestResult(void)>> testList =
    {
        // Constructor Tests
        DefaultConstructor_Single,
        DefaultConstructor_Array,
        NullConstructor_Single,
        NullConstructor_Array,
        RawConstructor_Single,
        RawConstructor_Array,
        CopyConstructor_Single,
        CopyConstructor_Array,
        MoveConstructor_Single,
        MoveConstructor_Array,

        // Bool and Relational Operator Tests
        BoolOperator_Single,
        BoolOperator_Array,
        EquivalenceOperator_Single,
        EquivalenceOperator_Array,
        NonEquivalenceOperator_Single,
        NonEquivalenceOperator_Array,
        LessThanOperator_Single,
        LessThanOperator_Array,
        GreaterThanOperator_Single,
        GreaterThanOperator_Array,
        LessThanOrEqualOperator_Single,
        LessThanOrEqualOperator_Array,
        GreaterThanOrEqualOperator_Single,
        GreaterThanOrEqualOperator_Array,

        // Assignment Operator Overload Tests
        AssignmentOperatorRaw_Single,
        AssignmentOperatorRaw_Array,
        AssignmentOperatorNull_Single,
        AssignmentOperatorNull_Array,
        AssignmentOperatorCopy_Single,
        AssignmentOperatorCopy_Array,
        AssignmentOperatorMove_Single,
        AssignmentOperatorMove_Array,
        AssignmentOperatorSelf_Single,
        AssignmentOperatorSelf_Array,

        // Public Method Tests
        Release_Single,
        Release_Array,
        SetNull_Single,
        SetNull_Array,
        SetRaw_Single,
        SetRaw_Array,
        SetCopy_Single,
        SetCopy_Array,
        SetMove_Single,
        SetMove_Array,
        SetSelf_Single,
        SetSelf_Array
    };

    return testList;
}

// Constructor Tests
UnitTestResult TTLTests::CountedPointer::DefaultConstructor_Single( )
{
    TTL::CountedPointer<size_t> ptr;

    UTL_TEST_ASSERT(ptr.Get( ) == nullptr);
    UTL_TEST_ASSERT(ptr.Count( ) == 0);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::DefaultConstructor_Array( )
{
    TTL::CountedPointer<size_t[ ]> arrPtr;

    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Count( ) == 0);

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::NullConstructor_Single( )
{
    TTL::CountedPointer<size_t> ptr(nullptr);

    UTL_TEST_ASSERT(ptr.Get( ) == nullptr);
    UTL_TEST_ASSERT(ptr.Count( ) == 0);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::NullConstructor_Array( )
{
    TTL::CountedPointer<size_t[ ]> arrPtr(nullptr);

    UTL_TEST_ASSERT(arrPtr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arrPtr.Count( ) == 0);

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::RawConstructor_Single( )
{
    const size_t VAL = 1 << 10;
    size_t* p = nullptr;
    TTL::CountedPointer<size_t>* pPtr = nullptr;

    try
    {
        p = new size_t(VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(*p == VAL);

    try
    {
        pPtr = new TTL::CountedPointer<size_t>(p);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pPtr->Get( ) != nullptr);
    UTL_TEST_ASSERT(pPtr->Count( ) == 1);

    UTL_TEST_ASSERT(pPtr->Get( ) == p);
    UTL_TEST_ASSERT(*(pPtr->Get( )) == *p);
    UTL_TEST_ASSERT(*(pPtr->Get( )) == VAL);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::RawConstructor_Array( )
{
    const size_t ARR_SIZE = 10;
    size_t* arr = nullptr;
    TTL::CountedPointer<size_t[ ]>* pArr = nullptr;

    try
    {
        arr = new size_t[ARR_SIZE];

        for ( size_t i = 0; i < ARR_SIZE; i++ )
        {
            arr[i] = i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    try
    {
        pArr = new TTL::CountedPointer<size_t[ ]>(arr);
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(pArr->Get( ) != nullptr);
    UTL_TEST_ASSERT(pArr->Count( ) == 1);

    UTL_TEST_ASSERT(pArr->Get( ) == arr);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(pArr->Get( )[i] == arr[i]);
        UTL_TEST_ASSERT(pArr->Get( )[i] == i);
    }

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::CopyConstructor_Single( )
{
    const size_t VAL = 1 << 10;
    TTL::CountedPointer<size_t> ptr;

    try
    {
        ptr.Set(new size_t(VAL));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(ptr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(ptr.Count( ) == 1);
    UTL_SETUP_ASSERT(*(ptr.Get( )) == VAL);

    TTL::CountedPointer<size_t> copyPtr(ptr);

    UTL_TEST_ASSERT(copyPtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(copyPtr.Get( ) == ptr.Get( ));
    UTL_TEST_ASSERT(*copyPtr.Get( ) == *ptr.Get( ));
    UTL_TEST_ASSERT(copyPtr.Count( ) == 2);
    UTL_TEST_ASSERT(copyPtr.Count( ) == ptr.Count( ));

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::CopyConstructor_Array( )
{
    const size_t ARR_SIZE = 10;
    TTL::CountedPointer<size_t[ ]> arrPtr;

    try
    {
        arrPtr.Set(new size_t[ARR_SIZE]);
        for ( size_t i = 0; i < ARR_SIZE; i++ )
        {
            arrPtr.Get( )[i] = i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(arrPtr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arrPtr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arrPtr.Get( )[i] == i);
    }

    TTL::CountedPointer<size_t[ ]> copyPtr(arrPtr);

    UTL_TEST_ASSERT(copyPtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(copyPtr.Get( ) == arrPtr.Get( ));
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(copyPtr.Get( )[i] == arrPtr.Get( )[i]);
    }

    UTL_TEST_ASSERT(copyPtr.Count( ) == 2);
    UTL_TEST_ASSERT(copyPtr.Count( ) == arrPtr.Count( ));

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::MoveConstructor_Single( )
{
    const size_t VAL = 1 << 10;
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> ptr;

    try
    {
        ptr.Set(new size_t(VAL));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(ptr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(ptr.Count( ) == 1);
    UTL_SETUP_ASSERT(*(ptr.Get( )) == VAL);

    p = ptr.Get( );
    TTL::CountedPointer<size_t> movePtr(std::move(ptr));

    UTL_TEST_ASSERT(movePtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(ptr.Get( ) == nullptr);
    UTL_TEST_ASSERT(ptr.Count( ) == 0);
    UTL_TEST_ASSERT(movePtr.Get( ) == p);
    UTL_TEST_ASSERT(*(movePtr.Get( )) == *p);
    UTL_TEST_ASSERT(*(movePtr.Get( )) == VAL);
    UTL_TEST_ASSERT(movePtr.Count( ) == 1);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::MoveConstructor_Array( )
{
    const size_t ARR_SIZE = 10;
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t[ ]> arr;

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
        for ( size_t i = 0; i < ARR_SIZE; i++ )
        {
            arr.Get( )[i] = i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(arr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Get( )[i] == i);
    }

    p = arr.Get( );
    TTL::CountedPointer<size_t[ ]> movePtr(std::move(arr));

    UTL_TEST_ASSERT(movePtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(arr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arr.Count( ) == 0);
    UTL_TEST_ASSERT(movePtr.Get( ) == p);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(movePtr.Get( )[i] == p[i]);
        UTL_TEST_ASSERT(movePtr.Get( )[i] == i);
    }

    UTL_TEST_ASSERT(movePtr.Count( ) == 1);

    UTL_TEST_SUCCESS( );
}



// Bool and Relational Operator Tests
UnitTestResult TTLTests::CountedPointer::BoolOperator_Single( )
{
    TTL::CountedPointer<size_t> ptr;

    UTL_TEST_ASSERT(!ptr);

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(ptr);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::BoolOperator_Array( )
{
    const size_t ARR_SIZE = 10;
    TTL::CountedPointer<size_t> ptr;

    UTL_TEST_ASSERT(!ptr);

    try
    {
        ptr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(ptr);

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::EquivalenceOperator_Single( )
{
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> emptyPtr;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t>* pCopy = nullptr;
    TTL::CountedPointer<size_t> otherPtr;

    UTL_TEST_ASSERT(emptyPtr == nullptr);
    UTL_TEST_ASSERT(emptyPtr == ptr);
    UTL_TEST_ASSERT(emptyPtr == otherPtr);

    UTL_TEST_ASSERT(emptyPtr == emptyPtr);
    UTL_TEST_ASSERT(ptr == ptr);
    UTL_TEST_ASSERT(otherPtr == otherPtr);

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = ptr.Get( );
    UTL_TEST_ASSERT(ptr == p);
    UTL_TEST_ASSERT(ptr == ptr);
    UTL_TEST_ASSERT((ptr == nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr == ptr) == false);
    UTL_TEST_ASSERT((ptr == otherPtr) == false);

    try
    {
        pCopy = new TTL::CountedPointer<size_t>(ptr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(ptr == *pCopy);
    UTL_TEST_ASSERT(*pCopy == p);
    UTL_TEST_ASSERT(*pCopy == *pCopy);
    UTL_TEST_ASSERT((emptyPtr == *pCopy) == false);
    UTL_TEST_ASSERT((otherPtr == *pCopy) == false);

    try
    {
        otherPtr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT((otherPtr == nullptr) == false);
    UTL_TEST_ASSERT((otherPtr == emptyPtr) == false);
    UTL_TEST_ASSERT(otherPtr == otherPtr);
    UTL_TEST_ASSERT((otherPtr == ptr) == false);
    UTL_TEST_ASSERT((otherPtr == *pCopy) == false);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::EquivalenceOperator_Array( )
{
    const size_t* p = nullptr;
    const size_t ARR_SIZE = 10;
    TTL::CountedPointer<size_t[ ]> emptyPtr;
    TTL::CountedPointer<size_t[ ]> arr;
    TTL::CountedPointer<size_t[ ]>* pCopy = nullptr;
    TTL::CountedPointer<size_t[ ]> otherArr;

    UTL_TEST_ASSERT(emptyPtr == nullptr);
    UTL_TEST_ASSERT(emptyPtr == arr);
    UTL_TEST_ASSERT(emptyPtr == otherArr);

    UTL_TEST_ASSERT(emptyPtr == emptyPtr);
    UTL_TEST_ASSERT(arr == arr);
    UTL_TEST_ASSERT(otherArr == otherArr);

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = arr.Get( );
    UTL_TEST_ASSERT(arr == p);
    UTL_TEST_ASSERT(arr == arr);
    UTL_TEST_ASSERT((arr == nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr == arr) == false);
    UTL_TEST_ASSERT((arr == otherArr) == false);

    try
    {
        pCopy = new TTL::CountedPointer<size_t[ ]>(arr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(arr == *pCopy);
    UTL_TEST_ASSERT(p == *pCopy);
    UTL_TEST_ASSERT(*pCopy == *pCopy);
    UTL_TEST_ASSERT((*pCopy == nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr == *pCopy) == false);
    UTL_TEST_ASSERT((otherArr == *pCopy) == false);

    try
    {
        otherArr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT((otherArr == nullptr) == false);
    UTL_TEST_ASSERT((otherArr == emptyPtr) == false);
    UTL_TEST_ASSERT(otherArr == otherArr);
    UTL_TEST_ASSERT((otherArr == arr) == false);
    UTL_TEST_ASSERT((otherArr == *pCopy) == false);

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::NonEquivalenceOperator_Single( )
{
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> emptyPtr;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t>* pCopy = nullptr;
    TTL::CountedPointer<size_t> otherPtr;

    UTL_TEST_ASSERT((emptyPtr != nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr != ptr) == false);
    UTL_TEST_ASSERT((emptyPtr != otherPtr) == false);

    UTL_TEST_ASSERT((emptyPtr != emptyPtr) == false);
    UTL_TEST_ASSERT((ptr != ptr) == false);
    UTL_TEST_ASSERT((otherPtr != otherPtr) == false);

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = ptr.Get( );
    UTL_TEST_ASSERT(ptr != nullptr);
    UTL_TEST_ASSERT((ptr != ptr) == false);
    UTL_TEST_ASSERT((ptr != p) == false);
    UTL_TEST_ASSERT(emptyPtr != ptr);
    UTL_TEST_ASSERT(ptr != otherPtr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t>(ptr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(*pCopy != nullptr);
    UTL_TEST_ASSERT((*pCopy != *pCopy) == false);
    UTL_TEST_ASSERT((ptr != *pCopy) == false);
    UTL_TEST_ASSERT((*pCopy != p) == false);
    UTL_TEST_ASSERT(emptyPtr != *pCopy);
    UTL_TEST_ASSERT(*pCopy != otherPtr);

    try
    {
        otherPtr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(otherPtr != nullptr);
    UTL_TEST_ASSERT(otherPtr != emptyPtr);
    UTL_TEST_ASSERT((otherPtr != otherPtr) == false);
    UTL_TEST_ASSERT(otherPtr != p);
    UTL_TEST_ASSERT(otherPtr != ptr);
    UTL_TEST_ASSERT(otherPtr != *pCopy);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::NonEquivalenceOperator_Array( )
{
    const size_t ARR_SIZE = 10;
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t[ ]> emptyPtr;
    TTL::CountedPointer<size_t[ ]> arr;
    TTL::CountedPointer<size_t[ ]>* pCopy = nullptr;
    TTL::CountedPointer<size_t[ ]> otherArr;

    UTL_TEST_ASSERT((emptyPtr != nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr != arr) == false);
    UTL_TEST_ASSERT((emptyPtr != otherArr) == false);

    UTL_TEST_ASSERT((emptyPtr != emptyPtr) == false);
    UTL_TEST_ASSERT((arr != arr) == false);
    UTL_TEST_ASSERT((otherArr != otherArr) == false);

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = arr.Get( );
    UTL_TEST_ASSERT(arr != nullptr);
    UTL_TEST_ASSERT((arr != arr) == false);
    UTL_TEST_ASSERT((arr != p) == false);
    UTL_TEST_ASSERT(emptyPtr != arr);
    UTL_TEST_ASSERT(arr != otherArr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t[ ]>(arr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(*pCopy != nullptr);
    UTL_TEST_ASSERT((*pCopy != *pCopy) == false);
    UTL_TEST_ASSERT(*pCopy != emptyPtr);
    UTL_TEST_ASSERT((*pCopy != arr) == false);
    UTL_TEST_ASSERT((*pCopy != p) == false);
    UTL_TEST_ASSERT(*pCopy != otherArr);

    try
    {
        otherArr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(otherArr != nullptr);
    UTL_TEST_ASSERT(otherArr != emptyPtr);
    UTL_TEST_ASSERT((otherArr != otherArr) == false);
    UTL_TEST_ASSERT(otherArr != p);
    UTL_TEST_ASSERT(otherArr != arr);
    UTL_TEST_ASSERT(otherArr != *pCopy);

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::LessThanOperator_Single( )
{
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> emptyPtr;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t>* pCopy = nullptr;
    TTL::CountedPointer<size_t> otherPtr;

    UTL_TEST_ASSERT((emptyPtr < nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr < ptr) == false);
    UTL_TEST_ASSERT((emptyPtr < otherPtr) == false);

    UTL_TEST_ASSERT((emptyPtr < emptyPtr) == false);
    UTL_TEST_ASSERT((ptr < ptr) == false);
    UTL_TEST_ASSERT((otherPtr < otherPtr) == false);

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = ptr.Get( );
    UTL_TEST_ASSERT(nullptr < ptr);
    UTL_TEST_ASSERT(emptyPtr < ptr);
    UTL_TEST_ASSERT((ptr < p) == false);
    UTL_TEST_ASSERT((ptr < ptr) == false);
    UTL_TEST_ASSERT(otherPtr < ptr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t>(ptr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr < *pCopy);
    UTL_TEST_ASSERT(emptyPtr < *pCopy);
    UTL_TEST_ASSERT((*pCopy < *pCopy) == false);
    UTL_TEST_ASSERT((ptr < *pCopy) == false);
    UTL_TEST_ASSERT((p < *pCopy) == false);
    UTL_TEST_ASSERT(otherPtr < *pCopy);

    try
    {
        otherPtr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr < otherPtr);
    UTL_TEST_ASSERT(emptyPtr < otherPtr);
    UTL_TEST_ASSERT((otherPtr < otherPtr) == false);
    if ( ptr.Get( ) < otherPtr.Get( ) )
    {
        UTL_TEST_ASSERT(ptr < otherPtr);
        UTL_TEST_ASSERT(p < otherPtr);
    }
    else
    {
        UTL_TEST_ASSERT(otherPtr < ptr);
        UTL_TEST_ASSERT(otherPtr < p);
    }

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::LessThanOperator_Array( )
{
    const size_t ARR_SIZE = 10;
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t[ ]> emptyPtr;
    TTL::CountedPointer<size_t[ ]> arr;
    TTL::CountedPointer<size_t[ ]>* pCopy = nullptr;
    TTL::CountedPointer<size_t[ ]> otherArr;

    UTL_TEST_ASSERT((emptyPtr < nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr < arr) == false);
    UTL_TEST_ASSERT((emptyPtr < otherArr) == false);

    UTL_TEST_ASSERT((emptyPtr < emptyPtr) == false);
    UTL_TEST_ASSERT((arr < arr) == false);
    UTL_TEST_ASSERT((otherArr < otherArr) == false);

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = arr.Get( );
    UTL_TEST_ASSERT(nullptr < arr);
    UTL_TEST_ASSERT(emptyPtr < arr);
    UTL_TEST_ASSERT((arr < p) == false);
    UTL_TEST_ASSERT((arr < arr) == false);
    UTL_TEST_ASSERT(otherArr < arr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t[ ]>(arr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr < *pCopy);
    UTL_TEST_ASSERT(emptyPtr < *pCopy);
    UTL_TEST_ASSERT((*pCopy < *pCopy) == false);
    UTL_TEST_ASSERT((arr < *pCopy) == false);
    UTL_TEST_ASSERT((p < *pCopy) == false);
    UTL_TEST_ASSERT(otherArr < *pCopy);

    try
    {
        otherArr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr < otherArr);
    UTL_TEST_ASSERT(emptyPtr < otherArr);
    UTL_TEST_ASSERT((otherArr < otherArr) == false);
    if ( arr.Get( ) < otherArr.Get( ) )
    {
        UTL_TEST_ASSERT(arr < otherArr);
        UTL_TEST_ASSERT(p < otherArr);
    }
    else
    {
        UTL_TEST_ASSERT(otherArr < arr);
        UTL_TEST_ASSERT(otherArr < p);
    }

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::GreaterThanOperator_Single( )
{
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> emptyPtr;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t>* pCopy = nullptr;
    TTL::CountedPointer<size_t> otherPtr;

    UTL_TEST_ASSERT((emptyPtr > nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr > ptr) == false);
    UTL_TEST_ASSERT((emptyPtr > otherPtr) == false);

    UTL_TEST_ASSERT((emptyPtr > emptyPtr) == false);
    UTL_TEST_ASSERT((ptr > ptr) == false);
    UTL_TEST_ASSERT((otherPtr > otherPtr) == false);

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = ptr.Get( );
    UTL_TEST_ASSERT(ptr > nullptr);
    UTL_TEST_ASSERT(ptr > emptyPtr);
    UTL_TEST_ASSERT((ptr > p) == false);
    UTL_TEST_ASSERT((ptr > ptr) == false);
    UTL_TEST_ASSERT(ptr > otherPtr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t>(ptr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(*pCopy > nullptr);
    UTL_TEST_ASSERT(*pCopy > emptyPtr);
    UTL_TEST_ASSERT((*pCopy > *pCopy) == false);
    UTL_TEST_ASSERT((ptr > *pCopy) == false);
    UTL_TEST_ASSERT((p > *pCopy) == false);
    UTL_TEST_ASSERT(*pCopy > otherPtr);

    try
    {
        otherPtr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(otherPtr > nullptr);
    UTL_TEST_ASSERT(otherPtr > emptyPtr);
    UTL_TEST_ASSERT((otherPtr > otherPtr) == false);
    if ( otherPtr.Get( ) > ptr.Get( ) )
    {
        UTL_TEST_ASSERT(otherPtr > ptr);
        UTL_TEST_ASSERT(otherPtr > p);
    }
    else
    {
        UTL_TEST_ASSERT(ptr > otherPtr);
        UTL_TEST_ASSERT(p > otherPtr);
    }

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::GreaterThanOperator_Array( )
{
    const size_t ARR_SIZE = 10;
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t[ ]> emptyPtr;
    TTL::CountedPointer<size_t[ ]> arr;
    TTL::CountedPointer<size_t[ ]>* pCopy = nullptr;
    TTL::CountedPointer<size_t[ ]> otherArr;

    UTL_TEST_ASSERT((emptyPtr > nullptr) == false);
    UTL_TEST_ASSERT((emptyPtr > arr) == false);
    UTL_TEST_ASSERT((emptyPtr > otherArr) == false);

    UTL_TEST_ASSERT((emptyPtr > emptyPtr) == false);
    UTL_TEST_ASSERT((arr > arr) == false);
    UTL_TEST_ASSERT((otherArr > otherArr) == false);

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = arr.Get( );
    UTL_TEST_ASSERT(arr > nullptr);
    UTL_TEST_ASSERT(arr > emptyPtr);
    UTL_TEST_ASSERT((arr > p) == false);
    UTL_TEST_ASSERT((arr > arr) == false);
    UTL_TEST_ASSERT(arr > otherArr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t[ ]>(arr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(*pCopy > nullptr);
    UTL_TEST_ASSERT(*pCopy > emptyPtr);
    UTL_TEST_ASSERT((*pCopy > *pCopy) == false);
    UTL_TEST_ASSERT((*pCopy > arr) == false);
    UTL_TEST_ASSERT((*pCopy > p) == false);
    UTL_TEST_ASSERT(*pCopy > otherArr);

    try
    {
        otherArr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(otherArr > nullptr);
    UTL_TEST_ASSERT(otherArr > emptyPtr);
    UTL_TEST_ASSERT((otherArr > otherArr) == false);
    if ( otherArr.Get( ) > arr.Get( ) )
    {
        UTL_TEST_ASSERT(otherArr > arr);
        UTL_TEST_ASSERT(otherArr > p);
    }
    else
    {
        UTL_TEST_ASSERT(arr > otherArr);
        UTL_TEST_ASSERT(p > otherArr);
    }

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::LessThanOrEqualOperator_Single( )
{
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> emptyPtr;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t>* pCopy = nullptr;
    TTL::CountedPointer<size_t> otherPtr;

    UTL_TEST_ASSERT(emptyPtr <= nullptr);
    UTL_TEST_ASSERT(emptyPtr <= ptr);
    UTL_TEST_ASSERT(emptyPtr <= otherPtr);

    UTL_TEST_ASSERT(emptyPtr <= emptyPtr);
    UTL_TEST_ASSERT(ptr <= ptr);
    UTL_TEST_ASSERT(otherPtr <= otherPtr);

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = ptr.Get( );
    UTL_TEST_ASSERT(nullptr <= ptr);
    UTL_TEST_ASSERT(emptyPtr <= ptr);
    UTL_TEST_ASSERT(ptr <= p);
    UTL_TEST_ASSERT(ptr <= ptr);
    UTL_TEST_ASSERT(otherPtr <= ptr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t>(ptr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr <= *pCopy);
    UTL_TEST_ASSERT(emptyPtr <= *pCopy);
    UTL_TEST_ASSERT(*pCopy <= *pCopy);
    UTL_TEST_ASSERT(ptr <= *pCopy);
    UTL_TEST_ASSERT(p <= *pCopy);
    UTL_TEST_ASSERT(otherPtr <= *pCopy);

    try
    {
        otherPtr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr <= otherPtr);
    UTL_TEST_ASSERT(emptyPtr <= otherPtr);
    UTL_TEST_ASSERT(otherPtr <= otherPtr);
    if ( ptr.Get( ) <= otherPtr.Get( ) )
    {
        UTL_TEST_ASSERT(ptr <= otherPtr);
        UTL_TEST_ASSERT(p <= otherPtr);
    }
    else
    {
        UTL_TEST_ASSERT(otherPtr <= ptr);
        UTL_TEST_ASSERT(otherPtr <= p);
    }

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::LessThanOrEqualOperator_Array( )
{
    const size_t ARR_SIZE = 10;
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t[ ]> emptyPtr;
    TTL::CountedPointer<size_t[ ]> arr;
    TTL::CountedPointer<size_t[ ]>* pCopy = nullptr;
    TTL::CountedPointer<size_t[ ]> otherArr;

    UTL_TEST_ASSERT(emptyPtr <= nullptr);
    UTL_TEST_ASSERT(emptyPtr <= arr);
    UTL_TEST_ASSERT(emptyPtr <= otherArr);

    UTL_TEST_ASSERT(emptyPtr <= emptyPtr);
    UTL_TEST_ASSERT(arr <= arr);
    UTL_TEST_ASSERT(otherArr <= otherArr);

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = arr.Get( );
    UTL_TEST_ASSERT(nullptr <= arr);
    UTL_TEST_ASSERT(emptyPtr <= arr);
    UTL_TEST_ASSERT(arr <= p);
    UTL_TEST_ASSERT(arr <= arr);
    UTL_TEST_ASSERT(otherArr <= arr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t[ ]>(arr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr <= *pCopy);
    UTL_TEST_ASSERT(emptyPtr <= *pCopy);
    UTL_TEST_ASSERT(*pCopy <= *pCopy);
    UTL_TEST_ASSERT(arr <= *pCopy);
    UTL_TEST_ASSERT(p <= *pCopy);
    UTL_TEST_ASSERT(otherArr <= *pCopy);

    try
    {
        otherArr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(nullptr <= otherArr);
    UTL_TEST_ASSERT(emptyPtr <= otherArr);
    UTL_TEST_ASSERT(otherArr <= otherArr);
    if ( arr.Get( ) <= otherArr.Get( ) )
    {
        UTL_TEST_ASSERT(arr <= otherArr);
        UTL_TEST_ASSERT(p <= otherArr);
    }
    else
    {
        UTL_TEST_ASSERT(otherArr <= arr);
        UTL_TEST_ASSERT(otherArr <= p);
    }

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::GreaterThanOrEqualOperator_Single( )
{
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> emptyPtr;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t>* pCopy = nullptr;
    TTL::CountedPointer<size_t> otherPtr;

    UTL_TEST_ASSERT(emptyPtr >= nullptr);
    UTL_TEST_ASSERT(emptyPtr >= ptr);
    UTL_TEST_ASSERT(emptyPtr >= otherPtr);

    UTL_TEST_ASSERT(emptyPtr >= emptyPtr);
    UTL_TEST_ASSERT(ptr >= ptr);
    UTL_TEST_ASSERT(otherPtr >= otherPtr);

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = ptr.Get( );
    UTL_TEST_ASSERT(ptr >= nullptr);
    UTL_TEST_ASSERT(ptr >= emptyPtr);
    UTL_TEST_ASSERT(ptr >= p);
    UTL_TEST_ASSERT(ptr >= ptr);
    UTL_TEST_ASSERT(ptr >= otherPtr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t>(ptr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(*pCopy >= nullptr);
    UTL_TEST_ASSERT(*pCopy >= emptyPtr);
    UTL_TEST_ASSERT(*pCopy >= *pCopy);
    UTL_TEST_ASSERT(ptr >= *pCopy);
    UTL_TEST_ASSERT(p >= *pCopy);
    UTL_TEST_ASSERT(*pCopy >= otherPtr);

    try
    {
        otherPtr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(otherPtr >= nullptr);
    UTL_TEST_ASSERT(otherPtr >= emptyPtr);
    UTL_TEST_ASSERT(otherPtr >= otherPtr);
    if ( otherPtr.Get( ) >= ptr.Get( ) )
    {
        UTL_TEST_ASSERT(otherPtr >= ptr);
        UTL_TEST_ASSERT(otherPtr >= p);
    }
    else
    {
        UTL_TEST_ASSERT(ptr >= otherPtr);
        UTL_TEST_ASSERT(p >= otherPtr);
    }

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::GreaterThanOrEqualOperator_Array( )
{
    const size_t ARR_SIZE = 10;
    const size_t* p = nullptr;
    TTL::CountedPointer<size_t[ ]> emptyPtr;
    TTL::CountedPointer<size_t[ ]> arr;
    TTL::CountedPointer<size_t[ ]>* pCopy = nullptr;
    TTL::CountedPointer<size_t[ ]> otherArr;

    UTL_TEST_ASSERT(emptyPtr >= nullptr);
    UTL_TEST_ASSERT(emptyPtr >= arr);
    UTL_TEST_ASSERT(emptyPtr >= otherArr);

    UTL_TEST_ASSERT(emptyPtr >= emptyPtr);
    UTL_TEST_ASSERT(arr >= arr);
    UTL_TEST_ASSERT(otherArr >= otherArr);

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = arr.Get( );
    UTL_TEST_ASSERT(arr >= nullptr);
    UTL_TEST_ASSERT(arr >= emptyPtr);
    UTL_TEST_ASSERT(arr >= p);
    UTL_TEST_ASSERT(arr >= arr);
    UTL_TEST_ASSERT(arr >= otherArr);

    try
    {
        pCopy = new TTL::CountedPointer<size_t[ ]>(arr);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(*pCopy >= nullptr);
    UTL_TEST_ASSERT(*pCopy >= emptyPtr);
    UTL_TEST_ASSERT(*pCopy >= *pCopy);
    UTL_TEST_ASSERT(*pCopy >= arr);
    UTL_TEST_ASSERT(*pCopy >= p);
    UTL_TEST_ASSERT(*pCopy >= otherArr);

    try
    {
        otherArr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(otherArr >= nullptr);
    UTL_TEST_ASSERT(otherArr >= emptyPtr);
    UTL_TEST_ASSERT(otherArr >= otherArr);
    if ( otherArr.Get( ) >= arr.Get( ) )
    {
        UTL_TEST_ASSERT(otherArr >= arr);
        UTL_TEST_ASSERT(otherArr >= p);
    }
    else
    {
        UTL_TEST_ASSERT(arr >= otherArr);
        UTL_TEST_ASSERT(p >= otherArr);
    }

    UTL_TEST_SUCCESS( );
}



// Assignment Operator Overload Tests
UnitTestResult TTLTests::CountedPointer::AssignmentOperatorRaw_Single( )
{
    const size_t VAL = 1 << 10;
    size_t* p = nullptr;
    TTL::CountedPointer<size_t> ptr;

    try
    {
        p = new size_t(VAL);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(*p == VAL);

    try
    {
        ptr = p;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(ptr.Get( ) != nullptr);
    UTL_TEST_ASSERT(ptr.Get( ) == p);
    UTL_TEST_ASSERT(ptr.Count( ) == 1);
    UTL_TEST_ASSERT(*(ptr.Get( )) == VAL);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::AssignmentOperatorRaw_Array( )
{
    const size_t ARR_SIZE = 10;
    size_t* p = nullptr;
    TTL::CountedPointer<size_t[ ]> arr;

    try
    {
        p = new size_t[ARR_SIZE];
        for ( size_t i = 0; i < ARR_SIZE; i++ )
        {
            p[i] = i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(p[i] == i);
    }

    try
    {
        arr = p;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(arr.Get( ) != nullptr);
    UTL_TEST_ASSERT(arr.Get( ) == p);
    UTL_TEST_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(arr.Get( )[i] == p[i]);
        UTL_TEST_ASSERT(arr.Get( )[i] == i);
    }

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::AssignmentOperatorNull_Single( )
{
    TTL::CountedPointer<size_t> ptr;

    try
    {
        ptr.Set(new size_t(0));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(ptr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(ptr.Count( ) == 1);

    ptr = nullptr;

    UTL_TEST_ASSERT(ptr.Get( ) == nullptr);
    UTL_TEST_ASSERT(ptr.Count( ) == 0);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::AssignmentOperatorNull_Array( )
{
    const size_t ARR_SIZE = 10;
    TTL::CountedPointer<size_t> arr;

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(arr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Count( ) == 1);

    arr = nullptr;

    UTL_TEST_ASSERT(arr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arr.Count( ) == 0);

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::AssignmentOperatorCopy_Single( )
{
    const size_t VAL = 1 << 10;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t> copyPtr;

    bool threw = false;

    try
    {
        ptr.Set(new size_t(VAL));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(ptr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(*(ptr.Get( )) == VAL);
    UTL_SETUP_ASSERT(ptr.Count( ) == 1);

    try
    {
        ptr = ptr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);
    UTL_TEST_ASSERT(ptr.Get( ) != nullptr);
    UTL_TEST_ASSERT(*(ptr.Get( )) == VAL);
    UTL_TEST_ASSERT(ptr.Count( ) == 1);

    threw = false;
    try
    {
        copyPtr = ptr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(threw == false);

    UTL_TEST_ASSERT(ptr.Get( ) != nullptr);
    UTL_TEST_ASSERT(*(ptr.Get( )) == VAL);
    UTL_TEST_ASSERT(copyPtr.Get( ) == ptr.Get( ));
    UTL_TEST_ASSERT(*(copyPtr.Get( )) == *(ptr.Get( )));
    UTL_TEST_ASSERT(ptr.Count( ) == 2);
    UTL_TEST_ASSERT(copyPtr.Count( ) == ptr.Count( ));

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::AssignmentOperatorCopy_Array( )
{
    const size_t ARR_SIZE = 10;
    TTL::CountedPointer<size_t[ ]> arr;
    TTL::CountedPointer<size_t[ ]> copyArr;

    bool threw = false;

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
        for ( size_t i = 0; i < ARR_SIZE; i++ )
        {
            arr.Get( )[i] = i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(arr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Get( )[i] == i);
    }

    try
    {
        arr = arr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);
    UTL_TEST_ASSERT(arr.Get( ) != nullptr);
    UTL_TEST_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(arr.Get( )[i] == i);
    }

    threw = false;
    try
    {
        copyArr = arr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(threw == false);

    UTL_TEST_ASSERT(arr.Get( ) != nullptr);
    UTL_TEST_ASSERT(copyArr.Get( ) == arr.Get( ));
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(copyArr.Get( )[i] == arr.Get( )[i]);
        UTL_TEST_ASSERT(copyArr.Get( )[i] == i);
    }
    UTL_TEST_ASSERT(arr.Count( ) == 2);
    UTL_TEST_ASSERT(copyArr.Count( ) == arr.Count( ));

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::AssignmentOperatorMove_Single( )
{
    const size_t VAL = 1 << 10;

    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> ptr;
    TTL::CountedPointer<size_t> movePtr;

    bool threw = false;

    try
    {
        ptr.Set(new size_t(VAL));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = ptr.Get( );
    UTL_SETUP_ASSERT(ptr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(p == ptr.Get( ));
    UTL_SETUP_ASSERT(*(ptr.Get( )) == VAL);
    UTL_SETUP_ASSERT(ptr.Count( ) == 1);

    try
    {
        movePtr = std::move(ptr);
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(threw == false);

    UTL_TEST_ASSERT(ptr.Get( ) == nullptr);
    UTL_TEST_ASSERT(ptr.Count( ) == 0);

    UTL_TEST_ASSERT(movePtr.Get( ) != nullptr);
    UTL_TEST_ASSERT(movePtr.Get( ) == p);

    UTL_TEST_ASSERT(*(movePtr.Get( )) == *p);
    UTL_TEST_ASSERT(*(movePtr.Get( )) == VAL);
    
    UTL_TEST_ASSERT(movePtr.Count( ) == 1);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::AssignmentOperatorMove_Array( )
{
    const size_t ARR_SIZE = 10;

    const size_t* p = nullptr;
    TTL::CountedPointer<size_t> arr;
    TTL::CountedPointer<size_t> moveArr;

    bool threw = false;

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
        for ( size_t i = 0; i < ARR_SIZE; i++ )
        {
            arr.Get( )[i] = i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    p = arr.Get( );
    UTL_SETUP_ASSERT(arr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(p == arr.Get( ));
    UTL_SETUP_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Get( )[i] == i);
    }

    try
    {
        moveArr = std::move(arr);
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }
    catch ( const std::exception& e )
    {
        UTL_TEST_EXCEPTION(e.what( ));
    }

    UTL_TEST_ASSERT(threw == false);

    UTL_TEST_ASSERT(arr.Get( ) == nullptr);
    UTL_TEST_ASSERT(arr.Count( ) == 0);

    UTL_TEST_ASSERT(moveArr.Get( ) != nullptr);
    UTL_TEST_ASSERT(moveArr.Get( ) == p);

    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(moveArr.Get( )[i] == p[i]);
        UTL_TEST_ASSERT(moveArr.Get( )[i] == i);
    }

    UTL_TEST_ASSERT(moveArr.Count( ) == 1);

    UTL_TEST_SUCCESS( );
}


UnitTestResult TTLTests::CountedPointer::AssignmentOperatorSelf_Single( )
{
    const size_t VAL = 1 << 10;
    bool threw = false;
    TTL::CountedPointer<size_t> ptr;

    try
    {
        ptr = ptr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);

    threw = false;
    try
    {
        ptr = std::move(ptr);
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);

    try
    {
        ptr.Set(new size_t(VAL));
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(ptr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(*(ptr.Get( )) == VAL);
    UTL_SETUP_ASSERT(ptr.Count( ) == 1);

    threw = false;
    try
    {
        ptr = ptr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);
    UTL_TEST_ASSERT(ptr.Get( ) != nullptr);
    UTL_TEST_ASSERT(*(ptr.Get( )) == VAL);
    UTL_TEST_ASSERT(ptr.Count( ) == 1);

    threw = false;
    try
    {
        ptr = std::move(ptr);
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);
    UTL_TEST_ASSERT(ptr.Get( ) != nullptr);
    UTL_TEST_ASSERT(*(ptr.Get( )) == VAL);
    UTL_TEST_ASSERT(ptr.Count( ) == 1);

    UTL_TEST_SUCCESS( );
}

UnitTestResult TTLTests::CountedPointer::AssignmentOperatorSelf_Array( )
{
    const size_t ARR_SIZE = 10;
    bool threw = false;
    TTL::CountedPointer<size_t[ ]> arr;

    try
    {
        arr = arr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);

    threw = false;
    try
    {
        arr = std::move(arr);
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);

    try
    {
        arr.Set(new size_t[ARR_SIZE]);
        for ( size_t i = 0; i < ARR_SIZE; i++ )
        {
            arr.Get( )[i] = i;
        }
    }
    catch ( const std::exception& e )
    {
        UTL_SETUP_EXCEPTION(e.what( ));
    }

    UTL_SETUP_ASSERT(arr.Get( ) != nullptr);
    UTL_SETUP_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_SETUP_ASSERT(arr.Get( )[i] == i);
    }

    threw = false;
    try
    {
        arr = arr;
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);
    UTL_TEST_ASSERT(arr.Get( ) != nullptr);
    UTL_TEST_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(arr.Get( )[i] == i);
    }

    threw = false;
    try
    {
        arr = std::move(arr);
    }
    catch ( const std::invalid_argument& )
    {
        threw = true;
    }

    UTL_TEST_ASSERT(threw);
    UTL_TEST_ASSERT(arr.Get( ) != nullptr);
    UTL_TEST_ASSERT(arr.Count( ) == 1);
    for ( size_t i = 0; i < ARR_SIZE; i++ )
    {
        UTL_TEST_ASSERT(arr.Get( )[i] == i);
    }

    UTL_TEST_SUCCESS( );
}



// Public Method Tests
UnitTestResult TTLTests::CountedPointer::Release_Single( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}

UnitTestResult TTLTests::CountedPointer::Release_Array( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}


UnitTestResult TTLTests::CountedPointer::SetNull_Single( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}

UnitTestResult TTLTests::CountedPointer::SetNull_Array( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}


UnitTestResult TTLTests::CountedPointer::SetRaw_Single( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}

UnitTestResult TTLTests::CountedPointer::SetRaw_Array( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}


UnitTestResult TTLTests::CountedPointer::SetCopy_Single( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}

UnitTestResult TTLTests::CountedPointer::SetCopy_Array( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}


UnitTestResult TTLTests::CountedPointer::SetMove_Single( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}

UnitTestResult TTLTests::CountedPointer::SetMove_Array( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}


UnitTestResult TTLTests::CountedPointer::SetSelf_Single( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}

UnitTestResult TTLTests::CountedPointer::SetSelf_Array( )
{
    UTL_SKIP_TEST("Skipping test pending implementation.");
}

