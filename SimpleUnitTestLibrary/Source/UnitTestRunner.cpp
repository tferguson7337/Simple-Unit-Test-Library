#include <UnitTestRunner.h>

// Explicit Template Instantiation
template class UnitTestRunner<char>;
template class UnitTestRunner<wchar_t>;

/// Method Definitions \\\

/// Ctors \\\

template <class T>
UnitTestRunner<T>::UnitTestRunner(_In_ const std::basic_string<T>& testName) :
    mTestSetData(testName),
    mLogger()
{

}

template <class T>
UnitTestRunner<T>::UnitTestRunner(_In_ std::basic_string<T>&& testName) noexcept :
    mTestSetData(std::move(testName))
{
    
}

/// Operator Overloads \\\

template <class T>
UnitTestRunner<T>& UnitTestRunner<T>::operator=(_In_ UnitTestRunner&& src) noexcept
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
IUnitTestLogger<T>& UnitTestRunner<T>::GetLogger( ) const noexcept
{
    return mLogger;
}

template<class T>
TestSetData<T> & UnitTestRunner<T>::GetTestSetData( ) noexcept
{
    return mTestSetData;
}

template<class T>
const TestSetData<T> & UnitTestRunner<T>::GetTestSetData( ) const noexcept
{
    return mTestSetData;
}

// Public Methods

template <class T>
bool UnitTestRunner<T>::AddUnitTest(_In_ UnitTest&& test)
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
bool UnitTestRunner<T>::AddUnitTest(_In_ std::function<UnitTestResult(void)>&& test)
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
bool UnitTestRunner<T>::AddUnitTests(_In_ std::list<UnitTest>&& tests)
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
bool UnitTestRunner<T>::AddUnitTests(_In_ std::list<std::function<UnitTestResult(void)>>&& tests)
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
void UnitTestRunner<T>::ClearUnitTests( ) noexcept
{
    mUnitTests.clear( );
}

template <class T>
bool UnitTestRunner<T>::RunUnitTests( )
{
    bool ret = true;

    mLogger.InitializeWorkerThread( );

    mTestSetData.ResetCounters( );
    mTestSetData.SetTotalTestCount(static_cast<uint32_t>(mUnitTests.size( )));

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

        if ( r != ResultType::Success && r != ResultType::NotRun )
        {
            ret = false;
        }

        if ( r != ResultType::UnhandledException )
        {
            mLogger.LogUnitTestResult(test.GetUnitTestResult( ));
        }
        else
        {
            const std::string& funcName = test.GetUnitTestResult( ).GetFunctionName( );
            const std::string& fileName = test.GetUnitTestResult( ).GetFileName( );
            const uint32_t& line = test.GetUnitTestResult( ).GetLineNumber( );

            mLogger.LogUnitTestResult(UnitTestResult(r, funcName.c_str( ), funcName.size( ), fileName.c_str( ), fileName.size( ), line, eStr));
        }

        mTestSetData.IncrementResultCounter(r);
    }

    mLogger.LogTestSetSummary(mTestSetData);

    mLogger.TeardownWorkerThread( );

    return ret;
}