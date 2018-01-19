
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
        builders/FocalElementContainer.h
        builders/GenericFocalElementContainer.cpp
        builders/GenericFocalElementContainer.h
        builders/HashableFocalElementContainer.cpp
        builders/HashableFocalElementContainer.h
        builders/FocalElementContainerDispatcher.h
        builders/DefaultFocalElementContainerDispatcher.cpp
        builders/DefaultFocalElementContainerDispatcher.h
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


