#include "commande.h"
#include <QSqlError>
#include "connection.h"


bool Commande::ajouter() {
    QSqlQuery query;
    int newOrderNumber = 1;
       query.prepare("SELECT MAX(TO_NUMBER(REGEXP_SUBSTR(ID, '[0-9]+'))) FROM SP_COMMANDE WHERE id LIKE 'c%'");
    if (query.exec() && query.next()) {
               newOrderNumber = query.value(0).toInt() + 1;
       }
    QSqlQuery query1;
    query1.prepare("SELECT NOMBRE FROM SP_COMMANDE WHERE CIN_C = :cin_c");
    query1.bindValue(":cin_c", cin_c);
    int maxnombre = 0;

    if (query1.exec()) {

            while (query1.next()) {
                int nombre = query1.value(0).toInt();
                if (nombre > maxnombre) {
                    maxnombre = nombre;
                }
            }

        }
    int newnombre = maxnombre + 1;

    QString year = QString::number(QDate::currentDate().year());
    QString idFormatted = QString("c%1 #%2").arg(newOrderNumber).arg(year);
    setID(idFormatted);
    query.prepare("INSERT INTO SP_COMMANDE (ID, CIN_C, CLIENT, DATE_COM,DELAI, ETAT, PRIORITE, NOMBRE, MONTANT) "
                  "VALUES (:id, :cin_c, :client, :date_com, :delai, :etat, :priorite, :nombre, :montant)");

    query.bindValue(":id", id);
    query.bindValue(":cin_c", QString::number(cin_c));
    query.bindValue(":client", client);
    query.bindValue(":date_com", date_com);
    query.bindValue(":delai", QString::number(delai));
    query.bindValue(":etat", etat);
    query.bindValue(":priorite", priorite);
    query.bindValue(":nombre", QString::number(newnombre));
    query.bindValue(":montant", montant);

    if (!query.exec()) {
        qDebug() << "Error inserting into SP_COMMANDE:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel * Commande::afficher(){
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("SELECT * FROM SP_COMMANDE");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("CIN_C"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("CLIENT"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("DATE_COM"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("DELAI"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("ETAT"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("PRIORITE"));
    model->setHeaderData(7,Qt::Horizontal,QObject::tr("NOMBRE"));
    model->setHeaderData(8,Qt::Horizontal,QObject::tr("MONTANT"));
    return model;
}
bool Commande::supprimer(QString id){
    QSqlQuery query;
    QSqlQuery query1;
    query1.prepare("delete from CONTENIR where ID_C= :id");
    query1.bindValue(":id",id);
    if (!query1.exec()) {
            qDebug() << "error:" << query1.lastError().text();
            return false;
        }
    query.prepare("delete from SP_COMMANDE where ID= :id");
    query.bindValue(":id",id);
    if (!query.exec()) {
            qDebug() << "error:" << query.lastError().text();
            return false;
        }
    return true;
}

void Commande::load(QTableWidget* tableWidget){
    QSqlQueryModel* model = afficher();

        // Debug: Print first data point
        qDebug() << model->data(model->index(0, 0));

        QStandardItemModel* model2 = new QStandardItemModel();
        for (int i = 0; i < model->rowCount(); i++) {
            for (int j = 0; j < model->columnCount(); j++) {
                model2->setItem(i, j, new QStandardItem(model->data(model->index(i, j)).toString()));
            }
        }

        // Set row and column counts for the table widget
        int rowCount = model2->rowCount();
        int columnCount = model2->columnCount();
        tableWidget->setRowCount(rowCount);
        tableWidget->setColumnCount(columnCount);

        // Populate the QTableWidget with data
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < columnCount; ++col) {
                QString data = model2->data(model2->index(row, col)).toString();
                tableWidget->setItem(row, col, new QTableWidgetItem(data));
            }
        }
}

void Commande::onAccepteClicked_contenir(QTableWidget* tableau, QLabel* montantTot, QComboBox* idMed, QSpinBox* quantSpin, float& montant) {
    int medId = idMed->currentText().toInt();
    int quantite = quantSpin->value();

    if (quantite <= 0) {
        QMessageBox::warning(nullptr, "Invalid Input", "La quantité doit être supérieure à zéro.");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT PRIX, LIBELLE, CATEGORIE, QUANTITE FROM SP_MEDICAMENT WHERE ID = :medId");
    query.bindValue(":medId", medId);

    if (!query.exec()) {
        qDebug() << "Failed to execute query: " << query.lastError().text();
        return;
    }

    if (query.next()) {
        float prix = query.value("PRIX").toFloat();
        QString libelle = query.value("LIBELLE").toString();
        QString categorie = query.value("CATEGORIE").toString();
        int stockQuantity = query.value("QUANTITE").toInt();

        // Step 5: Check stock availability
        if (stockQuantity < quantite) {
            QMessageBox::information(nullptr, "Stock Insufficient",
                                     QString("Le médicament %1 est en rupture de stock.").arg(libelle));
            return;
        }
        float itemTotal = prix * quantite;
        bool itemExists = false;

        for (int row = 0; row < tableau->rowCount(); ++row) {
            QString existingLibelle = tableau->item(row, 0)->text();
            if (existingLibelle == libelle) {
                int currentQuantity = tableau->item(row, 2)->text().toInt();
                int newQuantity = currentQuantity + quantite;
                tableau->setItem(row, 2, new QTableWidgetItem(QString::number(newQuantity)));

                float newItemTotal = prix * newQuantity;
                tableau->setItem(row, 3, new QTableWidgetItem(QString::number(newItemTotal)));

                montant += itemTotal;
                montantTot->setText(QString("%1 TND").arg(montant));

                QSqlQuery updateQuery;
                updateQuery.prepare("UPDATE CONTENIR SET QUANTITE = :quantity, PRIX = :prix "
                                    "WHERE ID_C = :orderId AND ID_MED = :medId");
                updateQuery.bindValue(":quantity", newQuantity);
                updateQuery.bindValue(":prix", newItemTotal);
                updateQuery.bindValue(":orderId", getID()); // Assuming c.getID() gets the order ID
                updateQuery.bindValue(":medId", medId);

                if (!updateQuery.exec()) {
                    qDebug() << "Update failed: " << updateQuery.lastError().text();
                }

                itemExists = true;
                break;
            }
        }

        if (!itemExists) {
            int row = tableau->rowCount();
            tableau->insertRow(row);
            tableau->setItem(row, 0, new QTableWidgetItem(libelle));
            tableau->setItem(row, 1, new QTableWidgetItem(categorie));
            tableau->setItem(row, 2, new QTableWidgetItem(QString::number(quantite)));
            tableau->setItem(row, 3, new QTableWidgetItem(QString::number(itemTotal)));

            // Update the total amount
            montant += itemTotal;
            montantTot->setText(QString("%1 TND").arg(montant));

            // Insert a new entry into the database
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO CONTENIR (ID_C, ID_MED, LIBELLE, CATEGORIE, QUANTITE, PRIX) "
                                "VALUES (:orderId, :medId, :libelle, :categorie, :quantity, :prix)");
            insertQuery.bindValue(":orderId", getID()); // Assuming c.getID() gets the order ID
            insertQuery.bindValue(":medId", medId);
            insertQuery.bindValue(":libelle", libelle);
            insertQuery.bindValue(":categorie", categorie);
            insertQuery.bindValue(":quantity", quantite);
            insertQuery.bindValue(":prix", itemTotal);

            if (!insertQuery.exec()) {
                qDebug() << "Insert failed: " << insertQuery.lastError().text();
            }
        }

        int finalStockQuantity = stockQuantity - quantite;
        QSqlQuery stockUpdateQuery;
        stockUpdateQuery.prepare("UPDATE SP_MEDICAMENT SET QUANTITE = :newQuantite WHERE ID = :medId");
        stockUpdateQuery.bindValue(":newQuantite", finalStockQuantity);
        stockUpdateQuery.bindValue(":medId", medId);

        if (!stockUpdateQuery.exec()) {
            qDebug() << "Stock update failed: " << stockUpdateQuery.lastError().text();
        }
    } else {
        QMessageBox::warning(nullptr, "Medication Not Found", "Le médicament sélectionné n'existe pas.");
    }
}

void Commande::onExitClicked_contenir(QTabWidget* tabWidget,QTableWidget* table,QTableWidget* tableau, QLabel* montantTot, float& montant,QLineEdit* suppmodif) {
    QSqlQuery query;
    query.prepare("UPDATE SP_COMMANDE SET MONTANT = :totalPrice WHERE id = :orderId");
    query.bindValue(":totalPrice", montant);
    query.bindValue(":orderId", getID());

    if (query.exec()) {
        tabWidget->setCurrentIndex(0);

        tableau->clearContents();
        tableau->setRowCount(0);
        montantTot->setText(QString("0 TND"));
        load(table);

        QMessageBox::information(nullptr, "Commande ajoutée", "La commande a été bien ajoutée.");

        updateCompleter(suppmodif);
        tabWidget->setTabEnabled(1, false);
        montant = 0.0f;
    } else {
        QMessageBox::warning(nullptr, "Erreur", "Échec de la mise à jour du montant dans la base de données.");
    }
}

void Commande::updateCompleter(QLineEdit* suppmodif) {
    QStringList idList;
    QSqlQuery query("SELECT ID FROM SP_COMMANDE");
    while (query.next()) {
        idList << query.value(0).toString();
    }
    QCompleter *completer = new QCompleter(idList, suppmodif);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchStartsWith);

    QListView *popup = qobject_cast<QListView *>(completer->popup());
    if (popup) {
        popup->setStyleSheet("QListView { background-color: rgb(169, 228, 255); color: black; }");
    }
    suppmodif->setCompleter(completer);
}
void Commande::onsupprimerclicked(QLineEdit* suppmodif, QLabel* suppmodifmsg, QTableWidget* table){
    QString id = suppmodif->text();
       QSqlQuery chercher;
       chercher.prepare("SELECT ID FROM SP_COMMANDE WHERE ID = :id");
       chercher.bindValue(":id", id);
       QMessageBox::StandardButton reply;

       if (chercher.exec() && chercher.next()) {
           reply = QMessageBox::question(nullptr, "Confirme suppression",
                                         "Êtes-vous sûr de vouloir supprimer cette commande?",
                                         QMessageBox::Yes | QMessageBox::No);
           if(reply == QMessageBox::Yes){
               // Assuming `supprimer` is a method of `Commande` to delete the order
               if (supprimer(id)) {
                   suppmodifmsg->setText(QString("Commande d'ID %1 a été supprimée").arg(id));
                   load(table);  // Reload the table with updated data
                   updateCompleter(suppmodif);  // Update the completer for the ID input field
               }
               else {
                   suppmodifmsg->setText("Erreur lors de la suppression.");
               }
           }
           else {
               suppmodifmsg->setText("Suppression annulée.");
           }
       } else {
           suppmodifmsg->setText(QString("ID %1 n'existe pas").arg(id));
       }

       QTimer::singleShot(2500, suppmodifmsg, &QLabel::clear);
}

void Commande::loadcommande(QString id, QComboBox* clientmodif, QDateTimeEdit* datemodif, QLineEdit* delaimodif, QComboBox* etatmodif, QComboBox* prioritemodif, QLineEdit* montantmodif){
    QSqlQuery query;
        query.prepare("SELECT CIN_C, CLIENT, DATE_COM, DELAI, ETAT, PRIORITE, MONTANT FROM SP_COMMANDE WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec() && query.next()) {
            // Populate the UI components with the data from the query result
            QString clientinfo = query.value(1).toString() + " #" + query.value(0).toString();
            clientmodif->setCurrentText(clientinfo);
            datemodif->setDate(query.value(2).toDate());
            delaimodif->setText(QString::number(query.value(3).toInt()));
            etatmodif->setCurrentText(query.value(4).toString());
            prioritemodif->setCurrentText(query.value(5).toString());
            montantmodif->setText(QString::number(query.value(6).toFloat()));
        }
}

void Commande::onAccepteClicked_modif(QTabWidget* tabWidget,QTableWidget* table,QLineEdit* suppmodif, QComboBox* clientmodif, QLineEdit* montantmodif, QComboBox* prioritemodif, QLineEdit* delaimodif, QComboBox* etatmodif,QDateTimeEdit* datemodif, QString id){
    int newnombre;
        QString fullText = clientmodif->currentText();
        QStringList parts = fullText.split('#');
        QString cin_c;
        QString client;
        bool ok;
        int cin;

        if (parts.size() > 1) {
            cin_c = parts[1].trimmed();
            client = parts[0].trimmed();
        }

        cin = cin_c.toInt(&ok);

        QSqlQuery query1;
        query1.prepare("SELECT NOMBRE FROM SP_COMMANDE WHERE CIN_C = :cin_c");
        query1.bindValue(":cin_c", cin);

        int maxnombre = 0;
        if (query1.exec()) {
            while (query1.next()) {
                int nombre = query1.value(0).toInt();
                if (nombre > maxnombre) {
                    maxnombre = nombre;
                }
            }
        }

        newnombre = maxnombre + 1;

        float montant = montantmodif->text().toFloat();
        QString montantfinal = QString::number(montant, 'f', 2);
        float mt = montantfinal.toFloat();

        QSqlQuery query;
        query.prepare("UPDATE SP_COMMANDE SET CIN_C = :cin_c, CLIENT = :client, PRIORITE = :priorite, DATE_COM = :date, DELAI = :delai, ETAT = :etat, NOMBRE = :nombre, MONTANT = :montant WHERE id = :id");
        query.bindValue(":id", id);
        query.bindValue(":cin_c", cin);
        query.bindValue(":client", client);
        query.bindValue(":priorite", prioritemodif->currentText());
        query.bindValue(":date", datemodif->date());
        query.bindValue(":delai", delaimodif->text().toInt());
        query.bindValue(":etat", etatmodif->currentText());
        query.bindValue(":nombre", newnombre);
        query.bindValue(":montant", mt);
        if (query.exec()) {
            tabWidget->setTabEnabled(2, false);
            tabWidget->setCurrentIndex(0);
            load(table);
            updateCompleter(suppmodif);
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to update the command.");
        }
}

void Commande::onModifierClicked(QTabWidget* tabWidget,QString* id,QLineEdit* suppmodif,QLabel* suppmodifmsg, QComboBox* clientmodif, QDateTimeEdit* datemodif, QLineEdit* delaimodif, QComboBox* etatmodif, QComboBox* prioritemodif, QLineEdit* montantmodif){
    *id=suppmodif->text();
    QSqlQuery chercher;
    chercher.prepare("SELECT COUNT(*) FROM SP_COMMANDE WHERE ID = :id");
    chercher.bindValue(":id", *id);
    if(chercher.exec() && chercher.next()){
        int n=chercher.value(0).toInt();
        if(n==0){
            suppmodifmsg->setText(QString("ID %1 n'existe pas").arg(*id));
            QTimer::singleShot(2500, suppmodifmsg, &QLabel::clear);

        }
        else{
            tabWidget->setTabEnabled(2, true);
            tabWidget->setCurrentIndex(2);
            loadcommande(*id, clientmodif, datemodif, delaimodif, etatmodif, prioritemodif, montantmodif);
        }
    }
}
void Commande::onExporterClicked(){
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "*.pdf");
    if (fileName.isEmpty())
        return;

    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName.append(".pdf");
    }
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(30, 30, 30, 30));

    QPainter painter(&writer);

    QFont titleFont("Arial", 18, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(writer.width() / 2 - 100, 100, "COMMANDE");

    QFont contentFont("Arial", 10);
    painter.setFont(contentFont);

    int y = 200;
    int x=30;
    QSqlQuery query;
    query.prepare("SELECT ID, CIN_C, CLIENT, DATE_COM, DELAI, ETAT, PRIORITE, NOMBRE, MONTANT FROM SP_COMMANDE");
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Failed to retrieve orders from the database.");
        qDebug() << "Query execution error:" << query.lastError().text();
        return;
    }
    painter.drawLine(50, y, writer.width() - 50, y);
    y += 200;
    painter.drawText(x, y, "ID");
    x+=1000;
    painter.drawText(x, y, "CIN");
     x+=1000;
    painter.drawText(x, y, "NOM");
     x+=1000;
    painter.drawText(x, y, "DATE");
     x+=1000;
    painter.drawText(x, y, "PRIORITE");
     x+=1000;
    painter.drawText(x, y, "ETAT");
     x+=1000;
    painter.drawText(x, y, "NOMBRE");
     x+=1000;
    painter.drawText(x, y, "MONTANT");

    y += 100;
    painter.drawLine(10, y, writer.width() - 50, y);
    y += 200;
    while (query.next()) {
        QString orderId = query.value("ID").toString();
        QString cin_c = query.value("CIN_C").toString();
        QString clientName = query.value("CLIENT").toString();
        QString orderDate = query.value("DATE_COM").toDate().toString("yyyy-MM-dd");
        QString priority = query.value("PRIORITE").toString();
        QString status = query.value("ETAT").toString();
        int quantity = query.value("NOMBRE").toInt();
        float amount = query.value("MONTANT").toFloat();
        if (y > writer.height() - 100) {
            writer.newPage();
            y = 100;
        }
        x=30;
        painter.drawText(x, y, orderId);
         x+=1000;
        painter.drawText(x, y, cin_c);
         x+=1000;
        painter.drawText(x, y, clientName);
         x+=1000;
        painter.drawText(x, y, orderDate);
         x+=1000;
        painter.drawText(x, y, priority);
         x+=1000;
        painter.drawText(x, y, status);
         x+=1000;
        painter.drawText(x, y, QString::number(quantity));
         x+=1000;
        painter.drawText(x, y, QString("%1 TND").arg(QString::number(amount, 'f', 2)));

        y += 100;
        painter.drawLine(10, y, writer.width() - 50, y);
        y += 150;
    }
    painter.end();
    QMessageBox::information(nullptr, "Export Successful", "All orders have been successfully exported to PDF.");
}
void Commande::onTriClicked(QTableWidget* table,QComboBox* par){
    QSqlQuery query;
           if(par->currentText()=="PAR ETAT"){
           query.prepare(
               "SELECT ID, CIN_C, CLIENT, DATE_COM, DELAI, ETAT, PRIORITE, NOMBRE, MONTANT "
               "FROM SP_COMMANDE "
               "ORDER BY CASE "  "   WHEN ETAT = 'en cours' THEN 1 " "   WHEN ETAT = 'livre' THEN 2 "   "   WHEN ETAT = 'annuler' THEN 3 "
               "   ELSE 4 END"
           );
           }
           else if(par->currentText()=="PAR MONTANT"){
               query.prepare(
                       "SELECT ID, CIN_C, CLIENT, DATE_COM, DELAI, ETAT, PRIORITE, NOMBRE, MONTANT "
                       "FROM SP_COMMANDE "
                       "ORDER BY MONTANT DESC"
                   );

           }
           else if(par->currentText()=="PAR DATE"){
               query.prepare(
                       "SELECT ID, CIN_C, CLIENT, DATE_COM, DELAI, ETAT, PRIORITE, NOMBRE, MONTANT "
                       "FROM SP_COMMANDE "
                       "ORDER BY DATE_COM DESC"
                   );

           }

           if (!query.exec()) {
               QMessageBox::warning(nullptr, "Error", "Failed to retrieve sorted orders from the database.");
               qDebug() << "Query execution error:" << query.lastError().text();
               return;
           }

           table->clearContents();
           table->setRowCount(0);
           int row = 0;
           while (query.next()) {
               table->insertRow(row);
               table->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
               table->setItem(row, 1, new QTableWidgetItem(query.value("CIN_C").toString()));
               table->setItem(row, 2, new QTableWidgetItem(query.value("CLIENT").toString()));
               table->setItem(row, 3, new QTableWidgetItem(query.value("DATE_COM").toDate().toString("yyyy-MM-dd")));
               table->setItem(row, 4, new QTableWidgetItem(QString::number(query.value("DELAI").toInt())));
               table->setItem(row, 6, new QTableWidgetItem(query.value("PRIORITE").toString()));
               table->setItem(row, 5, new QTableWidgetItem(query.value("ETAT").toString()));
               table->setItem(row, 7, new QTableWidgetItem(QString::number(query.value("NOMBRE").toInt())));
               table->setItem(row, 8, new QTableWidgetItem(QString::number(query.value("MONTANT").toFloat(), 'f', 2)));

               row++;
           }
           QMessageBox::information(nullptr, "Trié",QString( "Les commandes ont été triées %1").arg(par->currentText()));
}

