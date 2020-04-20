
local BGFX_DIR = "bgfx"
local BIMG_DIR = "bimg"
local BX_DIR = "bx"
local GLFW_DIR = "glfw"

workspace "Sne"
	architecture "x64"
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

	targetdir ("bin/".. outputdir .."/%{prj.name)")
	objdir ("bin-int/".. outputdir .."/%{prj.name)")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"SNE_PLATFORM_WINDOWS",
			"SNE_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		filter "configurations:Debug"
			defines "SNE_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "SNE_RELEASE"
			symbols "On"

		--filters {"system:windows", "configurations:Release"}
		--buildoptions "/MT"


	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"

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
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"SNE_PLATFORM_WINDOWS",
		}

		links
		{
			"Sne"
		}

		filter "configurations:Debug"
			defines "SNE_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "SNE_RELEASE"
			symbols "On"




		--filters {"system:windows", "configurations:Release"}
		--buildoptions "/MT"
