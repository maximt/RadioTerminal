#include "radioterminal.h"
#include "ui_radioterminal.h"
#include <QMessageBox>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

RadioTerminal::RadioTerminal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RadioTerminal)
{
    ui->setupUi(this);
    statusText = new QLabel(this);
    statusBar()->addWidget(statusText);

    loadSettings();

    ui->spinBox_Interval->setValue(timerInterval);
    ui->lineEdit_WarningCommand->setText(warning_cmd);
    ui->spinBox_REF->setValue(warning_ref);
    ui->spinBox_MODLEV->setValue(warning_modlev);

    initActionsConnections();
    fillPortsInfo();

    serial = new QSerialPort(this);

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &RadioTerminal::handleError);
    connect(serial, &QSerialPort::readyRead, this, &RadioTerminal::readData);
    openSerialPort();

    runTimer();
}

void RadioTerminal::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &RadioTerminal::openSerialPort);
    connect(ui->actionExit, &QAction::triggered, this, &RadioTerminal::close);
}

RadioTerminal::~RadioTerminal()
{
    closeSerialPort();
    saveSettings();

    delete serial;
    delete statusText;
    delete statusTimer;
    delete ui;
}

void RadioTerminal::runTimer()
{
    if(!statusTimer)
    {
       statusTimer = new QTimer(this);
       connect(statusTimer, SIGNAL(timeout()), this, SLOT(updateStatus()));
    }
    else
    {
        statusTimer->stop();
    }

    statusTimer->start(timerInterval * 1000);
}

void RadioTerminal::openSerialPort()
{
    closeSerialPort();

    if(currentPort.isEmpty() ||
            currentPort == "None")
       return;

    serial->setPortName(currentPort);
    serial->setBaudRate(serial_BaudRate);
    serial->setDataBits(serial_DataBits);
    serial->setParity(serial_Parity);
    serial->setStopBits(serial_StopBits);
    serial->setFlowControl(QSerialPort::NoFlowControl);


    QString msg = QString("Port: %1 %2");

    if (serial->open(QIODevice::ReadWrite)) {
        msg = msg.arg(serial->portName(), "OK");
    } else {
        msg = msg.arg(serial->portName(), serial->errorString());
    }

    statusText->setText(msg);
}

void RadioTerminal::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
}

void RadioTerminal::writeData(const QString &text, bool sync)
{
    QString t(text);
    t.append(serial_eol);
    QByteArray data = t.toLocal8Bit();
    serial->write(data);
    serial->flush();

    if(sync)
        serial->waitForBytesWritten(serial_timeout);

    printConsole(t);
}

void RadioTerminal::printConsole(const QString &text)
{
    QDateTime now = QDateTime::currentDateTime();
    QString msg("[%1]: %2");
    msg = msg.arg(now.toString(Qt::DefaultLocaleShortDate), text.simplified());
    ui->textOutput->appendPlainText(msg);
}

void RadioTerminal::readData()
{
    serial->waitForReadyRead(serial_timeout);
    buffer.append(serial->readAll());

    QStringList lines;
    while(true)
    {
        int br = buffer.indexOf(serial_eol);

        if(br == -1)
            break;

        QString s = buffer.left(br);
        buffer.remove(0, br + serial_eol.length());
        lines.append(s);
    }

    foreach (QString line, lines) {
        if(!line.isEmpty())
        {
            line.append(serial_eol);
            processData(line);

            printConsole(line);
        }
    }

}

void RadioTerminal::processData(const QString &text)
{
    QRegularExpressionMatch match;

    //warnings

    match = QRegularExpression(radio_warning_FWD_PWR_MAX, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_REF_PWR_MAX, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_IPA_MAX, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_VPA_MAX, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_VPA_MIN, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_OVER_TEMP_L1, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_OVER_TEMP_L2, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_TEMP_IS_NORMAL, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    match = QRegularExpression(radio_warning_NO_FUSE, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processWarning(text);
        return;
    }

    //status

    match = QRegularExpression(radio_out_RP, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processRP(match);
        return;
    }

    match = QRegularExpression(radio_out_RE, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processRE(match);
        return;
    }

    match = QRegularExpression(radio_out_RF, QRegularExpression::CaseInsensitiveOption).match(text);
    if(match.hasMatch())
    {
        processRF(match);
        return;
    }
}

void RadioTerminal::handleError(QSerialPort::SerialPortError error)
{

}

void RadioTerminal::on_lineInput_returnPressed()
{
    QString text = ui->lineInput->text();
    writeData(text);
}

void RadioTerminal::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    ui->serialPortInfoListBox->addItem("None");
    ui->serialPortInfoListBox->addItem("Custom");

    QString description;
    QString manufacturer;
    QString serialNumber;

    bool custom = true;

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
        if(info.portName() == currentPort)
            custom = false;
    }

    if(custom)
        ui->serialPortInfoListBox->addItem(currentPort);

    ui->serialPortInfoListBox->setCurrentText(currentPort);


    connect(ui->serialPortInfoListBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &RadioTerminal::checkCustomDevicePathPolicy);

}

