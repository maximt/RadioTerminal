#ifndef OPTIONS_H
#define OPTIONS_H

#include <QtSerialPort/QSerialPort>

const QString settings_org("promsvyaz");
const QString settings_app("RadioTerminal");

const QString settings_interval("interval");
const QString settings_warning_command("warning_command");
const QString settings_warning_ref("warning_ref");
const QString settings_warning_modlev("warning_modlev");
const QString settings_port("port");

const QString serial_eol("\n");

const qint32 serial_BaudRate= QSerialPort::Baud9600;
const QSerialPort::DataBits serial_DataBits= QSerialPort::Data8;
const QSerialPort::Parity serial_Parity= QSerialPort::NoParity;
const QSerialPort::StopBits serial_StopBits= QSerialPort::OneStop;
const qint32 serial_timeout = 500;

const QString radio_cmd_RP("##RP");
const QString radio_out_RP("#I1=(?<I1>\\d+\\.\\d+)A U1=(?<U1>\\d+\\.\\d+)V FWD=(?<FWD>\\d+)% FWD=(?<FWDW>\\d+)W REF=(?<REF>\\d+)W EFF=(?<EFF>\\d+\\.\\d+)% TEMP=(?<TEMP>\\d+\\.\\d+)C");

const QString radio_cmd_RE("##RE");
const QString radio_out_RE("#FREQ=(?<FREQ>\\d+\\.\\d+)FM MODLEV=(?<MODLEV>\\d+)% RLEV=(?<RLEV>\\d+)% LLEV=(?<LLEV>\\d+)%");

const QString radio_cmd_RF("##RF");
const QString radio_out_RF("#F1=(?<F1>\\w+)");

const QString radio_cmd_SPON("##SPON");
const QString radio_cmd_SPOFF("##SPOFF");
const QString radio_cmd_SL("##SL%1");

const QString radio_warning_FWD_PWR_MAX("#FWD PWR MAX");
const QString radio_warning_REF_PWR_MAX("#REF PWR MAX");
const QString radio_warning_IPA_MAX("#IPA\\d+ MAX");
const QString radio_warning_VPA_MAX("#VPA\\d+ MAX");
const QString radio_warning_VPA_MIN("#VPA\\d+ MIN");
const QString radio_warning_OVER_TEMP_L1("#OVER TEMP L1");
const QString radio_warning_OVER_TEMP_L2("#OVER TEMP L2");
const QString radio_warning_TEMP_IS_NORMAL("#TEMP IS NORMAL");
const QString radio_warning_NO_FUSE("#NO FUSE");

#endif // OPTIONS_H
