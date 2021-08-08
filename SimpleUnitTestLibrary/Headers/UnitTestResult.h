#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <chrono>
#include <cstring>
#include <stdexcept>

// SUTL
#include "ResultType.h"


//
//
//  Class:      UnitTestResult
//
//  Purpose:    Encapsulates the result of a unit test.
//
//
class UnitTestResult
{
    // No copy.
    UnitTestResult(const UnitTestResult&) = delete;
    UnitTestResult& operator=(const UnitTestResult&) = delete;

private:

    // Private Data Members //

    ResultType m_Result;

    const char* m_pFuncName;
    size_t m_FuncNameLen;

    const char* m_pFileName;
    size_t m_FileNameLen;

    const char* m_pInfo;
    size_t m_InfoLen;
    bool m_bCleanupInfo;

    uint32_t m_LineNum;

    mutable uint64_t m_TestDurationMicroseconds;


    // Function/Filename Extraction Helper Methods //

    static inline bool IsPathSeparator(_In_ const char c) noexcept
    {
        return (c == '/') || (c == '\\');
    }

    static inline bool IsClosingCharacter(_In_ const char c) noexcept
    {
        return (c == ']') || (c == '>') || (c == ')');
    }

    static inline bool IsOpeningCharacter(_In_ const char c) noexcept
    {
        return (c == '[') || (c == '<') || (c == '(');
    }

    static inline bool IsWhitespace(_In_ const char c) noexcept
    {
        return c == ' ';
    }

    _Ret_z_ inline const char* ExtractFuncName(_In_z_count_(len) const char* const str, _In_ size_t len) noexcept
    {
        const char* p = str + len - sizeof(char);
        size_t n = 0;
        m_FuncNameLen = 0;
        while (p != str)
        {
            const char c = *p;
            if (IsClosingCharacter(c))
            {
                ++n;
            }
            else if (IsOpeningCharacter(c))
            {
                --n;
            }
            else if (IsWhitespace(c) && (n == 0))
            {
                --m_FuncNameLen;
                ++p;
                break;
            }

            ++m_FuncNameLen;
            --p;
        }

        return p;
    }

    _Ret_z_ inline const char* ExtractFileName(_In_z_count_(len) const char* const str, _In_ size_t len) noexcept
    {
        const char* p = str + len - sizeof(char);
        m_FileNameLen = 0;
        while (p != str)
        {
            if (IsPathSeparator(*p))
            {
                --m_FileNameLen;
                ++p;
                break;
            }

            ++m_FileNameLen;
            --p;
        }

        return p;
    }

    // Misc Helper Methods //

    // Note: This does not cleanup m_pInfo.
    constexpr void Reset() noexcept
    {
        m_Result = ResultType::NotRun;
        m_pFuncName = nullptr;
        m_FuncNameLen = 0;
        m_pFileName = nullptr;
        m_FileNameLen = 0;
        m_pInfo = nullptr;
        m_InfoLen = 0;
        m_bCleanupInfo = false;
        m_LineNum = 0;
        m_TestDurationMicroseconds = 0;
    }

    constexpr bool IsExceptionFailure() const noexcept
    {
        return (m_Result == ResultType::SetupException)
            || (m_Result == ResultType::TestException)
            || (m_Result == ResultType::CleanupException);
    }

public:
    // Ctors //

    // Default constexpr constructor (zero init)
    constexpr UnitTestResult() noexcept :
        m_Result(ResultType::NotRun),
        m_pFuncName(nullptr),
        m_FuncNameLen(0),
        m_pFileName(nullptr),
        m_FileNameLen(0),
        m_pInfo(nullptr),
        m_InfoLen(0),
        m_bCleanupInfo(false),
        m_LineNum(0),
        m_TestDurationMicroseconds(0)
    { }

    // Used for non-exception test result construction.
    UnitTestResult(
        _In_ const ResultType result,
        _In_z_count_(funcLen) const char* const pFunc, _In_ const size_t funcLen,
        _In_z_count_(fileLen) const char* const pFile, _In_ const size_t fileLen,
        _In_ const uint32_t line) noexcept :
        UnitTestResult(result, pFunc, funcLen, pFile, fileLen, line, nullptr, 0)
    { }

    // Used for exception test result construction.
    UnitTestResult(
        _In_ const ResultType result,
        _In_z_count_(funcLen) const char* const pFunc, _In_ const size_t funcLen,
        _In_z_count_(fileLen) const char* const pFile, _In_ const size_t fileLen,
        _In_ const uint32_t line,
        _In_ const std::exception& e) :
        UnitTestResult(result, pFunc, funcLen, pFile, fileLen, line, e.what(), strlen(e.what()))
    { }

    //
    // Note:
    // 
    // If ResultType indicates an exception (SUTL_*_EXCEPTION), then the pInfo field
    // is assumed to be the exception string, which will be copied.
    //
    // In the non-exception case, pInfo is assumed to be a string-literal if not null.
    //
    UnitTestResult(
        _In_ const ResultType result,
        _In_z_count_(funcLen) const char* const pFunc, _In_ const size_t funcLen,
        _In_z_count_(fileLen) const char* const pFile, _In_ const size_t fileLen,
        _In_ const uint32_t line,
        _In_opt_z_count_(infoLen) const char* const pInfo, _In_ const size_t infoLen) noexcept :
        m_Result(result),
        m_pFuncName(nullptr),
        m_FuncNameLen(0),
        m_pFileName(nullptr),
        m_FileNameLen(0),
        m_pInfo(pInfo),
        m_InfoLen(infoLen),
        m_bCleanupInfo(false),
        m_LineNum(line),
        m_TestDurationMicroseconds(0)
    {
        if (IsExceptionFailure())
        {
            // Allocate buffer, copy string, null-terminate it.
            auto pExceptionStringCopyBuffer = new char[m_InfoLen + 1];
            memcpy(pExceptionStringCopyBuffer, m_pInfo, m_InfoLen);
            pExceptionStringCopyBuffer[m_InfoLen] = '\0';

            // Transfer ownership to data member.
            m_pInfo = pExceptionStringCopyBuffer;
        }

        m_pFuncName = ExtractFuncName(pFunc, funcLen);
        m_pFileName = ExtractFileName(pFile, fileLen);
    }

