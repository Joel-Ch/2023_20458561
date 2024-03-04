﻿#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->addAction(ui->actionItem_Options);

    // connections
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    connect(ui->actionItem_Options, &QAction::triggered, this, &MainWindow::on_actionItem_Options_triggered);

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
        QColor colour(255, 255, 255);

        /* Create child item */
        ModelPart *childItem = new ModelPart({name, visible, colour});

        /* Append to tree top-level */
        rootItem->appendChild(childItem);

        /* Add 5 sub-items */
        for (int j = 0; j < 5; j++)
        {
            QString name = QString("Item %1, %2").arg(i).arg(j);
            QString visible("true");
            QColor colour(255, 255, 255);

            ModelPart *childChildItem = new ModelPart({name, visible, colour});

            /* Append to parent */
            childItem->appendChild(childChildItem);
        }
    }

    /* Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    /* Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* Create an object and add to renderer (this will change later to display a CAD model) */
    /* Will just copy and paste cylinder example from before */
    // This creates a polygonal cylinder model with eight circumferential facets
    // (i.e., in practice an octagonal prism).
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // The mapper is responsible for pushing the geometry into the graphics
    // library. It may also do color mapping, if scalars or other attributes are
    // defined.
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // The actor is a grouping mechanism: besides the geometry (mapper), it
    // also has a property, transformation matrix, and/or texture map.
    // Here we set its color and rotate it around the X and Y axes.
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    /* Reset Camera (probably needs to go in its own function that is called whenever
    model is changed) */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString name = selectedPart->data(0).toString();
    bool visible = selectedPart->data(1).toBool();
    QString colour = selectedPart->data(2).toString();



    // This causes MainWindow to emit the signal that will then be received by the statusbar’s slot
    emit statusUpdateMessage(name + QString::number(visible) + colour, 0);
}

void MainWindow::handleButton2()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString name = selectedPart->getName();
    bool visible = selectedPart->visible();
    QString colour = selectedPart->getColour().name();

    // This causes MainWindow to emit the signal that will then be received by the statusbar’s slot
	emit statusUpdateMessage(name + QString::number(visible) + colour, 0);
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
    // Open a file dialog
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));

    emit statusUpdateMessage(QString("File Opened: ") + fileName,0);
    // Rename the selected item
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    selectedPart->setName(fileName);

}


// Slot to receive the dialog data
void MainWindow::receiveDialogData(const QString& name, const bool& visible, const QColor& colour) {

    // Display the data in the status bar
    emit statusUpdateMessage(QString("Colour: R%1 G%2 B%3")
        .arg(colour.red())
        .arg(colour.green())
        .arg(colour.blue()) + 
        QString(" Name: ") + name + 
        QString(" Visible? ") + QString::number(visible), 0);

    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    selectedPart->setName(name);
    selectedPart->setVisible(visible);
    selectedPart->setColour(colour);
    
}

void MainWindow::on_actionItem_Options_triggered()
{
    // Disconnect the action's signal - otherwise it goes twice
    disconnect(ui->actionItem_Options, &QAction::triggered, this, &MainWindow::on_actionItem_Options_triggered);

    // Status Bar Message
    emit statusUpdateMessage(QString("Item Options"),0);

    // Get the selected item
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Open the dialog with the selected item's data
    openDialog(selectedPart->getName(), selectedPart->visible(), selectedPart->getColour());

    // Reconnect the action's signal
    connect(ui->actionItem_Options, &QAction::triggered, this, &MainWindow::on_actionItem_Options_triggered);
}




void MainWindow::openDialog(const QString& name, const bool& isVisible, const QColor& colour)
{
    // Create a dialog object
    Dialog _dialog(this);

    // Connect the dialog's signal to the MainWindow's slot
    connect(&_dialog, &Dialog::sendingData, this, &MainWindow::receiveDialogData);

    // Set the dialog's initial values
    _dialog.setInitialValues(name, isVisible, colour);

    // Show the dialog
    _dialog.exec();
}