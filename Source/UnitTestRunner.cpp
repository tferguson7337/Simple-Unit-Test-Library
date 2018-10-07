#include <UnitTestRunner.h>

// Explicit Template Instantiation
template class UnitTestRunner<char>;
template class UnitTestRunner<wchar_t>;

/// Method Definitions \\\

/// Ctors \\\

template <class T>
UnitTestRunner<T>::UnitTestRunner(const std::basic_string<T>& testName) :
    mTestSetData(testName),
    mLogger()
{

}

template <class T>
UnitTestRunner<T>::UnitTestRunner(std::basic_string<T>&& testName) noexcept :
    mTestSetData(std::move(testName))
{
    
}

/// Operator Overloads \\\

template <class T>
UnitTestRunner<T>& UnitTestRunner<T>::operator=(UnitTestRunner&& src) noexcept
{
    mUnitTests = std::move(src.mUnitTests);

    return *this;
}

// Getters

template <class T>
const std::list<UnitTest>& UnitTestRunner<T>::GetUnitTests( ) const noexcept
{
    return mUnitTests;
}

template <class T>
const std::filesystem::path& UnitTestRunner<T>::GetLogFile( ) const noexcept
{
    return mLogger.GetTargetFile( );
}

template <class T>
bool UnitTestRunner<T>::GetConsoleOutput( ) const noexcept
{
    return mLogger.GetPrintToConsole( );
}

template<class T>
const TestSetData<T> & UnitTestRunner<T>::GetTestSetData( ) const noexcept
{
    return mTestSetData;
}

// Setters

template <class T>
bool UnitTestRunner<T>::SetLogFile(const std::filesystem::path& log)
{
    return mLogger.SetTargetFile(log);
}

template <class T>
void UnitTestRunner<T>::SetConsoleOutput(bool consoleOut)
{
    mLogger.SetPrintToConsole(consoleOut);
}


// Public Methods

template <class T>
bool UnitTestRunner<T>::AddUnitTest(UnitTest&& test)
{
    try
    {
        mUnitTests.push_back(std::move(test));
    }
    catch ( ... )
    {
        return false;
    }

    return true;
}

template <class T>
bool UnitTestRunner<T>::AddUnitTest(std::function<UnitTestResult(void)>&& test)
{
    try
    {
        mUnitTests.emplace_back(std::move(test));
    }
    catch ( ... )
    {
        return false;
    }

    return true;
}

template <class T>
bool UnitTestRunner<T>::AddUnitTests(std::list<UnitTest>&& tests)
{
    try
    {
        mUnitTests.splice(mUnitTests.end( ), tests);
        tests.clear( );
    }
    catch ( ... )
    {
        return false;
    }

    return true;
}

template <class T>
bool UnitTestRunner<T>::AddUnitTests(std::list<std::function<UnitTestResult(void)>>&& tests)
{
    try
    {
        for ( auto& t : tests )
        {
            mUnitTests.emplace_back(std::move(t));
        }

        tests.clear( );
    }
    catch ( ... )
    {
        return false;
    }

    return true;
}

template <class T>
bool UnitTestRunner<T>::RunUnitTests( )
{
    bool ret = true;

    mTestSetData.ResetCounters( );
    mTestSetData.SetTotalTestCount(static_cast<uint32>(mUnitTests.size( )));

    mLogger.LogTestSetHeader(mTestSetData);

    for ( UnitTest& test : mUnitTests )
    {
        ResultType r = ResultType::NotRun;
        std::string eStr = "";

        try
        {
            test.RunTest( );
            r = test.GetUnitTestResult( ).GetResult( );
        }
        catch ( const std::exception& e )
        {
            r = ResultType::UnhandledException;
            eStr = e.what( );
        }
        catch ( ... )
        {
            r = ResultType::UnhandledException;
            eStr = "<Unknown Unhandled Exception>";
        }

        if ( r != ResultType::Success )
        {
            ret = false;
        }

        if ( r != ResultType::UnhandledException )
        {
            mLogger.LogUnitTestResult(test.GetUnitTestResult( ));
        }
        else
        {
            mLogger.LogUnitTestResult(UnitTestResult(r, test.GetUnitTestResult( ).GetFunctionName( ), "", 0, eStr));
        }

        mTestSetData.IncrementResultCounter(r);
    }

    mLogger.LogTestSetSummary(mTestSetData);

    return ret;
}