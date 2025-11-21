#CMake Min Version
cmake_minimum_required(VERSION 3.20)

#Minimum C++ Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("P4_Scene_Streaming")

# If having issues with copying dlls, replace the $VCPKG_ROOT with your full path to vcpkg install
set(ENV{VCPKG_ROOT} "C:/Program Files/vcpkg")

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(re2 REQUIRED)

# Handling third party refs
include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty CACHE PATH "Base Directory if libs" FORCE)
# Fetch Content

# Asset Dir
set(RESOURCES_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets CACHE PATH "Base Directory of resources" FORCE)

set(PROTO_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/proto/generated" CACHE PATH "Proto Files" FORCE)
set(COMMON_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src_common" CACHE PATH "Common Files" FORCE)

file(GLOB_RECURSE PROTO_SRC "${PROTO_SRC_PATH}/*.cc")
file(GLOB_RECURSE PROTO_HEADERS "${PROTO_SRC_PATH}/*.h")

# Common 
file(GLOB_RECURSE COMMON_SRC "${COMMON_SRC_PATH}/*.cpp" "${COMMON_SRC_PATH}/*.h")

# ImGui Setup
set(IMGUI_PATH "${FETCHCONTENT_BASE_DIR}/DearImGUI")
set(IMGUI_SOURCES 
	"${IMGUI_PATH}/imgui.cpp"
	"${IMGUI_PATH}/imgui_draw.cpp"
	"${IMGUI_PATH}/imgui_widgets.cpp"
	"${IMGUI_PATH}/imgui_tables.cpp"
	"${IMGUI_PATH}/imgui_demo.cpp"
	"${IMGUI_PATH}/backends/imgui_impl_glfw.cpp"
	"${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(
	imgui PUBLIC 
	${IMGUI_PATH} 
	"${IMGUI_PATH}/backends"
	$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
)

# Server Exe Setup
set(SERVER_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src_server CACHE PATH "Project Server SRC" FORCE)
file(GLOB_RECURSE SERVER_SRC CONFIGURE_DEPENDS "${SERVER_SRC_PATH}/*.cpp" "${SERVER_SRC_PATH}/*.h")
add_executable(SceneStreaming_Server "SceneStreaming_Server.cpp" "SceneStreaming_Server.h" ${PROTO_SRC} ${PROTO_HEADERS} ${COMMON_SRC} ${SERVER_SRC})
target_compile_definitions(SceneStreaming_Server PUBLIC "PROTOBUF_USE_DLLS")
target_link_libraries(
	SceneStreaming_Server PUBLIC
	protobuf::libprotobuf
	gRPC::grpc
	gRPC::grpc++
	gRPC::grpc++_reflection
	OpenGL::GL
)
target_link_libraries(
	SceneStreaming_Server PRIVATE
	glfw
	glad::glad
	imgui
	tinyobjloader::tinyobjloader
)

# Client Exe Setup
set(CLIENT_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src_client CACHE PATH "Project Client SRC" FORCE)
file(GLOB_RECURSE CLIENT_SRC CONFIGURE_DEPENDS "${CLIENT_SRC_PATH}/*.cpp" "${CLIENT_SRC_PATH}/*.h")
add_executable(SceneStreaming_Client "SceneStreaming_Client.cpp" "SceneStreaming_Client.h" ${PROTO_SRC} ${PROTO_HEADERS} ${COMMON_SRC} ${CLIENT_SRC})
target_compile_definitions(SceneStreaming_Client PUBLIC "PROTOBUF_USE_DLLS")
target_link_libraries(
	SceneStreaming_Client PUBLIC
	protobuf::libprotobuf
	gRPC::grpc
	gRPC::grpc++
	gRPC::grpc++_reflection
	OpenGL::GL
)
target_link_libraries(
	SceneStreaming_Client PRIVATE
	glfw
	glad::glad
	imgui
	tinyobjloader::tinyobjloader
)

set(VCPKG_BIN_DIR "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/bin")

# For copying the re2 and cares dlls to Server
#  add_custom_command(TARGET SceneStreaming_Server POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#      "${VCPKG_BIN_DIR}/cares.dll"
#      "${VCPKG_BIN_DIR}/re2.dll"
#      $<TARGET_FILE_DIR:SceneStreaming_Server>
#  )

# For copying the re2 and cares dlls to Client
#  add_custom_command(TARGET SceneStreaming_Client POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#      "${VCPKG_BIN_DIR}/cares.dll"
#      "${VCPKG_BIN_DIR}/re2.dll"
#      $<TARGET_FILE_DIR:SceneStreaming_Client>
#  )

# Post Build Assets copy for server
add_custom_command(
	TARGET SceneStreaming_Server POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/Assets 
			$<TARGET_FILE_DIR:SceneStreaming_Server>/Assets
)

# Post Build Assets copy for client (testing only)
add_custom_command(
	TARGET SceneStreaming_Client POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/Assets 
			$<TARGET_FILE_DIR:SceneStreaming_Server>/Assets
)
