project "entt"
	kind "StaticLib"
	language "C++"
	
	files
	{
		"**.cpp",
		"**.hpp",
		"**.inl",
	}

	includedirs
    {
        "src"
    }

	defines 
	{
		"ENTT_NOEXCEPTION", 
		"ENTT_ASSERT"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
