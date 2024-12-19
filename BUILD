cc_library(
    name = "source",
    srcs = ["src/Tuple.cpp"], 
    hdrs = ["inc/Tuple.h"], 
    includes = ["inc"]
)


cc_test(
    name = "tuple_test", 
    size = "small",
    srcs = ["tests/tuple_test.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)
