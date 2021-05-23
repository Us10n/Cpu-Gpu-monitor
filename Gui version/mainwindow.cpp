#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->speedslider, &QAbstractSlider::valueChanged, this, &MainWindow::changespeed);

    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"Cpu#", "Утилизация, %", "Частота, MHz", "Температрура, C"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->verticalHeader()->hide();

    update();

    timer = new QTimer;
    speed = 1000;
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(speed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadGpuLabels(std::vector<QString> vector)
{
    ui->gpuid->setText(vector[0]);
    ui->gpuname->setText(vector[1]);
    ui->gputemp->setText(vector[2]);
    ui->gpumemusage->setText(vector[3]);
    ui->gpuutil->setText(vector[4]);
    ui->gpumemutil->setText(vector[5]);
    ui->gpugraphclock->setText(vector[6]);
    ui->gpumemclock->setText(vector[7]);
}

void MainWindow::loadCpuLabels(std::vector<QString> vector)
{
    ui->cpuname->setText(vector[0]);
    ui->cpucores->setText(vector[1]);
    ui->cpucache->setText(vector[2]);
    if (vector.size() == 3)
    {
        return;
    }
    bool isEmpry = false;
    if (ui->tableWidget->rowCount() == 0)
    {
        isEmpry = true;
    }
    int procnum = std::stoi(vector[3].toStdString(), nullptr, 10);
    int corenum = std::stoi(vector[1].toStdString(), nullptr, 10);
    for (int i = 0; i < procnum; i++)
    {
        std::stringstream sstream;
        sstream << "Cpu%" << i;
        if (isEmpry)
        {
            ui->tableWidget->insertRow(i);
        }
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(sstream.str())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(vector[4 + i * 2]));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(vector[5 + i * 2]));
        if (i % (procnum / corenum) == 0)
        {
            if(vector[vector.size() - corenum + i / (procnum / corenum)]=='0')
            {
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem("N/A"));
            }
            else
            {
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(vector[vector.size() - corenum + i / (procnum / corenum)]));
            }
        }
    }
}

void MainWindow::update()
{
    std::string string;
    QProcess proc;
    std::stringstream sstream;
    gpulable.clear();
    cpulable.clear();
    proc.start("./Monitor/monitor");
    proc.waitForFinished();

    ui->statusbar->showMessage("");

    bool cpuerror = false;
    bool gpuerror = false;

    sstream << proc.readAllStandardOutput().toStdString();
    //    qDebug()<<sstream.str().c_str();
    for (int i = 0;; i++)
    {
        std::getline(sstream, string);
        if (sstream.eof())
        {
            if (i == 0)
            {
                ui->statusbar->showMessage("[ERROR] Программа для мониторинга не найдена");
                return;
            }
            break;
        }
        //gpu error
        if (string[1] == 'E' && i == 0)
        {
            gpuerror = true;
            ui->statusbar->showMessage(QString::fromStdString(string));
            ui->tableWidget->clear();
            for (int j = 0; j < 8; j++)
            {
                gpulable.push_back("N/A");
            }
            continue;
        }
        //cpu error
        if (string[1] == 'E' && (i == 1 || i == 8))
        {
            cpuerror = true;
            ui->statusbar->showMessage(QString::fromStdString(string));
            for (int k = 0; k < 3; k++)
            {
                cpulable.push_back("N/A");
            }
            break;
        }
        if (!gpuerror)
        {
            if (i < 8)
            {
                gpulable.push_back(QString::fromStdString(string));
            }
            if (i >= 8)
            {
                cpulable.push_back(QString::fromStdString(string));
            }
        }
        else
        {
            cpulable.push_back(QString::fromStdString(string));
        }
    }
    if (gpuerror && cpuerror)
    {
        ui->statusbar->showMessage("[ERROR] Ошибка загрузки информации о Cpu/Gpu");
    }
    loadCpuLabels(cpulable);
    loadGpuLabels(gpulable);
}

void MainWindow::changespeed(int pos)
{
    timer->stop();
    timer->start((4 - pos) * 1000);
}
