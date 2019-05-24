/** imageviewer.cpp
 *
 * Class for displaying current frame. Contains implementation of all methods, signals and slots.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{

}

/** Event handler which call subroutin every time when update() is called
*
* This callback display processed frame
*
* @param paintEvent
*
*/
void ImageViewer::paintEvent(QPaintEvent *) {
   QPainter p(this);
   if (!m_img.isNull()) {
      setAttribute(Qt::WA_OpaquePaintEvent);
      // display frame
      p.drawImage(0, 0, m_img);
      painted = true;
   }
}

/** Q_SLOT setImage()
*
* when some signal is emmitted, this SLOT save current frame in a field and run displaying of this frame.
*
* @param current frame which needs to be displayed
*
*/
Q_SLOT void ImageViewer::setImage(const QImage &img) {
   if (!painted) qDebug() << "Viewer dropped frame!";
   if (m_img.size() == img.size() && m_img.format() == img.format()
       && m_img.bytesPerLine() == img.bytesPerLine())
      std::copy_n(img.bits(), img.sizeInBytes(), m_img.bits());
   else
      m_img = img.copy();
   painted = false;
   if (m_img.size() != size()) setFixedSize(m_img.size());
   m_track.track(m_img);
   update();
}
