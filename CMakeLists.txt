cmake_minimum_required(VERSION 3.10)
project(Fryy_55scorner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

include_directories(SYSTEM "D:/libs/D++ v10.1/include/dpp-10.1")
link_directories("D:/libs/D++ v10.1/lib/dpp-10.1")

add_executable(Fryy_55scorner
    src/main.cpp
    src/include/logger.cpp
)
set_target_properties(Fryy_55scorner PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin$<0:>)
target_link_libraries(Fryy_55scorner dpp)