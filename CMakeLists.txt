cmake_minimum_required(VERSION 3.24)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor image_processor.cpp negative.cpp gray_scale.cpp glass.cpp paint.cpp sketch.cpp parser.cpp image.cpp parser.cpp edge.cpp crop.cpp blur.cpp sharp.cpp wave.cpp)
