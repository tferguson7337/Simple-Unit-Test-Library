#pragma once

#if !defined(SUTL_USE_MODULES)
#include <algorithm>
#include <ranges>
#include <string_view>
#include <vector>

#include "APIAnnotations.h"
#include "SimpleUnitTestLibrary.Suite.h"
#endif


namespace SimpleUnitTestLibrary
{
    struct [[nodiscard]] Runner
    {
        std::string_view m_SuiteNameFilterSV;

        [[nodiscard]] constexpr std::vector<Suite::RunResults> operator()() const
        {
            std::vector<Suite::RunResults> runResults;

            auto SuiteNameFilter = [this](const Suite* pSuite)
            {
                auto Predicate = [](const char lhs, const char rhs)
                {
                    auto ToLower = [](const char c)
                    {
                        if ('A' <= c && c <= 'Z')
                        {
                            return static_cast<char>(c | static_cast<char>(0x20));
                        }

                        return c;
                    };
                    return ToLower(lhs) == ToLower(rhs);
                };
                return std::ranges::contains_subrange(
                    pSuite->GetSuiteName(),
                    m_SuiteNameFilterSV,
                    Predicate);
            };

            auto filteredSuiteRefView{
                Internal_::g_RuntimeSuiteRegistry
                | std::views::filter(SuiteNameFilter)
                | std::views::transform([](const Suite* pSuite) static constexpr -> const Suite& { return *pSuite; })};

            for (const auto& suite : filteredSuiteRefView)
            {
                runResults.push_back(suite());
            }
            return runResults;
        }
    };
}

namespace SUTL = SimpleUnitTestLibrary;
