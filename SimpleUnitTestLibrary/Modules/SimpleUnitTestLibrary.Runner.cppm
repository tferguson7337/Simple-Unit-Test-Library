module;

// Legacy Private Includes //

#include "..\Headers\APIAnnotations.h"


export module SimpleUnitTestLibrary.Runner;

export import <ranges>;
export import <string_view>;
export import <vector>;

export import SimpleUnitTestLibrary.Suite;

export
{
#include "..\Headers\SimpleUnitTestLibrary.Runner.h"
}
