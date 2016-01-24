DOMjura (DOMjudge results application)
==================================================

DOMjura is a [DOMjudge](https://domjudge.org/) plugin parser that makes a nice results presentation.

DOMjura is easy to use: you just specify the DOMjudge URL and you are good to go.

## Requirements

DOMjura now requires the following software:

* Qt5 or higher
* DOMjudge 5.0 or higher
* The DejaVu Sans Mono font (bold and bold oblique)

## Compiling

Compiling can be done by installing Qt5, and then doing the following in the source directory (or by using Qt Creator):

```bash
qmake
make
./DOMjura
```

Note that the last line may be different on Mac OS X and Windows.

## Todo

In the future, DOMjura will support a mode in which one can use it during the contest as a live scorebaord. This is still in the works and is thus disabled in the UI.

## Screenshots

Note that these screenshots are now outdated, but the general idea still is the same

__The main window__

<img src="https://github.com/nickygerritsen/DOMjura/raw/master/resources/Screenshots/Mainwindow.png" />

__The settings dialog__

<img src="https://github.com/nickygerritsen/DOMjura/raw/master/resources/Screenshots/Settings.png" />

__The results presentation__

<img width="800" src="https://github.com/nickygerritsen/DOMjura/raw/master/resources/Screenshots/Results.png" />

## License

DOMjura is licensed under the [MIT License](LICENSE).

Copyright 2011-2015 Nicky Gerritsen.
