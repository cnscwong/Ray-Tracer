cc_library(
    name = "source",
    srcs = ["src/Tuple.cpp", "src/common.cpp"], 
    hdrs = ["inc/Tuple.h", "inc/common.h"], 
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
