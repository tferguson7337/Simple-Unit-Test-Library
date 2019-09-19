#pragma once

#include "TestSetData.h"
#include "UnitTest.h"
#include "../Interfaces/IUnitTestLogger.h"

#include <filesystem>
#include <list>

/// Forward Declaration
template <class T>
class UnitTestRunner;

///
//
//  Class:      IUnitTestRunner
//  
//  Purpose:    Define public interface for UnitTestRunner class.
//
///
template <class T>
class IUnitTestRunner
{
public:
    IUnitTestRunner() = default;
    virtual ~IUnitTestRunner() = default;

    virtual const std::list<UnitTest>& GetUnitTests() const = 0;
    virtual IUnitTestLogger<T>& GetLogger() const noexcept = 0;
    virtual TestSetData<T>& GetTestSetData() noexcept = 0;
    virtual const TestSetData<T>& GetTestSetData() const noexcept = 0;

    virtual bool AddUnitTest(UnitTest&&) = 0;
    virtual bool AddUnitTest(std::function<UnitTestResult(void)>&&) = 0;

    virtual bool AddUnitTests(std::list<UnitTest>&&) = 0;
    virtual bool AddUnitTests(std::list<std::function<UnitTestResult(void)>>&&) = 0;

    virtual void ClearUnitTests() noexcept = 0;

    virtual bool RunUnitTests() = 0;
};
