#include "mainwindowW.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "smtp2.h"
#include <QSerialPort>
#include <QDebug>
#include <QByteArray>
#include "mainwinw.h"
#include "ui_mainwin.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
       ui(new Ui::MainWindow)
   {
    ui->setupUi(this);

    // Initialisation de la connexion série pour RFID
    rfidSerialPort = new QSerialPort(this);
    setupRFID();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete rfidSerialPort;  // Nettoyage de l'objet QSerialPort
}

void MainWindow::setupRFID()
{
    // Configuration du port série pour RFID
    rfidSerialPort->setPortName("COM4");  // Remplacez "COM3" par le port approprié sur votre machine
    rfidSerialPort->setBaudRate(QSerialPort::Baud9600);  // Définir le débit en bauds
    rfidSerialPort->setDataBits(QSerialPort::Data8);  // 8 bits de données
    rfidSerialPort->setParity(QSerialPort::NoParity);  // Pas de parité
    rfidSerialPort->setStopBits(QSerialPort::OneStop);  // 1 bit de stop
    rfidSerialPort->setFlowControl(QSerialPort::NoFlowControl);  // Aucun contrôle de flux

    if (rfidSerialPort->open(QIODevice::ReadWrite)) {
        connect(rfidSerialPort, &QSerialPort::readyRead, this, &MainWindow::on_rfidDataReceived);
        qDebug() << "Port série RFID ouvert avec succès.";
    } else {
        qDebug() << "Impossible d'ouvrir le port série RFID.";
    }
}

void MainWindow::on_pushButtonlog_clicked()
{
    QString username = ui->lineEdituserr->text();
    QString password = ui->lineEditpasswordd->text();

    QSqlQuery query;
    query.prepare("SELECT NOM_E, POSTE_E FROM EMPLOYE WHERE USER_E = :username AND PAS_E = :password");

    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            this->hide();  // Hide the current window (MainWindow)
            Mainwin = new MainWin(this);  // Set MainWindow as the parent of MainWin
            Mainwin->show();
        } else {
            QMessageBox::warning(this, "Login Error", "Username and password are not correct.");
        }
    } else {
        QString errorMsg = query.lastError().text();
        qDebug() << "Error executing query: " << errorMsg;
        QMessageBox::warning(this, "Database Error", "Failed to execute query: " + errorMsg);
    }
}
void MainWindow::on_rfidDataReceived() {
    QByteArray rfidData = rfidSerialPort->readAll();
    QString rfidTag = QString(rfidData).trimmed();  // Convertir les données en chaîne de caractères et supprimer les espaces

    qDebug() << "Données RFID reçues: " << rfidTag;

    // ID de carte RFID valide (exemple)
    QString validCardID = "b38fdc1b";  // Remplacez par l'ID valide de votre carte

    // Comparaison de l'ID reçu avec l'ID valide
    if (rfidTag == validCardID) {
        QMessageBox::information(this, "Accès Autorisé", "Carte RFID valide. Bienvenue !");
        // Logique pour autoriser l'accès
    } else {
        QMessageBox::critical(this, "Accès Refusé", "Carte RFID invalide. Accès interdit.");
        // Logique pour refuser l'accès
    }
}


void MainWindow::on_forgot_pb_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

QString MainWindow::generateVerificationCode()
{
    std::srand(std::time(nullptr));
    int code = std::rand() % 9000 + 1000;
    return QString::number(code);
}
#include "smtp2.h"
void MainWindow::mailSent(QString status, const QString &verificationCode)
{
    if (status == "Message sent") {
        sentVerificationCode = verificationCode;
        qDebug() << "Verification code sent:" << sentVerificationCode;
        QMessageBox::warning(this, tr("Qt Simple SMTP client"), tr("Message sent!\n\n"));
    }
}

void MainWindow::sendMail(const QString &verificationCode)
{
    QString user = "culeks.here@gmail.com";
    QString pass = "dwdybsjavvzkllvq";
    QString host = "smtp.gmail.com";
    int port = 465;
    int timeout = 30000;

    QString from = "culeks.here@gmail.com";
    QString to = ui->rcpt->text();

    Smtp2 *smtp2 = new Smtp2(user, pass, host, port, timeout);
    smtp2->sendMail(from, to, "Verification Code", verificationCode);  // Envoi du code de vérification
}

void MainWindow::on_sendBtn_clicked()
{
    QString verificationCode = generateVerificationCode();
    sendMail(verificationCode);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_verify_pb_clicked()
{
    QString enteredCode = ui->codebt->text().trimmed();
    QString expectedCode = "587965";

    if (enteredCode == expectedCode) {
        ui->stackedWidget->setCurrentIndex(3);
    } else {
        QMessageBox::warning(this, tr("Verification Failed"), tr("Incorrect verification code. Please try again."));
    }
}

void MainWindow::on_cont_pb_clicked()
{
    QString email = ui->rcpt->text().trimmed();
    QString existingPassword = ui->pas1->text().trimmed();
    QString newPassword = ui->pas2->text().trimmed();

    QString storedPassword;
    QSqlQuery query;
    query.prepare("SELECT PAS_E FROM EMPLOYE WHERE EMAIL_E = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        storedPassword = query.value(0).toString();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to retrieve existing password."));
        return;
    }

    if (existingPassword != storedPassword) {
        QMessageBox::warning(this, tr("Error"), tr("Incorrect existing password."));
        return;
    }

    query.prepare("UPDATE EMPLOYE SET PAS_E = :newPassword WHERE EMAIL_E = :email");
    query.bindValue(":newPassword", newPassword);
    query.bindValue(":email", email);

    if (query.exec()) {
        QMessageBox::information(this, tr("Password Updated"), tr("Password updated successfully."));
        ui->stackedWidget->setCurrentIndex(0);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to update password."));
    }

    ui->stackedWidget->setCurrentIndex(0);
}
