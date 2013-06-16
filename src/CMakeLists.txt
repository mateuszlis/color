add_library(commandLine colorCommandLine.cxx)
add_executable(color main.cc)
target_link_libraries(color commandLine)
