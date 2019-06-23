#pragma once
#include<opencv2/opencv.hpp>

namespace cc {

	using namespace cv;
	using namespace std;
	
	bool pointInRect(Rect r, Point p) {
		if (p.x > r.x && p.x < r.x + r.width && p.y > r.y && p.y < r.y + r.height) {
			return true;
		}
		return false;
	}

	void filter(InputArray input, OutputArray output, int kernelx, int kernely, int lthresh, int hthresh) {
		Mat closing, opening, dilatation, thresh, erod;
		Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(kernelx, kernely));
		morphologyEx(input, closing, MORPH_CLOSE, kernel);
		morphologyEx(closing, opening, MORPH_OPEN, kernel);
		erode(opening, erod, kernel);
		dilate(erod, dilatation, kernel);
		threshold(dilatation, output, lthresh, hthresh, THRESH_BINARY);
	}

	void detect(Mat& src, InputOutputArray input, vector<Point> &output, int minarea, int maxarea) {
		vector<vector<Point>> hulls;
		vector<vector<Point>> contours;
		findContours(input, contours, RETR_EXTERNAL, CHAIN_APPROX_TC89_KCOS);

		//Rect r(x, y);
		//Mat roi = src(r);
		//Mat color(roi.size(), CV_8UC3, Scalar(0, 125, 125));
		//color.convertTo(color, COLOR_GRAY2BGR);
		//double alpha = 0.3;
		//addWeighted(color, alpha, roi, 1.0 - alpha, 0.0, roi);

		for (int i = 0; i < contours.size(); i++) {

			int area = contourArea(contours[i]);

			if (area > minarea && area < maxarea) {

				vector<Point> hull;
				convexHull(contours[i], hull);
				hulls.push_back(hull);

				Moments M = moments(contours[i]);
				int cx = int(M.m10 / M.m00);
				int cy = int(M.m01 / M.m00);

				Point2f center = Point2f(cx, cy);

				Rect r = boundingRect(contours[i]);
				rectangle(src, r, Scalar(0, 255, 0));
				string s = to_string(cx).append(", ").append(to_string(cy));
				putText(src, s, Point(cx, cy), FONT_HERSHEY_PLAIN, 1, Scalar(0, 255, 255));

				output.push_back(center);
			}
		}
		drawContours(src, hulls, -1, Scalar(0, 0, 255), 3);
	}

	void countCars(vector<Point> &cars, vector<Point> &trackPt, Point x, Point y, int maxrad, int *cars_count) {
		Rect bor(x, y);
		if (cars.size() == 0) {
			for (int i = 0; i < trackPt.size(); i++) {
				if (pointInRect(bor, trackPt[i]))
					cars.push_back(trackPt[i]);
			}
		}
		else {
			for (int j = cars.size() - 1; j >= 0; j--) {
				for (int i = trackPt.size() - 1; i >= 0; i--) {
					int dist = (int)abs(cars[j].x - trackPt[i].x);
					dist += (int)abs(cars[j].y - trackPt[i].y);
					if (dist < maxrad) {
						if (pointInRect(bor, trackPt[i])) {
							cars[j] = trackPt[i];
							trackPt.erase(trackPt.begin() + i);
						}
						else {
							*cars_count += 1;
							cout << *cars_count << endl;
							cars.erase(cars.begin() + j);
							trackPt.erase(trackPt.begin() + i);
							break;
						}
					}
				}
			}
			for (int i = 0; i < trackPt.size(); i++) {
				if (pointInRect(bor, trackPt[i]))
					cars.push_back(trackPt[i]);
			}
		}
	}


}