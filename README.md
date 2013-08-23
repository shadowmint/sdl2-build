### Build

  mkdir build
  cd build
  cmake ..
  make

  mkdir py
  cd py
  cmake ../py-env
  make

### Mac deployable

  mkdir deploy
  cp py/python deploy
  cp -r py/lib deploy
  cp -r py/bin deploy
  cp build/libsbundle.dylib deploy
  
  GO!
