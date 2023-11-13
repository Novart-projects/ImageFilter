cmake_minimum_required(VERSION 3.24)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor image_processor.cpp filters/basic/negative.cpp filters/basic/gray_scale.cpp filters/custom/glass.cpp filters/custom/paint.cpp filters/custom/sketch.cpp command-parser/parser.cpp image/image.cpp command-parser/parser.cpp filters/basic/edge.cpp filters/basic/crop.cpp filters/basic/blur.cpp filters/basic/sharp.cpp filters/custom/wave.cpp)