void RadioTerminal::checkCustomDevicePathPolicy(int idx)
{
    currentPort = ui->serialPortInfoListBox->itemText(idx);
    bool isCustom = (currentPort == "Custom");
    ui->serialPortInfoListBox->setEditable(isCustom);
    if (isCustom){
        ui->serialPortInfoListBox->clearEditText();
    }

    openSerialPort();
}

void RadioTerminal::loadSettings()
{
    QSettings settings(settings_org, settings_app, this);

    currentPort = settings.value(settings_port, "").toString();
    timerInterval = settings.value(settings_interval, 30).toInt();
    warning_cmd = settings.value(settings_warning_command, "").toString();
    warning_ref = settings.value(settings_warning_ref, 0.0).toDouble();
    warning_modlev = settings.value(settings_warning_modlev, 0.0).toDouble();
}

void RadioTerminal::saveSettings()
{
    QSettings settings(settings_org, settings_app, this);

    settings.setValue(settings_port, currentPort);
    settings.setValue(settings_interval, timerInterval);
    settings.setValue(settings_warning_command, warning_cmd);
    settings.setValue(settings_warning_ref, warning_ref);
    settings.setValue(settings_warning_modlev, warning_modlev);
    settings.sync();
}

void RadioTerminal::updateStatus()
{
    if(!serial->isOpen())
        return;

    writeData(radio_cmd_RP, true);
    readData();
    writeData(radio_cmd_RE, true);
    readData();
    writeData(radio_cmd_RF, true);
    readData();
}

void RadioTerminal::processRP(const QRegularExpressionMatch &match)
{
    ui->label_I1->setText(match.captured("I1"));
    ui->label_U1->setText(match.captured("U1"));
    ui->label_FWD->setText(match.captured("FWD"));
    ui->label_FWDW->setText(match.captured("FWDW"));
    ui->label_REF->setText(match.captured("REF"));
    ui->label_EFF->setText(match.captured("EFF"));
    ui->label_TEMP->setText(match.captured("TEMP"));

    float ref = match.captured("REF").toFloat();
    if(warning_ref > 0 && ref >= warning_ref)
    {
        QString msg("REF: %1 W");
        processWarning(msg.arg(ref));
    }

}

void RadioTerminal::processRE(const QRegularExpressionMatch &match)
{
    ui->label_FREQ->setText(match.captured("FREQ"));
    ui->label_MODLEV->setText(match.captured("MODLEV"));
    ui->label_LLEV->setText(match.captured("LLEV"));
    ui->label_RLEV->setText(match.captured("RLEV"));

    float mod = match.captured("MODLEV").toFloat();
    if(warning_modlev > 0 && mod >= warning_modlev)
    {
        QString msg("MODLEV: %1 %");
        processWarning(msg.arg(mod));
    }
}

void RadioTerminal::processRF(const QRegularExpressionMatch &match)
{
    ui->label_F1->setText(match.captured("F1"));
}

void RadioTerminal::processWarning(const QString &text)
{
    QStringList args;
    args.append(text);
    QProcess::execute(warning_cmd, args);

    printConsole("Warning!");
}


void RadioTerminal::on_pushButton_Update_clicked()
{
    updateStatus();
}

void RadioTerminal::on_spinBox_Interval_valueChanged(int v)
{
    timerInterval = v;
    runTimer();
}

void RadioTerminal::on_lineEdit_WarningCommand_textEdited(const QString &arg1)
{
    warning_cmd = arg1;
}

void RadioTerminal::on_spinBox_REF_valueChanged(double arg1)
{
    warning_ref = arg1;
}

void RadioTerminal::on_spinBox_MODLEV_valueChanged(double arg1)
{
    warning_modlev = arg1;
}

void RadioTerminal::on_btn_Save_clicked()
{
    saveSettings();
}
