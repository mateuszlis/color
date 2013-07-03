add_subdirectory(gmock-1.6.0)
find_package(GTest REQUIRED)
include_directories(gmock-1.6.0/gtest/include)
include_directories(gmock-1.6.0/include)
include_directories(${BASE_SRC_DIR})
add_executable (RulesTest RulesUnitTest.cc IntermediateResultUnitTest.cc RuleBoxUnitTest.cc )
add_executable (ConfigTest ConfigUnitTest.cc )
#every test has its own valgrind version
#so it is easy to turn off time-consuming valgrind tests in ctest while
#debugging
add_test(RulesTest ${EXECUTABLE_OUTPUT_PATH}/RulesTest)
add_test(RulesTest|Valgrind valgrind --leak-check=full
    --undef-value-errors=no -v --error-exitcode=1 ${EXECUTABLE_OUTPUT_PATH}/RulesTest --gtest_filter=-*zero_color_error*) 
#cppcheck
add_test(cppcheckInfo  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=information -q )
add_test(cppcheckUnusedFunctions  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=unusedFunction -q )
add_test(cppcheckStyle  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=style -q )
add_test(cppcheckAll  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=all -q )

target_link_libraries(RulesTest Rules gtest gtest_main gmock ${Boost_LIBRARIES})
target_link_libraries(ConfigTest Config Rules gtest gtest_main gmock ${Boost_LIBRARIES})
