
#include <QImageReader>
#include <QImage>
#include <QDebug>
#include <QtMath>
#include <QColor>

void SaveImage(QImage naszobraz) {
  qDebug() << naszobraz.save("convertedImage.jpg");
}

QImage ConvertFromFishEye(QImage naszobraz) {

  int W,H,WO,HO,xd,yd;
  // float foc=4.0/7.3,
  float foc=4.0/12.0;
  float r,xf,yf,cosfi,sinfi,D,DO;
  float vStrenght=0.78;

  W = naszobraz.width();
  H = naszobraz.height();
  WO = naszobraz.width()/2;
  HO = naszobraz.height()/2;

  QImage convertedImage(WO,HO, QImage::Format_RGB32);

  DO=foc*qSqrt(WO*WO+HO*HO);
  D=foc*qSqrt(W*W+H*H);

  for(int ix = 0; ix < WO; ix++) {
    for (int iy=0; iy < HO; iy++) {

      xf = (float)(ix-WO/2);
      yf = (float)(iy-HO/2);
      r = qSqrt( xf*xf+yf*yf ) * vStrenght;
      cosfi = xf / r;
      sinfi = yf / r;

      // Stereographic // vStrenght = 0.78
      xf = cosfi * 2 * D * qTan(0.5 * qAtan2(r,DO));
      yf = sinfi * 2 * D * qTan(0.5 * qAtan2(r,DO));

      // Equidistant // vStrenght =
      // xf = cosfi * D * qAtan2(r,DO);
      // yf = sinfi * D * qAtan2(r,DO);

      // Equisolid angle // vStrenght =
      // xf = cosfi * 2 * D * qSin(0.5 * qAtan2(r,DO));
      // yf = sinfi * 2 * D * qSin(0.5 * qAtan2(r,DO));

      // Orthographic // vStrenght = 1.2
      // xf = cosfi * D * qSin(qAtan2(r,DO));
      // yf = sinfi * D * qSin(qAtan2(r,DO));

      xd = (int)xf;
      yd = (int)yf;
      xd += W / 2;
      yd += H / 2;

      if ((xd>0)&&(xd<W)&&(yd>0)&&(yd<H)) {
        QColor NewColorOfPixel = naszobraz.pixel(xd, yd);
        convertedImage.setPixel(ix, iy, NewColorOfPixel.rgba());
      }
    }
  }

  SaveImage(convertedImage);
  return convertedImage; /*albo decydujemy się na return albo zmieniamy typ na void*/
}

void LoadImage() {
  QImage naszobraz;
  naszobraz.load("images/image.jpg");
  qDebug() <<"QImage" << naszobraz.size();
  naszobraz.alphaChannel();
  ConvertFromFishEye(naszobraz);

}

int main(int argc){
  LoadImage();

  return 0;
}
