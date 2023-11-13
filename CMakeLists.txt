cmake_minimum_required(VERSION 3.24)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor image_processor.cpp filters/negative.cpp filters/gray_scale.cpp filters/glass.cpp custom_filters/paint.cpp custom_filters/sketch.cpp parser.cpp image.cpp parser.cpp filters/edge.cpp filters/crop.cpp filters/blur.cpp filters/sharp.cpp custom_filters/wave.cpp)
