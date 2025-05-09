#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle(QObject::tr("Image cropper"));
    window.setWindowIcon(QIcon(":/iconApp.png"));
    window.show();
    return app.exec();
}
