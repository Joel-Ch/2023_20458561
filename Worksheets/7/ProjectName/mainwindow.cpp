#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Constructors Destructors etc
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->addAction(ui->actionItem_Options);
    ui->treeView->addAction(ui->actionDelete_Item);

    // connections
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    connect(ui->actionItem_Options, &QAction::triggered, this, &MainWindow::on_actionItem_Options_triggered);
    connect(ui->actionDelete_Item, &QAction::triggered, this, &MainWindow::on_actionDelete_Item_triggered);

    /* Create/allocate the ModelList */
    this->partList = new ModelPartList("Parts List");

    /* Link it to the tree view in the GUI */
    ui->treeView->setModel(this->partList);

    // Manually create a model tree - there are much better and more flexible ways of doing this, e.g. with nested functions. This is just a quick example as a starting point.
    //ModelPart *rootItem = this->partList->getRootItem();
    //  /* Add 3 top level items */
    //for (int i = 0; i < 3; i++)
    //{
    //    /* Create strings for both data columns */
    //    QString name = QString("TopLevel %1").arg(i);
    //    QString name = QString("TopLevel");
    //    QString visible("true");
    //    QColor colour(255, 255, 255);
    //    /* Create child item */
    //    ModelPart *childItem = new ModelPart({name, visible, colour});
    //    /* Append to tree top-level */
    //    rootItem->appendChild(childItem);
    //    /* Add 5 sub-items */
    //    for (int j = 0; j < 5; j++)
    //    {
    //        QString name = QString("Item %1, %2").arg(i).arg(j);
    //        QString visible("true");
    //        QColor colour(255, 255, 255);
    //        ModelPart *childChildItem = new ModelPart({name, visible, colour});
    //        /* Append to parent */
    //        childItem->appendChild(childChildItem);
    //    }
    //}

    /* Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    ///* Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    ///* Create an object and add to renderer (this will change later to display a CAD model) */
    ///* Will just copy and paste cylinder example from before */
    //// This creates a polygonal cylinder model with eight circumferential facets
    //// (i.e., in practice an octagonal prism).
    //vtkNew<vtkCylinderSource> cylinder;
    //cylinder->SetResolution(8);

    //// The mapper is responsible for pushing the geometry into the graphics
    //// library. It may also do color mapping, if scalars or other attributes are
    //// defined.
    //vtkNew<vtkPolyDataMapper> cylinderMapper;
    //cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    //// The actor is a grouping mechanism: besides the geometry (mapper), it
    //// also has a property, transformation matrix, and/or texture map.
    //// Here we set its color and rotate it around the X and Y axes.
    //vtkNew<vtkActor> cylinderActor;
    //cylinderActor->SetMapper(cylinderMapper);
    //cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    //cylinderActor->RotateX(30.0);
    //cylinderActor->RotateY(-45.0);

    //renderer->AddActor(cylinderActor);

    ///* Reset Camera (probably needs to go in its own function that is called whenever
    //model is changed) */
    //renderer->ResetCamera();
    //renderer->GetActiveCamera()->Azimuth(30);
    //renderer->GetActiveCamera()->Elevation(30);
    //renderer->ResetCameraClippingRange();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// -----------------------------------------------------------------------------------------------
// Slots
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

void MainWindow::on_actionDelete_Item_triggered()
{
    // Disconnect the action's signal - otherwise it goes twice
    disconnect(ui->actionDelete_Item, &QAction::triggered, this, &MainWindow::on_actionDelete_Item_triggered);

    // Status Bar Message
    emit statusUpdateMessage(QString("Delete Item"), 0);

    // Get the selected item
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Delete the selected item
    QModelIndex parentIndex = index.parent();
    int row = index.row();
    if (partList->removeRow(row, parentIndex)) {
		emit statusUpdateMessage(QString("Item Deleted"), 0);
	}
    else {
		emit statusUpdateMessage(QString("Item Not Deleted"), 0);
	}

    // Reconnect the action's signal
    connect(ui->actionDelete_Item, &QAction::triggered, this, &MainWindow::on_actionDelete_Item_triggered);

    // Update the tree view
    partList->dataChanged(parentIndex, parentIndex);

    // Update the render window
    updateRender();
}

// -----------------------------------------------------------------------------------------------
// File Menus

