#include "client.h"
#include <QString>
#include <QNetworkAccessManager>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <QUrl>
#include <QEventLoop>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

client::client()
{nom="";
    prenom="";
    sexe="";
    mail="";
    cin="";
    tel=0;
    d=QDate::currentDate();
     manager = new QNetworkAccessManager();
}
client:: client(QString nom,QString prenom,QString sexe,QString mail ,QString cin,int tel,QDate d)
{
this->nom=nom;
    this->prenom=prenom;
    this->sexe=sexe;
    this->mail=mail;
    this->cin=cin;
    this->tel=tel;
    this->d=d;
      manager = new QNetworkAccessManager();

}

QSqlQueryModel *client  :: afficher ()
{
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select * from SP_CLIENT");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("CIN"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("prénom"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("nom"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("mail"));

    model->setHeaderData(4,Qt::Horizontal,QObject::tr("sexe"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("telephone"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("Date de naissance"));

    return model;

}
bool client :: supprimer(QString cin1)
{
  QSqlQuery query;
  query.prepare("delete from SP_CLIENT where cin_c= :CIN");
   query.bindValue(":CIN",cin1);

   return query.exec() && query.numRowsAffected() > 0;

}//dima traja3 true
bool client ::ajouter()
{
  QSqlQuery query;


  query.prepare("insert into SP_CLIENT (cin_c,prenom_c,nom_c,mail,sexe,telephone_c,daten)values(:cin,:prenom,:nom,:mail,:sexe,:tel,:d)");
  //creation des variable liéés
  query.bindValue(":nom",nom);
  query.bindValue(":prenom",prenom);
  query.bindValue(":sexe",sexe);
  query.bindValue(":mail",mail);
  query.bindValue(":cin",cin);
  query.bindValue(":tel",tel);
   query.bindValue(":d",d);

 printf("done ajout");

  return query.exec(); // exec() envoie la requéte pour l'exécuter
}
QSqlQuery client::getClientByBirthdate(const QDate &date) {
    QSqlQuery query;
    query.prepare("SELECT nom_c, prenom_c, sexe, mail, telephone_c, cin_c "
                  "FROM SP_CLIENT "
                  "WHERE EXTRACT(MONTH FROM daten) = :month "
                  "AND EXTRACT(DAY FROM daten) = :day");
    query.bindValue(":month", date.month());
    query.bindValue(":day", date.day());

    if (!query.exec()) {
        qDebug() << "Error in getClientByBirthdate: " << query.lastError();
    }
    return query;
}



bool client::modifier(QString mail, QString nom, QString prenom, QString sexe, QString cin1, int tel) {
    QSqlQuery query;
    bool success = true;


    if (!prenom.isEmpty()) {
        query.prepare("UPDATE SP_CLIENT SET prenom_c = :prenom WHERE cin_c = :cin2");
        query.bindValue(":cin2", cin1);
        query.bindValue(":prenom", prenom);
        success = success && query.exec();
    }


    if (!nom.isEmpty()) {
        query.prepare("UPDATE SP_CLIENT SET nom_c = :nom WHERE cin_c = :cin2");
        query.bindValue(":cin2", cin1);
        query.bindValue(":nom", nom);
        success = success && query.exec();
    }


    if (!sexe.isEmpty()) {
        query.prepare("UPDATE SP_CLIENT SET sexe = :sexe WHERE cin_c = :cin2");
        query.bindValue(":cin2", cin1);
        query.bindValue(":sexe", sexe);
        success = success && query.exec();
    }


    if (!mail.isEmpty()) {
        query.prepare("UPDATE SP_CLIENT SET mail = :mail WHERE cin_c = :cin2");
        query.bindValue(":cin2", cin1);
        query.bindValue(":mail", mail);
        success = success && query.exec();
    }


    if (tel != 0) {
        query.prepare("UPDATE client SET telephone_c = :tel WHERE cin_c = :cin2");
        query.bindValue(":cin2", cin1);
        query.bindValue(":tel", tel);
        success = success && query.exec();
    }



    return success;
}
bool client::test(QString cinn, int tell, QString sexee, QString maill) {

    if (cinn.length() != 8 || tell<10000000 || tell>99999999 )
        return false;
    if(sexee != "male"  &&  sexee != "femelle" )

        return false;

  int pos1 = maill.indexOf("@");
   int pos2 = maill.indexOf(".com");
 if((pos1==-1)  || (pos2==-1)|| (pos1 >= pos2))
   return false;

    return true;
}

QSqlQueryModel* client::trier()
{

    QSqlQueryModel * model=new QSqlQueryModel();
    QSqlQuery query;
     query.prepare("SELECT * FROM SP_CLIENT ORDER BY prenom_c ASC");

    if (query.exec())
    {
       model->setQuery(std::move(query));
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("CIN"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("prénom"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("nom"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("mail"));

    model->setHeaderData(4,Qt::Horizontal,QObject::tr("sexe"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("telephone"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("date de naissance"));}



    return model;
}
QSqlQueryModel *client  :: chercher (QString cin)
{
   QSqlQueryModel * model=new QSqlQueryModel();
   QSqlQuery query;
    query.prepare("select * from SP_CLIENT where cin_c= :CIN");
   query.bindValue(":CIN", cin);
   if (query.exec())
   {
      model->setQuery(std::move(query));
   model->setHeaderData(0,Qt::Horizontal,QObject::tr("CIN"));
   model->setHeaderData(1,Qt::Horizontal,QObject::tr("prénom"));
   model->setHeaderData(2,Qt::Horizontal,QObject::tr("nom"));
   model->setHeaderData(3,Qt::Horizontal,QObject::tr("mail"));

   model->setHeaderData(4,Qt::Horizontal,QObject::tr("sexe"));
   model->setHeaderData(5,Qt::Horizontal,QObject::tr("telephone"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("date de naissance"));}

   return model;

}


bool client::PDF() {
    // Step 1: Execute SQL query to retrieve data
    QSqlQuery query("SELECT * FROM SP_CLIENT");


    // Step 2: Create the PDF file
    QString filePath = "C:/Users/habib/OneDrive/Desktop/habiba/clients.pdf"; // Set your desired path
    QPdfWriter writer(filePath);

    writer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&writer);

    // Check if painter is initialized
    if (!painter.isActive()) {
        qDebug() << "Failed to initialize QPainter";
        return false;
    }

    // Set font for the PDF title
    painter.setFont(QFont("Arial", 16));
    painter.drawText(200, 50, "Liste des clients");

    int y = 500;  // Initial position for drawing data

    // Step 3: Draw headers in the PDF
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(50, y, "CIN");
    painter.drawText(650, y, "prenom");
    painter.drawText(1550, y, "nom");
    painter.drawText(2350, y, "mail");
     painter.drawText(3300, y, "sexe");
     painter.drawText(3700, y, "telephone");
      painter.drawText(4500, y, "date de naissance");

    y += 200;  // Move down for next data row

    // Step 4: Draw the data in the PDF
   painter.setFont(QFont("Arial", 6));
    while (query.next()) {
       painter.drawText(50, y, query.value(0).toString());
        painter.drawText(650, y, query.value(1).toString());
        painter.drawText(1550, y, query.value(2).toString());
        painter.drawText(2350, y, query.value(3).toString());
        painter.drawText(3300, y, query.value(4).toString());
         painter.drawText(3700, y, query.value(5).toString());
           painter.drawText(4500, y, query.value(6).toString());
       y += 200;  // Move down for next row

        if (y > writer.height() - 50) {
            writer.newPage();  // Create a new page if current one is full
            y = 100;  // Reset y to start from top
        }
    }

    painter.end();  // End the PDF creation process
    qDebug() << "PDF export complete! The file is saved at: " << filePath;

    return true;  // Return true if PDF is successfully created
}

QtCharts::QChartView* client::stat() {
    QtCharts::QBarSet* setMale = new QtCharts::QBarSet("Masculin");
    QtCharts::QBarSet* setFemale = new QtCharts::QBarSet("Féminin");

    int maleCount = 0, femaleCount = 0;

    QSqlQuery query;
    if (query.exec("SELECT sexe FROM SP_CLIENT")) {
        while (query.next()) {
            QString sexe = query.value(0).toString();
            if (sexe.toLower() == "male") {
                ++maleCount;
            } else if (sexe.toLower() == "femelle") {
                ++femaleCount;
            }
        }
    } else {
        qDebug() << "Query error: " << query.lastError().text();
    }

    *setMale << maleCount;
    *setFemale << femaleCount;

    QtCharts::QBarSeries* series = new QtCharts::QBarSeries();
    series->append(setMale);
    series->append(setFemale);

    QtCharts::QChart* chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des clients par sexe");
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    QStringList categories;
    categories << "Clients";
    QtCharts::QBarCategoryAxis* axisX = new QtCharts::QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QtCharts::QValueAxis* axisY = new QtCharts::QValueAxis();
    axisY->setRange(0, qMax(1, qMax(maleCount, femaleCount) + 1));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QtCharts::QChartView* chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

