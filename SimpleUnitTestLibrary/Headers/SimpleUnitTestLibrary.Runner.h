#pragma once

#if !defined(SUTL_USE_MODULES)
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

            auto filteredSuiteRefView{
                Internal_::g_RuntimeSuiteRegistry
                | std::views::filter([this](const Suite* pSuite) constexpr { return pSuite->GetSuiteName().contains(m_SuiteNameFilterSV); })
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
