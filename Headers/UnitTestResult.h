#pragma once

#include <Macros.h>
#include <Uncopyable.h>
#include <Types.h>

#include <ResultType.h>

#include <string>

///
//
//  Class:      UnitTestResult (final)
//
//  Purpose:    Encapsulates the result of a unit test.
//
///
class UnitTestResult final : public Uncopyable
{
private:
    /// Private Data Members \\\

    ResultType mResult;
    uint32 mLineNum;
    std::string mFuncName;
    std::string mFileName;
    std::string mResultInfo;

    using ExtractPredicate = bool(*)(const char);
    static constexpr const char* ExtractNameCommon(const ascii*, const size_t, ExtractPredicate) noexcept;
    static constexpr const char* ExtractFileName(const ascii*, const size_t) noexcept;
    static constexpr const char* ExtractFuncName(const ascii*, const size_t) noexcept;
    static constexpr bool IsPathSeparator(const ascii) noexcept;
    static constexpr bool IsWhitespace(const ascii) noexcept;

public:
    /// Ctors \\\

    UnitTestResult(ResultType = ResultType::NotRun, const std::string& = "", const std::string& = "", uint32 = 0, const std::string& = "") noexcept;

    // Move Ctor
    UnitTestResult(UnitTestResult&&) noexcept;

    /// Dtor \\\

    ~UnitTestResult( ) = default;

    /// Operator Overloads \\\

    UnitTestResult& operator=(UnitTestResult&&) noexcept;
    explicit operator bool( ) const noexcept;

    /// Getters \\\

    const ResultType& GetResult( ) const noexcept;
    const std::string& GetFunctionName( ) const noexcept;
    const std::string& GetFileName( ) const noexcept;
    const uint32& GetLineNumber( ) const noexcept;
    const std::string& GetResultInfo( ) const noexcept;

    /// Setters \\\

    void SetResult(const ResultType&) noexcept;
    void SetFunctionName(const std::string&);
    void SetFileName(const std::string&);
    void SetLineNumber(const uint32&) noexcept;
    void SetResultInfo(const std::string&);

    /// Unit Test Return Macros \\\

// Success
#define SUTL_TEST_SUCCESS( )        return UnitTestResult(ResultType::Success, __FUNCSIG__, __FILE__, __LINE__)

// Failures - No Exception Thrown
#define SUTL_SETUP_FAILURE(str)     return UnitTestResult(ResultType::SetupFailure, __FUNCSIG__, __FILE__, __LINE__, str)
#define SUTL_TEST_FAILURE(str)      return UnitTestResult(ResultType::TestFailure, __FUNCSIG__, __FILE__, __LINE__, str)
#define SUTL_CLEANUP_FAILURE(str)   return UnitTestResult(ResultType::CleanupFailure, __FUNCSIG__, __FILE__, __LINE__, str)

// Failures - Exception Caught
#define SUTL_SETUP_EXCEPTION(str)   return UnitTestResult(ResultType::SetupException, __FUNCSIG__, __FILE__, __LINE__, str)
#define SUTL_TEST_EXCEPTION(str)    return UnitTestResult(ResultType::TestException, __FUNCSIG__, __FILE__, __LINE__, str)
#define SUTL_CLEANUP_EXCEPTION(str) return UnitTestResult(ResultType::CleanupException, __FUNCSIG__, __FILE__, __LINE__, str)

// Test Asserts
#define SUTL_SETUP_ASSERT(cond)     if (!!(cond) == false) SUTL_SETUP_FAILURE(STRINGIFY(cond))
#define SUTL_TEST_ASSERT(cond)      if (!!(cond) == false) SUTL_TEST_FAILURE(STRINGIFY(cond))
#define SUTL_CLEANUP_ASSERT(cond)   if (!!(cond) == false) SUTL_CLEANUP_FAILURE(STRINGIFY(cond))

// Skip Test
#define SUTL_SKIP_TEST(str)         return UnitTestResult(ResultType::NotRun, __FUNCSIG__, __FILE__, __LINE__, str)
};
