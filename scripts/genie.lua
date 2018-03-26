-- https://www.youtube.com/watch?v=_vArtdDTrTM
-- https://github.com/bkaradzic/bgfx/blob/master/scripts/genie.lua
-- https://github.com/nem0/LumixEngine/blob/master/projects/genie.lua

MODULE_DIR	 = path.getabsolute("../")
JAM_DIR		 = path.getabsolute("..")

local BUILD_DIR = path.join(JAM_DIR, ".build")
local THIRD_PARTY_DIR = path.join(JAM_DIR, "3rdparty")

SDL2_DIR = os.getenv("SDL2_DIR")
BGFX_DIR = path.join(THIRD_PARTY_DIR, "bgfx")
BX_DIR   = path.join(THIRD_PARTY_DIR, "bx")
BIMG_DIR = path.join(THIRD_PARTY_DIR, "bimg")

function exit()
	print("For more info see: https://github.com/smeets/autumn-jam")
	os.exit()
end

function check_dep(lib)
	if not os.isdir(path.join(THIRD_PARTY_DIR, lib)) then
		print(lib .. " not found")
		exit()
	end
end
check_dep("bgfx")
check_dep("bx")
check_dep("bimg")
check_dep("vg-renderer")

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
		links {"X11", "pthread"}

	configuration { "linux-*", "x64" }
		buildoptions { "-m64" }
		buildoptions_cpp {
			"-std=c++11"
		}

	configuration { "vs*" }
		includedirs {
			path.join(THIRD_PARTY_DIR, "SDL2/include")
		}
		libdirs {
			path.join(THIRD_PARTY_DIR, "SDL2/lib/x64")
		}

	configuration{}

	configurations {
		"Debug",
		"Release",
	}

	platforms { "x64" }
	flags {
		"NoPCH",
		"NoExceptions",
		"NoRTTI", 					-- Ha!
		"NoEditAndContinue",		-- rip visual studio
	}

	location(path.join(BUILD_DIR, _ACTION))

	includedirs {
		"../src",
		path.join(THIRD_PARTY_DIR, "**/include")
	}

	language "C++"
	startproject "i_dont_know"

dofile (path.join(THIRD_PARTY_DIR, "bx/scripts/toolchain.lua"))
if not toolchain(BUILD_DIR, THIRD_PARTY_DIR) then
	return -- no action specified
end


function copyLib()
end

project "i_dont_know"
	kind "ConsoleApp"

	links {
		"bgfx",
		"bx",
		"bimg",
		"bimg_decode",
		"SDL2"
	}

	includedirs {
		path.join(THIRD_PARTY_DIR, "**/include"),
	}

	files {
		path.join(JAM_DIR, "src/main.cpp"),
		path.join(JAM_DIR, "src/**"),
		path.join(THIRD_PARTY_DIR, "vg-renderer/src/**")
	}

	configuration "Debug"
		defines { "DEBUG", "_DEBUG" }
		flags { "Symbols", "WinMain" }

	configuration "Release"
		defines { "NODEBUG" }
		flags { "Optimize", "WinMain" }

	configuration { "linux-*" }
		links { "GL" }

	configuration { "vs*" }
		debugdir(path.join(BUILD_DIR, "win64_" .. _ACTION, "bin"))


dofile(path.join(THIRD_PARTY_DIR, "bgfx/scripts/bgfx.lua"))

group "libs"
bgfxProject("", "StaticLib", {})

dofile(path.join(THIRD_PARTY_DIR, "bx/scripts/bx.lua"))
dofile(path.join(THIRD_PARTY_DIR, "bimg/scripts/bimg.lua"))
dofile(path.join(THIRD_PARTY_DIR, "bimg/scripts/bimg_decode.lua"))
