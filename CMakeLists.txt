cmake_minimum_required(VERSION 3.10)
project(Fryy_55scorner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

include_directories(SYSTEM "D:/libs/D++ v10.1.2/include/dpp-10.1")
link_directories("D:/libs/D++ v10.1.2/lib/dpp-10.1")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${SRC_DIR}
        ${SRC_DIR}/data
        ${SRC_DIR}/general-purpose
        ${SRC_DIR}/utils
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin$<0:>)
target_link_libraries(${PROJECT_NAME} dpp)