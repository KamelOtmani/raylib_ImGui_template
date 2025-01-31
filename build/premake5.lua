newoption
{
	trigger = "graphics",
	value = "OPENGL_VERSION",
	description = "version of OpenGL to build raylib against",
	allowed = {
		{ "opengl11", "OpenGL 1.1"},
		{ "opengl21", "OpenGL 2.1"},
		{ "opengl33", "OpenGL 3.3"},
		{ "opengl43", "OpenGL 4.3"}
	},
	default = "opengl33"
}

function download_progress(total, current)
    local ratio = current / total;
    ratio = math.min(math.max(ratio, 0), 1);
    local percent = math.floor(ratio * 100);
    print("Download progress (" .. percent .. "%/100%)")
end

function check_raylib()
    os.chdir("external")
    if(os.isdir("raylib-master") == false) then
        if(not os.isfile("raylib-master.zip")) then
            print("Raylib not found, downloading from github")
            local result_str, response_code = http.download("https://github.com/raysan5/raylib/archive/refs/heads/master.zip", "raylib-master.zip", {
                progress = download_progress,
                headers = { "From: Premake", "Referer: Premake" }
            })
        end
        print("Unzipping to " ..  os.getcwd())
        zip.extract("raylib-master.zip", os.getcwd())
        os.remove("raylib-master.zip")
    end
    os.chdir("../")
end


function check_imgui()
    os.chdir("external")
	if(os.isdir("imgui") == false and os.isdir("imgui-docking") == false) then
		if(not os.isfile("imgui-master.zip")) then
			print("imgui not found, downloading from github")
			local result_str, response_code = http.download("https://github.com/ocornut/imgui/archive/refs/heads/docking.zip", "imgui-docking.zip", {
				progress = download_progress,
				headers = { "From: Premake", "Referer: Premake" }
			})
		end
		print("Unzipping to " ..  os.getcwd())
		zip.extract("imgui-docking.zip", os.getcwd())
		os.remove("imgui-docking.zip")
	end
	if(os.isdir("implot") == false and os.isdir("implot-master") == false) then
    
		if(not os.isfile("implot-master.zip")) then
			print("implot not found, downloading from github")
			local result_str, response_code = http.download("https://github.com/epezent/implot/archive/refs/heads/master.zip", "implot-master.zip", {
				progress = download_progress,
				headers = { "From: Premake", "Referer: Premake" }
			})
		end
		print("Unzipping to " ..  os.getcwd())
		zip.extract("implot-master.zip", os.getcwd())
		os.remove("implot-master.zip")
    end
    os.chdir("../")
end


function check_rlImGui()
    os.chdir("external")
	if(os.isdir("rlImGui") == false and os.isdir("rlImGui-master") == false) then
		if(not os.isfile("rlImGui-master.zip")) then
			print("imgui not found, downloading from github")
			local result_str, response_code = http.download("https://github.com/raylib-extras/rlImGui/archive/refs/heads/master.zip", "rlImGui-master.zip", {
				progress = download_progress,
				headers = { "From: Premake", "Referer: Premake" }
			})
		end
		print("Unzipping to " ..  os.getcwd())
		zip.extract("rlImGui-master.zip", os.getcwd())
		os.remove("rlImGui-master.zip")
	end
    os.chdir("../")
end

function build_externals()
     print("calling externals")
     check_raylib()
     check_imgui()
     check_rlImGui()
end

function platform_defines()
    defines{"PLATFORM_DESKTOP"}

    filter {"options:graphics=opengl43"}
        defines{"GRAPHICS_API_OPENGL_43"}

    filter {"options:graphics=opengl33"}
        defines{"GRAPHICS_API_OPENGL_33"}

    filter {"options:graphics=opengl21"}
        defines{"GRAPHICS_API_OPENGL_21"}

    filter {"options:graphics=opengl11"}
        defines{"GRAPHICS_API_OPENGL_11"}

    filter {"options:graphics=openges3"}
        defines{"GRAPHICS_API_OPENGL_ES3"}

    filter {"options:graphics=openges2"}
        defines{"GRAPHICS_API_OPENGL_ES2"}

    filter {"system:macosx"}
        disablewarnings {"deprecated-declarations"}

    filter {"system:linux"}
        defines {"_GLFW_X11"}
        defines {"_GNU_SOURCE"}
-- This is necessary, otherwise compilation will fail since
-- there is no CLOCK_MONOTOMIC. raylib claims to have a workaround
-- to compile under c99 without -D_GNU_SOURCE, but it didn't seem
-- to work. raylib's Makefile also adds this flag, probably why it went
-- unnoticed for so long.
-- It compiles under c11 without -D_GNU_SOURCE, because c11 requires
-- to have CLOCK_MONOTOMIC
-- See: https://github.com/raysan5/raylib/issues/2729

    filter{}
end

 raylib_dir = "external/raylib-master"
 imgui_dir = "external/imgui-docking"
 implot_dir = "external/implot-master"
 rlimgui_dir = "external/rlImGui-main"

workspaceName = 'MircostructureCpp'
baseName = path.getbasename(path.getdirectory(os.getcwd()));