void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage(QString("Opening File"),0);

    // Open a file dialog
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));

    // Check if a file was selected
    if (!fileName.isEmpty()) {
        emit statusUpdateMessage(QString("File Opened: ") + fileName, 0);
        // Get the current index
        QModelIndex index = ui->treeView->currentIndex();

        // Default values for the new item
        QString visible("true");
        QColor colour(255, 255, 255);

        // Create a new item
        ModelPart* newItem = new ModelPart({ fileName, visible, colour });

        // Check if an item is selected
        if (ui->treeView->selectionModel()->hasSelection()) {
             ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
            // Append the new item to the selected item
            selectedPart->appendChild(newItem);
        }
        else {
            // If no item is selected, create a new top-level item
            QList<QVariant> data = { fileName, visible, colour };
            QModelIndex parent; // An invalid QModelIndex so the item is added to the root
            partList->appendChild(parent, data);
        }

        // Update the tree view
        partList->dataChanged(index, index);

        // Load the STL file
        newItem->loadSTL(fileName);

        updateRender();
    }
    else {
        // If no file was selected
        emit statusUpdateMessage(QString("File Open Cancelled"), 0);
    }
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    emit statusUpdateMessage(QString("Opening Folder"), 0);

    // Open a directory dialog
    QString dirName = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        "C:\\",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // Check if a directory was selected
    if(!dirName.isEmpty()){
        emit statusUpdateMessage(QString("Folder Opened: ") + dirName, 0);

        // Get all STL files in the directory
        QDir directory(dirName);
        QStringList stlFiles = directory.entryList(QStringList() << "*.stl" << "*.STL", QDir::Files);

        foreach( QString fileName, stlFiles) {
            emit statusUpdateMessage(QString("File Opened: ") + fileName, 0);
            // Get the current index
            QModelIndex index = ui->treeView->currentIndex();

            // Default values for the new item
            QString visible("true");
            QColor colour(255, 255, 255);

            // Create a new item
            ModelPart* newItem = new ModelPart({ fileName, visible, colour });

            // Check if an item is selected
            if (ui->treeView->selectionModel()->hasSelection()) {
                ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
                // Append the new item to the selected item
                selectedPart->appendChild(newItem);
            }
            else {
                // If no item is selected, create a new top-level item
                QList<QVariant> data = { fileName, visible, colour };
                QModelIndex parent; // An invalid QModelIndex so the item is added to the root
                partList->appendChild(parent, data);
            }

            // Update the tree view
            partList->dataChanged(index, index);

            // Load the STL file
            newItem->loadSTL(directory.filePath(fileName));

        }
        updateRender();
	}
    // If no directory was selected
    else {
		emit statusUpdateMessage(QString("Folder Open Cancelled"), 0);
	}
}

// -----------------------------------------------------------------------------------------------
// Dialog Box

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

void MainWindow::receiveDialogData(const QString& name, const bool& visible, const QColor& colour) {

    // Display the data in the status bar
    emit statusUpdateMessage(QString("Colour: R%1 G%2 B%3")
        .arg(colour.red())
        .arg(colour.green())
        .arg(colour.blue()) + 
        QString(" Name: ") + name + 
        QString(" Visible? ") + QString::number(visible), 0);

    // Get the selected item
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Set the selected item's data
    selectedPart->setName(name);
    selectedPart->setVisible(visible);
    selectedPart->setColour(colour);
    
    updateRender();
}

// -----------------------------------------------------------------------------------------------
// Render Window

void MainWindow::updateRender() {
    renderer->RemoveAllViewProps();
    int rows = partList->rowCount(QModelIndex());
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }
    // Reset Camera
    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
    renderer->Render();
}

// TODO: Allow for top level file viewing
void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        /* Retrieve actor from selected part and add to renderer */
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor) {
            QColor qcolor = selectedPart->getColour();
            double red = qcolor.redF();
            double green = qcolor.greenF();
            double blue = qcolor.blueF();

            // Set the color of the actor
            actor->GetProperty()->SetColor(red, green, blue);
            
            // Check visibility and add or remove actor from renderer
            if (selectedPart->visible()) {
                if (!renderer->HasViewProp(actor)) {
                    renderer->AddActor(actor);
                }
            }
            else {
                renderer->RemoveActor(actor);
            }
        }
    }

    /* Check to see if this part has any children */
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    /* Loop through children and add their actors */
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}


//TODO: Add a select from viewport function