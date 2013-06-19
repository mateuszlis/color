add_library(commandLine colorCommandLine.cxx)
add_executable(color main.cc)
install(TARGETS color DESTINATION bin)
target_link_libraries(color commandLine)
