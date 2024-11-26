# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appgui_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appgui_autogen.dir/ParseCache.txt"
  "appgui_autogen"
  )
endif()
