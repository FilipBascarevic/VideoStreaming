/** buffer.h
 *
 * Class for buffering current frame.
 * This class contains definitions of some methods, signals and slots for catching a frame.
 * When frame is catched successfully, it will be saved in a buffer. If buffer contains more then 1 frame,
 * methods activate timer which starts releasing the buffer frame par frame, and starts frame processing.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include "tracker.h"
#include <QBasicTimer>

class Buffer : public QObject
{
    Q_OBJECT

    //receive frame buffer list
    std::list <cv::Mat> frames_buff;
    // timer which send frames to processing and displaying
    QBasicTimer m_timer;


public:
    explicit Buffer(QObject *parent = nullptr);
    ~Buffer() {}

    /** Q_SLOT pushBuff()
    *
    * when frame is ready, after emitting adequative signal, this slot bush frame in buffer
    * This SLOT emits framePush SIGNAL to inform other SLOT for reading from buff
    *
    */
    Q_SLOT void pushBuff(const cv::Mat &frame);

    /** Q_SIGNAL framePush()
    *
    * emitting this signal, frame is stored in buffer successfuly.
    * Can be connected to other method which read from buffer and send frame to other processing
    *
    */
    Q_SIGNAL void framePushed ();

    /** Q_SIGNAL framePoped()
    *
    * emitting this signal, frame is read from buffer successfuly.
    * Can be connected to other method which send frame to other processing
    *
    */
    Q_SIGNAL void framePoped (const cv::Mat &);

    /** Q_SLOT timerStart()
    *
    * when frame is successfully shifted in a buffer, we can start reading from them.
    * We read from buffer with a speed 30frames/s and send them to oter processing and displaying
    * So this SLOT starts timer if it is not active
    *
    */
    Q_SLOT void timerStart ();
private:
    /** Callback after timeout
    *
    * When timeout is occured, this function read the frame from buffer and emit framePoped signal
    *
    * @param timer pointer on which timeout is occurred
    *
    */
    void timerEvent(QTimerEvent * ev);

};

#endif // BUFFER_H
