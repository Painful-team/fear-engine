workspace "FearEngine"
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	architecture "x86_64"
	staticruntime "on"
	flags "MultiProcessorCompile"
	startproject "Engine"

	configurations
	{
		"Release",
		"Debug"
	}
	
group "Dependencies"
	include  "include/glad"
	include  "include/GLFW"
	include  "include/glm"
	include  "include/imgui"
group ""

project "Engine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	files 
	{
		"resources/shaders/*.*",
		".editorconfig",
		".clang-format",
		"src/**.hpp",
		"src/**.cpp",
		"include/containers/*.h",
		"include/tinyobj/*.*",
		"include/glm/glm/**.*",
		"include/boost/**.*",
		"include/stb/**.*",
		"include/entt/**.*",
		"include/json/**.*",
		"include/spdlog/include/**.*"
	}

	includedirs
	{
		"src",
		"include/containers",
		"include/tinyobj",
       	"include/glad/include",
       	"include/GLFW/include",
       	"include/glm/include",
		"include/boost",
       	"include/imgui",
       	"include/stb",
		"include/entt",
		"include/spdlog/include/",
		"include/json"
    }
		
	links 
	{ 
		"GLFW",
		"glad",
		"imgui",
		"glm"
	}

	defines 
	{
		"ENTT_NOEXCEPTION",
		"NOMINMAX",
		"SPDLOG_NOEXPECTION"
	}

	filter { "system:windows" }
		links "opengl32"

	configuration { "linux", "gmake2" }
		links 
		{ 
			"glfw",
			"gdi32",
			"opengl32"
		}

	filter "configurations:Debug"
		defines
		{
			"_DEBUG",
			"SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG"
		}		
		
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		defines
		{
			"NDEBUG",
			"SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF",
			"STBI_NO_FAILURE_STRINGS"
		}

newaction {
   trigger     = "clean",
   description = "clean the software",
   execute     = function ()
      print("Clean premakefiles...")
	  
	  print("Cleaning Visual studio files")
	  os.rmdir(".vs")
	  os.remove("*.sln")
      os.remove("**.vcxproj*")
	  print("Done")
	  
	  print("Cleaning compiled files")
	  os.rmdir("bin")
	  os.rmdir("obj")
	  print("Done")
	  
	  print("Cleaning makefiles")
	  os.remove("**Makefile")
	  os.remove("**.make")
	  print("Done")
	  
	  print("Cleaning cmake")
	  os.rmdir("**CMakeFiles/")
	  os.remove("*CMakeFiles")
	  os.remove("*CMakeCache.txt")
	  os.remove("**.cmake")
	  print("Done")
	  
      print("Cleaning done.")
   end
}
