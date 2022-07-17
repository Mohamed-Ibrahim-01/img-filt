#include "histogramwindow.h"
#include "ui_histogramwindow.h"

//histogramWindow::histogramWindow(QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::histogramWindow)
//{
//    ui->setupUi(this);
//    QBarSet *set0 = new QBarSet("Jane");
////    QBarSet *set1 = new QBarSet("mat");
////    QBarSet *set2 = new QBarSet("mego");
////    QBarSet *set3 = new QBarSet("me");
////    QBarSet *set4 = new QBarSet("dede");

//    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
////    *set1 << 5 << 0 << 0 << 4 << 0 << 7;
////    *set2 << 3 << 5 << 8 << 13 << 8 << 5;
////    *set3 << 5 << 6 << 7 << 3 << 4 << 5;
////    *set4 << 9 << 7 << 5 << 3 << 1 << 2;
//    QBarSeries *series = new QBarSeries();
//    series->append(set0);
////    series->append(set1);
////    series->append(set2);
////    series->append(set3);
////    series->append(set4);
//    QChart *chart = new QChart();
//    chart->addSeries(series);
//    chart->setTitle("Simple barchart example");
//    chart->setAnimationOptions(QChart::SeriesAnimations);
//    QStringList categories;
//    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->append(categories);
//    chart->addAxis(axisX, Qt::AlignBottom);
//    series->attachAxis(axisX);

//    QValueAxis *axisY = new QValueAxis();
//    axisY->setRange(0,15);
//    chart->addAxis(axisY, Qt::AlignLeft);
//    series->attachAxis(axisY);
//    chart->legend()->setVisible(true);
//    chart->legend()->setAlignment(Qt::AlignBottom);
//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);
//    ui->histogramLayout->addWidget(chartView);
//}


void histogramWindow::setHistColor(QVector<QBarSet *>& histSetVector, unsigned char depthItr){
    switch(depthItr){
        case 0:
            histSetVector[depthItr]->setColor(Qt::red);
            break;
        case 1:
            histSetVector[depthItr]->setColor(Qt::green);
            break;
        case 2:
            histSetVector[depthItr]->setColor(Qt::blue);
            break;
    }
}



histogramWindow::histogramWindow(QWidget *parent, QVector<QString> setNameVector, QVector<QVector<unsigned long>> data, QString chartTitle) :
    QDialog(parent),
    ui(new Ui::histogramWindow)
{
    ui->setupUi(this);

    int dataDepth = data.size(), dataSize = data[0].size();

    QVector<QBarSet *> histSetVector(dataDepth);

    for (unsigned char depthItr = 0; depthItr < dataDepth; depthItr++){
        histSetVector[depthItr] = new QBarSet(setNameVector[depthItr]);
        for (int dataItr = 0; dataItr < dataSize; dataItr++){
            *histSetVector[depthItr] << data[depthItr][dataItr];
        }
        setHistColor(histSetVector,depthItr);
    }



//    QBarSet *histSet = new QBarSet(setNameVector[0]);
//    histSet->setColor(Qt::red);
//    for(int counter = 0; counter < data.size(); counter++){
//        *histSet << data[0][counter];
//    }

    QBarSeries *series = new QBarSeries();
//    series->append(histSet);

    for(unsigned char depthItr = 0; depthItr < dataDepth; depthItr++){
        series->append(histSetVector[depthItr]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(chartTitle);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0,255);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);


    QValueAxis *axisY = new QValueAxis();
    double max = *std::max_element(data[0].constBegin(), data[0].constEnd());
    axisY->setRange(0,max);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->histogramLayout->addWidget(chartView);
}





/*
 *     QVector<QBarSet> barSets;
 *     QVector<QBarSeries> barSeries;
 *     QChart chart;
 *     QStringList legendNames;
 *     QBarCategoryAxis Xaxis;
 *     QValueAxis Yaxis;
 *     QChartView chartView;
 */
//histogramWindow::histogramWindow(QWidget *parent, const int& numberOfSets, const int& numberOfSeries, const QStringList& legendNames, const QVector<double>& data):
//    QDialog(parent),
//    ui(new Ui::histogramWindow)
//{
//    this->barSets.resize(numberOfSets);
//    for(int series = 0; series < numberOfSeries; series++){
//        for(int set = 0;  set < numberOfSets; set++){
//            this->barSets[set] << data[series+set];
//        }
//    }

//    for(int set = 0;  set < numberOfSets; set++){
//        this->barSeries.append(&this->barSets[set]);
//    }

////    auto [min, max] = std::minmax_element(data.begin(),data.end());


//    this->chart.addSeries(&this->barSeries);
//    this->chart.setTitle("Histogram");
//    this->chart.setAnimationOptions(QChart::SeriesAnimations);
//    this->Xaxis.append(legendNames);
//    this->chart.addAxis(&this->Xaxis, Qt::AlignBottom);
//    this->Yaxis.setRange(0,256);
//    this->chart.addAxis(&this->Yaxis, Qt::AlignLeft);
//    this->chart.legend()->setVisible(true);
//    this->chart.legend()->setAlignment(Qt::AlignBottom);
//    this->barSeries.attachAxis(&this->Yaxis);
//    this->chartView.setRenderHint(QPainter::Antialiasing);
//    ui->histogramLayout->addWidget(&this->chartView);
//}

histogramWindow::~histogramWindow()
{
    delete ui;
}



















