 newoption {
    trigger = "backend",
    value = "API",
    description = "Choose a backend file dialog (Linux only)",
    allowed = {
        { "gtk3", "GTK 3 - link to gtk3 directly" },      
        { "zenity", "Zenity - generate dialogs on the end users machine with zenity" }
    },
    default = "zenity"
}

project "NativeFileDialog"
kind "StaticLib"
language "C"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files
{
    "src/*.h",
    "src/include/*.h",
    "src/nfd_common.c"
}

includedirs {"src/include/"}

filter "system:linux"
    pic "On"

    systemversion "latest"
    staticruntime "On"

    files
    {

    }

    defines
    {

    }

filter "system:windows"
    systemversion "latest"
    staticruntime "On"
    
    buildoptions "/MT"

    files
    {
        "src/nfd_win.cpp"
    }

    defines 
    { 

    }

filter {"system:linux", "options:backend=gtk3"}
    language "C"
    files {"src/nfd_gtk.c"}
    buildoptions {"`pkg-config --cflags gtk+-3.0`"}

filter {"system:linux", "options:backend=zenity"}
    language "C"
    files {"src/nfd_zenity.c"}

  -- visual studio filters
filter "action:vs*"
    defines { "_CRT_SECURE_NO_WARNINGS" }   

filter "configurations:Debug"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    runtime "Release"
    optimize "On"

filter "configurations:Dist"
	runtime "Release"
	optimize "on"
	symbols "off"