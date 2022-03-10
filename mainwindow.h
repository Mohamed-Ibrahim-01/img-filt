#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public slots:
        void DisplayImage();

    public:
        MainWindow(QWidget *parent = nullptr);
        QImage imdisplay;
        QTimer* Timer;
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
