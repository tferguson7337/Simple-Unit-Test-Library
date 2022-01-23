#pragma once

// SAL
#include "APIAnnotations.h"

// STL
#include <functional>
#include <list>

// SUTL
#include "ResultType.h"
#include "TestQuantity.h"


namespace SUTL
{
	// Forward decls.
	class UnitTestResult;
	class UnitTest;

	// A few useful type aliases.
	using UnitTestFunction = std::function<UnitTestResult()>;
	using UnitTestList = std::list<UnitTest>;
}
