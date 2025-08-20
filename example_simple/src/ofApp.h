#pragma once

#include "ofMain.h"
#include "CentroidTracker.h"

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

    ofPoint rectCenter;
    ofPoint rectVel;
    float rectW = 80, rectH = 80;
    float rectWVel = 0, rectHVel = 0;
    bool rectVisible = true;
    int disappearFrame = 0;
    int reappearWait = 0;

	std::vector<std::vector<int>> rects;

	shared_ptr<CentroidTracker> centroidTracker = nullptr;
	std::vector<std::pair<int, std::pair<int, int>>> objects;
};