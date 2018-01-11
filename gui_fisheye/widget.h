#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
  
private slots:
	void StereographicClicked();

private:
    Ui::Widget *ui;
    QPushButton *stereographic_button;
};

#endif // WIDGET_H
