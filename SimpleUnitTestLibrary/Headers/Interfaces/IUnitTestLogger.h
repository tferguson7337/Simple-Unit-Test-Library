#pragma once

#include <filesystem>

/// Forward Declarations
template <class T>
class UnitTestLogger;

template <class T>
class TestSetData;

class UnitTestResult;

template <class T>
class IUnitTestLogger
{
public:
    IUnitTestLogger() = default;
    virtual ~IUnitTestLogger() = default;

    virtual UnitTestLogger<T>& operator=(UnitTestLogger<T>&&) noexcept = 0;

    virtual const std::filesystem::path& GetTargetFile() const = 0;
    virtual bool GetPrintToConsole() const noexcept = 0;
    virtual bool GetOnlyLogFailures() const noexcept = 0;

    virtual bool SetTargetFile(const std::filesystem::path&) = 0;
    virtual bool SetTargetFile(std::filesystem::path&&) = 0;
    virtual void SetPrintToConsole(const bool&) noexcept = 0;
    virtual void SetOnlyLogFailures(const bool&) noexcept = 0;

    virtual void LogTestSetHeader(const TestSetData<T>&) = 0;
    virtual void LogUnitTestResult(const UnitTestResult&) = 0;
    virtual void LogTestSetSummary(const TestSetData<T>&) = 0;
};
