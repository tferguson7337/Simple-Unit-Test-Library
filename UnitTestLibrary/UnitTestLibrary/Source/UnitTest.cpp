#include <UnitTest.h>


/// Method Definitons \\\

/// Ctors \\\
    
UnitTest::UnitTest(const std::function<UnitTestResult(void)>& func) :
    mTestFuncPtr(std::make_unique<std::function<UnitTestResult(void)>>(func))
{

}

UnitTest::UnitTest(UnitTest&& src) noexcept
{
    *this = std::move(src);
    src.mTestFuncPtr.reset( );
    src.mTestResult = std::move(UnitTestResult( ));
}

// Operator Overloads

UnitTest& UnitTest::operator=(UnitTest&& src) noexcept
{
    mTestFuncPtr = std::move(src.mTestFuncPtr);
    mTestResult = std::move(src.mTestResult);

    return *this;
}

// Getters

std::function<UnitTestResult(void)> UnitTest::GetUnitTestFunction( ) const
{
    return (mTestFuncPtr) ? *mTestFuncPtr : std::function<UnitTestResult(void)>( );
}

const UnitTestResult& UnitTest::GetUnitTestResult( ) const noexcept
{
    return mTestResult;
}


// Setters

void UnitTest::SetUnitTestFunction(std::function<UnitTestResult(void)> func)
{
    mTestFuncPtr = std::move(std::make_unique<std::function<UnitTestResult(void)>>(std::move(func)));

    mTestResult = std::move(UnitTestResult( ));
}


// Public Methods

const UnitTestResult& UnitTest::RunTest( )
{
    return mTestResult = (mTestFuncPtr) ? (*mTestFuncPtr)() : UnitTestResult( );
}
