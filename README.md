# kernel2 compressor

FF7's kernel2 compressor using black magic.

## Building

### Requirements

 - Qt 5

### With Qt Creator

Open `kernel2_compressor.pro` with Qt Creator IDE.
Please refer to the official documentation of Qt Creator
if you have troubles to compile and run kernel2 compressor.

### Command line

#### Windows

Use the command prompt given in your Qt installation
to set your environment (`%QTDIR%/%compiler%/bin/qtenv2.bat`).
If you want to compile with msvc, run `vcvarsall.bat`
from your Microsoft Visual Studio installation
(eg: `C:\Program Files\Microsoft Visual Studio 11.0\VC\vcvarsall.bat`).

Then run:

~~~sh
mkdir build
cd build
qmake /path/to/kernel2_compressor.pro
# For mingw32
mingw32-make
# For msvc
jom
~~~

#### Linux

Set your environment:

 - `PATH` should contains the path to the bin directory of your Qt installation (`$QTDIR/bin`)

Then run:

~~~sh
mkdir build
cd build
qmake /path/to/kernel2_compressor.pro
make
~~~

## How to contribute

Pull requests are welcome on [GitHub](https://github.com/myst6re/kernel2_compressor).
Ensure that you have commited in `develop` branch, otherwise I will not accept your
request.