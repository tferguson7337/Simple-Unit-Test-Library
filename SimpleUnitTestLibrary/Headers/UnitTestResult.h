#pragma once

// STL
#include <chrono>
#include <string>

// SUTL
#include "ResultType.h"

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

    uint32_t m_LTGTCounter = 0;

    ResultType m_Result;
    uint32_t m_LineNum;
    std::string m_FuncName;
    std::string m_FileName;
    std::string m_ResultInfo;
    mutable uint64_t m_TestDurationMicroseconds;

    using ExtractPredicate = bool(*)(const char&, uint32_t&);
    constexpr const char* ExtractNameCommon(_In_ const char*, _In_ const size_t&, _In_ ExtractPredicate) noexcept;
    constexpr const char* ExtractFileName(_In_ const char*, _In_ const size_t&) noexcept;
    constexpr const char* ExtractFuncName(_In_ const char*, _In_ const size_t&) noexcept;
    static constexpr bool IsPathSeparator(_In_ const char&, _Inout_ uint32_t&) noexcept;
    static constexpr bool IsWhitespace(_In_ const char&, _Inout_ uint32_t&) noexcept;

public:
    /// Ctors \\\

    UnitTestResult() noexcept;

    UnitTestResult(
        _In_ ResultType result,
        _In_ const char* func, _In_ const size_t& funcLen,
        _In_ const char* file, _In_ const size_t& fileLen, _In_ const uint32_t& line,
        _In_ const std::string& info = ""
    );

    // Move Ctor
    UnitTestResult(_Inout_ UnitTestResult&&) noexcept;

    /// Dtor \\\

    ~UnitTestResult() = default;

    /// Operator Overloads \\\

    UnitTestResult& operator=(_Inout_ UnitTestResult&&) noexcept;
    explicit operator bool() const noexcept;

    /// Getters \\\

    const ResultType& GetResult() const noexcept;
    const std::string& GetFunctionName() const noexcept;
    const std::string& GetFileName() const noexcept;
    const uint32_t& GetLineNumber() const noexcept;
    const std::string& GetResultInfo() const noexcept;
    const uint64_t& GetTestDurationMicroseconds() const noexcept;

    /// Setters \\\

    void SetResult(_In_ const ResultType&) noexcept;
    void SetFunctionName(_In_ const std::string&);
    void SetFileName(_In_ const std::string&);
    void SetLineNumber(_In_ const uint32_t&) noexcept;
    void SetResultInfo(_In_ const std::string&);

    // Treat as const, since test-duration is treated as mutable.
    void SetTestDurationMicroseconds(_In_ const std::chrono::duration<int64_t, std::micro>&) const noexcept;

    /// Public Methods \\\

    void Clear() noexcept;

    /// Unit Test Return Macros \\\

#if defined(_MSC_BUILD)

#define __PRETTY_FUNCTION__ __FUNCSIG__

#endif

// Success
#define SUTL_TEST_SUCCESS()        return UnitTestResult(ResultType::Success, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__)

// Failures - No Exception Thrown
#define SUTL_SETUP_FAILURE(str)     return UnitTestResult(ResultType::SetupFailure, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__, str)
#define SUTL_TEST_FAILURE(str)      return UnitTestResult(ResultType::TestFailure, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__, str)
#define SUTL_CLEANUP_FAILURE(str)   return UnitTestResult(ResultType::CleanupFailure, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__, str)

// Failures - Exception Caught
#define SUTL_SETUP_EXCEPTION(str)   return UnitTestResult(ResultType::SetupException, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__, str)
#define SUTL_TEST_EXCEPTION(str)    return UnitTestResult(ResultType::TestException, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__, str)
#define SUTL_CLEANUP_EXCEPTION(str) return UnitTestResult(ResultType::CleanupException, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__, str)

#define ___SUTL_STRINGIFY___(s)   #s
#define __SUTL_STRINGIFY__(s)    ___SUTL_STRINGIFY___(s)

// Test Asserts
#define SUTL_SETUP_ASSERT(cond)     if (!!(cond) == false) SUTL_SETUP_FAILURE(__SUTL_STRINGIFY__(cond))
#define SUTL_TEST_ASSERT(cond)      if (!!(cond) == false) SUTL_TEST_FAILURE(__SUTL_STRINGIFY__(cond))
#define SUTL_CLEANUP_ASSERT(cond)   if (!!(cond) == false) SUTL_CLEANUP_FAILURE(__SUTL_STRINGIFY__(cond))

// Skip Test
#define SUTL_SKIP_TEST(str)         return UnitTestResult(ResultType::NotRun, __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__), __FILE__, sizeof(__FILE__), __LINE__, str)
};
