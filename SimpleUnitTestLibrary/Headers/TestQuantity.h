#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include <type_traits>


namespace SUTL
{
    // TestQuantity //

    // Support test methods that want to trigger different behavior
    // or test data creation depending on none-some-all approach to test cases.
    //
    // E.g., can be used as a template argument for test-data creation methods,
    //       which can then lead to different-sized test data sets to be created.
    //
    // Can also be used to include/exclude certain test behavior for special edge cases
    // e.g., test special edge-case conditions for empty/full cases.
    enum class TestQuantity : uint32_t
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

        // ! Internal use only !
        _Invalid,
        _End,
        _Last = _End - 2,
        _Begin = 0
    };


    // Prefix Incrementation
    // Note: No enum range checking is done - be on your best behavior.
    _Ret_range_(TestQuantity::_Begin, TestQuantity::_End)
        inline TestQuantity& operator++(_Inout_ TestQuantity& tq) noexcept
    {
        using UT = std::underlying_type_t<TestQuantity>;
        tq = static_cast<TestQuantity>(static_cast<UT>(tq) + 1);
        return tq;
    }


    // Postfix Incrementation
    // Note: No enum range checking is done - be on your best behavior.
    _Ret_range_(TestQuantity::_Begin, TestQuantity::_End)
        inline TestQuantity operator++(_Inout_ TestQuantity& tq, _In_ int) noexcept
    {
        using UT = std::underlying_type_t<TestQuantity>;
        ++tq;
        return static_cast<TestQuantity>(static_cast<UT>(tq) - 1);
    }


    // Prefix Decrementation
    // Note: No enum range checking is done - be on your best behavior.
    _Ret_range_(TestQuantity::_Begin, TestQuantity::_End)
        inline TestQuantity operator--(_Inout_ TestQuantity& tq) noexcept
    {
        using UT = std::underlying_type_t<TestQuantity>;
        tq = static_cast<TestQuantity>(static_cast<UT>(tq) - 1);
        return tq;
    }


    // Postfix Decrementation
    // Note: No enum range checking is done - be on your best behavior.
    _Ret_range_(TestQuantity::_Begin, TestQuantity::_End)
        inline TestQuantity operator--(_Inout_ TestQuantity& tq, _In_ int) noexcept
    {
        using UT = std::underlying_type_t<TestQuantity>;
        --tq;
        return static_cast<TestQuantity>(static_cast<UT>(tq) + 1);
    }


    class TestQuantityUtil
    {
    private:

        static constexpr size_t s_TestQuantityStringArrayLength = static_cast<size_t>(TestQuantity::_End);

        static constexpr std::array<std::string_view, s_TestQuantityStringArrayLength> s_TestQuantityStrings
        {
            "None",
            "VeryLow",
            "Low",
            "MidLow",
            "Mid",
            "MidHigh",
            "High",
            "VeryHigh",
            "All",
            "InvalidTestQuantity"
        };

    public:
        // Note: purely static class, intended to hide internal static data.
        TestQuantityUtil() = delete;
        ~TestQuantityUtil() = delete;
        TestQuantityUtil(const TestQuantityUtil&) = delete;
        TestQuantityUtil(TestQuantityUtil&&) = delete;
        TestQuantityUtil& operator=(const TestQuantityUtil&) = delete;
        TestQuantityUtil& operator=(TestQuantityUtil&&) = delete;


        template <TestQuantity TQ>
        static consteval bool IsValidTestQuantity() noexcept
        {
            return (TQ >= TestQuantity::_Begin) && (TQ <= TestQuantity::_Last);
        }


        static constexpr bool IsValidTestQuantity(_In_ const TestQuantity tq) noexcept
        {
            return (tq >= TestQuantity::_Begin) && (tq <= TestQuantity::_Last);
        }


        static constexpr std::string_view ToString(_In_range_(TestQuantity::_Begin, TestQuantity::_Last) TestQuantity tq) noexcept
        {
            if (!IsValidTestQuantity(tq))
            {
                tq = TestQuantity::_Invalid;
            }

            return s_TestQuantityStrings[static_cast<size_t>(tq)];
        }
    };
}
