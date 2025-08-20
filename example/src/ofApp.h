#pragma once

#include "ofMain.h"
#include "CentroidTracker.h"

#include <opencv2/dnn.hpp>

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	ofVideoGrabber grabber;
	shared_ptr<CentroidTracker> centroidTracker = nullptr;
	cv::dnn::Net net;
	ofImage displayImage;
	cv::Mat cameraFrame, displayFrame;
	std::vector<std::pair<int, std::pair<int, int>>> objects;
};