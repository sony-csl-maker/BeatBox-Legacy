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