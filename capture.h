/** capture.h
 *
 * Class for catching current frame.
 * This class contains definitions of some methods, signals and slots for catching a frame.
 * When frame is cathch successfully, the signal frameReady is emitted.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#ifndef CAPTURE_H
#define CAPTURE_H

#include <QObject>
#include <algorithm>
#include <QBasicTimer>
#include <QDebug>
#include "tracker.h"


class Capture : public QObject
{
    Q_OBJECT
    Q_PROPERTY(cv::Mat frame READ frame NOTIFY frameReady USER true)

    // Current captured frame
    cv::Mat m_frame;
    // timer, we use it as while loop, because cv::VideoCapture -> read() is blocking function. On timer event
    // frame collects
    QBasicTimer m_timer;
    // Video capture
    QScopedPointer<cv::VideoCapture> m_videoCapture;
    // tracker only for debugging purpose
    AddressTracker m_track;

public:
    explicit Capture(QObject *parent = nullptr);
    ~Capture() { qDebug() << __FUNCTION__ << "reallocations" << m_track.reallocs; }

    // Use camera or URL
    bool isCamera = true;

    // Name of sourse of frames. It can be number order for camera, or URL in a case of streaming.
    QString videoSource = "0";

    /** Q_SIGNAL started()
    *
    * emitting this signal, process of capturing is starting.
    * Can be connected for one button, ond after pressing it, capturing process can be started.
    *
    */
    Q_SIGNAL void started();

    /** Q_SLOT Start capturing
    *
    * Initialization of VideoCapture and starting timer if Video Capture is opened properly
    *
    */
    Q_SLOT void start();

    /** Q_SLOT stop()
    *
    * when some signal is emmitted, process of capturing is stopped.
    * Can be connected for one button, ond after pressing it, capturing process will be stopped.
    *
    */
    Q_SLOT void stop() { m_timer.stop(); }

    /** Q_SIGNAL frameReady()
    *
    * emitting this signal, frame is catched successfuly.
    * Can be connected to other method which do processing of frame.
    *
    * @param current frame
    *
    */
    Q_SIGNAL void frameReady(const cv::Mat &);

    /** frame()
    *
    * this frame returns current frame
    *
    * @return current frame
    *
    */
    cv::Mat frame() const { return m_frame; }
private:
    /** Callback after timeout
    *
    * When timeout is occured, this function needs to catch the frame and emit frameReady signal
    *
    * @param timer pointer on which timeout is occurred
    *
    */
    void timerEvent(QTimerEvent * ev);

};

#endif // CAPTURE_H
