# Dicomlib
## _From Google Code Archive_

This repository contains a slightly modified version of the Trevor Morgan dicomlib library.

- Original library and files: https://code.google.com/archive/p/dicomlib/
- Original code license: https://opensource.org/licenses/BSD-3-Clause

Dicomlib was a pioneer, self-contained C++ library implementation for medical images in [DICOM][dicom] format by Trevor Morgan. While DICOM is obviously more than just a file format, current state-of-the-art implementations tend to keep it long rather than short, which usually turns libraries into real juggernauts (See state-of-the-art and stack-powerful [DCMTK library][dcmtk]).

Morgan's code is short enough to be packed into about 200kB and self-contained enough to prevent us from downloading and compiling a dozen of external C++ libraries (yes, it does require a few boost headers). Accordingly, we were able to compile it successfully with [em++][wasm] running it alongside [Qt][qt] (Compilation setup: Emscripten coupled to Qt 5.15.1 with em++ compiler v2.0.22).

While the compilation with em++ is quite straightforward (provided you previously defined the Boost include paths), please take note of the following issues (as of today, they will be likely solved shortly):

- Emscripten is sandboxed. Thus, it does not provide support for accessing local file systems (and DICOM images, consequently);
- Emscripten does not support exceptions (yes, there are [workarounds](https://emscripten.org/docs/porting/exceptions.html), but you know what). We have removed some of Morgan's original throw-catches (in particular for the reading of DICOM tags), but a few remain.
- QNetwork is not (fully) supported by Emscripten. Thus, QTcpServer/Socket won't help in the creation of your workstation, requiring QWebSockets (urgh!) to be used instead. Some of the original socket.zip and util.zip functions seem not compatible with Emscripten as well.
- The library is still the perfect lightweight solution for either "loading a file (or a stream) and putting it into an array of pixels" or "loading a DICOM tag and putting it into a std::string". We wrapped it into our [Content-Based Medical Image Retrieval tool][higiia] and it worked great.

## 3rd-party dependencies

- Self-contained header-only files from C++ [Boost library](https://www.boost.org/users/download/)


## Installation

Install Boost headers and adjust the include paths of `dicomlib.pro.example` file to point to the Boost folders. Then rename it to `dicomlib.pro`. After that, you can use build.sh to create `dicomlib.a`. Notice, you can configure the building type in the `build.sh` file itself by selecting the qmake binary of your QT setup (e.g., wasm\_32 (default) or gcc\_64).

#!/bin/sh
~/Qt/5.15.1/wasm_32/bin/qmake   #for Web-Assembly building
#~/Qt/5.15.1/gcc_64/bin/qmake   #for desktop-binary building
make

Then, run the following command.

```sh
cd dicomling
./build.sh && make clean
```

It will create the static library you can link to your DICOM project. Best of luck!

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [dicom]: <https://www.dicomstandard.org/>
   [dcmtk]: <https://dicom.offis.de/dcmtk.php.en>
   [wasm]: <https://emscripten.org/docs/getting_started/downloads.html>
   [qt]: <https://www.qt.io/download>
   [higiia]: <https://github.com/marcosivni/higiia>