    // Move constructor.
    constexpr UnitTestResult(_Inout_ UnitTestResult&& src) noexcept :
        UnitTestResult()
    {
        *this = std::move(src);
    }

    // Dtor //

    ~UnitTestResult() noexcept
    {
        if (IsExceptionFailure())
        {
            delete[] m_pInfo;
        }
    }

    // Assignment Overloads //

    // Move assignment
    constexpr UnitTestResult& operator=(_Inout_ UnitTestResult&& src) noexcept
    {
        if (this != &src)
        {
            m_Result = src.m_Result;
            m_pFuncName = src.m_pFuncName;
            m_FuncNameLen = src.m_FuncNameLen;
            m_pFileName = src.m_pFileName;
            m_FileNameLen = src.m_FileNameLen;
            m_pInfo = src.m_pInfo;
            m_InfoLen = src.m_InfoLen;
            m_bCleanupInfo = src.m_bCleanupInfo;
            m_LineNum = src.m_LineNum;
            m_TestDurationMicroseconds = src.m_TestDurationMicroseconds;

            src.Reset();
        }

        return *this;
    }


    // Operator Overloads //

    constexpr explicit operator bool() const noexcept
    {
        return (m_Result == ResultType::Success) || (m_Result == ResultType::NotRun);
    }


    // Getters //

    constexpr ResultType GetResult() const noexcept
    {
        return m_Result;
    }

    _Ret_z_ constexpr const char* GetFunctionName() const noexcept
    {
        return m_pFuncName;
    }

    constexpr size_t GetFunctionNameLength() const noexcept
    {
        return m_FuncNameLen;
    }

    _Ret_z_ constexpr const char* GetFileName() const noexcept
    {
        return m_pFileName;
    }

    constexpr size_t GetFileNameLength() const noexcept
    {
        return m_FileNameLen;
    }

    _Ret_z_ constexpr const char* GetInfo() const noexcept
    {
        return m_pInfo;
    }

    constexpr size_t GetInfoLength() const noexcept
    {
        return m_InfoLen;
    }

    constexpr uint32_t GetLineNumber() const noexcept
    {
        return m_LineNum;
    }

    constexpr uint64_t GetTestDurationMicroseconds() const noexcept
    {
        return m_TestDurationMicroseconds;
    }


    // Setters //

    // Treat as const, since test-duration is treated as mutable.
    constexpr void SetTestDurationMicroseconds(_In_ const std::chrono::duration<int64_t, std::micro>& dur) const noexcept
    {
        m_TestDurationMicroseconds = (dur.count() > 0) ? static_cast<uint64_t>(dur.count()) : 0;
    }


    // Public Methods //

    constexpr void Clear() noexcept
    {
        Reset();
    }
};

// Unit Test Return Macros //

#if defined(_MSC_VER)
#define _SUTL_FUNC_ __FUNCSIG__
#else
#define _SUTL_FUNC_ __PRETTY_FUNCTION__
#endif


// Success
#define SUTL_TEST_SUCCESS()         return UnitTestResult(ResultType::Success, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__)

// Skip Test
// Note: Macro str argument is expected to be a string literal.
#define SUTL_SKIP_TEST(str)         return UnitTestResult(ResultType::NotRun, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__, str, sizeof(str))

// Failures - No Exception Thrown
// Note: Macro str argument is expected to be a string literal.
#define SUTL_SETUP_FAILURE(str)     return UnitTestResult(ResultType::SetupFailure, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__, str, sizeof(str))
#define SUTL_TEST_FAILURE(str)      return UnitTestResult(ResultType::TestFailure, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__, str, sizeof(str))
#define SUTL_CLEANUP_FAILURE(str)   return UnitTestResult(ResultType::CleanupFailure, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__, str, sizeof(str))

// Failures - Exception Caught
// Note: Macro str argument is expected to be the "what" string from the exception.
#define SUTL_SETUP_EXCEPTION(exp)   return UnitTestResult(ResultType::SetupException, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__, exp)
#define SUTL_TEST_EXCEPTION(exp)    return UnitTestResult(ResultType::TestException, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__, exp)
#define SUTL_CLEANUP_EXCEPTION(exp) return UnitTestResult(ResultType::CleanupException, _SUTL_FUNC_, sizeof(_SUTL_FUNC_), __FILE__, sizeof(__FILE__), __LINE__, exp)

#define ___SUTL_STRINGIFY___(s)     #s
#define __SUTL_STRINGIFY__(s)       ___SUTL_STRINGIFY___(s)

// Test Asserts
#define SUTL_SETUP_ASSERT(cond)     if (!!(cond) == false) SUTL_SETUP_FAILURE(__SUTL_STRINGIFY__(cond))
#define SUTL_TEST_ASSERT(cond)      if (!!(cond) == false) SUTL_TEST_FAILURE(__SUTL_STRINGIFY__(cond))
#define SUTL_CLEANUP_ASSERT(cond)   if (!!(cond) == false) SUTL_CLEANUP_FAILURE(__SUTL_STRINGIFY__(cond))
