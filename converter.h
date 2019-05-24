/** converter.h
 *
 * Class for processing current frame.
 * This class contains definitions of some methods, signals and slots for processing a frame.
 * User can it to derive another calss. He/she can override imageProcessing method and implement another processing very easy.
 * When frame is processed successfully, the signal imageReady is emitted.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <algorithm>
#include <QBasicTimer>
#include <QDebug>
#include "tracker.h"


class Converter : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QImage image READ image NOTIFY imageReady USER true)
   Q_PROPERTY(bool processAll READ processAll WRITE setProcessAll)
   // timer, we use it as while loop, because cv::VideoCapture -> read() is blocking function. On timer event
   // frame collects
   QBasicTimer m_timer;
   // Image in cv::Mat format
   cv::Mat m_frame;
   // Image in QImage format
   QImage m_image;
   // processAll flag
   bool m_processAll = true;
   // tracker for debugging purpose
   AddressTracker m_track;

   /** This method check does received frame is dropped.
   *
   * If processAll is false, this function will explore does current frame is empty and it will start timer
   *
   * @param received frame
   *
   */
   void queue(const cv::Mat &frame);

   /** This method includes image processing and convert image from cv::Mat format to QImage format.
   *
   * @param frame in cv::Mat format
   *
   */
   void process(const cv::Mat &frame);

   /** Method for processing current frame
   *
   * This is vrtual method for image processing. It menas that user can overide it and make own algorithm for
   * frame processing.
   *
   * @param frame in cv::Mat format
   *
   * @return processed frame in cv::Mat format
   *
   */
   virtual cv::Mat imageProcessing(const cv::Mat &frame);

   /** Method for converting current frame
   *
   * This method converts frame from cv::Mat to QImage format
   *
   * @param frame in cv::Mat format
   *
   */
   void convert(const cv::Mat &frame);

   /** Callback after timeout
   *
   * When timeout is occured, this function needs to send proccessed frame in QIMage format
   * to function which will display it. Also, it stops timer if proccessAll is true;
   *
   * @param timer pointer on which timeout is occurred
   *
   */
   void timerEvent(QTimerEvent *ev);
public:
   explicit Converter(QObject * parent = nullptr);
   ~Converter() { qDebug() << __FUNCTION__ << "reallocations" << m_track.reallocs; }

   /** This method returns the status of processAll field
   *
   *
   * @return processAll field
   *
   */
   bool processAll() const { return m_processAll; }

   // processAll field. If it is true, process every received frame. Else process frames on every timeout
   bool isAdditionalProcessing = false;

   /** This method set the value of processAll field
   *
   *
   * @param ne processAll field value
   *
   */
   void setProcessAll(bool all) { m_processAll = all; }

   /** Q_SIGNAL imageReady()
   *
   * emitting this signal when frame is converted successfuly.
   * Can be connected to other method which do displaying of current frame in QWidgets.
   *
   * @param current frame in QImage
   *
   */
   Q_SIGNAL void imageReady(const QImage &);

   /** This method returns current frame in QImage format
   *
   *
   * @return current frame in QImage
   *
   */
   QImage image() const { return m_image; }

   /** Q_SLOT processFrame
   *
   * Starting processing depending of processAll field
   *
   */
   Q_SLOT void processFrame(const cv::Mat &frame) {
      if (m_processAll) process(frame); else queue(frame);
   }
};

#endif // CONVERTER_H
