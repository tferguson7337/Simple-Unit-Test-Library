module;

// Legacy Private Includes //

#include "..\Headers\APIAnnotations.h"


export module SimpleUnitTestLibrary.Suite;

export import <algorithm>;
export import <concepts>;
export import <cstdint>;
export import <format>;
export import <functional>;
export import <iterator>;
export import <numeric>;
export import <ranges>;
export import <string_view>;
export import <type_traits>;
export import <vector>;

export import SimpleUnitTestLibrary.Test;

export
{
#include "..\Headers\SimpleUnitTestLibrary.Suite.h"
}
