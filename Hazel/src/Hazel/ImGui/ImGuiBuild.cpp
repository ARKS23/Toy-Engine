#include "hzpch.h"

/* 编译控制单元: Loader宏只定义一次，后端实现只编译一次 */

#define IMGUI_IMPL_OPENGL_LOADER_GLAD // 告诉ImGui后端用引擎中的GLAD加载器
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"