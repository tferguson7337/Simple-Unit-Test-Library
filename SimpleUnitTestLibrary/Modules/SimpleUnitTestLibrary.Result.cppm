module;

#include "..\Headers\APIAnnotations.h"

export module SimpleUnitTestLibrary.Result;

export import <algorithm>;
export import <array>;
export import <charconv>;
export import <cstdint>;
export import <format>;
export import <source_location>;
export import <string>;
export import <string_view>;
export import <system_error>;

import SimpleUnitTestLibrary.Logger;
import SimpleUnitTestLibrary.Utils;

export
{
#include "..\Headers\SimpleUnitTestLibrary.Result.h"
}
