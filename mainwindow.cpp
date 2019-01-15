#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtXml>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //XML中的说明对应QDomProcessingInstruction
    //XML中的元素对应QDomElement类
    //XML中的属性对应QDomAttr类
    //XML中的文本内容对应QDomText类

    QFile file("my.xml");
    if(!file.exists()){

        QDomDocument doc;

        QDomProcessingInstruction instrunction = doc.createProcessingInstruction("xml",\
                                                                                 "version = \"1.0\" encoding = \"UTF-8\"");
        QDomElement root = doc.createElement(QString("书库"));
        QDomElement book = doc.createElement(QString("图书"));
        QDomAttr id = doc.createAttribute(QString("编号"));
        id.setValue(QString("1"));
        book.setAttributeNode(id);

        QDomElement title = doc.createElement(QString("书名"));
        QDomElement author = doc.createElement(QString("作者"));

        QDomText text;
        text = doc.createTextNode(QString("Qt"));
        title.appendChild(text);
        text = doc.createTextNode(QString("jeremy"));
        author.appendChild(text);

        doc.appendChild(instrunction);
        doc.appendChild(root);
        root.appendChild(book);
        book.appendChild(title);
        book.appendChild(author);

        book = doc.createElement(QString("图书"));
        id = doc.createAttribute(QString("编号"));
        title = doc.createElement(QString("书名"));
        author = doc.createElement(QString("作者"));

        id.setValue(QString("2"));
        book.setAttributeNode(id);
        text = doc.createTextNode(QString("Qt5 编程入门"));
        title.appendChild(text);
        text = doc.createTextNode(QString("霍亚飞"));
        author.appendChild(text);

        root.appendChild(book);
        book.appendChild(title);
        book.appendChild(author);

        QFile file("my.xml");
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return;
        QTextStream out(&file);
        out.setCodec("UTF-8");
        doc.save(out, 4, QDomNode::EncodingFromTextStream);
        file.close();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Display_clicked()
{
    ui->listWidget->clear();
    QFile file("my.xml");
    if(!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file)){

        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()){
        if(n.isElement()){
            QDomElement e = n.toElement();
            ui->listWidget->addItem(e.tagName() + e.attribute(QString("编号")));
            QDomNodeList list = e.childNodes();
            for(int i = 0; i < list.count(); i++){
                QDomNode node = list.at(i);
                if(node.isElement())
                    ui->listWidget->addItem("   " + node.toElement().tagName()\
                                            + " : " + node.toElement().text());

            }
        }
        n = n.nextSibling();
    }
}

void MainWindow::on_Check_clicked()
{
    xml_Function("check");
}

void MainWindow::on_Delete_clicked()
{
    xml_Function("delete");
}

void MainWindow::on_Update_clicked()
{
    xml_Function("update");
}

void MainWindow::on_ADD_clicked()
{
    ui->listWidget->clear();
    ui->listWidget->addItem(QString("无法添加!"));
    QFile file("my.xml");
    if(!file.open(QIODevice::ReadOnly))
        return;
    QDomDocument doc;
    if(!doc.setContent(&file)){
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement book = doc.createElement(QString("图书"));
    QDomAttr id = doc.createAttribute(QString("编号"));
    QDomElement title = doc.createElement(QString("书名"));
    QDomElement author = doc.createElement(QString("作者"));

    QString num = root.lastChild().toElement().attribute("编号");
    int count = num.toInt() + 1;
    id.setValue(QString::number(count));
    book.setAttributeNode(id);

    QDomText text;
    text = doc.createTextNode(ui->title->text());
    title.appendChild(text);
    text = doc.createTextNode(ui->author->text());
    author.appendChild(text);

    root.appendChild(book);
    book.appendChild(title);
    book.appendChild(author);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    ui->listWidget->clear();
    ui->listWidget->addItem(QString("添加成功"));
    ui->title->clear();
    ui->author->clear();
}

void MainWindow::xml_Function(const QString operate)
{
    ui->listWidget->clear();

    QFile file("my.xml");
    if(!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file)){
        file.close();
        return;
    }
    file.close();

    QDomNodeList list = doc.elementsByTagName("图书");

    for(int i = 0; i < list.count(); i++){
        QDomElement e = list.at(i).toElement();
        if(e.attribute(QString("编号")) == ui->book->text()){
            if(operate == "delete"){
                QDomElement root = doc.documentElement();
                root.removeChild(list.at(i));

                QFile file("my.xml");
                if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
                    return;
                }
                QTextStream out(&file);
                out.setCodec("UTF-8");
                doc.save(out, 4, QDomNode::EncodingFromTextStream);
                ui->listWidget->clear();
                ui->listWidget->addItem(QString("删除成功"));
            }else if(operate == "update"){
                QDomNodeList child = list.at(i).childNodes();
                child.at(0).toElement().firstChild().setNodeValue(ui->title->text());
                child.at(1).toElement().firstChild().setNodeValue(ui->author->text());

                QFile file("my.xml");
                if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                    return;

                QTextStream out(&file);
                out.setCodec("UTF-8");
                doc.save(out, 4, QDomNode::EncodingFromTextStream);
                file.close();

                ui->listWidget->clear();
                ui->listWidget->addItem(QString("更新成功"));
            }else if(operate == "check"){
                ui->listWidget->clear();
                ui->listWidget->addItem(e.tagName()
                                        + e.attribute(QString("编号")));
                QDomNodeList list = e.childNodes();
                for(int i = 0; i < list.count(); i++){
                    QDomNode node = list.at(i);
                    if(node.isElement()){
                        ui->listWidget->addItem("   "
                                                + node.toElement().tagName() + " : "
                                                + node.toElement().text());
                    }
                }
            }
        }
    }
}






















