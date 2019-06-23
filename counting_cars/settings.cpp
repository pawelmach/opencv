#include "settings.h"
#include"cvui.h"
#include"EnhancedWindow.h"

settings::settings(cv::Mat f)
{
	frame = f;
	kernelx = 5;
	kernely = 5;
	lthresh = 220;
	hthresh = 255;
	minarea = 300;
	maxarea = 25000;
	maxrad = 25;
	output = 1;
	path = "";

	history = 500;
	thresh = 16;
	nmixtures = 20;
}

settings::~settings()
{
}

void settings::morphsettings() {
	morph.begin(frame);
	if (!morph.isMinimized()) {
		cvui::text("Kernel x");
		cvui::space(10);
		cvui::trackbar(morph.width() - 20, &kernelx, 1, 10);
		cvui::space(10);
		cvui::text("Kernel y");
		cvui::space(10);
		cvui::trackbar(morph.width() - 20, &kernely, 1, 10);
		cvui::space(10);
		cvui::text("Low threshold");
		cvui::space(10);
		cvui::trackbar(morph.width() - 20, &lthresh, 0, 255);
		cvui::space(10);
		cvui::text("Upper threshold");
		cvui::space(10);
		cvui::trackbar(morph.width() - 20, &hthresh, 0, 255);
	}
	morph.end();
}

void settings::inputsettings() {
	input.begin(frame);
	if (!input.isMinimized()) {
		cvui::text(path);
		cvui::space(10);
		if (cvui::button("Open")) {
			pause = true;
			char filename[260];
			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "MP4\0*.mp4\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a File, yo!";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)) {
				path = filename;
			}
		}
	}
	input.end();
}

void settings::othersettings() {
	other.begin(frame);
	if (!other.isMinimized()) {
		cvui::text("Min area");
		cvui::space(10);
		cvui::trackbar(other.width() - 20, &minarea, 0, 5000);
		cvui::space(10);
		cvui::text("Max area");
		cvui::space(10);
		cvui::trackbar(other.width() - 20, &maxarea, 5000, 30000);
		cvui::space(10);
		cvui::text("Max radius");
		cvui::space(10);
		cvui::trackbar(other.width() - 20, &maxrad, 2, 30);
	}
	other.end();
}

void settings::selectsettings() {
	select.begin(frame);
	if (!select.isMinimized()) {
		if (cvui::button("Detect")) {
			output = 1;
		}
		if (cvui::button("Background Sub")) {
			output = 2;
		}
		if (cvui::button("Threshold")) {
			output = 3;
		}
		if (cvui::button("Select ROI")) {
			selecting = true;
		}
		else {
			selecting = false;
		}
	}
	select.end();
}

void settings::displayResult(int r) {
	result.begin(frame);
	if (!result.isMinimized()) {
		std::string s = "Counted cars: ";
		s.append(std::to_string(r));
		cvui::text(s);
	}
	result.end();
}

void settings::backsubsettings() {
	backsub.begin(frame);
	if (!backsub.isMinimized()) {
		cvui::text("History");
		cvui::space(10);
		cvui::trackbar(backsub.width() - 20, &history, 20, 2000);
		cvui::space(10);
		cvui::text("N mixtures");
		cvui::space(10);
		cvui::trackbar(backsub.width() - 20, &nmixtures, 1, 20);
		cvui::space(10);
		cvui::text("Threshold");
		cvui::space(10);
		cvui::trackbar(backsub.width() - 20, &thresh, 1, 19);
	}
	backsub.end();
}

void settings::controlssettings() {
	control.begin(frame);
	if (!control.isMinimized()) {
		std::string s;
		if (pause) {
			s = "Resume";
		}
		else {
			s = "Pause";
		}
		if (cvui::button(s)) {
			pause = !pause;
		}
		if (cvui::button("Restart")) {
			restart = true;
			pause = true;
		}
	}
	control.end();
}