--if (baseName ~= 'raylib-quickstart') then
    workspaceName = baseName
--end

if (os.isdir('build_files') == false) then
    os.mkdir('build_files')
end

if (os.isdir('external') == false) then
    os.mkdir('external')
end

workspace (workspaceName)
    location "../"
    configurations { "Debug", "Release"}
    platforms { "x64", "x86", "ARM64"}
    cppdialect "C++latest"
    defaultplatform ("x64")

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter { "platforms:x64" }
        architecture "x86_64"

    filter { "platforms:Arm64" }
        architecture "ARM64"

    filter {}

    targetdir "bin/%{cfg.buildcfg}/"

    build_externals()

    startproject(workspaceName)

    project (workspaceName)
        kind "ConsoleApp"
        location "build_files/"
        targetdir "../bin/%{cfg.buildcfg}"
        language "C++"
        cppdialect "C++latest"
        warnings "Extra" -- enable all warnings
        flags { 
            "MultiProcessorCompile",
            -- "LinkTimeOptimization",
            -- "FatalCompileWarnings", -- uncomment this to treat warnings as errors
         }
        filter "action:vs*"
            debugdir "$(SolutionDir)"

        filter {"action:vs*", "configurations:Release"}
            kind "WindowedApp"
            entrypoint "mainCRTStartup"

        filter{}

        vpaths 
        {
            ["Header Files/*"] = { "../include/**.h",  "../include/**.hpp", "../src/**.h", "../src/**.hpp"},
            ["Source Files/*"] = {"../src/**.c", "src/**.cpp"},
        }
        files {"../src/**.c", "../src/**.cpp", "../src/**.h", "../src/**.hpp", "../include/**.h", "../include/**.hpp"}
    
        includedirs { "../src" }
        includedirs { "../include" }

        links {"raylib", "rlImGui"}

        includedirs { raylib_dir .. "/src" }
        includedirs { raylib_dir .."/src/external" }
        includedirs { raylib_dir .."/src/external/glfw/include" }
        includedirs { rlimgui_dir , imgui_dir , implot_dir}
        platform_defines()

        filter "action:vs*"
            defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
            dependson {"raylib"}
            links {"raylib.lib"}
            characterset ("Unicode")
            buildoptions { "/Zc:__cplusplus" }

        filter "system:windows"
            defines{"_WIN32"}
            links {"winmm", "gdi32"}
            libdirs {"../bin/%{cfg.buildcfg}"}

        filter "system:linux"
            links {"pthread", "m", "dl", "rt", "X11"}

        filter "system:macosx"
            links {"OpenGL.framework", "Cocoa.framework", "IOKit.framework", "CoreFoundation.framework", "CoreAudio.framework", "CoreVideo.framework", "AudioToolbox.framework"}

        filter{}

    project "raylib"
        kind "StaticLib"
    
        platform_defines()

        location "build_files/"

        language "C"
        targetdir "../bin/%{cfg.buildcfg}"

        filter "action:vs*"
            defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
            characterset ("Unicode")
            buildoptions { "/Zc:__cplusplus" }
        filter{}

        includedirs {raylib_dir .. "/src", raylib_dir .. "/src/external/glfw/include" }
        vpaths
        {
            ["Header Files"] = { raylib_dir .. "/src/**.h"},
            ["Source Files/*"] = { raylib_dir .. "/src/**.c"},
        }
        files {raylib_dir .. "/src/*.h", raylib_dir .. "/src/*.c"}

        removefiles {raylib_dir .. "/src/rcore_*.c"}

        filter { "system:macosx", "files:" .. raylib_dir .. "/src/rglfw.c" }
            compileas "Objective-C"

        filter{}

project "rlImGui"
        kind "StaticLib"
        location "build_files/"
        targetdir "../bin/%{cfg.buildcfg}"
        language "C++"
        cdialect "C99"
        cppdialect "C++17"
        includedirs {raylib_dir .."/src" }
        includedirs {raylib_dir .."/src/external" }
        includedirs {raylib_dir .."/src/external/glfw/include" }
        platform_defines()
    
        filter "action:vs*"
            defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
    
        filter{}

        includedirs { rlimgui_dir , imgui_dir , implot_dir}
        vpaths 
        {
            ["Header Files"] = { rlimgui_dir .. "*.h"},
            ["Source Files"] = { rlimgui_dir .. "*.cpp"},
            ["ImGui Files"] = { imgui_dir .. "*.h",imgui_dir .. "*.cpp" },
            ["ImPlot Files"] = { implot_dir .. "*.h",implot_dir .. "*.cpp" },
        }
        files {
            imgui_dir .. "/*.h",
            imgui_dir .. "/*.cpp",
            implot_dir .. "/*.h",
            implot_dir .. "/*.cpp",
            rlimgui_dir .. "/*.cpp",
            rlimgui_dir .. "/*.h",
            rlimgui_dir .. "/extras/**.h",
            -- rlimgui_dir .. "/rlImGui.cpp"
        }
    
        defines {"IMGUI_DISABLE_OBSOLETE_FUNCTIONS","IMGUI_DISABLE_OBSOLETE_KEYIO"}