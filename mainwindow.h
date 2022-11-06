#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include <QDebug>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class keyboard;

class QMGLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  QMGLineEdit(QWidget *parent = 0);
  ~QMGLineEdit();
keyboard *keyboard1;
 void set_keyboard(keyboard *parent_keyboard);
 QTimer *inactive_timer;

public slots:
  void update_char(QString entry);
  void inactive_counter()
  {
        if(this->hasFocus())
            emit im_focussed();
  }

signals:
  void im_focussed();


protected:
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);
};

class keyboard : public QWidget
{
    Q_OBJECT

private:
    QPushButton *btn_a;


public:
    keyboard(QWidget *parent = nullptr);
    ~keyboard();
    QMGLineEdit *current_focus;
    QMGLineEdit *prev_focus;
    QGridLayout *alpha_num_grid;
    bool connection;
    bool is_upper_case_layout;

public slots:
    void click_event()
    {
         QPushButton* button = qobject_cast<QPushButton*>(sender());


         if(button->objectName() == "shift")
         {
             for(int idx = 0; idx < alpha_num_grid->count(); idx++)
             {
                 QPushButton *abtn = qobject_cast<QPushButton*>(  alpha_num_grid->itemAt(idx)->widget() );


                 if(abtn->objectName().length() > 1)
                     continue;

                 if(abtn->objectName().contains(QRegExp("[a-z]")))
                 {
                     abtn->setObjectName(abtn->objectName().toUpper());

                 }
                 else if(abtn->objectName().contains(QRegExp("[A-Z]")))
                 {
                     abtn->setObjectName(abtn->objectName().toLower());
                 }
                 abtn->setText(abtn->objectName());

             }
             button->clearFocus();
             return;
         }

         emit btn_press(button->objectName());
         button->clearFocus();

    }

    void who_has_focus_connect_it()
    {
        current_focus = qobject_cast<QMGLineEdit*>(sender());

        qDebug()<< current_focus->text();
        if(connection)
        {
            if(current_focus == prev_focus)
                return;
            else
            {
                disconnect(this,SIGNAL(btn_press(QString)), (QObject*)prev_focus, SLOT(update_char(QString)));
                connection=false;
                qDebug() << "Clearing old connection";
            }
        }

        if(!connection)
        {
            connect(this,SIGNAL(btn_press(QString)), (QObject*)current_focus, SLOT(update_char(QString)));
            prev_focus = qobject_cast<QMGLineEdit*>(sender());
            connection=true;
        }
        qDebug() << "connectio set";
    }

signals:
    void btn_press(QString);



};





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    QMGLineEdit *liel;
    keyboard *keyboard1;




private:
    Ui::MainWindow *ui;

public slots:


};



#endif // MAINWINDOW_H
