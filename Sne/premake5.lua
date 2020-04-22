workspace "Sne"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glfw"] = "extlibs/glfw/include"
IncludeDir["bgfx"] = "extlibs/bgfx/include"

group "Dependencies"
	include "extlibs/glfw"
	include "extlibs/bgfx"

group ""

project "Sne"
	location "Sne"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/".. outputdir .."/%{prj.name)")
	objdir ("bin-int/".. outputdir .."/%{prj.name)")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		--"GLFW_INCLUDE_NONE",
		"SNE_BUILD_DLL"
	}
	
	includedirs
	{
		"%{prj.name/src}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.bgfx}"
	}
	
	links
	{
		"glfw",
		"bgfx",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		filter "configurations:Debug"
			defines "SNE_DEBUG"
			symbols "on"

		filter "configurations:Release"
			defines "SNE_RELEASE"
			symbols "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/".. outputdir .."/%{prj.name)")
	objdir ("bin-int/".. outputdir .."/%{prj.name)")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Sne/src",
		"Sne/extlibs"
	}
	
	links
	{
		"Sne"
	}
	
	filter "system:windows"
		systemversion "latest"

		filter "configurations:Debug"
			defines "SNE_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "SNE_RELEASE"
			runtime "Release"
			symbols "On"
