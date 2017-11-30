#include <QCoreApplication>
#include <QImageReader>
#include <QImage>
#include <QDebug>
#include <QtMath>
void SaveImage(QImage naszobraz) {
    qDebug() << naszobraz.save("/Users/adrix/transformimg/swinak6.jpg");

}


QImage ConvertFromFishEye(QImage naszobraz) {

    qDebug() << "Convert";
    QVariant vTheta = 0;
   // QColor color(0,0,0);

    int vHalfImageWidth = naszobraz.width()/2;
    int vHalfImageHeight = naszobraz.height()/2;
    qDebug() << "Width: " << vHalfImageWidth << "\nHeight: " << vHalfImageHeight;
    double vStrenght = 1; //Its only aproximate, dont know what value is correct
    QVariant vCorrectionRadius = qSqrt(qPow(vHalfImageWidth, 2) + qPow(vHalfImageHeight, 2)) / vStrenght;
    qDebug() << "CorrectionRadius" << vCorrectionRadius;

    for(int width = 0; width < naszobraz.width(); width++) {
        for (int height=0; height < naszobraz.height(); height++) {
            QVariant vNewWidthPosition = width - vHalfImageWidth;
            QVariant vNewHeightPosition = height - vHalfImageHeight;
            QVariant vDistance = qSqrt(qPow(vNewWidthPosition.toFloat(), 2) + qPow(vNewHeightPosition.toFloat(), 2));
            QVariant vRadius = vDistance.toFloat() / vCorrectionRadius.toFloat();
            //qDebug() << "Radius" << vRadius;

            if (vRadius == 0) {
                vTheta = 1;
               // qDebug() << "jestem 0";
            } else {
                vTheta = qAtan(vRadius.toFloat()) / vRadius.toFloat();
                //vTheta = qAtan2(vNewWidthPosition,vNewHeightPosition);
                //vTheta = qTan(vRadius.toFloat()) / vRadius.toFloat();

            }
            QVariant vSourceWidthPixel = vHalfImageWidth + vTheta.toFloat() * vNewWidthPosition.toFloat() * 1; // 1 is zoom, for now dont use it
            QVariant vSourceHeightPixel = vHalfImageHeight + vTheta.toFloat() * vNewHeightPosition.toFloat() * 1; // 1 is zoom, for now dont use it

            QColor NewColorOfPixel = naszobraz.pixel(vSourceWidthPixel.toInt(), vSourceHeightPixel.toInt());

            naszobraz.setPixel(width, height, NewColorOfPixel.rgba());

          //naszobraz.setPixel(width,height,ColorX.rgba());
            //QColor
            //naszobraz.SetPixel()
        }
    }


   SaveImage(naszobraz);

}

void LoadImage() {
    QImage naszobraz;
    naszobraz.load("/Users/adrix/transformimg/Panotools5618.jpg");
    qDebug() <<"QImage" << naszobraz.size();
    naszobraz.alphaChannel();
    ConvertFromFishEye(naszobraz);

}

int main(int argc)
{
    LoadImage();
}