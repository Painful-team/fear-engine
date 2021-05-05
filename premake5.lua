workspace "FearEngine"
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	configurations 
	{
		"Release", 
		"Debug"
	}

	architecture "x86_64"
	staticruntime "on"
	flags "MultiProcessorCompile"
	startproject "Engine"

	
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
		".editorconfig",
		"src/**.hpp",
		"src/**.cpp",
		"include/glm/glm/**.*",
		"include/stb/**.h"
	}

	includedirs
	{
		"src",
       	"include/glad/include",
       	"include/GLFW/include",
       	"include/glm/include",
       	"include/imgui",
       	"include/stb"
    }
		
	links 
	{ 
		"GLFW",
		"glad",
		"imgui",
		"glm"
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
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"


newaction {
   trigger     = "clean",
   description = "clean the software",
   execute     = function ()
      print("Clean premakefiles...")
	  
	  print("Cleaning vs files")
	  os.remove ("*.sln")
	  os.rmdir(".vs")
	  os.remove ("*.sln")
      os.remove("**.vcxproj*")
	  print("Done")
	  
	  print("Cleaning makefiles")
	  os.remove("**Makefile")
	  os.remove("**.make")
	  print("Done")
	  
	  print("Cleaning cmake")
	  os.rmdir ("**CMakeFiles/")
	  os.remove ("*CMakeFiles")
	  os.remove ("*CMakeCache.txt")
	  os.remove ("**.cmake")
	  print("Done")
	  
      print("Cleaning done.")
   end
}
