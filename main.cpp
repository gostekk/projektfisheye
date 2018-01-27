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
	double vFoc,
	QString DirectionToSaveImage) {

  int vImageWidth,vImageHeight,vHalfImageWidth,vHalfImageHeight,xd,yd;
  // float vFoc=4.0/7.3,
  // float vFoc=4.0/12.0;
  float vRadius,xf,yf,cosfi,sinfi,vDistanceSource,vDistanceNew;
  //float vStrenght=0.78;

  vImageWidth = naszobraz.width();
  vImageHeight = naszobraz.height();
  vHalfImageWidth = naszobraz.width()/2;
  vHalfImageHeight = naszobraz.height()/2;

  QImage convertedImage(vHalfImageWidth,vHalfImageHeight, QImage::Format_RGB32);

  vDistanceNew = vFoc *qSqrt(vHalfImageWidth*vHalfImageWidth+vHalfImageHeight*vHalfImageHeight);
  vDistanceSource = vFoc *qSqrt(vImageWidth*vImageWidth+vImageHeight*vImageHeight);

  for(int ix = 0; ix < vHalfImageWidth; ix++) {
    for (int iy=0; iy < vHalfImageHeight; iy++) {

      xf = (float)(ix-vHalfImageWidth/2);
      yf = (float)(iy-vHalfImageHeight/2);
      vRadius = qSqrt( xf*xf+yf*yf ) * vStrenght;
      cosfi = xf / vRadius;
      sinfi = yf / vRadius;

	if (ConversionType == "Stereographic") {
		// Stereographic // vStrenght = 0.78
		xf = cosfi * 2 * vDistanceSource * qTan(0.5 * qAtan2(vRadius,vDistanceNew));
		yf = sinfi * 2 * vDistanceSource * qTan(0.5 * qAtan2(vRadius,vDistanceNew));
	}
	if (ConversionType == "Equidistant") {
		// Equidistant // vStrenght =
		xf = cosfi * vDistanceSource * qAtan2(vRadius,vDistanceNew);
		yf = sinfi * vDistanceSource * qAtan2(vRadius,vDistanceNew);
	}
	if (ConversionType == "Equisolid") {
		// Equisolid angle // vStrenght =
		xf = cosfi * 2 * vDistanceSource * qSin(0.5 * qAtan2(vRadius,vDistanceNew));
		yf = sinfi * 2 * vDistanceSource * qSin(0.5 * qAtan2(vRadius,vDistanceNew));
	}
	if (ConversionType == "Orthographic") {
		// Orthographic // vStrenght = 1.2
		xf = cosfi * vDistanceSource * qSin(qAtan2(vRadius,vDistanceNew));
		yf = sinfi * vDistanceSource * qSin(qAtan2(vRadius,vDistanceNew));
	}
      xd = (int)xf;
      yd = (int)yf;
      xd += vImageWidth / 2;
      yd += vImageHeight / 2;

      if ((xd>0)&&(xd<vImageWidth)&&(yd>0)&&(yd<vImageHeight)) {
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
