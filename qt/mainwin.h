#ifndef MAINWIN_H
#define MAINWIN_H

#include "client.h"
#include "smtp.h"
#include "arduino.h"
#include "commande.h"
#include <QDialog>
#include <QDate>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "employe.h"
#include <QDialog>
namespace Ui {
class MainWin;  // Updated the class name to match the new header file name
}

class MainWin :public QDialog
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);
    ~MainWin();
    QString readRFID();
    void effacer();
    void on_stat_clicked();
    void afficher_arduino_com();
    QSqlQueryModel *sortBySalaireAsc();
    QSqlQueryModel *sortBySalaireDesc();
       Ui::MainWin *ui;
private slots:
    //client
    void on_ajouter_clicked(); // Add client
    void on_supprimer_clicked(); // Delete client
    void on_modifier_clicked(); // Modify client
    void on_trier_clicked(); // Sort clients
    void on_chercher_clicked(); // Search for a client
    void on_exporter_clicked(); // Export data
    void on_mail_clicked(); // Send email
    void updateCalendarWithBirthdays(); // Update calendar with birthdays
    void on_calendarWidget_clicked(const QDate &date); // Calendar widget click handler
    void on_client_clicked();
    //commande
    void on_ajouter_com_clicked();
    void on_supprimer_com_clicked();
    void on_modifier_com_clicked();
    void on_exporter_com_clicked();
    void on_accepte_com_clicked();
    void on_exit_com_clicked();
    void on_annuler_com_clicked();
    void on_accepte_2_com_clicked();
    void on_tri_com_clicked();
    void on_search_com_textChanged(const QString &arg1);
    void on_stat_com_clicked();
    void on_retourne_com_clicked();
    void on_med_moins_cmd_clicked();
    void on_quest_clicked();
    void on_med_plus_rapporte_clicked();
    void on_delai_moy_clicked();
    void on_meilleur_client_clicked();
    void on_revenue_annee_clicked();
    void on_commande_annulees_clicked();
    void on_retourne_2_com_clicked();
    void on_cmnd_clicked();

    void on_employee_clicked();
    void on_pb_supprimer_clicked();

    void on_pb_modifier_2_clicked();

    void on_pb_ajouter_clicked();


    void handleMessageBoxResult(int result);

    void on_logoutpb_clicked();


    void on_tableViewSelectionChanged(const QModelIndex &current, const QModelIndex &previous);


    void on_radioButton_clicked();


    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);
    void on_sortAscButton_clicked();
        void on_sortDescButton_clicked();









        void on_pb_pdf_clicked();



void Stat_emp2();

void on_refrech_pb_clicked();

void on_pushButton_clicked();

void on_statrole_clicked();

void on_Imprimer_clicked();
void handleMessageBoxResult2();



private:

    client cl; // Client object
    Smtp s; // SMTP object for sending emails
    arduino a; // Arduino object for hardware communication
    QByteArray data,rfid;
    Commande c;
    QString id;
    float montant=0.0f;
    QTextBrowser *chat;
    employe *E;
      int selectedEmployeeRow;
         QSortFilterProxyModel *proxyModel;
         bool showLabel;

};

#endif // MAINWIN_H
