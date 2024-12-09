#include "arduino.h"
#include <QDebug>
arduino::arduino()
{
    data="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;
}
QString arduino::getarduino_port_name(){
    return arduino_port_name;
}
QSerialPort* arduino::getserial(){
    return serial;
}
int arduino::connect_arduino() {
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        // Log the port names being checked
        qDebug() << "Checking port:" << serial_port_info.portName();

        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            // Log vendor and product ID for debugging
            qDebug() << "Vendor ID:" << serial_port_info.vendorIdentifier()
                     << "Product ID:" << serial_port_info.productIdentifier();

            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_producy_id) {

                // Arduino found
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                qDebug() << "Arduino found on port:" << arduino_port_name;
                break;  // Stop once the Arduino is found
            }
        }
    }

    // Log if no Arduino is found
    if (!arduino_is_available) {
        qDebug() << "Arduino not found!";
        return -1;  // Arduino not found
    }

    // Try to open the serial port
    serial->setPortName(arduino_port_name);
    if (serial->open(QSerialPort::ReadWrite)) {
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        qDebug() << "Successfully connected to Arduino on port:" << arduino_port_name;
        return 0;  // Successfully connected
    } else {
        // Log error if opening the serial port fails
        qDebug() << "Failed to open serial port!";
        return 1;  // Failed to open the port
    }
}

int arduino::close_arduino(){
    if(serial->isOpen()){
        serial->close();
        return 0;
    }
    return 1;
}

void arduino::write_to_arduino(QByteArray d){
    if (serial->isWritable()){
        serial->write(d);
    }
    else{
        qDebug() <<"error en ecriture";
    }
}



QByteArray arduino::read_from_arduino(){
    if(serial->isReadable()){
        data=serial->readAll();
        return data;
    }

}

