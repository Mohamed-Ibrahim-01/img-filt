#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "opencv2/core/core.hpp"
#include "ImgStore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ImgStore& store;
        ~MainWindow();

    private:
        std::string currentFileName = "";
        Ui::MainWindow *ui;
        void resizeEvent(QResizeEvent* event);

    private slots:
        void autoUpadateLabelSize();
        void loadImage();
        void setLoadedImage(bool loaded, std::string imageName);

    signals:
        void imageLoaded(bool loaded, std::string imageName);
};
#endif // MAINWINDOW_H
