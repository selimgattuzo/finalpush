#include "mainwinw.h"
#include "ui_mainwin.h"
#include "client.h"
#include "commande.h"
#include <QMessageBox>
#include <QSqlError>
#include "connection.h"
#include <QVBoxLayout>
#include <QTimer>

MainWin::MainWin(QWidget *parent) :  // Updated constructor to match class name
    QMainWindow(parent),  // Keep the parent as QWidget or QDialog
      ui(new Ui::MainWin) ,
  selectedEmployeeRow(-1),
  E(new employe()),
  proxyModel(new QSortFilterProxyModel(this))// Updated to match class name
{
    ui->setupUi(this);
        ui->tabWidget_com->setCurrentIndex(0);
        ui->tabWidget->setCurrentIndex(0);
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget_com->setTabText(3, "Statistique");
        ui->tabWidget_com->setTabText(0, "main commande");
        ui->tabWidget_com->setTabText(1, "Contenue");
        ui->tabWidget_com->setTabText(2, "Modifier");
        ui->tabWidget_com->setTabText(4, "Question");
        ui->tabWidget_com->setTabEnabled(3, false);
        ui->tabWidget_com->setTabEnabled(4, false);
        ui->tabWidget_com->setTabEnabled(2, false);
        ui->tabWidget_com->setTabEnabled(1, false);
        ui->search_com->setPlaceholderText("RECHERCHE...");
        ui->suppmodif_com->setPlaceholderText("ENTREZ L'ID...");
        chat = ui->browser_com;
connect(ui->statb, &QPushButton::clicked, this, &MainWin::Stat_emp2);
        ui->cin_e->setValidator(new QIntValidator(0, 99999999, this));
        ui->id_e->setValidator(new QIntValidator(0, 99999999, this));
        ui->salaire_e->setValidator(new QIntValidator(0, 99999999, this));
        ui->telephone_e->setValidator(new QIntValidator(0, 99999999, this));

        connect(ui->Imprimer, &QPushButton::clicked, this, &MainWin::on_Imprimer_clicked);

        // Assuming afficher() returns a QSqlQueryModel
        QSqlQueryModel *employeeModel = E->afficher();  // Renamed the first model to employeeModel
        proxyModel->setSourceModel(employeeModel);

        QStandardItemModel *standardModel = new QStandardItemModel(employeeModel->rowCount(), employeeModel->columnCount(), this);

        // Set the horizontal header names to match the database column names
        for (int col = 0; col < employeeModel->columnCount(); ++col) {
            standardModel->setHorizontalHeaderItem(col, new QStandardItem(employeeModel->headerData(col, Qt::Horizontal).toString()));
        }

        // Populate the table with data
        for (int row = 0; row < employeeModel->rowCount(); ++row) {
            for (int col = 0; col < employeeModel->columnCount(); ++col) {
                QModelIndex index = employeeModel->index(row, col);
                QString data = employeeModel->data(index).toString();
                QStandardItem *item = new QStandardItem(data);
                standardModel->setItem(row, col, item);
            }
        }

        ui->tab_employes->setModel(standardModel);

        connect(ui->tab_employes->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                this, SLOT(on_tableViewSelectionChanged(QModelIndex, QModelIndex)));
        c.updateCompleter(ui->suppmodif_com);
        effacer();

        // Renamed the second model to clientModel
        QSqlQueryModel* clientModel = cl.afficher();  // Renamed to clientModel

        if (clientModel) {
            ui->tableView->setModel(clientModel);
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de charger les données dans la table.");
        }

        // Mettre à jour le calendrier avec les anniversaires (si applicable)
        updateCalendarWithBirthdays();

        // Connecter Arduino (si applicable)
        if (a.connect_arduino() != 0) {
            qDebug() << "Failed to connect to Arduino! Continuing without it.";
        } else {
            qDebug() << "Arduino connected successfully.";
            QTimer *rfidTimer = new QTimer(this);  // Timer to periodically read RFID
            connect(rfidTimer, &QTimer::timeout, this, &MainWin::afficher_arduino_com);  // Connect the timeout to readRFID
            rfidTimer->start(5000);  // Start the timer to read RFID every 5000 ms (5 seconds)
        }
    }
MainWin::~MainWin()  // Destructor, updated to match the new class name
{
    delete ui;
     delete E;
}

