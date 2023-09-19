project(server)
file(GLOB SOURCES "src/*.cpp")
add_executable(server ${SOURCES})