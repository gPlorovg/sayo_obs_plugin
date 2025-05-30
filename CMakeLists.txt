cmake_minimum_required(VERSION 3.28...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE
        src/plugin-main.cpp
        src/server_gRPC/grpc_client.cpp
        src/server_gRPC/grpc_client.h
        src/server_gRPC/sayo.pb.cc
        src/server_gRPC/sayo.grpc.pb.cc
)

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

if(ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets Core)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header -Wno-comma>
  )
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON
  )
endif()

set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SAMPLERATE REQUIRED samplerate)

include_directories(${SAMPLERATE_INCLUDE_DIRS})
link_directories(${SAMPLERATE_LIBRARY_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${SAMPLERATE_LIBRARIES})

# === Protobuf ===
find_package(Protobuf REQUIRED)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${Protobuf_INCLUDE_DIRS})

# === gRPC ===
find_package(gRPC REQUIRED)

# === Добавить к библиотеке все нужные файлы (если ещё не добавил) ===
# add_library(${CMAKE_PROJECT_NAME} MODULE ... server_gRPC/sayo.pb.cc server_gRPC/sayo.grpc.pb.cc)

# === Самое главное: линкуем все зависимости ===
target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
        OBS::libobs
        ${SAMPLERATE_LIBRARIES}
        ${Protobuf_LIBRARIES}
        gRPC::grpc++
        gRPC::grpc
)