
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("CentroidTracker Demo: Moving Rect");
    centroidTracker = std::make_shared<CentroidTracker>(20);
    rects.clear();
    objects.clear();
    rectCenter.set(ofGetWidth()/2, ofGetHeight()/2);
    rectVel.set(ofRandom(-4,4), ofRandom(-4,4));
    rectW = ofRandom(60,120);
    rectH = ofRandom(60,120);
    rectWVel = ofRandom(-1,1);
    rectHVel = ofRandom(-1,1);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(rectVisible){
        rectCenter += rectVel;
        rectW += rectWVel;
        rectH += rectHVel;
        if(rectCenter.x-rectW/2 < 0 || rectCenter.x+rectW/2 > ofGetWidth()) rectVel.x *= -1;
        if(rectCenter.y-rectH/2 < 0 || rectCenter.y+rectH/2 > ofGetHeight()) rectVel.y *= -1;
        if(rectW < 40 || rectW > 160) rectWVel *= -1;
        if(rectH < 40 || rectH > 160) rectHVel *= -1;
        if(ofRandom(1) < 0.02) rectVel.set(ofRandom(-4,4), ofRandom(-4,4));
        if(ofRandom(1) < 0.02) rectWVel = ofRandom(-1,1);
        if(ofRandom(1) < 0.02) rectHVel = ofRandom(-1,1);
        int x1 = rectCenter.x - rectW/2;
        int y1 = rectCenter.y - rectH/2;
        int x2 = rectCenter.x + rectW/2;
        int y2 = rectCenter.y + rectH/2;
        rects.clear();
        rects.push_back({x1, y1, x2, y2});
    }else{
        rects.clear();
    }
    if(rectVisible && ofRandom(1) < 0.02){
        rectVisible = false;
        reappearWait = (int)ofRandom(60,180);
        disappearFrame = 0;
    }
    if(!rectVisible){
        disappearFrame++;
        if(disappearFrame > reappearWait){
            rectCenter.set(ofRandom(100, ofGetWidth()-100), ofRandom(100, ofGetHeight()-100));
            rectVel.set(ofRandom(-4,4), ofRandom(-4,4));
            rectW = ofRandom(60,120);
            rectH = ofRandom(60,120);
            rectWVel = ofRandom(-1,1);
            rectHVel = ofRandom(-1,1);
            rectVisible = true;
        }
    }

    // track rects
    objects = centroidTracker->update(rects);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(240);
    if(rectVisible && !rects.empty()){
        ofSetColor(0, 180, 255, 80);
        auto& r = rects[0];
        ofDrawRectangle(r[0], r[1], r[2]-r[0], r[3]-r[1]);
    }
    for(const auto& obj : objects){
        int id = obj.first;
        const auto& path = centroidTracker->path_keeper[id];
        if(path.size() > 1){
            int k = 1;
            for(size_t i=1; i<path.size(); ++i){
                int thickness = int(std::sqrt(20.0f / float(k + 1) * 2.5f));
                ofSetColor(255,0,0);
                ofSetLineWidth(thickness);
                ofDrawLine(path[i-1].first, path[i-1].second, path[i].first, path[i].second);
                k++;
            }
        }
    }
    ofSetLineWidth(1);
    ofSetColor(0);
    
    // for tracked objects
    for(const auto& obj : objects){
        int id = obj.first;
        int cx = obj.second.first;
        int cy = obj.second.second;
        ofDrawCircle(cx, cy, 8);
        ofDrawBitmapStringHighlight("ID:"+ofToString(id), cx+10, cy-10);
    }
    
    ofSetColor(0);
    ofDrawBitmapString("", 20, 20);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::keyPressed(int key){}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}