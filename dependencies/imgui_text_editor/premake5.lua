project "ImGui_TextEditor"
	kind "StaticLib"
	language "C++"	
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "TextEditor.cpp",
        "TextEditor.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
        path.join(_WORKING_DIR, 'dependencies/imgui')
	}

	links 
	{ 
        "ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/MT"
		
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
