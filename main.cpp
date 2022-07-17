#include "mainwindow.h"
#include "histogramwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
//    QVector<QVector<unsigned long>> data(3,QVector<unsigned long>(256));
//    for(auto& mem : data){
//        for (int counter= 0; counter < data[0].size(); counter++){
//            mem[counter] = counter;
//        }
//    }
//    histogramWindow x(nullptr,{"red","green","blue"},data,"hist");
//    x.show();
    w.show();
    return a.exec();
}
