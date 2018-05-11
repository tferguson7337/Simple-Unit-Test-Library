#ifndef _UNIT_TEST_INTERFACE_H
#define _UNIT_TEST_INTERFACE_H

#include <functional>

/// Forward Declaration
class UnitTestResult;

///
//
//  Class:      UnitTestInterface
//  
//  Purpose:    Define public interface for UnitTest class.
//
///
class UnitTestInterface
{
public:
    /// Ctor/Dtor \\\
    
    UnitTestInterface( ) = default;
    virtual ~UnitTestInterface( ) = default;

    /// Getters \\\

    virtual std::function<UnitTestResult(void)> GetUnitTestFunction( ) const = 0;
    virtual const UnitTestResult& GetUnitTestResult( ) const = 0;

    /// Setters \\\

    virtual void SetUnitTestFunction(std::function<UnitTestResult(void)>) = 0;

    /// Public Methods \\\

    virtual const UnitTestResult& RunTest( ) = 0;
};

#endif // _UNIT_TEST_INTERFACE_H