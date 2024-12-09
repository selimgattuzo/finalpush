#ifndef CLIENT_H
#define CLIENT_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QPdfWriter>
#include <QPainter>
#include <QDebug>
#include <QPagedPaintDevice>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>


#include <QDate>
#include <QSqlError>

#include <QCalendarWidget>
#include <QDate>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QSslSocket>
#include <QTextStream>
#include <QDebug>

#include "smtp.h"
class QChartView;
class client
{
    QString nom,prenom,sexe,mail,cin;
    int tel;
    QDate d;
      QNetworkAccessManager *manager;
public:
    client();
    client(QString,QString,QString,QString,QString,int,QDate);

        QString getsexe(){return sexe;}
        QString getnom(){return nom;}
        QString getprenom(){return prenom;}
        QString getmail(){return mail;}
        QString getcin(){return cin;}
        int gettel(){return tel;}

        void setsexe(QString s){sexe=s;}
        void setnom(QString n){nom=n;}
        void setprenom(QString p){prenom=p;}
        void setmail(QString m){mail=m;}

        void setcin(QString c){cin=c;}
        void settel(int t){tel=t;}

        bool ajouter();

            QSqlQueryModel * afficher();

            bool supprimer(QString);
            bool modifier(QString,QString,QString,QString,QString,int);
            bool test(QString,int,QString,QString);
            QSqlQueryModel *  trier();
            QSqlQueryModel * chercher (QString );
            bool  PDF();

            QSqlQuery getClientByBirthdate( const QDate &);
           // bool sendEmail( QString &, QString &,  QString &);
 QtCharts::QChartView* stat();





};

#endif // CLIENT_H
