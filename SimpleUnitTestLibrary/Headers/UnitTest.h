#pragma once

// SAL
#include "APIAnnotations.h"

// SUTL
#include "TestTypes.h"
#include "UnitTestResult.h"

// STL
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <type_traits>


namespace SUTL
{
    //
    //
    //  Class:      UnitTest
    //  
    //  Purpose:    Encapsulate unit test information.
    //
    //
    class UnitTest
    {
    private:
        // Private Data Members //

        UnitTestFunction m_TestFunc;
        UnitTestResult m_TestResult;

        UnitTest() noexcept = default;

        struct ScopedTestDurationRecorder
        {
            using Clock = std::chrono::system_clock;
            using TimePoint = Clock::time_point;

            TimePoint m_BeginTimePoint = Clock::now();
            UnitTestResult& m_TestResultRef;

            ScopedTestDurationRecorder(_Inout_ UnitTestResult& result) noexcept :
                m_TestResultRef(result)
            { }

            ~ScopedTestDurationRecorder()
            {
                const auto delta = Clock::now() - m_BeginTimePoint;
                const auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(delta);
                m_TestResultRef.SetTestDurationMicroseconds(durationMicroseconds);
            }
        };

    public:
        // Ctors //

        explicit UnitTest(_In_ UnitTestFunction func) :
            m_TestFunc(std::move(func))
        {
            if (!m_TestFunc)
            {
                throw std::invalid_argument("Attempted to create UnitTest object w/ empty function object.");
            }
        }

        UnitTest(_Inout_ UnitTest&& src) noexcept :
            m_TestFunc(std::move(src.m_TestFunc)),
            m_TestResult(std::move(src.m_TestResult))
        { }

        // Dtor //

        ~UnitTest() noexcept = default;

        // Operator Overloads //

        UnitTest& operator=(_Inout_ UnitTest&& src) noexcept
        {
            if (this != &src)
            {
                m_TestFunc = std::move(src.m_TestFunc);
                m_TestResult = std::move(src.m_TestResult);
            }

            return *this;
        }

        // Getters //

        UnitTestFunction GetUnitTestFunction() const noexcept
        {
            return m_TestFunc;
        }

        const UnitTestResult& GetUnitTestResult() const noexcept
        {
            return m_TestResult;
        }

        // Public Methods //

        void RunTest() noexcept
        {
            try
            {
                ScopedTestDurationRecorder scopedTestDurationRecorder(m_TestResult);
                m_TestResult = m_TestFunc();
            }
            catch (const std::exception& e)
            {
                std::stringstream ss;
                ss << "Unhandled exception - Test[0x" << std::hex << std::setfill('0') << std::setw(8)
                    << m_TestFunc.target<UnitTestResult(*)()>() << "] Exception[" << e.what() << "]";
                m_TestResult.SetUnhandledException(ss.str());
            }
        }
    };
}
