#pragma once

// SAL
#include <sal.h>


// Support test methods that want to trigger different behavior
// or test data creation depending on none-some-all approach to test cases.
//
// E.g., can be used as a template argument for test-data creation methods,
//       which can then lead to different-sized test data sets to be created.
//
// Can also be used to include/exclude certain test behavior for special edge cases
// e.g., test special edge-case conditions for empty/full cases.
enum class TestQuantity : size_t
{
    None = 0,
    VeryLow,
    Low,
    MidLow,
    Mid,
    MidHigh,
    High,
    VeryHigh,
    All,

    End,
    Begin = 0
};

// Returns true if TQ is a valid TestQuantity enum value.
template <TestQuantity TQ>
static constexpr bool IsValidTestQuantity()
{
    return TQ >= TestQuantity::Begin && TQ <= TestQuantity::End;
}


// Prefix Incrementation
// Will increment up until tq == TestQuantity::End
_Ret_range_(TestQuantity::Begin, TestQuantity::End)
inline TestQuantity& operator++(_Inout_ TestQuantity& tq)
{
    if (tq < TestQuantity::Begin)
    {
        tq = TestQuantity::Begin;
    }
    else if (tq < TestQuantity::End)
    {
        tq = static_cast<TestQuantity>(static_cast<size_t>(tq) + 1);
    }

    return tq;
}

// Postfix Incrementation
// Will increment up until tq == TestQuantity::End
_Ret_range_(TestQuantity::Begin, TestQuantity::End)
inline TestQuantity operator++(_Inout_ TestQuantity& tq, _In_ int)
{
    TestQuantity old = (tq < TestQuantity::Begin) ? TestQuantity::Begin :
        (tq > TestQuantity::End) ? TestQuantity::End : tq;
    ++tq;
    return old;
}
