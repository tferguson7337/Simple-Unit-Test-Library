#pragma once

// STL
#include <type_traits>

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

    TQEnd,
    TQBegin = 0
};

// Prefix Incrementation
// Will increment up until tq == TestQuantity::TQEnd
inline TestQuantity& operator++(_Inout_ TestQuantity& tq)
{
    if (tq < TestQuantity::TQEnd)
    {
        tq = static_cast<TestQuantity>(static_cast<std::underlying_type_t<TestQuantity>>(tq) + 1);
    }

    return tq;
}

// Postfix Incrementation
// Will increment up until tq == TestQuantity::TQEnd
inline TestQuantity operator++(_Inout_ TestQuantity& tq, _In_ int)
{
    TestQuantity old = tq;
    ++tq;
    return old;
}


// Returns true if TQ is a valid TestQuantity enum value.
template <TestQuantity TQ>
static constexpr bool IsValidTestQuantity()
{
    return TQ >= TestQuantity::TQBegin && TQ <= TestQuantity::TQEnd;
}