void Commande::onRechercheClicked(QTableWidget* table,QLineEdit* search){
    QString searchText = search->text();
        if (searchText.isEmpty()) {
            search->setPlaceholderText("RECHERCHE...");

                for (int row = 0; row < table->rowCount(); ++row) {
                    table->setRowHidden(row, false);
                }
            } else {
                for (int row = 0; row < table->rowCount(); ++row) {
                    bool matchFound = false;

                    QTableWidgetItem *item = table->item(row, 0);
                    QTableWidgetItem *item1 = table->item(row, 1);
                    QTableWidgetItem *item2 = table->item(row, 2);
                    QTableWidgetItem *item3 = table->item(row, 3);
                    QTableWidgetItem *item4 = table->item(row, 5);
                    QTableWidgetItem *item5 = table->item(row, 6);

                    if ((item && item->text().contains(searchText, Qt::CaseInsensitive)) ||
                        (item1 && item1->text().contains(searchText, Qt::CaseInsensitive)) ||
                        (item2 && item2->text().contains(searchText, Qt::CaseInsensitive)) ||
                        (item3 && item3->text().contains(searchText, Qt::CaseInsensitive)) ||
                        (item4 && item4->text().contains(searchText, Qt::CaseInsensitive)) ||
                        (item5 && item5->text().contains(searchText, Qt::CaseInsensitive))) {
                        matchFound = true;
                    }

                    table->setRowHidden(row, !matchFound);
                }
            }
}
void Commande::statnbr(QVBoxLayout *charts){
    QSqlQuery query;
    int count1 = 0, count2 = 0, count3 = 0;

    query.prepare("SELECT DISTINCT cin_c FROM SP_COMMANDE");
    if (!query.exec()) {
        qWarning() << "Error fetching clients";
        return;
    }

    while (query.next()) {
        QString cin = query.value(0).toString(); // Client CIN (ID)

        int maxNombre = 0;
        QSqlQuery subQuery;
        subQuery.prepare("SELECT MAX(Nombre) FROM SP_COMMANDE WHERE cin_c = :cin");
        subQuery.bindValue(":cin", cin);

        if (subQuery.exec() && subQuery.next()) {
            maxNombre = subQuery.value(0).toInt();
        }
        if (maxNombre == 1) {
            count1++;
        } else if (maxNombre == 2) {
            count2++;
        } else if (maxNombre >= 3) {
            count3++;
        }
    }

    int total = count1 + count2 + count3;
    if (total == 0) return;

    QPieSeries *series = new QPieSeries();

    QPieSlice *slice1 = series->append("Nombre = 1", count1 * 100.0 / total);
        QPieSlice *slice2 = series->append("Nombre = 2", count2 * 100.0 / total);
        QPieSlice *slice3 = series->append("Nombre >= 3", count3 * 100.0 / total);

        slice1->setColor(Qt::blue);
        slice2->setColor(Qt::green);
        slice3->setColor(Qt::red);

        series->setLabelsVisible(true);
        for (auto slice : series->slices()) {
            double percentage = (slice->percentage() * 100);
            QString label = QString("%1: %2%")
                                        .arg(slice->label())
                                        .arg(QString::number(percentage, 'f', 2));
                    slice->setLabel(label);
        }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pourcentage de Nombre de Commandes par Client");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    charts->addWidget(chartView);
}
void Commande::statannee(QVBoxLayout* annee){
     QSqlQuery query;
     QBarSeries *series = new QBarSeries();
     QStringList categories;

     for (int year = 2024; year <= 2026; ++year) {
         int count = 0;
         query.prepare("SELECT COUNT(*) FROM SP_COMMANDE WHERE EXTRACT(YEAR FROM DATE_COM) = :year");
         query.bindValue(":year", year);
         if (query.exec() && query.next()) {
             count = query.value(0).toInt();
         }

         QBarSet *set = new QBarSet(QString::number(year));
         *set << count;
         series->append(set);
         categories << QString::number(year);
     }

     QChart *chart = new QChart();
     chart->addSeries(series);
     chart->setTitle("Nombre de Commandes par Année");

     QBarCategoryAxis *axisX = new QBarCategoryAxis();
     axisX->append(categories);
     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);

     QValueAxis *axisY = new QValueAxis();
     axisY->setRange(0, 100);
     chart->addAxis(axisY, Qt::AlignLeft);
     series->attachAxis(axisY);

     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);

     annee->addWidget(chartView);

}
void Commande::comboboxcom(QComboBox* combomed,QComboBox* comboclient){
    combomed->clear();
    QSqlQuery query;
    query.prepare("SELECT id FROM SP_MEDICAMENT");
    if(query.exec()){
        while (query.next()){
            QString id = query.value(0).toString(); // Get the id from the query result
            combomed->addItem(id);
        }
    }

    comboclient->clear();
    QSqlQuery query2;
    query2.prepare("SELECT cin_c,nom_c FROM SP_CLIENT");
    if(query2.exec()){
        while(query2.next()){
        QString cin = query2.value(0).toString();
        QString name = query2.value(1).toString();   // Get the CIN (cin_c)

       QString nameWithCin = name + "#" + cin;
       comboclient->addItem(nameWithCin);
        }
    }else {
        qDebug() << "Failed to execute query for SP_CLIENT:" << query2.lastError().text();
    }
}


