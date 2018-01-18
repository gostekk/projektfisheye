#include <QImageReader>
#include <QImage>
#include <QDebug>
#include <QtMath>
#include <QColor>

void SaveImage(QImage naszobraz, QString DirectionToSaveImage) {
  qDebug() <<"Saved: " << naszobraz.save(QString("%1.%2").
	arg(DirectionToSaveImage).
	arg("jpg"));
}

void ConvertFromFishEye(
	QImage naszobraz, 
	QString ConversionType,
	double vStrenght,
	double vRadius,
	QString DirectionToSaveImage) {

  int W,H,WO,HO,xd,yd;
  // float foc=4.0/7.3,
  float foc=4.0/12.0;
  float r,xf,yf,cosfi,sinfi,D,DO;
  //float vStrenght=0.78;

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
 
	if (ConversionType == "Stereographic") {
		// Stereographic // vStrenght = 0.78
		xf = cosfi * 2 * D * qTan(0.5 * qAtan2(r,DO));
		yf = sinfi * 2 * D * qTan(0.5 * qAtan2(r,DO));
	}
	if (ConversionType == "Equidistant") {
		// Equidistant // vStrenght =
		xf = cosfi * D * qAtan2(r,DO);
		yf = sinfi * D * qAtan2(r,DO);
	}
	if (ConversionType == "Equisolid") {
		// Equisolid angle // vStrenght =
		xf = cosfi * 2 * D * qSin(0.5 * qAtan2(r,DO));
		yf = sinfi * 2 * D * qSin(0.5 * qAtan2(r,DO));
	}
	if (ConversionType == "Orthographic") {
		// Orthographic // vStrenght = 1.2
		xf = cosfi * D * qSin(qAtan2(r,DO));
		yf = sinfi * D * qSin(qAtan2(r,DO));
	}
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

  SaveImage(convertedImage, DirectionToSaveImage);
}

QImage LoadImage(QString LoadedImageDirection) {
  QImage naszobraz;
  QString DirectoryToSave;
  naszobraz.load(LoadedImageDirection);
  qDebug() <<"QImage" << naszobraz.size();
  naszobraz.alphaChannel();
  return naszobraz;

}
