all:
	g++ ./helloworld.cpp

test:
	ifdef TEST
		bazel test --test_output=summary //tests:$(TEST)
	else
		bazel test --test_output=summary //tests:all
	endif