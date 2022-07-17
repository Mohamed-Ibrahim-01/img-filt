#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QDialog>
#include <QtCharts>

namespace Ui {
class histogramWindow;
}

class histogramWindow : public QDialog
{
    Q_OBJECT

public:
//    explicit histogramWindow(QWidget *parent = nullptr);
    explicit histogramWindow(QWidget *parent, QVector<QString> setNameVector, QVector<QVector<unsigned long>> data, QString chartTitle);

//    histogramWindow(QWidget *parent = nullptr, const int& numberOfSets = 0, const int& numberOfSeries = 0,
//                    const QStringList& legendNames = QStringList(""), const QVector<double>& data = QVector<double>(0));

    ~histogramWindow();

private:
    Ui::histogramWindow *ui;
    void setHistColor(QVector<QBarSet *>& histSetVector, unsigned char depthItr);
};

#endif // HISTOGRAMWINDOW_H

/**
 *  the number of sets
 *  the number of series
 *  the data
 *  the legend names
 *
 */
