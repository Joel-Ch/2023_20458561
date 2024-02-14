#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"
#include "ModelPart.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handleButton1();
    void handleButton2();
    void handleTreeClicked();

signals:
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    void on_actionOpen_File_triggered();

private:
    void receiveDialogData(const QString& name, const bool& visible, const QColor& colour);


    Ui::MainWindow *ui;
    ModelPartList* partList;
};
#endif // MAINWINDOW_H
