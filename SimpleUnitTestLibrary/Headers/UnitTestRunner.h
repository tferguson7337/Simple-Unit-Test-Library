#pragma once

#include "UnitTestLogger.h"
#include "Interfaces/IUnitTestRunner.h"

#include "TestSetData.h"

template <class T>
class UnitTestRunner final : public virtual IUnitTestRunner<T>
{
private:
    std::list<UnitTest> mUnitTests;
    UnitTestLogger<T> mLogger;
    TestSetData<T> mTestSetData;

public:
    /// Ctors \\\

    explicit UnitTestRunner(_In_ const std::basic_string<T>&);
    explicit UnitTestRunner(_In_ std::basic_string<T>&&) noexcept;

    /// Dtor \\\

    ~UnitTestRunner( ) = default;

    /// Operator Overloads \\\

    UnitTestRunner& operator=(_In_ UnitTestRunner&&) noexcept;

    /// Getters \\\

    const std::list<UnitTest>& GetUnitTests( ) const noexcept;
    const std::filesystem::path& GetLogFile( ) const noexcept;
    bool GetConsoleOutput( ) const noexcept;
    const TestSetData<T>& GetTestSetData( ) const noexcept;

    /// Setters \\\

    bool SetLogFile(_In_ const std::filesystem::path&);
    void SetConsoleOutput(_In_ const bool&);

    /// Public Methods \\\

    bool AddUnitTest(_In_ UnitTest&&);
    bool AddUnitTest(_In_ std::function<UnitTestResult(void)>&&);
    bool AddUnitTests(_In_ std::list<UnitTest>&&);
    bool AddUnitTests(_In_ std::list<std::function<UnitTestResult(void)>>&&);

    bool RunUnitTests( );
};
