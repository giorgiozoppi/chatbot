cmake_minimum_required(VERSION 3.11.3)

set(CMAKE_CXX_STANDARD 17)

project(Membot)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

file(GLOB project_SRCS src/*.cpp)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
add_executable(membot ${project_SRCS})
target_link_libraries(membot ${wxWidgets_LIBRARIES})
target_include_directories(membot PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_compile_options(membot PRIVATE -Wall -Wextra -Werror -g -ggdb -O0)
#target_compile_options(membot PRIVATE -Wall -Wextra -Werror -g -ggdb -O0 -fno-elide-constructors -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer)