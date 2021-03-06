#include "tasklist.h"
#include "ui_tasklist.h"
#include<QtSql>
#include<QCheckBox>
#include<QFont>
#include<QTextBrowser>

taskList::taskList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::taskList)
{
    ui->setupUi(this);
    //connect to database
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("E:\\QtProject\\database\\taskDB.db");
    bool db_ok=db.open();
    if(db_ok){
        qDebug()<<"Database Connected";
    }
}

taskList::~taskList()
{
    delete ui;
}

void taskList::setTitle(QString title){
    ui->label->setText(title);
}

void taskList::showUpcomingTask(){

    QDateTime current=QDateTime::currentDateTime();
    QSqlQuery query;
    query.prepare("SELECT * FROM allTask WHERE deadline>:current order by deadline");
    query.bindValue(":current",current);

    qDebug()<<query.exec();

    int taskFieldNo=query.record().indexOf("task");
    int deadlineFieldNo=query.record().indexOf("deadline");
    int indexFieldNo=query.record().indexOf("id");
    int X=20,Y=50,width=0,height=17,inc=0;                  //for checkbox positioning
    //QTextBrowser *taskArea=new QTextBrowser(this);


    while(query.next()){
        QString task=query.value(taskFieldNo).toString();
        if(task.length()>50){
           task.truncate(50);                          //shorten task
        }
        QString deadline=query.value(deadlineFieldNo).toString();
        QString id=query.value(indexFieldNo).toString();
        int sz=task.length()+deadline.length()+id.length()+500;
        width=sz;
        //checkbox
        QCheckBox *check=new QCheckBox(this);
        check->setGeometry(X,Y+inc,width,height);
        check->setFont(QFont("Meiryo UI",9,5,false));
        check->setText(task+deadline);
       check->show();
        inc+=30;
    }
    taskArea->show();
}

void taskList::showExpiredTask()
{
    QDateTime current=QDateTime::currentDateTime();
    QSqlQuery query;
    query.prepare("SELECT * FROM allTask WHERE deadline<=:current order by deadline DESC");
    query.bindValue(":current",current);

    qDebug()<<query.exec();

    int taskFieldNo=query.record().indexOf("task");
    int deadlineFieldNo=query.record().indexOf("deadline");
    int indexFieldNo=query.record().indexOf("id");
    int X=10,Y=50,width=471,height=431;
    QTextBrowser *displayBox=new QTextBrowser(this);
    displayBox->setGeometry(X,Y,width,height);
    displayBox->setFont(QFont("Meiryo UI",9,5,false));
    displayBox->setStyleSheet("QTextBrowser{color:#D24B4B}");

    while(query.next()){
        QString task=query.value(taskFieldNo).toString();
        QString deadline=query.value(deadlineFieldNo).toString();
        QString id=query.value(indexFieldNo).toString();
        QString icon;
        displayBox->append("<strong>"+task+"</strong>");
        displayBox->append("<em>"+deadline+"<\em>");
        displayBox->append("\n");
    }
    displayBox->show();
}
