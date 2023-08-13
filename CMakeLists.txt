cmake_minimum_required (VERSION 3.10)
project (CRV)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_compile_options(-Werror=switch)

add_library(crv src/crv.c src/crv.h src/crv_constants.h)

add_executable(test_headeronly test/test_headeronly.c test/test.c)
target_include_directories(test_headeronly PUBLIC src)

add_executable(test_staticlib test/test_staticlib.c test/test.c)
target_include_directories(test_staticlib PUBLIC src)
target_link_libraries(test_staticlib crv)

find_package(Capstone)
if(Capstone_FOUND)
    target_link_libraries(test_staticlib ${Capstone_LIBRARIES})
    target_link_libraries(test_headeronly ${Capstone_LIBRARIES})
    target_include_directories(test_staticlib PRIVATE ${Capstone_INCLUDE_DIRS})
    target_include_directories(test_headeronly PRIVATE ${Capstone_INCLUDE_DIRS})
    target_compile_definitions(test_staticlib PRIVATE HAVE_CAPSTONE)
    target_compile_definitions(test_headeronly PRIVATE HAVE_CAPSTONE)
endif()