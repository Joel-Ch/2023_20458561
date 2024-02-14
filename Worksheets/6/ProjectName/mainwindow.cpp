#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <qfiledialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connections
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    /* Create/allocate the ModelList */
    this->partList = new ModelPartList("Parts List");

    /* Link it to the tree view in the GUI */
    ui->treeView->setModel(this->partList);

    /* Manually create a model tree - there are much better and more flexible ways of doing this,
    e.g. with nested functions. This is just a quick example as a starting point. */
    ModelPart *rootItem = this->partList->getRootItem();

    /* Add 3 top level items */
    for (int i = 0; i < 3; i++)
    {
        /* Create strings for both data columns */
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        /* Create child item */
        ModelPart *childItem = new ModelPart({name, visible});

        /* Append to tree top-level */
        rootItem->appendChild(childItem);

        /* Add 5 sub-items */
        for (int j = 0; j < 5; j++)
        {
            QString name = QString("Item %1, %2").arg(i).arg(j);
            QString visible("true");

            ModelPart *childChildItem = new ModelPart({name, visible});

            /* Append to parent */
            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1()
{
    // This causes MainWindow to emit the signal that will then be received by the statusbar’s slot
    emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
}

void MainWindow::handleButton2()
{
    // Create a dialog object
    Dialog _dialog(this);

    // Connect the dialog's signal to the MainWindow's slot
    connect(&_dialog, &Dialog::sendingDialogData, this, &MainWindow::receiveDialogData);

    // Show the dialog
    _dialog.exec();
}

void MainWindow::handleTreeClicked()
{
    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* In this case, we will retrieve the name string from the internal QVariant data array */
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

// Open a file dialog
void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage(QString("Opening File: "),0);
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));
    emit statusUpdateMessage(QString("File Opened: ") + fileName,0);
}


// Slot to receive the dialog data
void MainWindow::receiveDialogData(const QString& name, const bool& visible, const QColor& colour) {

    // for now we can just display the data in the status bar to show we have received it
    emit statusUpdateMessage(QString("Colour: R%1 G%2 B%3")
        .arg(colour.red())
        .arg(colour.green())
        .arg(colour.blue()) + 
        QString(" Name: ") + name + 
        QString(" Visible? ") + QString::number(visible), 0);
}