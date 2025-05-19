#pragma once

#include "APIAnnotations.h"
#include "SimpleUnitTestLibrary.Result.h"
#include "SimpleUnitTestLibrary.Macros.h"

#include <concepts>
#include <functional>
#include <type_traits>


namespace SimpleUnitTestLibrary
{
    namespace Evaluators
    {
        struct Result;
    }
    

    namespace Concepts
    {
        template <typename ObjT>
        concept EvaluableAsBool = requires (const ObjT obj)
        {
            { !obj } -> std::convertible_to<bool>;
            { !!obj } -> std::convertible_to<bool>;
        };

        template <typename Obj1T, typename Obj2T>
        concept EqComparable = requires (const Obj1T obj1, const Obj2T obj2)
        {
            { obj1 == obj2 } -> std::convertible_to<bool>;
        };

        template <typename Obj1T, typename Obj2T>
        concept NeqComparable = requires (const Obj1T obj1, const Obj2T obj2)
        {
            { obj1 != obj2 } -> std::convertible_to<bool>;
        };

        template <typename Obj1T, typename Obj2T>
        concept LTComparable = requires (const Obj1T obj1, const Obj2T obj2)
        {
            { obj1 < obj2 } -> std::convertible_to<bool>;
        };

        template <typename Obj1T, typename Obj2T>
        concept LTEComparable = requires (const Obj1T obj1, const Obj2T obj2)
        {
            { obj1 <= obj2 } -> std::convertible_to<bool>;
        };

        template <typename Obj1T, typename Obj2T>
        concept GTComparable = requires (const Obj1T obj1, const Obj2T obj2)
        {
            { obj1 > obj2 } -> std::convertible_to<bool>;
        };

        template <typename Obj1T, typename Obj2T>
        concept GTEComparable = requires (const Obj1T obj1, const Obj2T obj2)
        {
            { obj1 >= obj2 } -> std::convertible_to<bool>;
        };

        template <typename ObjT>
        concept BoolComparable = requires (const ObjT obj)
        {
            { obj == true } -> std::convertible_to<bool>;
            { obj == false } -> std::convertible_to<bool>;
            { true == obj } -> std::convertible_to<bool>;
            { false == obj } -> std::convertible_to<bool>;
        };

        template <typename RetT>
        concept ReturnTypeIsResultOrVoid =
            std::same_as<RetT, SUTL::Result> || std::same_as<RetT, void>;

        template <typename EvaluationFnT, typename... ArgTs>
        concept ValidEvaluationFunction = requires (EvaluationFnT fn, ArgTs&&... args)
        {
            { std::invoke(fn, std::forward<ArgTs>(args)...) } -> ReturnTypeIsResultOrVoid;
        };

        template <typename EvaluatorT, typename... EvaluationArgTs>
        concept ValidEvaluator = requires (
            EvaluatorT evaluator,
            EvaluationArgTs&&... args)
        {
            { std::invoke(evaluator, std::forward<EvaluationArgTs>(args)...) } -> ReturnTypeIsResultOrVoid;
        };
    }



    namespace Evaluators
    {
        struct [[nodiscard]] Result
        {
            SUTL::Result m_Result;

            constexpr Result() noexcept = default;

            constexpr Result(
                _Inout_ SUTL::Result&& result) noexcept :
                m_Result{std::move(result)}
            { }

            template <typename FnT, typename... ArgTs>
            constexpr Result& AndThen(
                FnT&& fn,
                ArgTs&&... args) requires Concepts::ValidEvaluationFunction<FnT, ArgTs...>
            {
                // Short circuit if previous run failed.
                if ((m_Result.m_ResultType != ResultType::Success) &&
                    (m_Result.m_ResultType != ResultType::NotRun))
                {
                    return *this;
                }
                if (std::constructible_from<decltype(fn(std::forward<ArgTs>(args)...)), Result>)
                {
                    m_Result = fn(std::forward<ArgTs>(args)...);
                }
                return *this;
            }

            [[nodiscard]] operator SUTL::Result& () noexcept
            {
                return m_Result;
            }
        };

        struct IsFalse
        {
            template <Concepts::BoolComparable ObjT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const ObjT& obj) const //noexcept(noexcept(!obj))
            {
                SUTL_TEST_ASSERT(obj == false);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsFalse, bool>);

        struct IsTrue
        {
            template <Concepts::BoolComparable ObjT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const ObjT& obj) const noexcept(noexcept(!!obj))
            {
                SUTL_TEST_ASSERT(obj == true);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsTrue, bool>);

        struct IsNull
        {
            template <Concepts::EqComparable<std::nullptr_t> ObjT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const ObjT& obj) const noexcept(noexcept(obj == nullptr))
            {
                SUTL_TEST_ASSERT(obj == nullptr);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsNull, void*>);

