### Build

  mkdir build
  cd build
  cmake ..
  make

### Ogg support

  To build with ogg support system wide ogg and vorbis libraries are required.
  Redistributable libs are built, but the configure script cannot detect them.

### OSX 

  Build libraries for OSX have hard coded library paths. Run the included
  'patch.py' to invoke install_name_tool and generate local bindings for 
  these.

  The local bindings use @loader_path and assume that all libraries will
  be located in a single common libraries directory.

  To manually edit the library paths use:

    otool -L blah.dylib

    install_name_tool -change @loader_path/blah.dylib @rpath/... blah.dylib