int Commande::question_annuler(){
    QSqlQuery query;
        query.prepare("SELECT COUNT(*) AS commandes_annulee FROM SP_COMMANDE WHERE ETAT = 'annuler'");

        if (query.exec() && query.next()) {
            return query.value(0).toInt();
        } else {
            qDebug() << "Erreur dans la requête SQL:" << query.lastError().text();
            return -1;
        }
}

float Commande::question_rev_annee(){
    QSqlQuery query;

    query.prepare("SELECT SUM(montant) AS total_annee FROM SP_COMMANDE WHERE EXTRACT(YEAR FROM Date_com) = EXTRACT(YEAR FROM SYSDATE); ");
    if (query.exec()) {
            if (query.next()) {
                return query.value(0).toFloat();
            }
   }
    else{
        qDebug() << "Erreur dans la requête SQL:" << query.lastError().text();
        return -1;

    }
}
int Commande::question_meill_client(int &cin_c,double &totmontant){
    QSqlQuery query;
        query.prepare(
            "SELECT cin_c, total_montant "
            "FROM ( "
            "   SELECT cin_c, SUM(montant) AS total_montant "
            "   FROM SP_COMMANDE "
            "   GROUP BY cin_c "
            "   ORDER BY total_montant DESC "
            ") "
            "WHERE ROWNUM = 1");

        if (query.exec() && query.next()) {
            cin_c = query.value(0).toInt();
            totmontant = query.value(1).toFloat();
            return 0;
        } else {
            qDebug() << "Erreur dans la requête SQL:" << query.lastError().text();
            return -1;
        }
}

