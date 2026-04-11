cmake_minimum_required(VERSION 3.28.3)
project(8085Microprocessor VERSION 1.0)

# set c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
# set(CXX_WARNINGS -Wno-macro-redefined -Wall -Wextra)
set(CXX_WARNINGS -Wall -Wextra)

# List all source files
set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(SOURCE_FILES
    ${SOURCE_DIR}/utils.cpp
    ${SOURCE_DIR}/ui_builder.cpp
    ${SOURCE_DIR}/view.cpp
    ${SOURCE_DIR}/model.cpp
    ${SOURCE_DIR}/instructions/command.cpp
    ${SOURCE_DIR}/instructions/invalid.cpp
    ${SOURCE_DIR}/instructions/adc.cpp
    ${SOURCE_DIR}/instructions/add.cpp
    ${SOURCE_DIR}/instructions/mov.cpp
    ${SOURCE_DIR}/instructions/mvi.cpp
    ${SOURCE_DIR}/instructions/sub.cpp
    ${SOURCE_DIR}/app_controller.cpp
    ${SOURCE_DIR}/8085_main.cpp
)

add_executable(8085 ${SOURCE_FILES})
target_include_directories(8085 PRIVATE ${INCLUDE_DIR})
target_link_libraries(8085 nCursesFacade ncurses)
# target_link_libraries(8085 nCursesFacade)
target_compile_options(8085 PRIVATE ${CXX_WARNINGS})

add_custom_command(TARGET 8085 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:8085> ${CMAKE_SOURCE_DIR}/bin/
    COMMENT "copying 8085 to bin/"
)
