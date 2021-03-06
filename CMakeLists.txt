################################################################################
#
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(PROJECT_NAME untitled)
SET(SOURCES
    src/Main.cpp
    src/App.hpp
    src/App.cpp
    src/Layer.hpp
    src/Layer.cpp
    src/LuaState.hpp
    src/LuaState.cpp
    src/Object.hpp
    src/Object.cpp
    src/Particle.hpp
    src/Particle.cpp
    src/PhysicsFS.hpp
    src/PhysicsFS.cpp
    src/PhysInputStream.hpp
    src/PhysInputStream.cpp
    src/PhysOutputStream.hpp
    src/PhysOutputStream.cpp
    src/State.hpp
    src/State.cpp
    src/StateManager.hpp
    src/StateManager.cpp
)

PROJECT(${PROJECT_NAME} CXX)

FIND_PACKAGE(SFML 2.3 REQUIRED COMPONENTS audio graphics network system window)
ADD_SUBDIRECTORY(physfs EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(luajit EXCLUDE_FROM_ALL)

IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
ENDIF()
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
ENDIF()

INCLUDE_DIRECTORIES(
    # list first so local headers override system headers
    ${CMAKE_CURRENT_BINARY_DIR}/luajit/
    ${CMAKE_CURRENT_SOURCE_DIR}/luajit/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/physfs/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${SFML_INCLUDE_DIR}
)

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${SOURCES}
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}

    liblua
    physfs-static
    ${SFML_LIBRARIES}
)

## Copy LuaJIT DLL/SO into root project directory
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
    COMMAND cmake -E copy_if_different
        "$<TARGET_FILE:liblua>" ${CMAKE_CURRENT_BINARY_DIR}
)

SET(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
IF(EXISTS "${DATA_DIR}" AND IS_DIRECTORY "${DATA_DIR}")
    ADD_CUSTOM_TARGET(copy_data)
    ADD_DEPENDENCIES(${PROJECT_NAME} copy_data)
    ADD_CUSTOM_COMMAND(TARGET copy_data
        COMMAND cmake -E copy_directory
            "${DATA_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/data"
    )
ENDIF()

################################################################################
# EOF
################################################################################

