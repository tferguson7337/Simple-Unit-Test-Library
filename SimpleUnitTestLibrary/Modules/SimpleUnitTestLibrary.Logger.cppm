module;

#include "..\Headers\APIAnnotations.h"

export module SimpleUnitTestLibrary.Logger;

export import <concepts>;
export import <cstdint>;
export import <format>;
export import <print>;
export import <source_location>;
export import <string>;
export import <string_view>;
export import <type_traits>;

import SimpleUnitTestLibrary.Utils;

export
{
#include "..\Headers\SimpleUnitTestLibrary.Logger.h"
}
