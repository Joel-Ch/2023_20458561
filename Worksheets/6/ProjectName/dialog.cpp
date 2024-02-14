#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::Dialog)
{
	ui->setupUi(this);

	// Set up the LCDs
	ui->RedLCD->setSegmentStyle(QLCDNumber::Flat);
	ui->RedLCD->setStyleSheet("QLCDNumber { color: red; background-color: black; }");
	ui->GreenLCD->setSegmentStyle(QLCDNumber::Flat);
	ui->GreenLCD->setStyleSheet("QLCDNumber { color: green; background-color: black; }");
	ui->BlueLCD->setSegmentStyle(QLCDNumber::Flat);
	ui->BlueLCD->setStyleSheet("QLCDNumber { color: blue; background-color: black; }");

	// Set up the sliders
	ui->RedSlider->setStyleSheet("QSlider::groove:horizontal{background: red;position: absolute;up: 4px; down: 4px;}QSlider::handle:horizontal{width: 10px;background: black;margin: -4px 0;}QSlider::add-page:horizontal{background: white;}QSlider::sub-page:horizontal{background: red;}");
	ui->GreenSlider->setStyleSheet("QSlider::groove:horizontal{background: red;position: absolute;up: 4px; down: 4px;}QSlider::handle:horizontal{width: 10px;background: black;margin: -4px 0;}QSlider::add-page:horizontal{background: white;}QSlider::sub-page:horizontal{background: green;}");
	ui->BlueSlider->setStyleSheet("QSlider::groove:horizontal{background: red;position: absolute;up: 4px; down: 4px;}QSlider::handle:horizontal{width: 10px;background: black;margin: -4px 0;}QSlider::add-page:horizontal{background: white;}QSlider::sub-page:horizontal{background: blue;}");

	// Set up the connections
	connect(ui->checkBox, &QCheckBox::stateChanged, this, &Dialog::handleVisibleCheck);
	connect(ui->lineEdit, &QLineEdit::textChanged, this, &Dialog::handleTextInput);

	connect(ui->RedSlider, &QSlider::valueChanged, this, &Dialog::handleRedSlider);
	connect(ui->GreenSlider, &QSlider::valueChanged, this, &Dialog::handleGreenSlider);
	connect(ui->BlueSlider, &QSlider::valueChanged, this, &Dialog::handleBlueSlider);

	// Set up the initial values
	ui->checkBox->setCheckState(Qt::Checked);

	updateColourDisplay();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::handleTextInput()
{
	
}

void Dialog::handleRedSlider()
{
	ui->RedLCD->display(ui->RedSlider->value());
	updateColourDisplay();
}

void Dialog::handleGreenSlider()
{
	ui->GreenLCD->display(ui->GreenSlider->value());
	updateColourDisplay();
}

void Dialog::handleBlueSlider()
{
	ui->BlueLCD->display(ui->BlueSlider->value());
	updateColourDisplay();
}

void Dialog::handleVisibleCheck()
{
	bool visible = ui->checkBox->isChecked();
}

void Dialog::updateColourDisplay()
{
	// Get the values from the sliders
	int red = ui->RedSlider->value();
	int green = ui->GreenSlider->value();
	int blue = ui->BlueSlider->value();

	// Set the background colour of the display widget
	QString styleSheet = QString("QWidget{ background-color: rgb(%1, %2, %3); }")
		.arg(red).arg(green).arg(blue);

	ui->ColourDisplay->setStyleSheet(styleSheet);
}