## BeatBox
Sony Computer Science Laboratories

#

> Build
```
cd /path/to/JUCE
cmake . -B cmake-build -DJUCE_BUILD_EXAMPLES=ON -DJUCE_BUILD_EXTRAS=ON
cmake . -B cmake-build-dir -D JUCE_BUILD_EXAMPLES=ON
cmake --build cmake-build-dir --target BeatBox
```

> Execute
```
./cmake-build-dir/examples/CMake/BeatBox/BeatBox_artefacts/BeatBox
```


#
Made by:

Victor PALLE <victor.palle@epitech.eu>

Maxime CARABINA <maxime.carabina@epitech.eu>

Martin VANAUD <martin.vanaud@epitech.eu>

