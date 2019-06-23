#pragma once
#ifndef _SETTING_H_

#define _SETTING_H_

#include<Windows.h>
#include<opencv2/opencv.hpp>

#include"EnhancedWindow.h"

class settings
{
public:
	settings(cv::Mat f);
	~settings();

	EnhancedWindow input = EnhancedWindow(20, 20, 400, 80, "Input file");
	EnhancedWindow morph = EnhancedWindow(20, 120, 200, 330, "Morph Settings");
	EnhancedWindow other = EnhancedWindow(20, 470, 200, 250, "Other Settings");
	EnhancedWindow select = EnhancedWindow(240, 120, 180, 200, "Select Output");
	EnhancedWindow result = EnhancedWindow(240, 340, 180, 50, "Result");
	EnhancedWindow backsub = EnhancedWindow(240, 410, 180, 250, "BG Sub Settings");
	EnhancedWindow control = EnhancedWindow(500, 600, 100, 100, "Controls");

	std::string path;
	cv::Mat frame;
	int kernelx;
	int kernely;
	int lthresh;
	int hthresh;
	int minarea;
	int maxarea;
	int maxrad;

	int history;
	int nmixtures;
	int thresh;

	bool pause = false;
	bool restart = false;

	int output;
	bool selecting;

	void morphsettings();

	void inputsettings();

	void selectsettings();
	
	void othersettings();

	void displayResult(int r);

	void backsubsettings();

	void controlssettings();
};

#endif // !SETTING_H
