#define CVUI_IMPLEMENTATION
#define WINDOW_NAME "Counting Cars"

#include<opencv2/opencv.hpp>

#include"cvui.h"
#include"EnhancedWindow.h"
#include"CCMethods.h"

#include"settings.h"

#include<iostream>
#include<conio.h>
#include<chrono>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap;
	//("C:\\Users\\Franc\\2019\\obrazy\\car-detect2\\traffic.mp4");

	int cars_count = 0;

	Mat src, fg, thresh;
	Mat frame = Mat(800, 1600, CV_8UC3);	

	Rect b(50, 50, 50, 50);

	settings s(frame);
	bool working = false;
	cvui::init(WINDOW_NAME);

	string path = "";

	EnhancedWindow main(450, 0, 874, 520, "Output", false);

	vector<Point> cars;
	Ptr<BackgroundSubtractorMOG2> pBackSub = createBackgroundSubtractorMOG2();

	while (1) {
		frame = Scalar(49, 52, 49);

		s.inputsettings();
		s.morphsettings();
		s.othersettings();
		s.selectsettings();
		s.displayResult(cars_count);
		s.backsubsettings();

		if (path.compare(s.path) != 0 || s.restart) {
			cap.open(s.path);
			path = s.path;
			cars_count = 0;
			pBackSub = createBackgroundSubtractorMOG2();
			cap >> src;
			s.restart = false;
		}

		if (cap.isOpened()) {
			s.controlssettings();
			if (s.selecting) {
				b = selectROI(src);
			}
			if (!s.pause) {
				int status = cvui::iarea(460, 30, 854, 480);

				pBackSub->setHistory(s.history);
				pBackSub->setNMixtures(s.nmixtures);
				pBackSub->setVarThreshold(s.thresh);

				cap >> src;
				pBackSub->apply(src, fg);

				vector<Point> trackPts;
				cc::filter(fg, thresh, s.kernelx, s.kernely, s.lthresh, s.hthresh);
				cc::detect(src, thresh, trackPts, s.minarea, s.maxarea);
			
				Mat roi = src(b);
				Mat color(roi.size(), CV_8UC3, Scalar(0, 125, 125));
				color.convertTo(color, COLOR_GRAY2BGR);
				double alpha = 0.3;
				addWeighted(color, alpha, roi, 1.0 - alpha, 0.0, roi);

				cc::countCars(cars, trackPts, Point(b.x, b.y), Point(b.x + b.width, b.y+b.height), s.maxrad, &cars_count);
			}

			main.begin(frame, false);
			cvui::context("Output");

			switch (s.output)
			{
				case 1: {
					cvui::image(src);
				} break;
				case 2: {
					cvtColor(fg, fg, COLOR_GRAY2BGR);
					cvui::image(fg);
				} break;
				case 3: {
					cvtColor(thresh, thresh, COLOR_GRAY2BGR);
					cvui::image(thresh);
				} break;
			}

			cvui::rect(frame, 460, 30, 854, 480, 90);
			cvui::context(WINDOW_NAME);
			main.end();
		}

		cvui::imshow(WINDOW_NAME, frame);

		if (waitKey(30) == 27) break;
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;
}