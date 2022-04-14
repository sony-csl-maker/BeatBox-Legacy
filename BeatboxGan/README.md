## Install dependencies

`sudo apt-get install libboost-all-dev`

`pip3 install torch-tensorrt -f https://github.com/NVIDIA/Torch-TensorRT/releases`

## How to build ?

```
cd build
cmake -DCMAKE_PREFIX_PATH=/path/to/libtorch ..
cmake --build . --config Release
```

`./BeatboxGan --help`

## How to compile and clean

```
cmake --build . --config Release
```

```
cmake --build . --target clean
```