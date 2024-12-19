TEST ?= all

all:
	g++ ./helloworld.cpp

test:
	bazel test --test_output=summary :$(TEST)