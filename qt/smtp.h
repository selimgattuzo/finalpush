#ifndef SMTP_H
#define SMTP_H
#include <QString>

class Smtp
{
public:
    Smtp();
    Smtp(QString, QString, QString);

    static int sendEmail(QString,QString,QString);
private:
    QString destinataire;
    QString object,body;
};

#endif // EMAIL_H
