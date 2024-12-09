#ifndef EMPLOYE_H
#define EMPLOYE_H
#include<QString>
#include<QDate>
#include<QSqlQueryModel>


class employe
{
public:
    employe();
    employe(int,int,QString,QString,int,QString,QString,int,QString,QString);
    QSqlQueryModel* sortByDateAsc();
      QSqlQueryModel* sortByDateDesc();
int getid_e();
int getcin_e();
QString getnom_e();
QString getposte_e();
int gettelephone_e();
QString getemail_e();
QString getdatee_e();
int getsalaire_e();
QString getuser_e();
QString getpas_e();

void setid_e(int);
void setcin_e(int);
void setnom_e(QString);
void setposte_e(QString);
void settelephone_e(int);
void setemail_e(QString);
void setdatee_e(QString);
void setsalaire_e(int);
void setuser_e(QString);
void setpas_e(QString);
bool ajouter();
QSqlQueryModel* afficher ();
bool supprimer (int);
bool modifierE(int);
 bool checkIfemployeExists(int);

private:
    int id_e,cin_e , telephone_e,salaire_e ;
    QString nom_e ,email_e, poste_e,datee_e,user_e,pas_e ;

};

#endif // EMPLOYE_H
