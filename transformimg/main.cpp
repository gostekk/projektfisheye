#include <QCoreApplication>
#include <QImageReader>
#include <QImage>
#include <QDebug>

void SaveImage(QImage naszobraz) {
    qDebug() << naszobraz.save("/Users/adrix/transformimg/swinak4.jpg");

}


QImage ConvertFromFishEye(QImage naszobraz) {

    qDebug() << "Convert";
    QColor color(0,0,0);
    for (int height=0; height < naszobraz.height(); ++height) {
        for(int width = 0; width < naszobraz.width(); ++width) {
          naszobraz.setPixel(width,height,color.rgba());
        }
    }
   SaveImage(naszobraz);

}

void LoadImage() {
    QImage naszobraz;
    naszobraz.load("/Users/adrix/transformimg/pepe.jpg");
    qDebug() <<"QImage" << naszobraz.size();
    naszobraz.alphaChannel();
    ConvertFromFishEye(naszobraz);

}

int main(int argc)
{
    LoadImage();
}
