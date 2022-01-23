#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <chrono>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <source_location>

// SUTL
#include "ResultType.h"


namespace SUTL
{
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

        struct InfoString
        {
            const char* m_pStr = nullptr;
            size_t m_Len = 0;
        };

        // Private Data Members //

        ResultType m_ResultType = ResultType::NotRun;

        std::string_view m_FuncName;
        std::string_view m_FileName;
        InfoString m_InfoString;

        uint32_t m_LineNum = 0;

        mutable uint64_t m_TestDurationMicroseconds = 0;


        // Function/Filename Extraction Helper Methods //

        static constexpr bool IsPathSeparator(_In_ const char c) noexcept
        {
            return (c == '/') || (c == '\\');
        }

        static constexpr bool IsClosingCharacter(_In_ const char c) noexcept
        {
            return (c == ']') || (c == '>') || (c == ')');
        }

        static constexpr bool IsOpeningCharacter(_In_ const char c) noexcept
        {
            return (c == '[') || (c == '<') || (c == '(');
        }

        static constexpr bool IsWhitespace(_In_ const char c) noexcept
        {
            return c == ' ';
        }

        constexpr std::string_view ExtractFuncName(_In_ std::string_view funcName) noexcept
        {
            size_t n = 0;
            size_t len = funcName.size();
            for (auto rItr = funcName.crbegin(), rEnd = funcName.crend(); rItr != rEnd; ++rItr)
            {
                const char c = *rItr;
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
                    ++len;
                    break;
                }

                --len;
            }

            return funcName.substr(len);
        }

        constexpr std::string_view ExtractFileName(_In_ std::string_view fileName) noexcept
        {
            size_t len = fileName.size();
            for (auto rItr = fileName.crbegin(), rEnd = fileName.crend(); rItr != rEnd; ++rItr)
            {
                if (IsPathSeparator(*rItr))
                {
                    break;
                }

                --len;
            }

            return fileName.substr(len);
        }

        // Misc Helper Methods //

        constexpr bool IsExceptionFailure() const noexcept
        {
            return (m_ResultType == ResultType::SetupException)
                || (m_ResultType == ResultType::TestException)
                || (m_ResultType == ResultType::CleanupException)
                || (m_ResultType == ResultType::UnhandledException);
        }

    public:
        // Ctors //

        // Default constexpr constructor (zero init)
        constexpr UnitTestResult() noexcept = default;

        // Used for non-exception test result construction.
        constexpr UnitTestResult(
            _In_ const ResultType result,
            _In_ const std::source_location srcLoc) noexcept :
            UnitTestResult(result, srcLoc, nullptr, 0)
        { }

        // Used for exception test result construction.
        UnitTestResult(
            _In_ const ResultType result,
            _In_ const std::source_location srcLoc,
            _In_ const std::exception& e) :
            UnitTestResult(result, srcLoc, e.what(), strlen(e.what()))
        { }

        //
        // Note:
        // 
        // If ResultType indicates an exception (SUTL_*_EXCEPTION), then the pInfo field
        // is assumed to be the exception string, which will be copied.
        //
        // In the non-exception case, pInfo is still copied (not going to assume string-literal case).
        //
        constexpr UnitTestResult(
            _In_ const ResultType result,
            _In_ const std::source_location srcLoc,
            _In_opt_z_count_(infoLen) const char* const pInfo, _In_ const size_t infoLen) noexcept :
            m_ResultType(result),
            m_LineNum(srcLoc.line()),
            m_TestDurationMicroseconds(0)
        {
            if (!!pInfo)
            {
                if (IsExceptionFailure())
                {
                    char* pStrCopy = new char[infoLen + 1];
                    memcpy(pStrCopy, pInfo, infoLen);
                    pStrCopy[infoLen] = '\0';
                    m_InfoString = InfoString{pStrCopy, infoLen};
                }
                else
                {
                    m_InfoString = InfoString{pInfo, infoLen};
                }
            }

            m_FuncName = ExtractFuncName(srcLoc.function_name());
            m_FileName = ExtractFileName(srcLoc.file_name());
        }

        // Move constructor.
        constexpr UnitTestResult(_Inout_ UnitTestResult&& src) noexcept :
            UnitTestResult()
        {
            *this = std::move(src);
        }

        // Dtor //

        ~UnitTestResult() noexcept { if (IsExceptionFailure()) { delete[] m_InfoString.m_pStr; } }

        // Assignment Overloads //

        // Move assignment
        constexpr UnitTestResult& operator=(_Inout_ UnitTestResult&& src) noexcept
        {
            if (this != &src)
            {
                m_ResultType = src.m_ResultType;
                m_FuncName = src.m_FuncName;
                m_FileName = src.m_FileName;
                m_InfoString = std::move(src.m_InfoString);
                m_LineNum = src.m_LineNum;
                m_TestDurationMicroseconds = src.m_TestDurationMicroseconds;

                src.m_ResultType = ResultType::_Invalid;
            }

            return *this;
        }


        // Operator Overloads //

        constexpr explicit operator bool() const noexcept
        {
            return (m_ResultType == ResultType::Success)
                || (m_ResultType == ResultType::NotRun);
        }

        // Getters //

        constexpr ResultType GetResultType() const noexcept
        {
            return m_ResultType;
        }

        constexpr std::string_view GetFunctionName() const noexcept
        {
            return m_FuncName;
        }

        constexpr std::string_view GetFileName() const noexcept
        {
            return m_FileName;
        }

        constexpr std::string_view GetInfoString() const noexcept
        {
            return std::string_view(m_InfoString.m_pStr, m_InfoString.m_Len);
        }

        constexpr uint32_t GetLineNumber() const noexcept
        {
            return m_LineNum;
        }

        constexpr uint64_t GetTestDurationMicroseconds() const noexcept
        {
            return m_TestDurationMicroseconds;
        }

        constexpr uint64_t GetTestDurationMilliseconds() const noexcept
        {
            return GetTestDurationMicroseconds() / 1000;
        }


        // Setters //

        // Treat as const, since test-duration is treated as mutable.
        constexpr void SetTestDurationMicroseconds(_In_ const std::chrono::duration<int64_t, std::micro>& dur) const noexcept
        {
            m_TestDurationMicroseconds = (dur.count() > 0) ? static_cast<uint64_t>(dur.count()) : 0;
        }

        void SetUnhandledException(_In_ const std::string& exceptionInfo)
        {
            m_ResultType = ResultType::UnhandledException;
            const size_t len = exceptionInfo.length();
            char* const pStr = new char[len + 1];
            memcpy(pStr, exceptionInfo.c_str(), len + 1);
            if (m_InfoString.m_pStr)
            {
                // Shouldn't happen, but avoid the leak.
                delete[] m_InfoString.m_pStr;
            }

            m_InfoString = InfoString{pStr, len};
        }
    };
}

