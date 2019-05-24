/** tracker.h
 *
 * Header file which contains method for collecting information about memory realocations.
 * We use it for debugging purpose.
 *
 * @author Filip Bascarevic
 *
 * @version 0.1 -- 20 May 2019
 */

#ifndef TRACKER_H
#define TRACKER_H

#include <QApplication>
#include <QWidget>
#include <opencv2/opencv.hpp>

Q_DECLARE_METATYPE(cv::Mat)

// using this structure only for debugging purpose
struct AddressTracker {
   const void *address = {};
   int reallocs = 0;
   void track(const cv::Mat &m) { track(m.data); }
   void track(const QImage &img) { track(img.bits()); }
   void track(const void *data) {
      if (data && data != address) {
         address = data;
         reallocs ++;
      }
   }
};

#endif // TRACKER_H