void MainWin::on_ajouter_clicked()
{
    ui->tableView->setModel(cl.afficher());
    QString cin = ui->L4->text();
    int tel = ui->L6->text().toInt();
    QString mail = ui->L5->text();
    QString nom = ui->L1->text();
    QString prenom = ui->L2->text();
    QString sexe = ui->L7->text();
    QDate date = ui->date->date();
     ui->tableView->setModel(cl.afficher());
    bool t = cl.test(cin, tel, sexe, mail);

    if (t)
    {
        client c(nom, prenom, sexe, mail, cin, tel, date);
        ui->tableView->setModel(cl.afficher());
        bool test = c.ajouter();
        if (test)
        {
            updateCalendarWithBirthdays();
            ui->tableView->setModel(cl.afficher());

            // Effacer les champs après l'ajout
           // Réinitialise à la date actuelle*/

            QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Ajout effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
            ui->L4->clear();
            ui->L6->clear();
            ui->L5->clear();
            ui->L1->clear();
            ui->L2->clear();
            ui->L7->clear();
            ui->date->setDate(QDate::currentDate());
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Ajout non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
        }
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Ajout non effectué attribut non correct.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWin::on_calendarWidget_clicked(const QDate &date) {
    QSqlQuery query = cl.getClientByBirthdate(date);

    if (query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        QString sexe = query.value(2).toString();
        QString mail = query.value(3).toString();
        int tel = query.value(4).toInt();

        QMessageBox::information(this, "Client Information",
                                 "Name: " + nom + " " + prenom + "\n"
                                 "Sex: " + sexe + "\n"
                                 "Email: " + mail + "\n"
                                 "Phone: " + QString::number(tel));
    } else {
        QMessageBox::information(this, "No Client Found", "No client information for this date.");
    }
}
void MainWin::updateCalendarWithBirthdays()
{

 Connection c;
 c.createconnect();
 QSqlQuery query;
    query.prepare("SELECT  cin_c, daten FROM SP_CLIENT");  // Query to get clients' birthdates

    if (query.exec()) {
        qDebug() << "Query executed successfully";

        QCalendarWidget *calendarWidget = ui->calendarWidget;


        QTextCharFormat defaultFormat;
        calendarWidget->setDateTextFormat(QDate(), defaultFormat);


        QTextCharFormat highlightFormat;
        highlightFormat.setBackground(Qt::yellow);


        while (query.next()) {
            QString cin = query.value("cin_c").toString();
            QDate birthDate = query.value("daten").toDate();


            qDebug() << "client CIN: " << cin << ", client daten: " << birthDate.toString();


            if (!birthDate.isValid()) {
                qDebug() << "Invalid date for CIN: " << cin;
                continue;
            }


            int currentYear = QDate::currentDate().year();
            QDate birthdayThisYear = birthDate.addYears(currentYear - birthDate.year());


            calendarWidget->setDateTextFormat(birthdayThisYear, highlightFormat);
        }
    } else {
        qDebug() << "Error executing query: " << query.lastError();
    }
}



void MainWin::on_supprimer_clicked()
{ ui->tableView->setModel(cl.afficher());
QString cin2=ui->L8->text();

bool test=cl.supprimer(cin2);
if (test)
{
    ui->tableView->setModel(cl.afficher());
    QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("suppression effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
       }
       else
           QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("supression non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
   }

void MainWin::on_modifier_clicked()
{  ui->tableView->setModel(cl.afficher());
    QString cin1=ui->L4->text();
    int tel=ui->L6->text().toInt();
    QString  mail=ui->L5->text();
    QString  nom=ui->L1->text();
    QString  prenom=ui->L2->text();
    QString  sexe=ui->L7->text();
    bool test=cl.modifier(mail,nom,prenom,sexe,cin1,tel);
    if (test)
    {
        ui->tableView->setModel(cl.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("modification effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
           }
           else
               QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("modification non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
       }

void MainWin::on_trier_clicked()
{

    QSqlQueryModel* sortedModel = cl.trier();


    if (sortedModel) {

        ui->tableView->setModel(sortedModel);
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Tri effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
    } else {

        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Tri non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}


void MainWin::on_chercher_clicked()
{
     QString cin1=ui->L8->text();
     QSqlQueryModel* sortedModel = cl.chercher(cin1);
     if (sortedModel) {

         ui->tableView->setModel(sortedModel);

         QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("recherche effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
     } else {

         QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("recherche non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
     }
}

void MainWin::on_exporter_clicked()
{
    bool test=cl.PDF();
    if (test)
    {

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("pdf effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
           }
           else
               QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("pdf non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);

}
void MainWin::on_stat_clicked() {
    // Créer le graphique des statistiques
    QtCharts::QChartView* chartView = cl.stat();  // Ensure cl.stat() returns a QtCharts::QChartView*

       if (chartView) {
           // Create a dialog to display the chart
           QDialog* statsDialog = new QDialog(this);
           statsDialog->setWindowTitle("Statistiques des clients");
           statsDialog->resize(600, 400);

           // Add the chart view to the dialog's layout
           QVBoxLayout* layout = new QVBoxLayout(statsDialog);
           layout->addWidget(chartView);
           statsDialog->setLayout(layout);

           // Display the dialog
           statsDialog->exec();

           // Inform the user of success
           QMessageBox::information(this, QObject::tr("OK"),
                                    QObject::tr("Statistiques affichées avec succès.\nClick OK to continue."),
                                    QMessageBox::Ok);
       } else {
           // Show an error message if the chart view creation failed
           QMessageBox::critical(this, QObject::tr("Not OK"),
                                 QObject::tr("Erreur lors de la génération des statistiques.\nClick Cancel to exit."),
                                 QMessageBox::Cancel);
       }
   }


void MainWin::on_mail_clicked()
{
    QString cin_to = ui->L10->text();  // Get CIN from the UI
    QString subject = ui->L11->text(); // Get subject from the UI
    QString body = ui->L12->toPlainText(); // Get body from the UI

    // Prepare the query to fetch the email of the client based on CIN
    QSqlQuery query;
    query.prepare("SELECT mail FROM SP_CLIENT WHERE cin_c = :cin_to");
    query.bindValue(":cin_to", cin_to);  // Bind the CIN parameter

    if (query.exec() && query.next()) {  // Execute the query and check if a result is returned
        QString to = query.value(0).toString();  // Get the email address from the result
        Smtp::sendEmail(to, subject, body);  // Send the email using the Smtp class
        qDebug() << "Mail sent to:" << to;
    } else {
        // Handle error if no matching CIN is found
        QMessageBox::critical(this, "Error", "No client found with the given CIN.");
    }
}

QString MainWin::readRFID() {
    QByteArray rfidTagID = a.read_from_arduino();  // Read RFID data
    QString data = QString(rfidTagID).trimmed();
    if (!rfidTagID.isEmpty()) {
          // Convert to QString
        qDebug() << "Received RFID Tag ID:" << data;
        ui->rfidlabel->setText(data);
        QSqlQuery query;
        query.prepare("SELECT cin_c, nom_c, prenom_c, daten, sexe, mail, telephone_c FROM SP_CLIENT WHERE rfid= :id");
        query.bindValue(":id",data);
        if(query.exec() && query.next()){
            int cin_c = query.value("cin_c").toInt();
                            QString nom_c = query.value("nom_c").toString();
                            QString prenom_c = query.value("prenom_c").toString();
                            QDate datenais = query.value("daten").toDate();
                            QString sexe = query.value("sexe").toString();
                            QString mail = query.value("mail").toString();
                            int telephone = query.value("telephone_c").toInt();
                            QString date = datenais.toString("yyyy-MM-dd");
                            // Display the client data in the UI (example using qDebug and QLabel)
                            qDebug() << "Client Details:";
                            qDebug() << "CIN:" << cin_c;
                            qDebug() << "Name:" << nom_c << prenom_c;
                            qDebug() << "Date of Birth:" << datenais;
                            qDebug() << "Gender:" << sexe;
                            qDebug() << "Email:" << mail;
                            qDebug() << "Phone:" << telephone;

                            // Optional: Update GUI elements with client details
                            ui->cinlabel->setText(QString::number(cin_c));           // Ensure you have corresponding QLabel in your UI
                            ui->nomlabel->setText(nom_c);
                            ui->prenomlabel->setText(prenom_c);
                            ui->datelabel->setText(date);
                            ui->sexelabel->setText(sexe);
                            ui->maillabel->setText(mail);
                            ui->telephonelabel->setText(QString::number(telephone));
                            return data;
        }
    }
}

void MainWin::on_client_clicked() {
    // Switch back to the table page (index 0 in QStackedWidget)
    ui->stackedWidget->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);

}

//commande
void MainWin::on_accepte_com_clicked()
{
    c.onAccepteClicked_contenir(ui->tableau_com, ui->montanttot, ui->id_med_com, ui->quant_com, montant);
}

void MainWin::on_exit_com_clicked()
{
    c.onExitClicked_contenir(ui->tabWidget_com, ui->table_com,ui->tableau_com, ui->montanttot, montant, ui->suppmodif_com);
}

void MainWin::on_ajouter_com_clicked()
{
    QString text = ui->client_com->currentText();
    QStringList parts = text.split('#');
    QString cin_c;
    QString client;
    bool ok;
    int cin;

    if (parts.size() > 1) {
        cin_c = parts[1].trimmed();
        client = parts[0].trimmed();
    }

    cin = cin_c.toInt(&ok);

    QDateTime date_com = ui->date_com->dateTime();
    QString priorite = ui->priorite->currentText();
    int nombre = 1;
    int delai = 0;
    QString etat = "en cours";
    float montant = 0.0f;
    QString id = "";

    c = Commande(id, cin, client, date_com, delai, etat, priorite, nombre, montant);

    if (c.ajouter()) {
        QSqlQuery query("SELECT last_insert_rowid() FROM SP_COMMANDE");
        if (query.next()) {
            QString id = query.value(0).toString();
            c.setID(id);
        }
        ui->tabWidget_com->setTabEnabled(1, true);
        ui->tabWidget_com->setCurrentIndex(1);
    } else {
        QMessageBox::warning(this, "Error", "La commande n'a pas été ajoutée.");
    }
}

void MainWin::on_supprimer_com_clicked()
{
    c.onsupprimerclicked(ui->suppmodif_com, ui->suppmodifmsg, ui->table_com); // Call the method to handle the delete

}

void MainWin::on_annuler_com_clicked()
{
    ui->tabWidget_com->setCurrentIndex(0);
    ui->tabWidget_com->setTabEnabled(2, false);
}
void MainWin::on_accepte_2_com_clicked()
{
    c.onAccepteClicked_modif(ui->tabWidget_com,ui->table_com,ui->suppmodif_com,ui->clientmodif, ui->montantmodif, ui->prioritemodif, ui->delaimodif, ui->etatmodif, ui->datemodif, id);
}
void MainWin::on_modifier_com_clicked()
{
    c.onModifierClicked(ui->tabWidget_com,&id,ui->suppmodif_com,ui->suppmodifmsg, ui->clientmodif, ui->datemodif, ui->delaimodif, ui->etatmodif, ui->prioritemodif, ui->montantmodif);
}

void MainWin::on_exporter_com_clicked()
{
    c.onExporterClicked();
}



void MainWin::on_tri_com_clicked()
{
    c.onTriClicked(ui->table_com,ui->par_com);


}
void MainWin::on_search_com_textChanged(const QString &arg1)
{
    c.onRechercheClicked(ui->table_com,ui->search_com);
}


void MainWin::effacer() {
    if (ui->chart_com && ui->anneechart_com) {
        QLayoutItem *item;
        QLayoutItem *item1;
        while (((item = ui->chart_com->takeAt(0)) != nullptr)&&((item1 = ui->anneechart_com->takeAt(0)) != nullptr)) {
            QWidget *widget = item->widget();
            QWidget *widget1 = item1->widget();
            if (widget && widget1) {
                widget->deleteLater();
                widget1->deleteLater();
            }
            delete item;
            delete item1;

        }
    }
}
void MainWin::on_stat_com_clicked()
{
    ui->tabWidget_com->setTabEnabled(3, true);
    ui->tabWidget_com->setCurrentIndex(3);
    c.statnbr(ui->chart_com);
    c.statannee(ui->anneechart_com);
}
void MainWin::on_retourne_com_clicked()
{
    ui->tabWidget_com->setTabEnabled(3, false);
    ui->tabWidget_com->setCurrentIndex(0);
    effacer();
}
void MainWin::on_quest_clicked()
{
    ui->tabWidget_com->setTabEnabled(4, true);
    ui->tabWidget_com->setCurrentIndex(4);
}
void MainWin::on_med_moins_cmd_clicked()
{
    QString medicament;
    int totalq;
    int test=c.question_med_moins(medicament,totalq);
    if(test==0){
         QString question = QString("<br><br><div style='text-align: right; vertical-align: middle; font-size: 14px;'>"
                                    "                                                      <b>\tutilisateur:</b> Quel médicament est le moins commandé ?</td></tr></table>");
         QString reponse = QString("<table style='width:100%;'><tr><td style='text-align: left; font-size: 14px;'>"
                                   "\t<b>chat:</b> Le médicament le moins commandé est: %1 avec %2 commandes.</td></tr></table>")
                                 .arg(medicament).arg(totalq);
         chat->append(question);
         chat->append(reponse);
    }
}
void MainWin::on_med_plus_rapporte_clicked()
{
      QString medicament;
      double totalprix;
      int test=c.question_med_plus(medicament,totalprix);
      if(test==0){
      QString question = QString("<br><br><div style='text-align: right; vertical-align: middle; font-size: 14px;'>"
                                 "<b>utilisateur:</b> Quel est le médicament qui rapporte le plus ?</td></tr></table>");
      QString reponse = QString("<table style='width:100%;'><tr><td style='text-align: left; font-size: 14px;'>"
                                "\t<b>chat:</b> Le médicament qui rapporte le plus est: %1 avec un revenu total de: %2 TND.</td></tr></table>")
                                 .arg(medicament).arg(totalprix);

     chat->append(question);
     chat->append(reponse);
      }
}
void MainWin::on_delai_moy_clicked()
{
               QString question = QString("<br><br><div style='text-align: right; vertical-align: middle; font-size: 14px;'>"
                                          "<b>utilisateur:</b> Quel est le délai moyen de livraison des commandes ?</td></tr></table>");

               QString reponse = QString("<table style='width:100%;'><tr><td style='text-align: left; font-size: 14px;'>"
                                        "\t<b>chat:</b> Le délai moyen de livraison des commandes est de %1 jours.</td></tr></table>")
                                .arg(c.question_average_delai());
               chat->append(question);
               chat->append(reponse);
}
void MainWin::on_meilleur_client_clicked()
{
    int cin_c;
    double totmontant;
    int test = c.question_meill_client(cin_c, totmontant);
    if(test==0){
        QString question = QString("<br><br><div style='text-align: right; vertical-align: middle; font-size: 14px;'>"
                                           "<b>utilisateur:</b> Qui est le meilleur client pour nous?</td></tr></table>");
        QString reponse = QString("<table style='width:100%;'><tr><td style='text-align: left; font-size: 14px;'>"
                                         "\t<b>chat:</b> Le meilleur client est le client avec le CIN %1 qui a payé un total de %2 TND.</td></tr></table>")
                                 .arg(cin_c).arg(totmontant);
        chat->append(question);
        chat->append(reponse);
    }
}
void MainWin::on_revenue_annee_clicked()
{
                QString question = QString("<br><br><div style='text-align: right; vertical-align: middle; font-size: 14px;'>"
                                           "<b>utilisateur:</b> Quels sont les revenus générés cette année?</td></tr></table>");
                QString reponse = QString("<table style='width:100%;'><tr><td style='text-align: left; font-size: 14px;'>"
                                         "\t<b>chat:</b> Les revenus générés cette annéé est %1 TND.</td></tr></table>")
                                 .arg(c.question_rev_annee());
                chat->append(question);
                chat->append(reponse);
}


void MainWin::on_commande_annulees_clicked()
{
    QString question = QString("<br><br><div style='text-align: right; vertical-align: middle; font-size: 14px;'>"
                                       "<b>utilisateur:</b> Combien de commandes ont été annulées?</td></tr></table>");

            QString reponse = QString("<table style='width:100%;'><tr><td style='text-align: left; font-size: 14px;'>"
                                     "\t<b>chat:</b> Nombre des commandes annulées= %1 commande.</td></tr></table>")
                             .arg(c.question_annuler());

            chat->append(question);
            chat->append(reponse);
}


void MainWin::on_retourne_2_com_clicked()
{
    ui->tabWidget_com->setTabEnabled(4, false);
    ui->tabWidget_com->setCurrentIndex(0);
    chat->clear();
}

void MainWin::afficher_arduino_com() {
    QSqlQuery query;
    QSqlQuery query2;
        QString datarfid = readRFID();
        if (datarfid != "") {
            query.prepare("SELECT cin_c FROM SP_CLIENT WHERE rfid= :rfid");
            query.bindValue(":rfid", datarfid);

            if (query.exec()) {
                if (query.next()) {
                    qDebug() << "RFID cin_c:" << datarfid;
                    int cin = query.value(0).toInt();
                    qDebug()<<"cin:" <<cin;

                    query2.prepare("SELECT SUM(montant) FROM SP_COMMANDE WHERE cin_c = :cin");
                    query2.bindValue(":cin", cin);

                    if (query2.exec() && query2.next()) {
                        double totalAmount = query2.value(0).toDouble();
                       if (!query2.value(0).isNull()) {
                            qDebug() << "Total montant cin_c:" << totalAmount;
                            QString amountString = QString::number(totalAmount, 'f', 0).rightJustified(4, '0');
                            QByteArray data = amountString.toUtf8();
                            a.write_to_arduino(data);
                            qDebug() << "montant arduino:" << data;
                       }
                    }
                }
            }
        }
}
void MainWin::on_cmnd_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget_com->setCurrentIndex(0);
    c.load(ui->table_com);
    c.comboboxcom(ui->id_med_com, ui->client_com);
}


void MainWin::on_employee_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->tabWidget_com->setCurrentIndex(0);
}

//selim
#include<QLabel>
#include<QMessageBox>
#include<QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlQueryModel>
#include<QApplication>
#include <iostream>
#include<QIntValidator>
#include<QBoxLayout>
#include<QLineEdit>
#include<QComboBox>
#include<QMessageBox>
#include <QApplication>
#include <QDebug>
#include<QWidget>
#include <employe.h>
#include "mainwindowW.h"
#include <mainwindowW.h>
#include <QSortFilterProxyModel>
#include <QPrinter>
#include <QPainter>
#include<QSqlRecord>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QTextDocumentWriter>
#include <QtCharts>
#include<QtCharts>
#include<QChartView>
#include<QPieSeries>
#include "smtp.h"
#include <QPrintDialog>

void MainWin::on_pb_ajouter_clicked()
{
   QMessageBox msgBox;
   msgBox.setText("Confirmation");
   msgBox.setInformativeText("Are you sure you want to add?");
   msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   msgBox.setDefaultButton(QMessageBox::Cancel);

   int reply = msgBox.exec();
   if (reply == QMessageBox::Cancel) {
       return;
   }
   int id_e = ui->id_e->text().toInt();
   int cin_e = ui->cin_e->text().toInt();
   QString nom_e = ui->nom_e->text();
   QString poste_e = ui->poste_e->text();
   int telephone_e = ui->telephone_e->text().toInt();
   QString email_e = ui->email_e->text();
   QString datee_e = ui->datee_e->text();
   int salaire_e = ui->salaire_e->text().toInt();
   QString user_e = ui->user_e->text();
   QString pas_e = ui->pas_e->text();

   if (ui->id_e->text().isEmpty() || ui->cin_e->text().isEmpty()
       || ui->nom_e->text().isEmpty() || ui->email_e->text().isEmpty()
       || ui->poste_e->text().isEmpty() || ui->telephone_e->text().isEmpty() || ui->datee_e->text().isEmpty() || ui->salaire_e->text().isEmpty()
       || ui->user_e->text().isEmpty() || ui->pas_e->text().isEmpty()) {
       QMessageBox::warning(this, tr("Champs manquants"),
                            tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
       return;
   }

   QDate date;





   // Character-only Validation
   QRegularExpression characterRegex("^[a-zA-Zéèêëàâäîïôöûüç -]*$");

   if (!characterRegex.match(nom_e).hasMatch() || !characterRegex.match(poste_e).hasMatch()) {
       QMessageBox::warning(this, tr("Caractères invalides"),
                            tr("Veuillez utiliser uniquement des caractères alphabétiques pour le nom, le prénom et l'adresse."), QMessageBox::Ok);
       return;
   }
   employe E(id_e, cin_e, nom_e, poste_e, telephone_e,email_e , datee_e, salaire_e,user_e,pas_e);
   bool test = E.ajouter();
   if (test)
   {
       ui->tab_employes->setModel(E.afficher());

       QMessageBox msgBox(QMessageBox::Information, QObject::tr("OK"),
                          QObject::tr("Ajout effectué. \nClick Cancel to exit."), QMessageBox::Cancel, this);

       // Connect the finished signal to a custom slot
       connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

       msgBox.exec();
   }
   else
   {
       QMessageBox msgBox(QMessageBox::Critical, QObject::tr("NOT OK"),
                          QObject::tr("Ajout non effectué. \nClick Cancel to exit."), QMessageBox::Cancel, this);

       // Connect the finished signal to a custom slot
       connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

       msgBox.exec();
   }
}

void MainWin::on_pb_supprimer_clicked()
{
   QMessageBox msgBox;
   msgBox.setText("Confirmation");
   msgBox.setInformativeText("Are you sure you want to delete?");
   msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   msgBox.setDefaultButton(QMessageBox::Cancel);

   int reply = msgBox.exec();
   if (reply == QMessageBox::Cancel) {
       return;
   }
   employe E;

   int id_e = ui->id_e->text().toInt();
   E.setid_e(id_e);

   bool exists = E.checkIfemployeExists(id_e);
   if (!exists) {
       QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                             QObject::tr("L'employé n'a pas été trouvé dans la base de données.").arg(id_e),
                             QMessageBox::Cancel);
       return;
   }

   bool test = E.supprimer(id_e);
   if (test)
   {
       QMessageBox msgBox(QMessageBox::Information, QObject::tr("OK"),
                          QObject::tr("Suppression effectuée. \nClick Cancel to exit."), QMessageBox::Cancel, this);

       // Connect the finished signal to a custom slot
       connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

       msgBox.exec();

       ui->tab_employes->setModel(E.afficher());

   }
   else
   {
       QMessageBox msgBox(QMessageBox::Critical, QObject::tr("Erreur"),
                          QObject::tr("Suppression non effectuée. \nClick Cancel to exit."), QMessageBox::Cancel, this);

       // Connect the finished signal to a custom slot
       connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

       msgBox.exec();
   }
}

void MainWin::on_pb_modifier_2_clicked()
{
   // Confirm with the user before proceeding with modification
   QMessageBox msgBox;
   msgBox.setText("Confirmation");
   msgBox.setInformativeText("Are you sure you want to modify?");
   msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   msgBox.setDefaultButton(QMessageBox::Cancel);

   int reply = msgBox.exec();
   if (reply == QMessageBox::Cancel) {
       return;  // Exit the function if the user cancels
   }

   // Check if the employee is selected
   if (selectedEmployeeRow == -1) {
       QMessageBox::warning(this, tr("Aucun employé sélectionné"),
                            tr("Veuillez sélectionner un employé à modifier."), QMessageBox::Ok);
       return;
   }

   // Retrieve data from the UI fields
   int id_e = ui->id_e->text().toInt();
   int cin_e = ui->cin_e->text().toInt();
   QString nom_e = ui->nom_e->text();
   QString poste_e = ui->poste_e->text();
   int telephone_e = ui->telephone_e->text().toInt();
   QString email_e = ui->email_e->text();
   QString datee_e = ui->datee_e->text();
   int salaire_e = ui->salaire_e->text().toInt();
   QString user_e = ui->user_e->text();
   QString pas_e = ui->pas_e->text();

   // Check for missing fields
   if (nom_e.isEmpty() || email_e.isEmpty() || poste_e.isEmpty() || telephone_e == 0 || datee_e.isEmpty() ||
       salaire_e == 0 || user_e.isEmpty() || pas_e.isEmpty()) {
       QMessageBox::warning(this, tr("Champs manquants"),
                            tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
       return;
   }

   // Create the employe object with the data from the form
   employe E(id_e, cin_e, nom_e, poste_e, telephone_e, email_e, datee_e, salaire_e, user_e, pas_e);

   // Modify employee using the correct id_e
   bool test = E.modifierE(id_e);  // Make sure modifierE() method is implemented correctly to update the employee in DB

   if (test) {
       // Update the table with new data
       ui->tab_employes->setModel(E.afficher());

       QMessageBox msgBox(QMessageBox::Information, QObject::tr("OK"),
                          QObject::tr("Modification effectuée. \nClick Cancel to exit."), QMessageBox::Cancel, this);

       // Connect the finished signal to a custom slot
       connect(&msgBox, &QMessageBox::finished, this, &MainWin::handleMessageBoxResult);

       msgBox.exec();
   } else {
       // Handle the case where modification fails
       QMessageBox msgBox(QMessageBox::Critical, QObject::tr("Erreur"),
                          QObject::tr("Employe non trouvé ou modification non effectuée\nClick Cancel to exit."), QMessageBox::Cancel, this);

       // Connect the finished signal to a custom slot
       connect(&msgBox, SIGNAL(finished(int)), this, SLOT(handleMessageBoxResult(int)));

       msgBox.exec();
   }
}
void MainWin::handleMessageBoxResult(int result)
{
   if (result == QMessageBox::Cancel)
   {
       // User clicked Cancel, do something if needed
   }
}
void MainWin::handleMessageBoxResult2()
{
   // Your slot implementation here
}

void MainWin::on_tableViewSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
   Q_UNUSED(previous);


   selectedEmployeeRow = current.row();


    ui->id_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 0).data().toString());
   ui->cin_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 1).data().toString());
   ui->nom_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 2).data().toString());
   ui->poste_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 3).data().toString());
   ui->telephone_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 4).data().toString());
   ui->email_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 5).data().toString());
   ui->datee_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 6).data().toString());
   ui->salaire_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 7).data().toString());
   ui->user_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 8).data().toString());
   ui->pas_e->setText(ui->tab_employes->model()->index(selectedEmployeeRow, 9).data().toString());



}

