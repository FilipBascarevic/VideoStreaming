#include <QCoreApplication>
#include "capture.h"
#include "buffer.h"
#include "converter.h"
#include "imageviewer.h"
#include <QThread>

class Thread final : public QThread { public: ~Thread() { quit(); wait(); } };


int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>();
    QApplication a(argc, argv);
    ImageViewer view;
    Capture capture;
    Buffer buffer;
    Converter converter;
    Thread captureThread, converterThread, bufferThread;
    // Everything runs at the same priority as the gui, so it won't supply useless frames.
    converter.setProcessAll(true);
    captureThread.start();
    bufferThread.start();
    converterThread.start();
    // Run capture class in captureThread
    capture.moveToThread(&captureThread);
    // Run buffer class in bufferThread
    buffer.moveToThread(&bufferThread);
    // Run converter class in converterThread
    converter.moveToThread(&converterThread);
    // Connect SIGNALS with SLOTS
    QObject::connect(&capture, &Capture::frameReady, &buffer, &Buffer::pushBuff);
    QObject::connect(&buffer, &Buffer::framePushed, &buffer, &Buffer::timerStart);
    QObject::connect(&buffer, &Buffer::framePoped, &converter, &Converter::processFrame);
    QObject::connect(&converter, &Converter::imageReady, &view, &ImageViewer::setImage);
    // Display
    view.show();
    // Display message when we start catching a frames
    QObject::connect(&capture, &Capture::started, [](){ qDebug() << "Capture started."; });
    // Using RTSP for catching frames. For using camera, isCamera=true and videoSource="0";
    capture.isCamera = false;
    capture.videoSource = "rtp://127.0.0.1:1234";
    // Start capturing
    QMetaObject::invokeMethod(&capture, "start");
    return a.exec();
}
