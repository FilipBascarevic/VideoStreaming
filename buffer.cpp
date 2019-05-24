/** capture.cpp
 *
 * Class for buffering current frame. Contains implementation of all methods, signals and slots.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#include "buffer.h"

Buffer::Buffer(QObject *parent) : QObject(parent)
{

}

/** Q_SLOT pushBuff()
*
* when frame is ready, after emitting adequative signal, this slot bush frame in buffer
* This SLOT emits framePush SIGNAL to inform other SLOT for reading from buff
*
*/
Q_SLOT void Buffer::pushBuff(const cv::Mat &frame)
{
    frames_buff.push_back(frame.clone());
    emit framePushed();
}

/** Q_SLOT timerStart()
*
* when frame is successfully shifted in a buffer, we can start reading from them.
* We read from buffer with a speed 30frames/s and send them to oter processing and displaying
* So this SLOT starts timer if it is not active
*
*/
Q_SLOT void Buffer::timerStart ()
{
    if(!m_timer.isActive())
        m_timer.start(33, this);
}

/** Callback after timeout
*
* When timeout is occured, this function read the frame from buffer and emit framePoped signal
*
* @param timer pointer on which timeout is occurred
*
*/
void Buffer::timerEvent(QTimerEvent * ev) {
   if (ev->timerId() != m_timer.timerId()) return;
   // in buffer is more frames at 1. If not, stop timer
   if(frames_buff.size() >= 1) {
       // read and pop frame in front
       cv::Mat image = frames_buff.front();
       // emit framePoped
       emit framePoped(image);
       frames_buff.pop_front();
       //cv::imshow("image", image);
   }
   else {
       m_timer.stop();
   }
}
