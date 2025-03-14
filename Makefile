TEST ?= all

run:
	g++ ./src/*.cpp -I ./inc/ -o main
	./main.exe

test:
	bazel test --test_output=summary :$(TEST)