double Commande::question_average_delai(){
    QSqlQuery query;
    query.prepare("SELECT AVG(delai) AS avrg_delai FROM SP_COMMANDE WHERE delai >0");
    if(query.exec()&& query.next()){
        return(query.value(0).toFloat());
    }
}

int Commande::question_med_moins(QString &med,int &totquant){
    QSqlQuery query;
    query.prepare("SELECT LIBELLE, total_quantity "
                  "FROM ("
                  "   SELECT LIBELLE, SUM(QUANTITE) AS total_quantity "
                  "   FROM CONTENIR "
                  "   GROUP BY LIBELLE "
                  "   ORDER BY total_quantity ASC "
                  ") "
                  "WHERE ROWNUM = 1");
    if (query.exec() && query.next()) {
          med = query.value(0).toString();
          totquant = query.value(1).toInt();
          return 0;
    }
    else{
        return -1;
    }
}

int Commande::question_med_plus(QString &med,double &prixtot){
    QSqlQuery query;
    query.prepare("SELECT LIBELLE, total_revenue "
                  "FROM ("
                  "   SELECT LIBELLE, SUM(PRIX) AS total_revenue "
                  "   FROM CONTENIR "
                  "   GROUP BY LIBELLE "
                  "   ORDER BY total_revenue DESC "
                  ") "
                  "WHERE ROWNUM = 1");
    if (query.exec()) {
            if (query.next()) {
                med = query.value(0).toString();
                prixtot = query.value(1).toFloat();
                return 0;
            }
    }
    else{
        return -1;
    }
}