// Unit Test Return Macros //

#if defined(_MSC_VER)
#define SUTL_FUNC __FUNCSIG__
#else
#define SUTL_FUNC __PRETTY_FUNCTION__
#endif


// Success
#define SUTL_TEST_SUCCESS()         do { return SUTL::UnitTestResult(SUTL::ResultType::Success, std::source_location::current()); } while (0)

// Skip Test
// Note: Macro str argument is expected to be a string literal.
#define SUTL_SKIP_TEST(str)         do { return SUTL::UnitTestResult(SUTL::ResultType::NotRun, std::source_location::current(), str, sizeof(str) - 1); } while(0)

// Failures - No Exception Thrown
// Note: Macro str argument is expected to be a string literal.
#define SUTL_SETUP_FAILURE(str)     do { return SUTL::UnitTestResult(SUTL::ResultType::SetupFailure, std::source_location::current(), str, sizeof(str) - 1); } while(0)
#define SUTL_TEST_FAILURE(str)      do { return SUTL::UnitTestResult(SUTL::ResultType::TestFailure, std::source_location::current(), str, sizeof(str) - 1); } while(0)
#define SUTL_CLEANUP_FAILURE(str)   do { return SUTL::UnitTestResult(SUTL::ResultType::CleanupFailure, std::source_location::current(), str, sizeof(str) - 1); } while(0)

// Failures - Exception Caught
// Note: Macro str argument is expected to be the "what" string from the exception.
#define SUTL_SETUP_EXCEPTION(exp)   do { return SUTL::UnitTestResult(SUTL::ResultType::SetupException, std::source_location::current(), exp); } while(0)
#define SUTL_TEST_EXCEPTION(exp)    do { return SUTL::UnitTestResult(SUTL::ResultType::TestException, std::source_location::current(), exp); } while(0)
#define SUTL_CLEANUP_EXCEPTION(exp) do { return SUTL::UnitTestResult(SUTL::ResultType::CleanupException, std::source_location::current(), exp); } while(0)

#define SUTL_STRINGIFY_(s)          #s
#define SUTL_STRINGIFY(s)           SUTL_STRINGIFY_(s)

// Test Asserts
#define SUTL_SETUP_ASSERT(cond)     do { if (!!(cond) == false) SUTL_SETUP_FAILURE(SUTL_STRINGIFY(cond)); } while(0)
#define SUTL_TEST_ASSERT(cond)      do { if (!!(cond) == false) SUTL_TEST_FAILURE(SUTL_STRINGIFY(cond)); } while(0)
#define SUTL_CLEANUP_ASSERT(cond)   do { if (!!(cond) == false) SUTL_CLEANUP_FAILURE(SUTL_STRINGIFY(cond)); } while(0)
