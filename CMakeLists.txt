cmake_minimum_required(VERSION 3.10)
project(CS-6220-HPC-Project-Monte-Carlo CXX)

find_package(MPI REQUIRED)

add_executable(monte_carlo_pi pi_calc.cpp)

target_link_libraries(monte_carlo_pi PUBLIC MPI::MPI_CXX)

set_target_properties(monte_carlo_pi PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
