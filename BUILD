cc_library(
    name = "source",
    srcs = ["src/Tuple.cpp", "src/common.cpp", "src/Colour.cpp", "src/Canvas.cpp", "src/Matrix.cpp", "src/Ray.cpp", "src/Intersection.cpp", "src/LightAndShading.cpp",
    "src/World.cpp", "src/LightData.cpp", "src/Camera.cpp", "src/Shape.cpp", "src/Pattern.cpp", "src/Group.cpp", "src/ObjParser.cpp", "src/CSG.cpp"], 
    hdrs = ["inc/Tuple.h", "inc/common.h", "inc/Colour.h", "inc/Canvas.h", "inc/Matrix.h", "inc/Ray.h", "inc/Intersection.h", "inc/LightAndShading.h",
    "inc/World.h", "inc/LightData.h", "inc/Camera.h", "inc/Config.h", "inc/Shape.h", "inc/Pattern.h", "inc/Group.h", "inc/ObjParser.h", "inc/CSG.h"], 
    includes = ["inc"]
)


cc_test(
    name = "tuple_tests", 
    size = "small",
    srcs = ["tests/tuple_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "colour_tests", 
    size = "small",
    srcs = ["tests/colour_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "canvas_tests", 
    size = "small",
    srcs = ["tests/canvas_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "matrix_tests", 
    size = "small",
    srcs = ["tests/matrix_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "ray_tests", 
    size = "small",
    srcs = ["tests/ray_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "intersection_tests", 
    size = "small",
    srcs = ["tests/intersection_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "light_shading_tests", 
    size = "small",
    srcs = ["tests/light_shading_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "world_tests", 
    size = "small",
    srcs = ["tests/world_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "camera_tests", 
    size = "small",
    srcs = ["tests/camera_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "shape_tests", 
    size = "small",
    srcs = ["tests/shape_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "pattern_tests", 
    size = "small",
    srcs = ["tests/pattern_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "obj_parser_tests", 
    size = "small",
    srcs = ["tests/obj_parser_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "csg_tests", 
    size = "small",
    srcs = ["tests/csg_tests.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)