#ifndef COMMANDE_H
#define COMMANDE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QStandardItem>
#include <QCompleter>
#include <QListView>
#include <QTimer>
#include <QDateTimeEdit>
#include <QPdfWriter>
#include <QPainter>
#include <QTabWidget>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts>

class Commande {
private:
    QString id;
    int cin_c;
    QString client;
    QDateTime date_com;
    int delai;
    QString etat;
    QString priorite;
    int nombre;
    float montant;

public:

    Commande()
        :id(""), cin_c(), client(""), date_com(QDateTime()), delai(0), etat("en cours"), priorite("faible"), nombre(1), montant(0.0f) {}


    Commande(QString id, int cin_c, QString client, QDateTime date_com, int delai, QString etat, QString priorite, int nombre = 1, float montant = 0.0f)
        :id(id),cin_c(cin_c), client(client), date_com(date_com), delai(delai), etat(etat), priorite(priorite), nombre(nombre), montant(montant) {}


    QString getID() const { return id; }
    int getCin() const { return cin_c; }
    QString getClient() const { return client; }
    QDateTime getDateCom() const { return date_com; }
    int getDelai() const { return delai; }
    QString getEtat() const { return etat; }
    QString getPriorite() const { return priorite; }
    int getNombre() const { return nombre; }
    float getMontant() const { return montant; }

    void setID(const QString& newid) { id = newid; }
    void setCin(const int& newcin) { cin_c = newcin; }
    void setClient(const QString& newclient) { client = newclient; }
    void setDateCom(const QDateTime& newdate) { date_com = newdate; }
    void setDelai(int newdelai) { delai = newdelai; }
    void setEtat(const QString& newetat) { etat = newetat; }
    void setPriorite(const QString& newpriorite) { priorite = newpriorite; }
    void setNombre(int newnombre) { nombre = newnombre; }
    void setMontant(float newmontant) { montant = newmontant; }

    bool ajouter();
    bool supprimer(QString);
    bool modifier(QString);
    QSqlQueryModel * afficher();
    void load(QTableWidget* tableWidget);
    void onAccepteClicked_contenir(QTableWidget* tableau, QLabel* montantTot, QComboBox* idMed, QSpinBox* quantSpin, float& montant);
    void onExitClicked_contenir(QTabWidget* tabWidget, QTableWidget* table,QTableWidget* tableau, QLabel* montantTot,float& montant,QLineEdit* suppmodif);
    void updateCompleter(QLineEdit* suppmodif);
    void onsupprimerclicked(QLineEdit* suppmodif, QLabel* suppmodifmsg, QTableWidget* table);
    void loadcommande(QString id, QComboBox* clientmodif, QDateTimeEdit* datemodif, QLineEdit* delaimodif, QComboBox* etatmodif, QComboBox* prioritemodif, QLineEdit* montantmodif);
    void onAccepteClicked_modif(QTabWidget* tabWidget, QTableWidget* table,QLineEdit* suppmodif,QComboBox* clientmodif, QLineEdit* montantmodif, QComboBox* prioritemodif, QLineEdit* delaimodif, QComboBox* etatmodif,QDateTimeEdit* datemodif, QString id);
    void onModifierClicked(QTabWidget* tabWidget,QString* id,QLineEdit* suppmodif,QLabel* suppmodifmsg, QComboBox* clientmodif, QDateTimeEdit* datemodif, QLineEdit* delaimodif, QComboBox* etatmodif, QComboBox* prioritemodif, QLineEdit* montantmodif);
    void onTriClicked(QTableWidget* table,QComboBox* par);
    void onRechercheClicked(QTableWidget* table,QLineEdit* search);
    void statnbr(QVBoxLayout* charts);
    void statannee(QVBoxLayout* annee);
    void onExporterClicked();
    float calculateMontant();
    int question_annuler();
    float question_rev_annee();
    int question_meill_client(int &,double &);
    double question_average_delai();
    int question_med_moins(QString &,int &);
    int question_med_plus(QString &,double &);
    void comboboxcom(QComboBox* combomed,QComboBox* comboclient);


};

#endif
