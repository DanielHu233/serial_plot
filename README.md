

This project make use of a third-party labrary - QcustomPlot. Which is a powerful library that builds many useful features for plotting in Qt.

The project is wrtten using Qt5(based on C++), so, open the project in Qt Creator(like the ide for Qt) using the .pro file.

The features are all realized in mainwindow.cpp. The parsing functions like send packet or receive packets have not been implemented yet. These depends on communication protocal specified by your.

The QCustom Plot allowed plotting to happen on a child of the mainwindow. So painting happens on the central widget as you can check by double clicking the .ui file. The kernel of the paint is done by widgetPaint(int j), so going through it is useful

Used a data creator since there's no data fed in, So creator is needed for testing. You can comment it out once you implement your own data parser.

Note that since the project is done in a Chinese company, most of the comments in the code is in Chinese.
