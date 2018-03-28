
add_executable(
        unidimensional_demo
        unidimensional_demo.cpp
)
add_executable(
        clipper_demo
        clipper_demo.cpp
)


target_link_libraries(
        unidimensional_demo
        evidential
)


target_link_libraries(
        clipper_demo
        evidential
        plotter
)