void MainWin::on_logoutpb_clicked()
{

}










void MainWin::on_radioButton_clicked()
{
   QString idFilter = ui->lineEdit->text().trimmed(); // Get the filter text for id_e

   // Get the original model (query result)
   QSqlQueryModel *originalModel = E->afficher();
   int numRows = originalModel->rowCount();
   int numCols = originalModel->columnCount();

   // Create a new standard item model to hold filtered data
   QStandardItemModel *filteredModel = new QStandardItemModel(this);

   // Set the horizontal header labels
   QStringList headerLabels;
   for (int col = 0; col < numCols; ++col) {
       headerLabels << originalModel->headerData(col, Qt::Horizontal).toString();
   }
   filteredModel->setHorizontalHeaderLabels(headerLabels);

   // Iterate over the rows of the original model to filter data
   for (int row = 0; row < numRows; ++row) {
       QModelIndex index = originalModel->index(row, 0); // Column 0 for 'id_e'
       QString data = originalModel->data(index).toString();
       if (data.contains(idFilter, Qt::CaseInsensitive)) { // Case-insensitive filter
           QList<QStandardItem *> items;
           for (int col = 0; col < numCols; ++col) {
               items.append(new QStandardItem(originalModel->data(originalModel->index(row, col)).toString()));
           }
           filteredModel->appendRow(items);
       }
   }

   // Set the filtered model to the table view
   ui->tab_employes->setModel(filteredModel);
}
void MainWin::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
   QString idFilter = ui->lineEdit->text().trimmed(); // Get the filter text for id_e

   // Get the original model
   QSqlQueryModel *originalModel = E->afficher();
   int numRows = originalModel->rowCount();
   int numCols = originalModel->columnCount();

   // Create a new standard item model to hold filtered data
   QStandardItemModel *filteredModel = new QStandardItemModel(this);

   // Set the horizontal header labels
   QStringList headerLabels;
   for (int col = 0; col < numCols; ++col) {
       headerLabels << originalModel->headerData(col, Qt::Horizontal).toString();
   }
   filteredModel->setHorizontalHeaderLabels(headerLabels);

   // Iterate over the rows of the original model to filter data
   for (int row = 0; row < numRows; ++row) {
       QModelIndex index = originalModel->index(row, 0); // Column 0 for 'id_e'
       QString data = originalModel->data(index).toString();
       if (data.contains(idFilter, Qt::CaseInsensitive)) { // Case-insensitive filter
           QList<QStandardItem *> items;
           for (int col = 0; col < numCols; ++col) {
               items.append(new QStandardItem(originalModel->data(originalModel->index(row, col)).toString()));
           }
           filteredModel->appendRow(items);
       }
   }

   // Set the filtered model to the table view
   ui->tab_employes->setModel(filteredModel);
}
void MainWin::on_sortAscButton_clicked()
{
   QSqlQueryModel *sortedModel = E->sortByDateAsc();
   ui->tab_employes->setModel(sortedModel);
}

