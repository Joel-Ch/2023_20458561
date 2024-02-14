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
QT_END_NAMESPACE /**< End of the Qt namespace. */

/**
 * @brief The MainWindow class represents the main window of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow object.
     * @param parent The parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow object.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Handles the button1 click event.
     */
    void handleButton1();

    /**
     * @brief Handles the button2 click event.
     */
    void handleButton2();

    /**
     * @brief Handles the tree clicked event.
     */
    void handleTreeClicked();

signals:
    /**
     * @brief Emits a status update message.
     * @param message The message to be displayed.
     * @param timeout The timeout duration in milliseconds.
     */
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    /**
     * @brief Handles the actionOpen_File triggered event.
     */
    void on_actionOpen_File_triggered();

private:
    /**
     * @brief Receives dialog data.
     * @param name The name data.
     * @param visible The visibility data.
     * @param colour The color data.
     */
    void receiveDialogData(const QString& name, const bool& visible, const QColor& colour);

    Ui::MainWindow *ui;
    ModelPartList* partList;
};
#endif // MAINWINDOW_H
