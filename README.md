# quicky

A notes app - qt4 to qt5 conversion, possibly qt6 later

## Building

Quicky is built with CMake and needs Qt 5.15 or later. On Haiku that is the
`qt5` and `qt5_devel` packages; `qt5_tools` provides `moc`, `uic` and `rcc`,
which CMake runs automatically.

    mkdir build && cd build
    cmake ..
    make

The binary is written to `build/Quicky_qt5`, and `make clean` removes the build
products. Pass `-DCMAKE_BUILD_TYPE=Debug` to keep the `qDebug()` tracing that
release builds compile out.

## Notes on the Qt 5 port

* The qmake project file has been replaced by `CMakeLists.txt`.
* `QString` no longer converts to `QUrl` implicitly in Qt 5, so the places
  that pass a file path as a document URL construct the `QUrl` explicitly.
* `QProcess::startDetached()` no longer splits a command line, so external
  link commands are split with `QProcess::splitCommand()` first.
* On Haiku, settings are kept in `~/config/settings`, the directory returned
  by `find_directory(B_USER_SETTINGS_DIRECTORY)`, instead of the `~/.config`
  location Qt would use on its own.
* The Wiki-to-HTML formatter still uses `QRegExp`, which is part of Qt 5 but
  was removed in Qt 6. A future Qt 6 port has to move it to
  `QRegularExpression`.
