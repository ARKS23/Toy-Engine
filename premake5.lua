workspace "Hazel"
	architecture "x64"
	startproject "Sandbox"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["GLAD"] = "Hazel/vendor/GLAD/include"
IncludeDir["imgui"] = "Hazel/vendor/imgui/include"

include "Hazel/vendor/GLAD"
include "Hazel/vendor/GLFW"
include "Hazel/vendor/imgui"

project "Hazel"
	location "Hazel"
	kind "SharedLib"
	language "C++"

	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/glm", -- 纯头文件库
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLAD/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/imgui"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		buildoptions { "/utf-8" }

		postbuildcommands
		{
			--("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
			("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/Hazel.dll")
		}

		filter "configurations:Debug"
			defines "HZ_DEBUG"
			--buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "HZ_RELEASE"
			--buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "HZ_DIST"
			--buildoptions "/MD"
			optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"Hazel/vendor/glm", -- 纯头文件库
		"Hazel/vendor/spdlog/include",
		"Hazel/src"
	}

	links
	{
		"Hazel"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

		buildoptions { "/utf-8" }

		filter "configurations:Debug"
			defines "HZ_DEBUG"
			--runtime "Debug"
			--buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "HZ_RELEASE"
			--runtime "Release"
			--buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "HZ_DIST"
			--runtime "Release"
			--buildoptions "/MD"
			optimize "On"