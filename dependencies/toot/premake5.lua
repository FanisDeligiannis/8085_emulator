project "toot"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "toot.h",
        "toot.c",
    }

    filter "system:windows"
        systemversion "latest"
        buildoptions "/MT"
        
        defines
        {
            "PLATFORM_WINDOWS",
        }
        
        links 
        { 
            "Kernel32.lib"
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
