#ifndef _UNIT_TEST_RUNNER_H
#define _UNIT_TEST_RUNNER_H

#include <Uncopyable.h>
#include <UnitTestLogger.h>
#include <Interface\UnitTestRunnerInterface.h>

template <class T>
class UnitTestRunner final : public UnitTestRunnerInterface<T>, public Uncopyable
{
private:
    std::list<UnitTest> mUnitTests;
    UnitTestLogger<T> mLogger;

public:
    /// Ctors \\\

    UnitTestRunner(const std::basic_string<T>& = std::basic_string<T>( ), bool = true) noexcept;
    UnitTestRunner(UnitTestRunner&&) noexcept;

    /// Dtor \\\

    ~UnitTestRunner( ) = default;

    /// Operator Overloads \\\

    UnitTestRunner& operator=(UnitTestRunner&&) noexcept;

    /// Getters \\\

    const std::list<UnitTest>& GetUnitTests( ) const;
    const std::basic_string<T>& GetLogFile( ) const;
    bool GetConsoleOutput( ) const;

    /// Setters \\\

    bool SetLogFile(const std::basic_string<T>&);
    void SetConsoleOutput(bool);

    /// Public Methods \\\

    bool AddUnitTest(UnitTest&&);
    bool AddUnitTest(std::function<UnitTestResult(void)>&&);
    bool AddUnitTests(std::list<UnitTest>&&);
    bool AddUnitTests(std::list<std::function<UnitTestResult(void)>>&&);

    bool RunUnitTests( );
};


#endif // _UNIT_TEST_RUNNER_H