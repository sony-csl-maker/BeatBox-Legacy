cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(custom_ops)

find_package(Torch REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)

add_executable(BeatboxGan Main.cpp)
target_link_libraries(BeatboxGan Boost::program_options)
target_link_libraries(BeatboxGan Boost::filesystem)
target_link_libraries(BeatboxGan "${TORCH_LIBRARIES}")
set_property(TARGET BeatboxGan PROPERTY CXX_STANDARD 20)