workspace "OpenGL"
	architecture "x64"
	startproject "xGlRunner"
	targetdir "build"
	--warnings "On"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- xGl
-- Only for building
project "xGl" 
	location "xGl"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	--warnings "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src"
	}


	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_XGL_WIN__",
			-- Why not lmao
			"_XGL_WINDOWS__"
		}

		links 
		{ 
			"opengl32.lib"
		}

		filter "configurations:Debug"
			defines "__XGL_DEBUG__"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "__XGL_RELEASE__"
			runtime "Release"
			optimize "on"

	filter "system:liunx"
		systemversion "latest"

		defines
		{
			"_XGL_LINUX__"
		}

		links 
		{
			"pthread",
			"dl",
			"Dl",
			"GL",
			"X11"
		}

		filter "configurations:Debug"
			defines "__XGL_DEBUG__"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "__XGL_RELEASE__"
			runtime "Release"
			optimize "on"

	filter "system:macosx"
		systemversion "11.0"

		defines
		{
			"_XGL_MAC__"
		}

		filter "configurations:Debug"
			defines "__XGL_DEBUG__"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "__XGL_RELEASE__"
			runtime "Release"
			optimize "on"


-- Test Project for running xGl
-- Will not be included
-- xGlRunner aka example
project "xGlRunner"
	location "xGlRunner"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	warnings "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"xGl/src"
	}

	links 
	{
	}

	filter "system:linux"
		systemversion "latest"

		links
		{
			"pthread",
			"dl",
			"X11"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		systemversion "latest"

		links {
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

	filter "system:macosx"
		systemversion "11.0"

		links {
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"