
#Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
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
        errors/InvalidBBAError.h)

ADD_LIBRARY(geometry SHARED
        geometry/Point2D.h
        geometry/Shape.h
        geometry/Rectangle.cpp
        geometry/Rectangle.h
        geometry/ClipperPolygon.cpp
        geometry/ClipperPolygon.h)

target_link_libraries(geometry polyclipping)
target_link_libraries(evidential geometry)


