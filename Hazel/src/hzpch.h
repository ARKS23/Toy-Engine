#pragma once

#include <iostream>
#include <memory.h>
#include <utility>
#include <algorithm>
#include <functional>

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "Hazel/Core/Log.h"
#include "Hazel/Debug/Instrumentor.h"

#include <entt.hpp>

#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
