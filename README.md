DOMjura (DOMjudge results application)
==================================================

DOMjura is a [DOMjudge](http://domjudge.sourceforge.net/) plugin parser that makes å nice results presentation. Furthermore, it can generate statistics about a programming contest.

DOMjura is easy to use: you just specify the DOMjudge URL (or use a local saved version of the plugin XML) and you are good to go.

## Compiling

Compiling can be done by installing Qt5, and then doing the following in the source directory (or by using Qt Creator):

```bash
qmake
make
./DOMjura
```

Note that the last line may be different on Mac OS X and Windows

## Screenshots

__The main window__

<img src="https://github.com/nickygerritsen/DOMjura/raw/master/resources/Screenshots/Mainwindow.png" />

__The settings dialog__

<img src="https://github.com/nickygerritsen/DOMjura/raw/master/resources/Screenshots/Settings.png" />

__The results presentation__

<img width="800" src="https://github.com/nickygerritsen/DOMjura/raw/master/resources/Screenshots/Results.png" />