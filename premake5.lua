workspace "8085_emulator"
	architecture "x64"
	startproject "GUI"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}

project "8085_assembler"
	location "8085_assembler"
	kind "StaticLib"
	language "C++"	
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/include",
	}

	links 
	{ 

	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS",
		}

	filter "system:linux"
		pic "On"
		systemversion "latest"


	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"
		symbols "Off"



project "8085_emu"
    location "8085_emu"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/include",
    }

    links 
    { 

    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS",
        }

	filter "system:linux"
		pic "On"
		systemversion "latest"


    filter "configurations:Debug"
        defines "_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "_DIST"
        runtime "Release"
        optimize "on"
		symbols "Off"


        
project "GUI"
	location "GUI"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/fonts/**.cpp"
	}

	includedirs
	{
        "8085_assembler/include",
        "8085_emu/include",
        "GUI/include",
		"dependencies/GLFW/include",
		"dependencies/imgui/",
		"dependencies/imgui_*",
		"dependencies/stb",
		"dependencies/NativeFileDialog/src/include",
	}

	filter "system:windows"
		systemversion "latest"

		links
		{
			"8085_assembler",
			"8085_emu",
			"ImGui",
			"ImGui_TextEditor",
			"NativeFileDialog",
			
			"GLFW",
			"opengl32.lib"
		}

		defines
		{
			"PLATFORM_WINDOWS",
			"NFD"
		}

	filter "system:linux"
		linkgroups "On"

		links
		{
			"8085_assembler",
			"8085_emu",
			"ImGui",
			"ImGui_TextEditor",
			"NativeFileDialog",
			
			"GLFW",
			"GL",
		}
		
		pic "On"
		systemversion "latest"

	filter {"system:linux", "options:linux_backend=gtk3"}
		defines "NFD"

	filter {"system:linux", "options:linux_backend=zenity"}
		defines "NFD"
		
	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		kind "WindowedApp"
		defines "_DIST"
		runtime "Release"
		optimize "On"
		symbols "Off"

		
group "Dependencies"
	include "dependencies/imgui"
	include "dependencies/imgui_text_editor"
	include "dependencies/GLFW"
	include "dependencies/nativefiledialog"
group ""