void MainWin::on_sortDescButton_clicked()
{
   QSqlQueryModel *sortedModel = E->sortByDateDesc();
   ui->tab_employes->setModel(sortedModel);
}




void MainWin::on_pb_pdf_clicked()
{
   QPrinter printer(QPrinter::PrinterResolution);
   printer.setOutputFormat(QPrinter::PdfFormat);
   QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", QString(), "PDF Files (*.pdf)");
   if (fileName.isEmpty()) {
       return; // Exit if the user cancels the file dialog
   }
   printer.setOutputFileName(fileName);

   // Create a painter
   QPainter painter;
   if (!painter.begin(&printer)) {
       QMessageBox::warning(this, "Warning", "Failed to open PDF file for writing.");
       return;
   }

   // Get the model from the table view
   QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tab_employes->model());
   if (!model) {
       QMessageBox::warning(this, "Warning", "Failed to retrieve table model.");
       return;
   }

   // Set up font and metrics
   QFont font = painter.font();
   font.setPointSize(10); // Adjust as needed
   painter.setFont(font);
   QFontMetrics fontMetrics(font);

   // Define some margins
   const int margin = 20;
   int yPosition = margin; // Start drawing content from the top

   // Draw table header
   int columnCount = model->columnCount();
   int rowHeight = fontMetrics.height() + 5;
   int tableWidth = printer.pageRect().width() - 2 * margin;

   // Draw header cells with lines
   int columnWidth = tableWidth / columnCount; // Adjust column width dynamically
   for (int column = 0; column < columnCount; ++column) {
       // Draw header text
       painter.drawText(margin + column * columnWidth, yPosition, model->headerData(column, Qt::Horizontal).toString());

       // Draw vertical line to separate columns (after the header)
       painter.drawLine(margin + (column + 1) * columnWidth, yPosition - rowHeight, margin + (column + 1) * columnWidth, yPosition + rowHeight);
   }

   yPosition += rowHeight; // Move down after header

   // Draw horizontal line under the header
   painter.drawLine(margin, yPosition, printer.pageRect().width() - margin, yPosition);

   // Draw table rows with lines
   int rowCount = model->rowCount();
   for (int row = 0; row < rowCount; ++row) {
       for (int column = 0; column < columnCount; ++column) {
           QString text = model->data(model->index(row, column)).toString();
           // Draw cell text
           painter.drawText(margin + column * columnWidth, yPosition, text);

           // Draw vertical line to separate columns (after the cell)
           painter.drawLine(margin + (column + 1) * columnWidth, yPosition - rowHeight, margin + (column + 1) * columnWidth, yPosition + rowHeight);
       }

       yPosition += rowHeight; // Move down after each row

       // Draw horizontal line after each row to separate them
       painter.drawLine(margin, yPosition, printer.pageRect().width() - margin, yPosition);

       // Handle page break if content exceeds page height
       if (yPosition > printer.pageRect().bottom()) {
           printer.newPage(); // Create a new page
           yPosition = margin; // Reset y position for the new page
       }
   }

   // Finish painting
   painter.end();

   // Show success message
   QMessageBox::information(this, "PDF Created", "PDF file has been successfully created.");
}


