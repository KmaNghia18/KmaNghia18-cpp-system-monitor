#include "ui/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("System Monitor");
    app.setOrganizationName("KmaNghia18");
    app.setApplicationVersion("1.0.0");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
