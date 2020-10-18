#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <functional>
#include <list>

// SUTL
#include "TestTypes.h"
#include "TestSetData.h"
#include "UnitTest.h"
#include "UnitTestLogger.h"


class UnitTestRunner
{
private:
    // Empty list used to ensure GetUnitTests remains no-throw.
    static std::list<UnitTest> ms_DummyList;

    std::list<UnitTest>* m_pUnitTests;
    mutable UnitTestLogger m_Logger;
    TestSetData m_TestSetData;

    void AllocateTestListIfNeeded()
    {
        if (!m_pUnitTests)
        {
            m_pUnitTests = new std::list<UnitTest>;
        }
    }

public:
    // Ctors //

    constexpr UnitTestRunner() noexcept :
        m_pUnitTests(nullptr)
    { }

    UnitTestRunner(_In_z_ const char* testSetName) noexcept :
        m_pUnitTests(nullptr),
        m_TestSetData(testSetName)
    { }

    constexpr UnitTestRunner(_In_z_count_(testSetNameLen) const char* testSetName, _In_ const size_t testSetNameLen) noexcept :
        m_pUnitTests(nullptr),
        m_TestSetData(testSetName, testSetNameLen)
    { }

    // Dtor //

    ~UnitTestRunner() noexcept
    {
        delete m_pUnitTests;
    }

    // Getters //

    constexpr const std::list<UnitTest>& GetUnitTests() const noexcept
    {
        return (!!m_pUnitTests) ? *m_pUnitTests : ms_DummyList;
    }

    constexpr UnitTestLogger& GetLogger() const noexcept
    {
        return m_Logger;
    }

    constexpr TestSetData& GetTestSetData() noexcept
    {
        return m_TestSetData;
    }
    
    constexpr const TestSetData& GetTestSetData() const noexcept
    {
        return m_TestSetData;
    }

    // Public Methods //

    _Success_(return) bool AddUnitTest(_Inout_ UnitTest&& unitTest) noexcept;
    _Success_(return) bool AddUnitTest(_In_ const UnitTestFunction pfFunc) noexcept;
    _Success_(return) bool AddUnitTest(_In_ const std::function<UnitTestResult(void)>& func) noexcept;

    _Success_(return) bool AddUnitTests(_Inout_ std::list<UnitTest>&& unitTestList) noexcept;
    _Success_(return) bool AddUnitTests(_Inout_ std::list<UnitTestFunction>&& functionList) noexcept;
    _Success_(return) bool AddUnitTests(_Inout_ std::list<std::function<UnitTestResult(void)>>&& functionList) noexcept;

    constexpr void ClearUnitTests() noexcept
    {
        if (!!m_pUnitTests)
        {
            m_pUnitTests->clear();
        }
    }

    bool RunUnitTests() noexcept;
};
