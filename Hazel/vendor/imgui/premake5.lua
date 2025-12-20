project "ImGui"
	kind "StaticLib"
	language "C++"
	staticruntime "on" 

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.h",
		"*.cpp",
		"backends/imgui_impl_glfw.h",
		"backends/imgui_impl_glfw.cpp",
		"backends/imgui_impl_opengl3.h",
		"backends/imgui_impl_opengl3.cpp",
        -- 用Docking 分支通常不需要 loader.h
		-- "backends/imgui_impl_opengl3_loader.h" 
	}

	includedirs
	{
		".",
		"backends",
		"../GLFW/include" -- 让 ImGui 能找到 GLFW
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"