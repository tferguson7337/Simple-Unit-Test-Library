#pragma once

#if defined(SUTL_USE_MODULES)
import SimpleUnitTestLibrary.Test;
import SimpleUnitTestLibrary.Result;
import SimpleUnitTestLibrary.Logger;
#else
#include "SimpleUnitTestLibrary.Test.h"
#include "SimpleUnitTestLibrary.Result.h"
#include "SimpleUnitTestLibrary.Logger.h"
#endif

#define SUTL_STRINGIFY_(thing_to_string_) # thing_to_string_
#define SUTL_STRINGIFY(thing_to_string_) SUTL_STRINGIFY_(thing_to_string_)

#define SUTL_TEST_SUCCESS() return SUTL::Result{.m_ResultType = SUTL::ResultType::Success, .m_SourceLocation = std::source_location::current(), .m_Info = {}}
#define SUTL_TEST_SKIP(reason_str_) return SUTL::Result{.m_ResultType = SUTL::ResultType::Success, .m_SourceLocation = std::source_location::current(), .m_Info = reason_str_}

#define SUTL_LOG(fmt_, ...) SUTL::Logger{}(fmt_ __VA_OPT__(, ) __VA_ARGS__)

#define SUTL_SETUP_ASSERT(expr_, ...)   if (!(expr_)) { __VA_OPT__(SUTL_LOG(__VA_ARGS__);) return SUTL::Result{.m_ResultType = SUTL::ResultType::SetupFailure, .m_SourceLocation = std::source_location::current(), .m_Info = SUTL_STRINGIFY(expr_)}; }
#define SUTL_TEST_ASSERT(expr_, ...)    if (!(expr_)) { __VA_OPT__(SUTL_LOG(__VA_ARGS__);) return SUTL::Result{.m_ResultType = SUTL::ResultType::TestFailure, .m_SourceLocation = std::source_location::current(), .m_Info = SUTL_STRINGIFY(expr_)}; }
#define SUTL_CLEANUP_ASSERT(expr_, ...) if (!(expr_)) { __VA_OPT__(SUTL_LOG(__VA_ARGS__);) return SUTL::Result{.m_ResultType = SUTL::ResultType::CleanupFailure, .m_SourceLocation = std::source_location::current(), .m_Info = SUTL_STRINGIFY(expr_)}; }


#define SUTL_CREATE_UNIT_TEST(func_) SUTL::Test(SUTL_STRINGIFY(func_), func_)
