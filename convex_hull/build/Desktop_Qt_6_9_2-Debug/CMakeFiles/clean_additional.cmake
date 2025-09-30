# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/convex_hull_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/convex_hull_autogen.dir/ParseCache.txt"
  "convex_hull_autogen"
  )
endif()
