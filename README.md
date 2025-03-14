# Table of Contents
- [Ray Tracer](#ray-tracer)
- [Installation](#installation)
- [Usage](#usage)
- [Testing](#testing)


# Ray Tracer

Generates images of 3D shapes/objects. Uses ray tracing to simulate light interacting with the object based on the object's properties.

# Installation
CPU version with testing('main' branch):
- [Mingw setup](https://code.visualstudio.com/docs/cpp/config-mingw)
- [gtest setup](https://google.github.io/googletest/quickstart-bazel.html)
- Make install steps(optional):
    - [Install Chocalatey](https://chocolatey.org/install)
    - ```sh
      choco install make
      ```
For running with an Nvidia GPU using the 'gpu' branch:
- [Visual Studio install](https://visualstudio.microsoft.com/vs/)
- [CUDA toolkit install](https://developer.nvidia.com/cuda-downloads)

# Usage
Refer to the main.cpp file comments to see what code to change

Run the main.cpp file with:
```sh
make run
```
or if you don't have make
```sh
g++ ./src/*.cpp -I ./inc/ -o main
./main.exe
```
# Testing
### Run all tests
```sh
make test
```
or if you don't have make
```sh
bazel test --test_output=summary :all
```
### Run a specific test
```sh
make test TEST={test file name without .cc}
```
or if you don't have make
```sh
bazel test --test_output=summary :{test file name without .cc}
```
### Generate BAZEL BUILD file
If you add any new files(.h, .cpp, .cc) and want to run tests, you will need to add these files to the Bazel BUILD file. You can autogenerate the build file by running the autobuild.py script with [Python](https://www.python.org/downloads/)
```sh
python autobuild.py
```