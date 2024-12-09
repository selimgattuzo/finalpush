#include "employe.h"
#include <QSqlQuery>
#include <QtDebug>
#include<QObject>
#include <QIntValidator>
#include<QSqlError>
#include<QString>
#include<QComboBox>
#include <QCoreApplication>
employe::employe()
{
     id_e =0 ;
     cin_e =0 ;
     telephone_e=0 ;
     nom_e="";
     poste_e="";
     email_e="";
     datee_e="";
      salaire_e = 0;
     user_e ="";
     pas_e ="";


    }
     employe::employe (int id_e, int  cin_e,QString  nom_e ,QString  poste_e,int telephone_e,QString email_e,QString datee_e,int salaire_e,QString user_e,QString pas_e)
      {
         this->id_e = id_e ;
        this->cin_e = cin_e ;
      this->nom_e=nom_e ;
         this->poste_e=poste_e ;
          this-> telephone_e =  telephone_e ;
            this-> email_e =  email_e ;
         this->datee_e = datee_e;
         this->salaire_e = salaire_e;
         this->user_e = user_e;
         this->pas_e = pas_e;
      }
     int employe::getid_e(){return id_e ;}
    int employe::getcin_e(){return cin_e ;}
    QString employe::getnom_e(){return nom_e ;}
    QString employe::getposte_e(){return poste_e ;}
    int employe::gettelephone_e(){return telephone_e ;}
    QString employe::getemail_e(){return email_e ;}
    QString employe::getdatee_e(){return datee_e ;}
     int employe::getsalaire_e(){return salaire_e ;}
     QString employe::getuser_e(){return user_e ;}
     QString employe::getpas_e(){return pas_e ;}


     void employe:: setid_e(int id_e ){this->id_e =id_e ;}
    void employe:: setcin_e(int cin_e ){this->cin_e =cin_e ;}
    void employe:: setnom_e(QString nom_e ){this->nom_e =nom_e ;}
  void employe:: setposte_e(QString poste_e ){this->poste_e =poste_e ;}
 void employe:: settelephone_e(int telephone_e ){this->telephone_e =telephone_e ;}
 void employe:: setemail_e(QString email_e ){this->email_e =email_e ;}
void employe:: setdatee_e(QString datee_e ){this->datee_e =datee_e ;}
  void employe:: setsalaire_e(int salaire_e ){this->salaire_e =salaire_e ;}
  void employe:: setuser_e(QString user_e ){this->user_e =user_e ;}
  void employe:: setpas_e(QString pas_e ){this->pas_e =pas_e ;}

  bool employe::ajouter()
  {
      QSqlQuery query;
      QString id_string = QString::number(id_e);


      query.prepare("INSERT INTO employe (id_e, cin_e, nom_e, poste_e, telephone_e,email_e , datee_e, salaire_e, user_e, pas_e) "
                    "VALUES (:id_e, :cin_e, :nom_e, :poste_e, :telephone_e, :email_e,  :datee_e, :salaire_e, :user_e, :pas_e)");

      query.bindValue(":id_e", id_string);
      query.bindValue(":cin_e", cin_e);
      query.bindValue(":nom_e", nom_e);
      query.bindValue(":poste_e", poste_e);
      query.bindValue(":telephone_e", telephone_e);
      query.bindValue(":email_e", email_e);
      query.bindValue(":datee_e", datee_e);
      query.bindValue(":salaire_e", salaire_e);
      query.bindValue(":user_e", user_e);
      query.bindValue(":pas_e", pas_e);

      return query.exec();
  }

    QSqlQueryModel* employe::afficher()
    {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("select * from employe");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr(" nom "));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr(" poste"));
         model->setHeaderData(4, Qt::Horizontal, QObject::tr("num_telephone"));
         model->setHeaderData(5, Qt::Horizontal, QObject::tr(" email "));
         model->setHeaderData(6, Qt::Horizontal, QObject::tr(" date d'embauche "));
         model->setHeaderData(7, Qt::Horizontal, QObject::tr(" salaire_e"));
         model->setHeaderData(8, Qt::Horizontal, QObject::tr(" user_e"));
         model->setHeaderData(9, Qt::Horizontal, QObject::tr(" pas_e"));
        return model;
    }
    bool employe::supprimer(int id_e)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM employe WHERE id_e = :id_e");
        query.bindValue(":id_e", id_e);
        return query.exec();
    }
    bool employe::checkIfemployeExists(int id_e)
    {
        QSqlQuery query;
        query.prepare("SELECT id_e FROM employe where id_e=:id_e");
        query.bindValue(":id_e", id_e);
        if (query.exec() && query.next()) {
            return true;
        } else {
            return false;
        }
    }
    bool employe::modifierE(int id_e)
    {
        QSqlQuery query;

        // Prepare the SQL query with placeholders for all fields, including email_e
        query.prepare("UPDATE employe SET "
                      "cin_e=:cin_e, nom_e=:nom_e, poste_e=:poste_e, "
                      "telephone_e=:telephone_e, email_e=:email_e, datee_e=:datee_e, salaire_e=:salaire_e, "
                      "user_e=:user_e, pas_e=:pas_e "
                      "WHERE id_e=:id_e");

        // Bind the values for each placeholder
        query.bindValue(":id_e", id_e);  // Directly bind the integer id_e
        query.bindValue(":cin_e", cin_e); // Bind cin_e (ensure it's an integer)
        query.bindValue(":nom_e", nom_e); // Bind nom_e (ensure it's a QString)
        query.bindValue(":poste_e", poste_e); // Bind poste_e (ensure it's a QString)
        query.bindValue(":telephone_e", telephone_e); // Bind telephone_e
        query.bindValue(":email_e", email_e); // Bind email_e
        query.bindValue(":datee_e", datee_e); // Bind datee_e
        query.bindValue(":salaire_e", salaire_e); // Bind salaire_e
        query.bindValue(":user_e", user_e); // Bind user_e (ensure it's a QString)
        query.bindValue(":pas_e", pas_e); // Bind pas_e (ensure it's a QString)

        // Execute the query
        if (!query.exec()) {
            qDebug() << "Update failed:" << query.lastError().text();
            return false;  // Return false if query execution fails
        }

        return true;  // Return true if query is successful
    }

    QSqlQueryModel* employe::sortByDateAsc()
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM employe ORDER BY DATEE_E ASC");
        model->setHeaderData(5, Qt::Horizontal, QCoreApplication::translate("employe", "DATEE_E"));
        return model;
    }

    QSqlQueryModel* employe::sortByDateDesc()
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM employe ORDER BY DATEE_E DESC");
        model->setHeaderData(5, Qt::Horizontal, QCoreApplication::translate("employe", "DATEE_E"));
        return model;
    }


