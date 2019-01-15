#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Display_clicked();

    void on_Check_clicked();

    void on_Delete_clicked();

    void on_Update_clicked();

    void on_ADD_clicked();

private:
    Ui::MainWindow *ui;

    void xml_Function(const QString);
};

#endif // MAINWINDOW_H
