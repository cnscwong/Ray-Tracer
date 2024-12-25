cc_library(
    name = "source",
    srcs = ["src/Tuple.cpp", "src/common.cpp", "src/Colour.cpp", "src/Canvas.cpp", "src/Matrix.cpp"], 
    hdrs = ["inc/Tuple.h", "inc/common.h", "inc/Colour.h", "inc/Canvas.h", "inc/Matrix.h"], 
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

