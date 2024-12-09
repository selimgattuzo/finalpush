/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QLabel *label_7;
    QLabel *label_5;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLineEdit *lineEdituserr;
    QLabel *label_4;
    QLineEdit *lineEditpasswordd;
    QLabel *label_3;
    QPushButton *forgot_pb;
    QPushButton *pushButtonlog;
    QWidget *page_2;
    QLineEdit *rcpt;
    QPushButton *sendBtn;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_8;
    QWidget *page_3;
    QLineEdit *codebt;
    QLabel *label_9;
    QPushButton *verify_pb;
    QLabel *label_10;
    QWidget *page_4;
    QLabel *label_11;
    QLineEdit *pas2;
    QLineEdit *pas1;
    QPushButton *cont_pb;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_6;

    void setupUi(QDialog *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1375, 845);
        label_7 = new QLabel(MainWindow);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(-20, 170, 651, 391));
        label_7->setStyleSheet(QStringLiteral("border-image: url(:/img/img/LOG-removebg-preview.png);"));
        label_5 = new QLabel(MainWindow);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(-60, 20, 1391, 811));
        label_5->setStyleSheet(QStringLiteral("border-image: url(:/img/img/61803.jpg);"));
        stackedWidget = new QStackedWidget(MainWindow);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(700, 10, 631, 811));
        stackedWidget->setStyleSheet(QLatin1String("*{/* styles.qss */\n"
"	border-image: url(:/img/img/61803.jpg);\n"
"}\n"
"/* Set the default font color to dark for readability */\n"
"* {\n"
"    color: #2c3e50; /* Dark gray text for better contrast */\n"
"    font-family: Arial, sans-serif; /* Clean, readable font */\n"
"}\n"
"\n"
"/* Style for the main window background */\n"
"QWidget {\n"
"    background-color: #D6EAF8; /* Light blue background */\n"
"}\n"
"\n"
"/* Style for input boxes (assuming they are QLineEdit or similar) */\n"
"QLineEdit {\n"
"    background-color: #ffffff; /* White background for input fields */\n"
"    border: 1px solid #2c3e50; /* Dark gray border */\n"
"    border-radius: 8px; /* Rounded corners */\n"
"    padding: 8px;\n"
"    color: #2c3e50; /* Dark text */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Style for input boxes (focused state) */\n"
"QLineEdit:focus {\n"
"    border: 1px solid #2980b9; /* Blue border on focus */\n"
"    background-color: #eaf2f7; /* Light blue background on focus */\n"
"}\n"
"\n"
"/* Style for push b"
                        "uttons (assuming they are QPushButtons or similar) */\n"
"QPushButton {\n"
"    background-color: #2980b9; /* Medium blue for buttons */\n"
"    border: 1px solid #1f618d; /* Darker blue border for buttons */\n"
"    border-radius: 10px; /* Rounded corners */\n"
"    padding: 10px 20px; /* Adjusted padding */\n"
"    color: white; /* White text */\n"
"    font-size: 14px; /* Slightly larger font for readability */\n"
"}\n"
"\n"
"/* Style for push buttons on hover */\n"
"QPushButton:hover {\n"
"    background-color: #1f618d; /* Darker blue on hover */\n"
"}\n"
"\n"
"/* Style for push buttons when pressed */\n"
"QPushButton:pressed {\n"
"    background-color: #154360; /* Even darker blue when pressed */\n"
"}\n"
"\n"
"/* Style for QStackedWidget */\n"
"QStackedWidget {\n"
"    background-color: #ffffff; /* White background */\n"
"    border: 2px solid #BDC3C7; /* Light gray border */\n"
"    border-radius: 10px;\n"
"\n"
"}\n"
"\n"
"/* Style for dialog windows */\n"
"QDialog {\n"
"    background-color: #ffffff; /"
                        "* White background for dialogs */\n"
"    border: 1px solid #2980b9; /* Blue border for dialogs */\n"
"    border-radius: 12px; /* Rounded corners */\n"
"    padding: 20px;\n"
"    color: #2c3e50; /* Dark text for readability */\n"
"}\n"
"\n"
"/* Style for labels */\n"
"QLabel {\n"
"    color: #2c3e50; /* Dark gray text for labels */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Style for combo boxes (QComboBox) */\n"
"QComboBox {\n"
"    background-color: #ffffff; /* White background for combo boxes */\n"
"    border: 1px solid #2c3e50;\n"
"    border-radius: 8px;\n"
"    padding: 8px;\n"
"    color: #2c3e50;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Style for combo boxes when focused */\n"
"QComboBox:focus {\n"
"    border: 1px solid #2980b9;\n"
"    background-color: #eaf2f7;\n"
"}\n"
"\n"
"/* Style for table views (QTableView) */\n"
"QTableView {\n"
"    background-color: #ffffff; /* White background for tables */\n"
"    border: 1px solid #ddd; /* Light border */\n"
"    gridline-color: #ddd; /* Light grid "
                        "lines */\n"
"}\n"
"\n"
"QTableView QHeaderView {\n"
"    background-color: #2980b9; /* Blue header background */\n"
"    color: white; /* White text for table header */\n"
"}\n"
"\n"
"/* Style for table rows */\n"
"QTableView::item {\n"
"    padding: 10px;\n"
"    border-bottom: 1px solid #ddd; /* Divider between rows */\n"
"}\n"
"\n"
"/* Style for table rows when hovered */\n"
"QTableView::item:hover {\n"
"    background-color: #eaf4f0; /* Very light blue hover effect */\n"
"}\n"
"\n"
"/* Style for scrollbars */\n"
"QScrollBar {\n"
"    background-color: #f1f1f1; /* Light background for scrollbars */\n"
"    border: 1px solid #ddd; /* Light gray border */\n"
"    width: 10px; /* Narrow scrollbar */\n"
"}\n"
"\n"
"/* Style for the vertical scrollbar */\n"
"QScrollBar:vertical {\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* Style for the horizontal scrollbar */\n"
"QScrollBar:horizontal {\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"/* Style for progress bars */\n"
"QProgressBar {\n"
"    background-color: #f1f8f"
                        "5; /* Very light green background */\n"
"    border: 1px solid #BDC3C7;\n"
"    border-radius: 5px;\n"
"    text-align: center;\n"
"    color: #2c3e50; /* Dark text */\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color: #2980b9; /* Medium blue progress chunk */\n"
"    border-radius: 5px;\n"
"}\n"
""));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        lineEdituserr = new QLineEdit(page);
        lineEdituserr->setObjectName(QStringLiteral("lineEdituserr"));
        lineEdituserr->setGeometry(QRect(220, 270, 231, 31));
        label_4 = new QLabel(page);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(110, 260, 81, 31));
        label_4->setStyleSheet(QStringLiteral("image: url(:/img/img/user_747376.png);"));
        lineEditpasswordd = new QLineEdit(page);
        lineEditpasswordd->setObjectName(QStringLiteral("lineEditpasswordd"));
        lineEditpasswordd->setGeometry(QRect(220, 340, 231, 31));
        lineEditpasswordd->setEchoMode(QLineEdit::Password);
        label_3 = new QLabel(page);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(100, 340, 81, 31));
        label_3->setStyleSheet(QStringLiteral("image: url(:/img/img/padlock.png);"));
        forgot_pb = new QPushButton(page);
        forgot_pb->setObjectName(QStringLiteral("forgot_pb"));
        forgot_pb->setGeometry(QRect(300, 440, 141, 41));
        pushButtonlog = new QPushButton(page);
        pushButtonlog->setObjectName(QStringLiteral("pushButtonlog"));
        pushButtonlog->setGeometry(QRect(120, 440, 121, 41));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        rcpt = new QLineEdit(page_2);
        rcpt->setObjectName(QStringLiteral("rcpt"));
        rcpt->setGeometry(QRect(197, 211, 261, 36));
        sendBtn = new QPushButton(page_2);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));
        sendBtn->setGeometry(QRect(250, 290, 93, 28));
        label = new QLabel(page_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 110, 481, 51));
        label->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 16px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        label_2 = new QLabel(page_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(121, 211, 70, 30));
        label_2->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 16px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        label_8 = new QLabel(page_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(170, 20, 261, 41));
        label_8->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 30px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        codebt = new QLineEdit(page_3);
        codebt->setObjectName(QStringLiteral("codebt"));
        codebt->setGeometry(QRect(180, 200, 281, 31));
        label_9 = new QLabel(page_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(220, 10, 191, 71));
        label_9->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 30px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        verify_pb = new QPushButton(page_3);
        verify_pb->setObjectName(QStringLiteral("verify_pb"));
        verify_pb->setGeometry(QRect(230, 280, 211, 61));
        label_10 = new QLabel(page_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(90, 110, 481, 71));
        label_10->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 16px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        label_11 = new QLabel(page_4);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(120, 30, 341, 71));
        label_11->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 30px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        pas2 = new QLineEdit(page_4);
        pas2->setObjectName(QStringLiteral("pas2"));
        pas2->setGeometry(QRect(220, 200, 231, 31));
        pas2->setEchoMode(QLineEdit::Password);
        pas1 = new QLineEdit(page_4);
        pas1->setObjectName(QStringLiteral("pas1"));
        pas1->setGeometry(QRect(220, 130, 231, 31));
        cont_pb = new QPushButton(page_4);
        cont_pb->setObjectName(QStringLiteral("cont_pb"));
        cont_pb->setGeometry(QRect(250, 260, 141, 31));
        label_12 = new QLabel(page_4);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(80, 120, 121, 51));
        label_12->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 15px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        label_13 = new QLabel(page_4);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(80, 200, 131, 31));
        label_13->setStyleSheet(QLatin1String("QLabel {\n"
"    color: black; /* Text color for labels */\n"
"    font-size: 15px; /* Adjust font size as needed */\n"
"    font-weight: bold; /* Make the text bold if desired */\n"
"    margin: 5px; /* Adjust margin as needed */\n"
"}"));
        stackedWidget->addWidget(page_4);
        label_6 = new QLabel(MainWindow);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(260, 230, 281, 201));
        label_6->setStyleSheet(QStringLiteral("border-image: url(:/img/img/LOG-removebg-preview.png);"));

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QDialog *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Dialog", Q_NULLPTR));
        label_7->setText(QString());
        label_5->setText(QString());
        label_4->setText(QString());
        label_3->setText(QString());
        forgot_pb->setText(QApplication::translate("MainWindow", "forgot password", Q_NULLPTR));
        pushButtonlog->setText(QApplication::translate("MainWindow", "Log in", Q_NULLPTR));
        sendBtn->setText(QApplication::translate("MainWindow", "Continue", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:10pt;\">Enter your email associated with your account and<br/>we'll send an emal to reset your password</span></p></body></html>", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Email ", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Reset password", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "Verification", Q_NULLPTR));
        verify_pb->setText(QApplication::translate("MainWindow", "Verify and Proceed", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:8pt;\">Please enter the 4 digit code that send to your email adresse</span></p></body></html>", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "Enter New Password", Q_NULLPTR));
        cont_pb->setText(QApplication::translate("MainWindow", "Continue", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Old Password", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "New Password", Q_NULLPTR));
        label_6->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
