#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadGpuLabels(std::vector<QString>);
    void loadCpuLabels(std::vector<QString>);

private slots:
    void update();
    void changespeed(int);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    std::vector<QString> cpulable, gpulable;
    int speed;
};
#endif // MAINWINDOW_H
