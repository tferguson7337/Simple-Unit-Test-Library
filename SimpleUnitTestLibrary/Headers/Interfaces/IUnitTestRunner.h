#pragma once

#include "UnitTest.h"

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
    IUnitTestRunner( ) = default;
    virtual ~IUnitTestRunner( ) = default;

    virtual UnitTestRunner<T>& operator=(UnitTestRunner<T>&&) noexcept = 0;

    virtual const std::list<UnitTest>& GetUnitTests( ) const = 0;
    virtual const std::filesystem::path& GetLogFile( ) const = 0;
    virtual bool GetConsoleOutput( ) const = 0;

    virtual bool SetLogFile(const std::filesystem::path&) = 0;
    virtual void SetConsoleOutput(const bool&) = 0;

    virtual bool AddUnitTest(UnitTest&&) = 0;
    virtual bool RunUnitTests( ) = 0;
};