        struct IsNotNull
        {
            template <Concepts::NeqComparable<std::nullptr_t> ObjT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const ObjT& obj) const noexcept(noexcept(obj != nullptr))
            {
                SUTL_TEST_ASSERT(obj != nullptr);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsNotNull, void*>);

        struct IsEqual
        {
            template <typename LhsT, typename RhsT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const LhsT& lhs,
                _In_ const RhsT& rhs) const noexcept(noexcept(lhs == rhs)) requires Concepts::EqComparable<LhsT, RhsT>
            {
                SUTL_TEST_ASSERT(lhs == rhs);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsEqual, void*, void*>);
        static_assert(Concepts::ValidEvaluator<IsEqual, int, short>);
        static_assert(Concepts::ValidEvaluator<IsEqual, float, double>);

        struct IsNotEqual
        {
            template <typename LhsT, typename RhsT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const LhsT& lhs,
                _In_ const RhsT& rhs) const noexcept(noexcept(lhs != rhs)) requires Concepts::NeqComparable<LhsT, RhsT>
            {
                SUTL_TEST_ASSERT(lhs != rhs);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsNotEqual, void*, void*>);
        static_assert(Concepts::ValidEvaluator<IsNotEqual, int, short>);
        static_assert(Concepts::ValidEvaluator<IsNotEqual, float, double>);

        struct IsLessThan
        {
            template <typename LhsT, typename RhsT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const LhsT& lhs,
                _In_ const RhsT& rhs) const noexcept(noexcept(lhs < rhs)) requires Concepts::LTComparable<LhsT, RhsT>
            {
                SUTL_TEST_ASSERT(lhs < rhs);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsLessThan, void*, void*>);
        static_assert(Concepts::ValidEvaluator<IsLessThan, int, short>);
        static_assert(Concepts::ValidEvaluator<IsLessThan, float, double>);

        struct IsLessThanOrEqualTo
        {
            template <typename LhsT, typename RhsT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const LhsT& lhs,
                _In_ const RhsT& rhs) const noexcept(noexcept(lhs <= rhs)) requires Concepts::LTEComparable<LhsT, RhsT>
            {
                SUTL_TEST_ASSERT(lhs <= rhs);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsLessThanOrEqualTo, void*, void*>);
        static_assert(Concepts::ValidEvaluator<IsLessThanOrEqualTo, int, short>);
        static_assert(Concepts::ValidEvaluator<IsLessThanOrEqualTo, float, double>);

        struct IsGreaterThan
        {
            template <typename LhsT, typename RhsT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const LhsT& lhs,
                _In_ const RhsT& rhs) const noexcept(noexcept(lhs > rhs)) requires Concepts::GTComparable<LhsT, RhsT>
            {
                SUTL_TEST_ASSERT(lhs > rhs);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsGreaterThan, void*, void*>);
        static_assert(Concepts::ValidEvaluator<IsGreaterThan, int, short>);
        static_assert(Concepts::ValidEvaluator<IsGreaterThan, float, double>);

        struct IsGreaterThanOrEqualTo
        {
            template <typename LhsT, typename RhsT>
            [[nodiscard]] constexpr SUTL::Result operator()(
                _In_ const LhsT& lhs,
                _In_ const RhsT& rhs) const noexcept(noexcept(lhs >= rhs)) requires Concepts::GTEComparable<LhsT, RhsT>
            {
                SUTL_TEST_ASSERT(lhs >= rhs);
                SUTL_TEST_SUCCESS();
            }
        };
        static_assert(Concepts::ValidEvaluator<IsGreaterThanOrEqualTo, void*, void*>);
        static_assert(Concepts::ValidEvaluator<IsGreaterThanOrEqualTo, int, short>);
        static_assert(Concepts::ValidEvaluator<IsGreaterThanOrEqualTo, float, double>);

        // Should work with other invocable types (e.g., lambdas) that return void/SUTL result
        static_assert(!Concepts::ValidEvaluator<decltype([]() { return true; })> );
        static_assert(Concepts::ValidEvaluator<decltype([]() { return; })> );
        static_assert(Concepts::ValidEvaluator<decltype([]() { SUTL_TEST_SUCCESS(); })> );
        static_assert(Concepts::ValidEvaluator<decltype([x = 0]() mutable { SUTL_TEST_ASSERT(x == 0); SUTL_TEST_SUCCESS(); })> );
        static_assert(Concepts::ValidEvaluator<decltype([](bool bFlag) { SUTL_TEST_ASSERT(bFlag); SUTL_TEST_SUCCESS(); }), bool>);
    }
}

namespace SUTL = SimpleUnitTestLibrary;
