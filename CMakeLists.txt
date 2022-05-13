# Set minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project_name and version
project(BarberShop VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(BarberShop src/main.cpp)

# Add the targeted cpp features
target_compile_features(BarberShop PUBLIC cxx_std_20)
