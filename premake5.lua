include("dependencies.lua")

workspace "DentyEngine"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    warnings "Extra"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "DentyEngine"
        kind "WindowedApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "On"

        buildoptions
        {
            "/bigobj"
        }

        targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "stdafx.h"
        pchsource "DentyEngine/src/stdafx.cpp"

        files
        {
            "DentyEngine/src/**.cpp",
            "DentyEngine/src/**.hpp",
            "DentyEngine/src/**.h",
            "DentyEngine/Vendor/imgui/**.cpp",
            "DentyEngine/Vendor/imgui/**.h",
            "DentyEngine/Vendor/imguizmo/**.cpp",
            "DentyEngine/Vendor/imguizmo/**.h"
        }

        includedirs
        {
            "DentyEngine/src/",
            "%{IncludeDir.vendor}",
            "%{IncludeDir.assimp}",
            "%{IncludeDir.bullet3}",
            "%{IncludeDir.bullet3_internal}",
            "%{IncludeDir.cereal}",
            "%{IncludeDir.directxtk}",
            "%{IncludeDir.filewatch}",
            "%{IncludeDir.imgui}",
            "%{IncludeDir.imguizmo}",
            "%{IncludeDir.mono}",
            "%{IncludeDir.nameof}",
            "%{IncludeDir.plog}",
            "%{IncludeDir.stb}"
        }

        links
        {
            "%{Library.directxtk}",
            "%{Library.mono_libmono_static_sgen}",
            "%{Library.d3d11}",
            "%{Library.dxgi}"
        }

        filter "files:DentyEngine/Vendor/imgui/**.cpp"
            flags "NoPCH"

        filter "files:DentyEngine/Vendor/imguizmo/**.cpp"
            flags "NoPCH"

        filter "system:windows"
            systemversion "latest"

            links
            {
                "%{Library.winsock}",
                "%{Library.winmm}",
                "%{Library.win_version}",
                "%{Library.bcrypt}",
                "%{Library.xinput}"
            }

        filter "configurations:Debug"
            defines "DENTY_DEBUG"
            runtime "Debug"
            symbols "On"

            libdirs
            {
                "%{LibraryDir.assimp_debug}",
                "%{LibraryDir.bullet3_debug}",
                "%{LibraryDir.directxtk_debug}",
                "%{LibraryDir.mono_debug}"
            }

            links
            {
                "%{Library.assimp_debug}",
                "%{Library.bullet3_common_debug}",
                "%{Library.bullet3_collision_debug}",
                "%{Library.bullet3_dynamics_debug}",
                "%{Library.bullet3_inverse_dynamics_debug}",
                "%{Library.bullet3_softbody_debug}",
                "%{Library.bullet3_linear_math_debug}"
            }

            postbuildcommands 
            {
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Debug/assimp-vc143-mtd.dll %{wks.location}/bin/%{outputdir}/DentyEngine/assimp-vc143-mtd.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Debug/zlibd1.dll %{wks.location}/bin/%{outputdir}/DentyEngine/zlibd1.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Debug/minizip.dll %{wks.location}/bin/%{outputdir}/DentyEngine/minizip.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Debug/poly2tri.dll %{wks.location}/bin/%{outputdir}/DentyEngine/poly2tri.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Debug/pugixml.dll %{wks.location}/bin/%{outputdir}/DentyEngine/pugixml.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/zlib1.dll %{wks.location}/bin/%{outputdir}/DentyEngine/zlib1.dll"

            }

        filter "configurations:Release"
            defines "DENTY_RELEASE"
            runtime "Release"
            symbols "On"

            libdirs
            {
                "%{LibraryDir.assimp_release}",
                "%{LibraryDir.bullet3_release}",
                "%{LibraryDir.directxtk_release}",
                "%{LibraryDir.mono_release}"
            }

            links
            {
                "%{Library.assimp_release}",
                "%{Library.bullet3_common_release}",
                "%{Library.bullet3_collision_release}",
                "%{Library.bullet3_dynamics_release}",
                "%{Library.bullet3_inverse_dynamics_release}",
                "%{Library.bullet3_softbody_release}",
                "%{Library.bullet3_linear_math_release}"
            }

            postbuildcommands 
            {
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/assimp-vc143-mt.dll %{wks.location}/bin/%{outputdir}/DentyEngine/assimp-vc143-mt.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/zlib1.dll %{wks.location}/bin/%{outputdir}/DentyEngine/zlib1.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/assimp-vc143-mt.dll %{wks.location}/bin/%{outputdir}/DentyEngine/assimp-vc143-mt.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/minizip.dll %{wks.location}/bin/%{outputdir}/DentyEngine/minizip.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/poly2tri.dll %{wks.location}/bin/%{outputdir}/DentyEngine/poly2tri.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/pugixml.dll %{wks.location}/bin/%{outputdir}/DentyEngine/pugixml.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/zlib1.dll %{wks.location}/bin/%{outputdir}/DentyEngine/zlib1.dll"
            }


        filter "configurations:Dist"
            defines "DENTY_DIST"
            runtime "Release"
            symbols "On"

            libdirs
            {
                "%{LibraryDir.assimp_release}",
                "%{LibraryDir.bullet3_release}",
                "%{LibraryDir.directxtk_release}",
                "%{LibraryDir.mono_release}"
            }

            links
            {
                "%{Library.assimp_release}",
                "%{Library.bullet3_common_release}",
                "%{Library.bullet3_collision_release}",
                "%{Library.bullet3_dynamics_release}",
                "%{Library.bullet3_inverse_dynamics_release}",
                "%{Library.bullet3_softbody_release}",
                "%{Library.bullet3_linear_math_release}"
            }

            postbuildcommands 
            {
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/assimp-vc143-mt.dll %{wks.location}/bin/%{outputdir}/DentyEngine/assimp-vc143-mt.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/minizip.dll %{wks.location}/bin/%{outputdir}/DentyEngine/minizip.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/poly2tri.dll %{wks.location}/bin/%{outputdir}/DentyEngine/poly2tri.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/pugixml.dll %{wks.location}/bin/%{outputdir}/DentyEngine/pugixml.dll",
                "{COPYFILE} %{wks.location}/DentyEngine/Vendor/DLL/Release/zlib1.dll %{wks.location}/bin/%{outputdir}/DentyEngine/zlib1.dll"
            }