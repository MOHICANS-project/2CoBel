cmake_minimum_required(VERSION 3.5)


set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)

find_package(PythonLibs 2.7 REQUIRED)
include_directories(fusion PRIVATE ${PYTHON_INCLUDE_DIRS})
link_libraries(${PYTHON_LIBRARIES})

add_definitions(-std=c++11 -DDEBUG -DWITHOUT_NUMPY -ggdb -O3 -Wall -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fbranch-probabilities)


#Compilation flags
set(CMAKE_CXX_FLAGS "-Wall -O3 -mssse3 -Wno-sign-compare -pthread")

#Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs ALL
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

#Eigen
SET(EIGEN3_INCLUDE_DIR "/usr/include/eigen3")
IF (NOT EIGEN3_INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF ()
INCLUDE_DIRECTORIES("${EIGEN3_INCLUDE_DIR}")


add_subdirectory(third_party/clipper/cpp)

ADD_LIBRARY(evidential SHARED
        focal_elements/FocalElement.h
        focal_elements/UnidimensionalFocalElement.cpp
        focal_elements/UnidimensionalFocalElement.h
        focal_elements/BoxSet2DFocalElement.cpp
        focal_elements/BoxSet2DFocalElement.h
        focal_elements/EigenMat2DFocalElement.cpp
        focal_elements/EigenMat2DFocalElement.h
        focal_elements/Clipper2DFocalElement.cpp
        focal_elements/Clipper2DFocalElement.h
        Evidence.cpp
        Evidence.h
        focal_elements/HashableFocalElement.h
        containers/FocalElementContainer.h
        containers/GenericFocalElementContainer.cpp
        containers/GenericFocalElementContainer.h
        containers/HashableFocalElementContainer.cpp
        containers/HashableFocalElementContainer.h
        containers/FocalElementContainerDispatcher.h
        containers/DefaultFocalElementContainerDispatcher.cpp
        containers/DefaultFocalElementContainerDispatcher.h
        focal_elements/CompositeFocalElement.cpp
        focal_elements/CompositeFocalElement.h
        errors/InvalidBBAError.h builders/GraphBuilder.cpp builders/GraphBuilder.h canonical_decomposition/CanonicalDecompositionDispatcher.h canonical_decomposition/CanonicalDecompositionStrategy.h canonical_decomposition/UnidimensionalCanonicalDecomposition.cpp canonical_decomposition/UnidimensionalCanonicalDecomposition.h canonical_decomposition/GraphBasedCanonicalDecompositon.cpp canonical_decomposition/GraphBasedCanonicalDecompositon.h canonical_decomposition/DefaultCanonicalDecompositionDispatcher.cpp canonical_decomposition/DefaultCanonicalDecompositionDispatcher.h)

ADD_LIBRARY(geometry SHARED
        geometry/Point2D.h
        geometry/Shape.h
        geometry/Rectangle.cpp
        geometry/Rectangle.h
        geometry/ClipperPolygon.cpp
        geometry/ClipperPolygon.h)

target_link_libraries(geometry polyclipping)
target_link_libraries(evidential geometry)

add_subdirectory(plotters)
add_subdirectory(demos)


