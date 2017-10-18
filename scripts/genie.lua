-- https://www.youtube.com/watch?v=_vArtdDTrTM
-- https://github.com/bkaradzic/bgfx/blob/master/scripts/genie.lua
-- https://github.com/nem0/LumixEngine/blob/master/projects/genie.lua

PROJ_DIR = path.getabsolute("..")
SOLUTION_DIR = PROJ_DIR .. "/project"
BIN_DIR = PROJ_DIR .. "/bin"

function default_config()
	configuration "Debug"
		targetdir(BIN_DIR .. "/Debug")
		defines { "DEBUG", "_DEBUG" }
		flags { "Symbols", "WinMain" }

	configuration "Release"
		targetdir(BIN_DIR .. "/Release")
		defines { "NODEBUG" }
		flags { "Optimize", "WinMain" }
end

solution "autumn_jam"
	configuration {"linux-*"}
		buildoptions {
			"-fPIC", 				 -- -fpic ?
			"-fstack-protector",
			"-Wa,--noexecstack",
			"-ffunction-sections",
			"-Wno-psabi",
			"-Wunused-value",
			"-Wundef",
			"-msse2",				 -- SSE2
			"-no-canonical-prefixes", -- keep it relative
		}
	configuration { "linux-*", "x64" }
		buildoptions { "-m64", }
		buildoptions_cpp {
			"-std=c++14"
		}

	configuration{}


	configurations {
		"Debug",
		"Release",
	}

	platforms { "x64" }
	flags {
		"FatalWarnings",
		"NoPCH",
		"NoExceptions",
		"NoRTTI", 					-- Ha!
		"NoEditAndContinue",		-- rip visual studio
	}
	location(SOLUTION_DIR)
	includedirs {
		"../src",
		"../external",
	}
	language "C++"


project "i_dont_know"
	kind "ConsoleApp"

	files { "../src/main.cpp" }

	default_config()

