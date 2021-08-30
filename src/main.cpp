
#include <QApplication>
#include <mainwindow.h>

#include <iostream>
using namespace std;


//    ifstream In("MyDicomFile.dcm",ios::binary);
//        DataSet data;
//        ReadFromStream(In,data);
//        cout << data;
//        return 0;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow win;
    win.show();

    return a.exec();

}
