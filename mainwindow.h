#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "opencv2/core/core.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;

    private slots:
        void loadImage();
        void setLoadedImage(bool loaded, std::string imageName);

    signals:
        void imageLoaded(bool loaded, std::string imageName);
};
#endif // MAINWINDOW_H
