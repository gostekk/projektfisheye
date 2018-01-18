#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QPalette>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
    QString ConversionType;
    QString DirectionToSaveImage;
  
private slots:
	void StereographicClicked();
	void EquidistantClicked();
	void EquisolidClicked();
	void OrthographicClicked();
	void OpenClicked();
	void SaveClicked();

private:
    Ui::Widget *ui;
    QImage ImageToConvert;
    void ConnectButtons();
};

#endif // WIDGET_H
