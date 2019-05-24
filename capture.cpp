/** capture.cpp
 *
 * Class for catching current frame. Contains implementation of all methods, signals and slots.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#include "capture.h"


Capture::Capture(QObject *parent) : QObject(parent)
{

}

/** Q_SLOT Start capturing
*
* Initialization of VideoCapture and starting timer if Video Capture is opened properly
*
*/
Q_SLOT void Capture::start() {
   // Initialize videoCapture with a source
   if (isCamera) {
       if (!m_videoCapture)
           m_videoCapture.reset(new cv::VideoCapture(videoSource.toInt(&isCamera)));
   }
   else {
       if (!m_videoCapture)
           m_videoCapture.reset(new cv::VideoCapture(videoSource.toStdString()));
   }

   // If videoCapture is opened properly, start timer
   if (m_videoCapture->isOpened()) {
      m_timer.start(0, this);
      emit started();
   }
}

/** Callback after timeout
*
* When timeout is occured, this function needs to catch the frame and emit frameReady signal
*
* @param timer pointer on which timeout is occurred
*
*/
void Capture::timerEvent(QTimerEvent * ev) {
   // Check is timeout occurred from timer in this class
   if (ev->timerId() != m_timer.timerId()) return;
   // Read the frame. If program couldn't read it, stop timer
   if (!m_videoCapture->read(m_frame)) { // Blocks until a new frame is ready
      m_timer.stop();
      return;
   }
   // Emit frameReady
   m_track.track(m_frame);
   emit frameReady(m_frame);
}
