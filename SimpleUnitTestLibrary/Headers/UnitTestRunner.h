#pragma once

#include "../Common/Headers/Uncopyable.h"
#include "UnitTestLogger.h"
#include "Interfaces/IUnitTestRunner.h"

#include "TestSetData.h"

template <class T>
class UnitTestRunner final : public virtual IUnitTestRunner<T>, public Uncopyable
{
private:
    std::list<UnitTest> mUnitTests;
    UnitTestLogger<T> mLogger;
    TestSetData<T> mTestSetData;

public:
    /// Ctors \\\

    explicit UnitTestRunner(const std::basic_string<T>&);
    explicit UnitTestRunner(std::basic_string<T>&&) noexcept;

    /// Dtor \\\

    ~UnitTestRunner( ) = default;

    /// Operator Overloads \\\

    UnitTestRunner& operator=(UnitTestRunner&&) noexcept;

    /// Getters \\\

    const std::list<UnitTest>& GetUnitTests( ) const noexcept;
    const std::filesystem::path& GetLogFile( ) const noexcept;
    bool GetConsoleOutput( ) const noexcept;
    const TestSetData<T>& GetTestSetData( ) const noexcept;

    /// Setters \\\

    bool SetLogFile(const std::filesystem::path&);
    void SetConsoleOutput(bool);

    /// Public Methods \\\

    bool AddUnitTest(UnitTest&&);
    bool AddUnitTest(std::function<UnitTestResult(void)>&&);
    bool AddUnitTests(std::list<UnitTest>&&);
    bool AddUnitTests(std::list<std::function<UnitTestResult(void)>>&&);

    bool RunUnitTests( );
};
