cmake_minimum_required(VERSION 3.25)
project(SafetyClang LANGUAGES C CXX)
# LLVM accepts the same major.minor with a patch release of at least 19.1.0;
# clang-tidy modules are not ABI-compatible across minor releases. Build
# against the same LLVM as the clang-tidy host; the validated reference is 19.1.7.
find_package(LLVM 19.1.0 REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(SafetyMisra MODULE src/MisraModule.cpp src/misra/Common.cpp
  src/misra/Directives.cpp src/misra/Basic.cpp src/misra/Declarations.cpp
  src/misra/Types.cpp src/misra/Expressions.cpp src/misra/Control.cpp
  src/misra/Pointers.cpp src/misra/Preprocessor.cpp src/misra/Library.cpp)
target_include_directories(SafetyMisra SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
target_compile_definitions(SafetyMisra PRIVATE ${LLVM_DEFINITIONS})
if(NOT LLVM_ENABLE_RTTI)
  target_compile_options(SafetyMisra PRIVATE -fno-rtti)
endif()
# Resolve Clang APIs from the matching clang-tidy host; linking another copy
# of static clang-tidy libraries would duplicate its module registry.
set_target_properties(SafetyMisra PROPERTIES PREFIX "")
include(CTest)
if(BUILD_TESTING)
 find_package(Python3 REQUIRED COMPONENTS Interpreter)
 find_program(CLANG_TIDY NAMES clang-tidy-19 REQUIRED)
 add_test(NAME prohibited-goto COMMAND ${Python3_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/check.py ${CLANG_TIDY}
  $<TARGET_FILE:SafetyMisra> ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
add_executable(safety-source-index src/SourceIndex.cpp)
target_include_directories(safety-source-index SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
target_compile_definitions(safety-source-index PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(safety-source-index PRIVATE clang-cpp LLVM)
if(BUILD_TESTING)
 add_test(NAME source-index COMMAND ${Python3_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/index.py $<TARGET_FILE:safety-source-index>
  ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
if(BUILD_TESTING)
 add_test(NAME original-policies COMMAND ${Python3_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/policies.py ${CLANG_TIDY}
  $<TARGET_FILE:SafetyMisra> ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
if(BUILD_TESTING)
 add_test(NAME misra-c2012 COMMAND ${Python3_EXECUTABLE}
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/misra.py ${CLANG_TIDY}
  $<TARGET_FILE:SafetyMisra> ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

install(TARGETS safety-source-index RUNTIME DESTINATION bin)
install(TARGETS SafetyMisra LIBRARY DESTINATION lib/safety)
