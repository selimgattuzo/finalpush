#include "smtp2.h"
#include <QSslSocket>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

Smtp2::Smtp2(const QString &user, const QString &pass, const QString &host, int port, int timeout)
{
    socket = new QSslSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    this->user = user;
    this->pass = pass;
    this->host = host;
    this->port = port;
    this->timeout = timeout;
}

void Smtp2::sendMail(const QString &from, const QString &to, const QString &subject, const QString &body, QStringList files)
{
    message = "To: " + to + "\n";
    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");

    message.append("MIME-Version: 1.0\n");
    message.append("Content-Type: multipart/mixed; boundary=frontier\n\n");

    message.append("--frontier\n");
    message.append("Content-Type: text/plain\n\n");
    message.append(body);
    message.append("\n\n");

    if (!files.isEmpty()) {
        qDebug() << "Files to be sent: " << files.size();
        foreach (QString filePath, files) {
            QFile file(filePath);
            if (file.exists()) {
                if (!file.open(QIODevice::ReadOnly)) {
                    qDebug("Couldn't open the file");
                    QMessageBox::warning(0, tr("Qt Simple smtp2 client"), tr("Couldn't open the file\n\n"));
                    return;
                }
                QByteArray bytes = file.readAll();
                message.append("--frontier\n");
                message.append("Content-Type: application/octet-stream\nContent-Disposition: attachment; filename=" + QFileInfo(file.fileName()).fileName() + ";\nContent-Transfer-Encoding: base64\n\n");
                message.append(bytes.toBase64());
                message.append("\n");
            }
        }
    } else {
        qDebug() << "No attachments found";
    }

    message.append("--frontier--\n");
    message.replace("\n", "\r\n");
    message.replace("\r\n.\r\n", "\r\n..\r\n");

    this->from = from;
    rcpt = to;
    state = Init;
    socket->connectToHostEncrypted(host, port); // "smtp2.gmail.com" and 465 for gmail TLS
    if (!socket->waitForConnected(timeout)) {
        qDebug() << socket->errorString();
    }

    t = new QTextStream(socket);
}

Smtp2::~Smtp2()
{
    delete t;
    delete socket;
}

void Smtp2::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "stateChanged " << socketState;
}

void Smtp2::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "error " << socketError;
}

void Smtp2::disconnected()
{
    qDebug() << "disconnected";
    qDebug() << "error " << socket->errorString();
}

void Smtp2::connected()
{
    qDebug() << "Connected ";
}

void Smtp2::readyRead()
{
    qDebug() << "readyRead";
    // SMTP2 is line-oriented
    QString responseLine;
    do {
        responseLine = socket->readLine();
        response += responseLine;
    } while (socket->canReadLine() && responseLine[3] != ' ');

    responseLine.truncate(3);

    qDebug() << "Server response code:" << responseLine;
    qDebug() << "Server response: " << response;

    if (state == Init && responseLine == "220") {
        // banner was okay, let's go on
        *t << "EHLO localhost" << "\r\n";
        t->flush();
        state = HandShake;
    } else if (state == HandShake && responseLine == "250") {
        socket->startClientEncryption();
        if (!socket->waitForEncrypted(timeout)) {
            qDebug() << socket->errorString();
            state = Close;
        }
        *t << "EHLO localhost" << "\r\n";
        t->flush();
        state = Auth;
    } else if (state == Auth && responseLine == "250") {
        // Trying AUTH
        qDebug() << "Auth";
        *t << "AUTH LOGIN" << "\r\n";
        t->flush();
        state = User;
    } else if (state == User && responseLine == "334") {
        // Trying User
        qDebug() << "Username";
        *t << QByteArray().append(user).toBase64() << "\r\n";
        t->flush();
        state = Pass;
    } else if (state == Pass && responseLine == "334") {
        // Trying pass
        qDebug() << "Pass";
        *t << QByteArray().append(pass).toBase64() << "\r\n";
        t->flush();
        state = Mail;
    } else if (state == Mail && responseLine == "235") {
        // HELO response was okay
        qDebug() << "MAIL FROM:<" << from << ">";
        *t << "MAIL FROM:<" << from << ">\r\n";
        t->flush();
        state = Rcpt;
    } else if (state == Rcpt && responseLine == "250") {
        *t << "RCPT TO:<" << rcpt << ">\r\n";
        t->flush();
        state = Data;
    } else if (state == Data && responseLine == "250") {
        *t << "DATA\r\n";
        t->flush();
        state = Body;
    } else if (state == Body && responseLine == "354") {
        *t << message << "\r\n.\r\n";
        t->flush();
        state = Quit;
    } else if (state == Quit && responseLine == "250") {
        *t << "QUIT\r\n";
        t->flush();
        state = Close;
        emit status(tr("Message sent"));
    } else if (state == Close) {
        deleteLater();
        return;
    } else {
        QMessageBox::warning(0, tr("Qt Simple smtp2 client"), tr("Unexpected reply from smtp2 server:\n\n") + response);
        state = Close;
        emit status(tr("Failed to send message"));
    }
    response = "";
}
