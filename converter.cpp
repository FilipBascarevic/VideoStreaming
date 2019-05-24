/** converter.cpp
 *
 * Class for processing current frame. Contains implementation of all methods, signals and slots.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#include "converter.h"

Converter::Converter(QObject *parent) : QObject(parent)
{

}

/** This function check does received frame is dropped.
*
* If processAll is false, this function will explore does current frame is empty and it will start timer
*
* @param received frame
*
*/
void Converter::queue(const cv::Mat &frame) {
   if (!m_frame.empty()) qDebug() << "Converter dropped frame!";
   m_frame = frame;
   if (! m_timer.isActive()) m_timer.start(0, this);
}

/** This method includes image processing and convert image from cv::Mat format to QImage format.
*
* @param frame in cv::Mat format
*
*/
void Converter::process(const cv::Mat &frame) {

   cv::Mat new_frame = imageProcessing(frame);
   convert (new_frame);
}

/** Method fr processing current frame
*
* This is vrtual method for image processing. It menas that user can overide it and make own algorithm for
* frame processing.
*
* @param frame in cv::Mat format
*
* @return processed frame in cv::Mat format
*
*/
cv::Mat Converter::imageProcessing(const cv::Mat &frame){
    int w = frame.cols / 2, h = frame.rows / 2;
    cv::Mat new_frame;
    cv::resize(frame, new_frame, cv::Size(w,h), 0, 0, cv::INTER_AREA);
    return new_frame;
}

/** Method for converting current frame
*
* This method converts frame from cv::Mat to QImage format
*
* @param frame in cv::Mat format
*
*/
void Converter::convert(const cv::Mat &frame) {
    Q_ASSERT(frame.type() == CV_8UC3);
    int w = frame.cols, h = frame.rows;
    if (m_image.size() != QSize{w,h})
       m_image = QImage(w, h, QImage::Format_RGB888);
    cv::Mat mat(h, w, CV_8UC3, m_image.bits(), m_image.bytesPerLine());
    // copy processed frame to mat
    frame.copyTo(mat);
    cv::cvtColor(mat, mat, CV_BGR2RGB);
    emit imageReady(m_image);
}

/** Callback after timeout
*
* When timeout is occured, this function needs to send proccessed frame in QIMage format
* to function which will display it. Also, it stops timer if proccessAll is true;
*
* @param timer pointer on which timeout is occurred
*
*/
void Converter::timerEvent(QTimerEvent *ev) {
   if (ev->timerId() != m_timer.timerId()) return;
   process(m_frame);
   m_frame.release();
   m_track.track(m_frame);
   m_timer.stop();
}
