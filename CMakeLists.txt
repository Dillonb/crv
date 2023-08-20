cmake_minimum_required (VERSION 3.18)
project (CRV)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_compile_options(-Werror=switch)


set (riscv_opcodes ${CMAKE_BINARY_DIR}/riscv-opcodes-master)
set (riscv_opcodes_zip ${CMAKE_BINARY_DIR}/riscv-opcodes.zip)

if (NOT EXISTS ${riscv_opcodes_zip})
    message("Downloading riscv-opcodes")
    file(DOWNLOAD "https://github.com/riscv/riscv-opcodes/archive/refs/heads/master.zip" ${riscv_opcodes_zip})
else()
    message("riscv-opcodes already downloaded")
endif()

if (NOT EXISTS ${riscv_opcodes})
    message("Extracting riscv-opcodes")
    file(ARCHIVE_EXTRACT
    INPUT ${riscv_opcodes_zip})
else()
    message("riscv-opcodes already extracted")
endif()

set(gen_emitters_script ${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen-emitters.py)
set(generated_emitters_header_dir ${CMAKE_CURRENT_BINARY_DIR})
set(generated_emitters_header ${generated_emitters_header_dir}/crv_emitters.h)

find_package(Python 3.6 REQUIRED)
add_custom_command(
    OUTPUT ${generated_emitters_header}
    COMMAND ${Python_EXECUTABLE} ${gen_emitters_script} 
            --output ${generated_emitters_header} 
            --opcodes ${riscv_opcodes}
    DEPENDS ${gen_emitters_script}
)

add_library(crv src/crv.c src/crv.h src/crv_constants.h ${generated_emitters_header})
target_include_directories(crv PUBLIC ${generated_emitters_header_dir})

add_executable(test test/test.c)
target_include_directories(test PUBLIC src)
target_link_libraries(test crv)

add_executable(test_functioncall test/test_functioncall.c)
target_include_directories(test_functioncall PUBLIC src)
target_link_libraries(test_functioncall crv)

find_package(Capstone)
if(Capstone_FOUND)
    target_link_libraries(test ${Capstone_LIBRARIES})
    target_include_directories(test PRIVATE ${Capstone_INCLUDE_DIRS})
    target_compile_definitions(test PRIVATE HAVE_CAPSTONE)
endif()