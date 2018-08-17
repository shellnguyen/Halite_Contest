-- premake5.lua
workspace "MyBot"
   location "%{'.'}/.vs"
   configurations { "Debug", "Release" }

project "MyBot"
   kind "ConsoleApp"
   language "C"
   targetdir "%{wks.location}"

   files { "**.hpp", "**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG", "_USE_MATH_DEFINES" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG", "_USE_MATH_DEFINES" }
      optimize "On"