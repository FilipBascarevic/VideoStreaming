/** imageviewer.h
 *
 * Class for displaying current frame.
 * This class contains definitions of some methods, signals and slots for processing a frame.
 * After imageReady signal, this calss will display current processed frame.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QObject>
#include <algorithm>
#include <QDebug>
#include <QPainter>
#include "tracker.h"

class ImageViewer : public QWidget
{
   Q_OBJECT
   Q_PROPERTY(QImage image READ image WRITE setImage USER true)
   // flag which inform abouth dropping a frame
   bool painted = true;
   // current frame which will be displayed
   QImage m_img;
   // tracker for debugging purpose
   AddressTracker m_track;

   /** Event handler which call subroutin every time when update() is called
   *
   * This callback display processed frame
   *
   * @param paintEvent
   *
   */
   void paintEvent(QPaintEvent *);
public:
   ImageViewer(QWidget * parent = nullptr);
   ~ImageViewer() { qDebug() << __FUNCTION__ << "reallocations" << m_track.reallocs; }

   /** Q_SLOT setImage()
   *
   * when some signal is emmitted, this SLOT save current frame in a field and run displaying of this frame.
   *
   * @param current frame which needs to be displayed
   *
   */
   Q_SLOT void setImage(const QImage &img);

   /** This method returns the last displaying frame
   *
   *
   * @return last displaying frame in QImage format
   *
   */
   QImage image() const { return m_img; }
};

#endif // IMAGEVIEWER_H
