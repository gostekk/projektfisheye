#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
       connect(
		stereographic_button, 
		SIGNAL (clicked()),
		this,
		SLOT (StereographicButton()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::StereographicClicked(){
	qDebug() << "Chrum";
}

