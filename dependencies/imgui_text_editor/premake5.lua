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
        "TextEditor.h",
		"LanguageDefinitions.cpp",
		"UnitTests.cpp",
		"ImGuiDebugPanel.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"BOOST_REGEX_STANDALONE"
	}

	includedirs
	{
        path.join(_WORKING_DIR, 'dependencies/imgui'),
		path.join(_WORKING_DIR, 'dependencies/boost_regex/include')
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
