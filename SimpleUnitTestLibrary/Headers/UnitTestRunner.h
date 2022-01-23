#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <algorithm>
#include <cstdint>
#include <list>
#include <string_view>

// SUTL
#include "TestTypes.h"
#include "TestSetData.h"
#include "UnitTest.h"
#include "UnitTestLogger.h"


namespace SUTL
{
    class UnitTestRunner
    {
    private:

        std::list<UnitTest> m_UnitTestList;
        mutable UnitTestLogger m_Logger;
        TestSetData m_TestSetData;

    public:
        // Ctors //

        // NOTE: Provided string must be valid for the lifespan of this object.
        UnitTestRunner(_In_ std::string_view testSetName) noexcept :
            m_TestSetData(testSetName)
        { }

        // Dtor //

        ~UnitTestRunner() noexcept = default;

        // Getters //


        const std::list<UnitTest>& GetUnitTests() const noexcept
        {
            return m_UnitTestList;
        }

        UnitTestLogger& GetLogger() const noexcept
        {
            return m_Logger;
        }

        TestSetData& GetTestSetData() noexcept
        {
            return m_TestSetData;
        }

        const TestSetData& GetTestSetData() const noexcept
        {
            return m_TestSetData;
        }

        // Public Methods //

        void AddUnitTest(_Inout_ UnitTest&& unitTest) noexcept
        {
            m_UnitTestList.push_back(std::move(unitTest));
        }

        void AddUnitTest(_In_ UnitTestFunction func) noexcept
        {
            m_UnitTestList.emplace_back(std::move(func));
        }

        void AddUnitTests(_Inout_ std::list<UnitTest>&& unitTestList) noexcept
        {
            m_UnitTestList.splice(m_UnitTestList.cend(), std::move(unitTestList));
        }

        void AddUnitTests(_Inout_ std::list<UnitTestFunction>&& functionList) noexcept
        {
            for (auto& func : functionList)
            {
                m_UnitTestList.emplace_back(std::move(func));
            }

            functionList.clear();
        }

        bool RunUnitTests() noexcept
        {
            m_TestSetData.ResetCounters();
            m_TestSetData.SetTotalTestCount(static_cast<uint32_t>(m_UnitTestList.size()));

            m_Logger.LogTestSetHeader(m_TestSetData);

            auto RunUnitTest = [this](UnitTest& test)
            {
                test.RunTest();
                const auto& testResult = test.GetUnitTestResult();
                m_TestSetData.IncrementResultCounter(testResult.GetResultType());
                m_TestSetData.AddToRunDurationMs(testResult.GetTestDurationMilliseconds());

                m_Logger.LogUnitTestResult(testResult);
                return !!testResult;
            };

            return std::ranges::all_of(m_UnitTestList, RunUnitTest);
        }
    };
}
