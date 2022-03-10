# img-filt
Computer Vision Task 1
## Build and Run
### Qt Creator
File &rarr; Open file or Project &rarr; Select `img-filt.pro` file
### Building from command line
```bash
# Building
cd img-filt
qmake -makefile -o ./build/Makefile img-filt.pro
cd build
make -f Makefile -j8

# Run the app
./img-filt
```
