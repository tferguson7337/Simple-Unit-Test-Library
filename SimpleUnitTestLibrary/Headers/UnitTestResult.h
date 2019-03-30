#pragma once

#include "../Common/Headers/Macros.h"
#include "../Common/Headers/Types.h"

#include "ResultType.h"

#include <string>

///
//
//  Class:      UnitTestResult (final)
//
//  Purpose:    Encapsulates the result of a unit test.
//
///
class UnitTestResult final
{
private:
    /// Private Data Members \\\

    ResultType mResult;
    uint32 mLineNum;
    std::string mFuncName;
    std::string mFileName;
    std::string mResultInfo;

    using ExtractPredicate = bool(*)(const utf8&);
    static constexpr const utf8* ExtractNameCommon(_In_ const utf8*, _In_ const size_t&, _In_ ExtractPredicate) noexcept;
    static constexpr const utf8* ExtractFileName(_In_ const utf8*, _In_ const size_t&) noexcept;
    static constexpr const utf8* ExtractFuncName(_In_ const utf8*, _In_ const size_t&) noexcept;
    static constexpr bool IsPathSeparator(_In_ const utf8&) noexcept;
    static constexpr bool IsWhitespace(_In_ const utf8&) noexcept;

public:
    /// Ctors \\\

    UnitTestResult(
        _In_ ResultType result = ResultType::NotRun,
        _In_ const utf8* func = nullptr, _In_ const size_t& funcLen = 0,
        _In_ const utf8* file = nullptr, _In_ const size_t& fileLen = 0, _In_ const uint32& line = 0,
        _In_ const std::string& info = ""
    ) noexcept;

    // Move Ctor
    UnitTestResult(_In_ UnitTestResult&&) noexcept;

    /// Dtor \\\

    ~UnitTestResult( ) = default;

    /// Operator Overloads \\\

    UnitTestResult& operator=(_In_ UnitTestResult&&) noexcept;
    explicit operator bool( ) const noexcept;

    /// Getters \\\

    const ResultType& GetResult( ) const noexcept;
    const std::string& GetFunctionName( ) const noexcept;
    const std::string& GetFileName( ) const noexcept;
    const uint32& GetLineNumber( ) const noexcept;
    const std::string& GetResultInfo( ) const noexcept;

    /// Setters \\\

    void SetResult(_In_ const ResultType&) noexcept;
    void SetFunctionName(_In_ const std::string&);
    void SetFileName(_In_ const std::string&);
    void SetLineNumber(_In_ const uint32&) noexcept;
    void SetResultInfo(_In_ const std::string&);

    /// Unit Test Return Macros \\\

// Success
#define SUTL_TEST_SUCCESS( )        return UnitTestResult(ResultType::Success, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__)

// Failures - No Exception Thrown
#define SUTL_SETUP_FAILURE(str)     return UnitTestResult(ResultType::SetupFailure, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__, str)
#define SUTL_TEST_FAILURE(str)      return UnitTestResult(ResultType::TestFailure, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__, str)
#define SUTL_CLEANUP_FAILURE(str)   return UnitTestResult(ResultType::CleanupFailure, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__, str)

// Failures - Exception Caught
#define SUTL_SETUP_EXCEPTION(str)   return UnitTestResult(ResultType::SetupException, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__, str)
#define SUTL_TEST_EXCEPTION(str)    return UnitTestResult(ResultType::TestException, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__, str)
#define SUTL_CLEANUP_EXCEPTION(str) return UnitTestResult(ResultType::CleanupException, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__, str)

// Test Asserts
#define SUTL_SETUP_ASSERT(cond)     if (!!(cond) == false) SUTL_SETUP_FAILURE(STRINGIFY(cond))
#define SUTL_TEST_ASSERT(cond)      if (!!(cond) == false) SUTL_TEST_FAILURE(STRINGIFY(cond))
#define SUTL_CLEANUP_ASSERT(cond)   if (!!(cond) == false) SUTL_CLEANUP_FAILURE(STRINGIFY(cond))

// Skip Test
#define SUTL_SKIP_TEST(str)         return UnitTestResult(ResultType::NotRun, __FUNCSIG__, _countof(__FUNCSIG__), __FILE__, _countof(__FILE__), __LINE__, str)
};
