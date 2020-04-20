
local BGFX_DIR = "bgfx"
local BIMG_DIR = "bimg"
local BX_DIR = "bx"
local GLFW_DIR = "glfw"

workspace "Sne"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
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
	
	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"SNE_BUILD_DLL",
		}

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
		"Sne/src"
	}
	filter "system:windows"
		systemversion "latest"
		
		links
		{
			"Sne"
		}

		filter "configurations:Debug"
			defines "SNE_DEBUG"
			symbols "on"

		filter "configurations:Release"
			defines "SNE_RELEASE"
<<<<<<< HEAD
			symbols "On"




		--filters {"system:windows", "configurations:Release"}
		--buildoptions "/MT"
=======
			symbols "on"
>>>>>>> 1f1d8f62a97d781d77b28734aeb7e16e8167a454
