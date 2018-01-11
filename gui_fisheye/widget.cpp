#include "widget.h"
#include "ui_widget.h"
#include "../main.cpp"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ConnectButtons();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::ConnectButtons() {
	
	connect(
		ui->stereographic_button, 
		SIGNAL (clicked()),
		this,
		SLOT (StereographicClicked()));

	connect(
		ui->equidistant_button, 
		SIGNAL (clicked()),
		this,
		SLOT (EquidistantClicked()));

	connect(
		ui->equisolid_button, 
		SIGNAL (clicked()),
		this,
		SLOT (EquisolidClicked()));

	connect(
		ui->orthographic_button, 
		SIGNAL (clicked()),
		this,
		SLOT (OrthographicClicked()));

	connect(
		ui->open_button, 
		SIGNAL (clicked()),
		this,
		SLOT (OpenClicked()));

	connect(
		ui->save_button, 
		SIGNAL (clicked()),
		this,
		SLOT (SaveClicked()));
}

void Widget::StereographicClicked() {
	ConversionType = "Stereographic";	
}

void Widget::EquidistantClicked() {
	ConversionType = "Equidistant";
}

void Widget::EquisolidClicked() {
	ConversionType = "Equisolid";
}

void Widget::OrthographicClicked() {
	ConversionType = "Orthographic";
}

void Widget::OpenClicked() {
	ImageToConvert = LoadImage();
}

void Widget::SaveClicked() {
	double vStrenght = ui->correction_box->value();
	double vRadius = ui->radius_box->value();
	qDebug() << vStrenght;
	ConvertFromFishEye(ImageToConvert, ConversionType, vStrenght, vRadius);
}