void MainWin::Stat_emp2(){

        qDebug() << "Stat_emp2 function called.";

        // Clear the existing layout if any
        QLayout *layout = ui->lab_chart_bar->layout();
        if (layout) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        }

        // Create the chart
        QChart *chart = new QChart();
        chart->setTitle("Répartition par expérience de travail");

        // Create the Pie Series
        QPieSeries *series = new QPieSeries();
        series->setHoleSize(0.35);

        // Get current year
        int currentYear = QDate::currentDate().year();

        // Fetch the datee_e from the database for each employee
        QSqlQuery query;
        query.prepare("SELECT datee_e FROM employe");
        if (!query.exec()) {
            qDebug() << "Query execution failed: " << query.lastError().text();
            return;
        }

        // Initialize counters for each category
        int experience1Year = 0;
        int experience2Years = 0;
        int experience3Years = 0;
        int experience5PlusYears = 0;

        // Process each employee's data
        while (query.next()) {
            QString datee_e = query.value("datee_e").toString();
            int employmentYear = datee_e.left(4).toInt();

            // Calculate experience
            int experience = currentYear - employmentYear;

            // Categorize based on experience
            if (experience <= 1) {
                experience1Year++;
            } else if (experience <= 3) {
                experience2Years++;
            } else if (experience <= 5) {
                experience3Years++;
            } else {
                experience5PlusYears++;
            }
        }

        // Add categorized data to the Pie chart
        if (experience1Year > 0) {
            series->append("1 Year", experience1Year);
        }
        if (experience2Years > 0) {
            series->append("2 Years", experience2Years);
        }
        if (experience3Years > 0) {
            series->append("3 Years", experience3Years);
        }
        if (experience5PlusYears > 0) {
            series->append("5+ Years", experience5PlusYears);
        }

        // Add series to the chart
        chart->addSeries(series);

        // Create the chart view
        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        // Add the chart view to the existing layout
        layout = ui->lab_chart_bar->layout();
        if (!layout) {
            layout = new QVBoxLayout(ui->lab_chart_bar);
            ui->lab_chart_bar->setLayout(layout);
        }
        layout->addWidget(chartView);

        // Resize the chart view
        chartView->resize(ui->lab_chart_bar->size());

        // Debug statement
        qDebug() << "Chart added to layout.";
    }

