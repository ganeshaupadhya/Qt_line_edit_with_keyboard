#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QVector>

#define QSTYLE_KEYBOARD_BUTTONS "QPushButton { background-color: #575552;color:white;border: 0px; border-radius:5px;} QPushButton:focus { background-color: #5EA8AD;}"
#define QSTYLE_KEYBOARD_BUTTONS_SPL "QPushButton {background-color: #5EA8AD;color:white;border: 0px; border-radius:5px;} QPushButton:focus { background-color: #575552;}"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(300,300);

    keyboard1 = new keyboard();
    liel = new QMGLineEdit(this);
    liel->setText("dd");
    liel->set_keyboard(keyboard1);
    liel->setGeometry(100,20,100,50);


    QMGLineEdit *liel2 = new QMGLineEdit(this);
    liel2->setText("wdd");
    liel2->set_keyboard(keyboard1);
    liel2->setGeometry(100,100,100,50);
    //liel->setEchoMode(QLineEdit::Password);

    this->setFocus();


}


MainWindow::~MainWindow()
{
    delete keyboard1;
    delete ui;
}

keyboard::keyboard(QWidget *parent)
{
    resize(100,100);
     setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowSystemMenuHint);


    this->setStyleSheet("background-color:  #2F2B26;");
    //this->setStyleSheet("QPushButton { background-color: #575552;color:white;border: 0px; border-radius:5px;} QPushButton:focus { background-color: #5EA8AD;}");


    connection = false;


    alpha_num_grid = new QGridLayout(this);


    int col = 0;
    int row = 0;
    char *qwerty = "1234567890-QWERTYUIOP-ASDFGHJKL-ZXCVBNM-./:";
    int total_keys = strlen(qwerty);

    for(int i= 0;i < total_keys; i++)
    {
        if(qwerty[i] == '-')
        {
            row++;
            col = 0;
            continue;
        }

        if(row == 3 && col==0)
        {
            QPushButton *btn = new QPushButton(this);
            btn->setObjectName("shift");
            connect(btn,SIGNAL(clicked()) , this, SLOT(click_event()));
            btn->setFixedSize(20,30);
            btn->setStyleSheet(QSTYLE_KEYBOARD_BUTTONS_SPL);
            QFont font ;
            font.setPointSize(1);
            btn->setFont(font);
            btn->setIcon(QIcon(":/images/shift.png"));
            alpha_num_grid->addWidget(btn, row, col++,1,1);
            //col++;
        }


        QPushButton *btn = new QPushButton(this);
        btn->setObjectName(QString(qwerty[i]));
        btn->setText(QString(qwerty[i]));
        connect(btn,SIGNAL(clicked()) , this, SLOT(click_event()));
        btn->setFixedSize(20,30);
        btn->setStyleSheet(QSTYLE_KEYBOARD_BUTTONS);
        alpha_num_grid->addWidget(btn, row, col++,1,1);

        if(row == 3 && col==8)
        {
            QPushButton *btn = new QPushButton( this);
            btn->setObjectName("bksp");
            connect(btn,SIGNAL(clicked()) , this, SLOT(click_event()));
            btn->setFixedSize(40,30);
            btn->setStyleSheet(QSTYLE_KEYBOARD_BUTTONS_SPL);
            QFont font ;
            font.setPointSize(1);
            btn->setFont(font);
            btn->setIcon(QIcon(":/images/bkspace.png"));
            //btn->setIconSize(QSize(20, 10));

            alpha_num_grid->addWidget(btn, row, col++,1,2);
            col++;
        }

        if(row == 4 && col > 2)
        {
            {
                QPushButton *btn = new QPushButton(this);
                btn->setObjectName("space");
                connect(btn,SIGNAL(clicked()) , this, SLOT(click_event()));
                btn->setStyleSheet(QSTYLE_KEYBOARD_BUTTONS);
                btn->setFixedSize(120,30);
                alpha_num_grid->addWidget(btn, row, col++,1,5);
            }
            col+=4;

            QPushButton *btn = new QPushButton(this);
            btn->setObjectName("done");
            connect(btn,SIGNAL(clicked()) , this, SLOT(click_event()));
            btn->setFixedSize(40,30);
            QFont font ;
            font.setPointSize(1);
            btn->setFont(font);
            btn->setStyleSheet(QSTYLE_KEYBOARD_BUTTONS_SPL);
             btn->setIcon(QIcon(":/images/done.png"));
            alpha_num_grid->addWidget(btn, row, col++,1,2);
            col++;

        }





    }

    setLayout(alpha_num_grid);


    qDebug() << "kwybiard reated";
}

keyboard::~keyboard()
{

}


QMGLineEdit::QMGLineEdit(QWidget *parent)
 : QLineEdit(parent)
{


}

QMGLineEdit::~QMGLineEdit()
{}

void QMGLineEdit::set_keyboard(keyboard *parent_keyboard)
{
    keyboard1 = parent_keyboard; //NULL;//new keyboard(); //parent_keyboard;

    inactive_timer = new QTimer(this);
    inactive_timer->setInterval(1000);
    inactive_timer->start();
    connect(inactive_timer,SIGNAL(timeout()) , this, SLOT(inactive_counter()));
    connect(this, SIGNAL(im_focussed()) , parent_keyboard, SLOT(who_has_focus_connect_it()));


}

void QMGLineEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);

  setReadOnly(false);
  emit im_focussed();

  keyboard1->show();

}

void QMGLineEdit::focusOutEvent(QFocusEvent *e)
{
    setReadOnly(true);
}

void QMGLineEdit:: update_char(QString entry)
{
        int cursor = this->cursorPosition();

        if(entry=="done")
        {
            this->setReadOnly(true);
            keyboard1->close();
            this->clearFocus();
            QWidget *parent= (QWidget *)this->parent();
            parent->setFocus();
            return;
        }


        QString before_cursor = this->text().mid(0, cursor);
        QString after_cursor = this->text().mid(cursor, this->text().size());

        if(entry == "space")
        {
             entry.clear();
             entry=" ";
        }

        if(entry == "bksp")
        {
             before_cursor.remove(before_cursor.size()-1, 1);
             cursor = before_cursor.size();
        }
        else
        {
            before_cursor.append(entry);
            cursor = cursor+entry.size();
        }
        before_cursor.append(after_cursor);

        this->setText(before_cursor);
        this->setCursorPosition(cursor);
        this->setFocus();
        this->setReadOnly(false);

}


