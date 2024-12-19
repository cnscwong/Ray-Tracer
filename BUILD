cc_library(
    name = "source",
    srcs = ["src/Tuple.cpp", "src/common.cpp", "src/Colour.cpp"], 
    hdrs = ["inc/Tuple.h", "inc/common.h", "inc/Colour.h"], 
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