void MainWin::on_refrech_pb_clicked()
{
   // Clear the line edits
   ui->cin_e->clear();
   ui->nom_e->clear();
   ui->poste_e->clear();
   ui->telephone_e->clear();
    ui->email_e->clear();
   ui->datee_e->clear();
   ui->salaire_e->clear();
   ui->user_e->clear();
   ui->pas_e->clear();

   // Reset selectedEmployeeRow to -1
   selectedEmployeeRow = -1;

   // Optionally, re-enable the signal-slot connection for handling selection changes
   connect(ui->tab_employes->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
           this, SLOT(on_tableViewSelectionChanged(QModelIndex, QModelIndex)));
}

void MainWin::on_pushButton_clicked()
{
   // Create a new dialog to display the charts
   QDialog *dialog = new QDialog(this);
   QVBoxLayout *layout = new QVBoxLayout(dialog);




   // Call Stat_emp2() to display the second statistic
   Stat_emp2();


   layout->addWidget(ui->lab_chart_bar);


   dialog->setWindowTitle("Employee Statistics");
   dialog->resize(800, 600); // Adjust size as needed

   // Show the dialog
   dialog->exec();
}

void MainWin::on_statrole_clicked()
{

}



void MainWin::on_Imprimer_clicked() {
   // Retrieve the data from tab_employe
   QAbstractItemModel *model = ui->tab_employes->model();
   if (!model)
       return;

   // Prepare the data for printing
   QString printData;
   int rows = model->rowCount();
   int columns = model->columnCount();
   for (int row = 0; row < rows; ++row) {
       for (int col = 0; col < columns; ++col) {
           QModelIndex index = model->index(row, col);
           QVariant data = model->data(index);
           printData.append(data.toString());
           printData.append("\t"); // Add tab as separator
       }
       printData.append("\n"); // Add newline after each row
   }


   QPrinter printer;
   QPrintDialog dialog(&printer, this);
   if (dialog.exec() == QDialog::Accepted) {
       QPainter painter(&printer);
       painter.drawText(100, 100, printData); // Adjust position as needed
   }
}


