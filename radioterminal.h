#ifndef RADIOTERMINAL_H
#define RADIOTERMINAL_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include <QSettings>
#include <QRegularExpression>
#include <QTimer>
#include <QProcess>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <options.h>

namespace Ui {
class RadioTerminal;
}

class RadioTerminal : public QMainWindow
{
    Q_OBJECT



public:
    explicit RadioTerminal(QWidget *parent = 0);
    ~RadioTerminal();

private slots:
    void runTimer();
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QString &text, bool sync = false);
    void printConsole(const QString &text);
    void readData();
    void processData(const QString &text);
    void handleError(QSerialPort::SerialPortError error);
    void checkCustomDevicePathPolicy(int idx);
    void updateStatus();

    void processRP(const QRegularExpressionMatch &match);
    void processRE(const QRegularExpressionMatch &match);
    void processRF(const QRegularExpressionMatch &match);
    void processWarning(const QString &text);

    void on_lineInput_returnPressed();
    void on_pushButton_Update_clicked();
    void on_spinBox_Interval_valueChanged(int v);
    void on_lineEdit_WarningCommand_textEdited(const QString &arg1);
    void on_spinBox_REF_valueChanged(double arg1);
    void on_spinBox_MODLEV_valueChanged(double arg1);
    void on_btn_Save_clicked();

private:
    Ui::RadioTerminal *ui;
    QLabel *statusText;
    QString currentPort;

    void loadSettings();
    void saveSettings();
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    void fillPortsInfo();
    QSerialPort *serial = NULL;
    QString buffer;
    QTimer *statusTimer = NULL;
    int timerInterval = 30;
    QString warning_cmd;
    double warning_ref = 0;
    double warning_modlev = 101;
};

#endif // RADIOTERMINAL_H
