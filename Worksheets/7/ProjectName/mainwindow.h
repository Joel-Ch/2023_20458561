#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"
#include "ModelPart.h"
#include "dialog.h"
#include <QMessageBox>
#include <qfiledialog.h>
#include <vtkCylinderSource.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include <vtkCamera.h>
#include <vtkProperty.h>

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
    /**
     * @brief Signal emitted when the dialog data is ready to be sent.
     * @param name The name sent back.
     * @param visible The visibility state.
     * @param colour The selected color.
     */
    void sendingDialogData(const QString& name, const bool& visible, const QColor& colour);

private slots:
    /**
     * @brief Handles the actionOpen_File triggered event.
     */
    void on_actionOpen_File_triggered();

    /**
    * @brief Tree right click event.
    */
    void on_actionItem_Options_triggered();

private:
    /**
     * @brief Receives dialog data.
     * @param name The name data.
     * @param visible The visibility data.
     * @param colour The color data.
     */
    void receiveDialogData(const QString& name, const bool& visible, const QColor& colour);

    /**
    * @brief Opens a dialog
    * @param name The name data
    * @param visible The visibility data
    * @param colour The color data
    */
    void openDialog(const QString& name, const bool& visible, const QColor& colour);

    /**
    * @brief Updates the render window.
    */
    void updateRender();

    /**
    * @brief Updates the render window from the tree.
    * @param index The index of the tree.
    */
    void updateRenderFromTree(const QModelIndex& index);

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;


    Ui::MainWindow *ui;
    ModelPartList* partList;
};
#endif // MAINWINDOW_H
