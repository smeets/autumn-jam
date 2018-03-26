-- https://www.youtube.com/watch?v=_vArtdDTrTM
-- https://github.com/bkaradzic/bgfx/blob/master/scripts/genie.lua
-- https://github.com/nem0/LumixEngine/blob/master/projects/genie.lua

MODULE_DIR	 = path.getabsolute("../")
JAM_DIR		 = path.getabsolute("..")

local BUILD_DIR = path.join(JAM_DIR, ".build")
local THIRD_PARTY_DIR = path.join(JAM_DIR, "3rdparty")

SDL2_DIR = os.getenv("SDL2_DIR")
BX_DIR = os.getenv("BX_DIR") or path.getabsolute(path.join(JAM_DIR, "../bx"))
BIMG_DIR = os.getenv("BIMG_DIR") or path.getabsolute(path.join(JAM_DIR, "../bimg"))
BGFX_DIR = os.getenv("BGFX_DIR") or path.getabsolute(path.join(JAM_DIR, "../bgfx"))

function exit()
	print("For more info see: https://github.com/smeets/autumn-jam")
	os.exit()
end

function check_dep(lib, dir)
	if not os.isdir(dir) then
		print(lib .. " not found at " .. dir)
		exit()
	end
end
check_dep("bgfx", 		 BGFX_DIR)
check_dep("bx", 		 BX_DIR)
check_dep("bimg", 		 BIMG_DIR)

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
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR,   "include"),
		path.join(THIRD_PARTY_DIR, "vg-renderer/include")
	}

	language "C++"
	startproject "i_dont_know"

dofile (path.join(BX_DIR, "scripts/toolchain.lua"))
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
		path.join(BX_DIR, 	"include"),
		path.join(BIMG_DIR,	"include"),
		path.join(BGFX_DIR, "include"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(SDL2_DIR, "include"),
		path.join(THIRD_PARTY_DIR, "vg-renderer/include")
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

dofile(path.join(BGFX_DIR,   "scripts/bgfx.lua"))

group "libs"
bgfxProject("", "StaticLib", {})

dofile(path.join(BX_DIR,   "scripts/bx.lua"))
dofile(path.join(BIMG_DIR, "scripts/bimg.lua"))
dofile(path.join(BIMG_DIR, "scripts/bimg_decode.lua"))
