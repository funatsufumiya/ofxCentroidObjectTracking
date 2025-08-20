
#include "ofApp.h"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>

namespace dnn = cv::dnn;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Centroid Object Tracking");
    grabber.setDeviceID(0);
    grabber.setDesiredFrameRate(30);
    grabber.initGrabber(400, 300);

    centroidTracker = new CentroidTracker(20);

    std::string modelTxt = ofToDataPath("model/deploy.prototxt", true);
    std::string modelBin = ofToDataPath("model/res10_300x300_ssd_iter_140000.caffemodel", true);
    net = dnn::readNetFromCaffe(modelTxt, modelBin);
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    if (grabber.isFrameNew()) {
        ofPixels & pixels = grabber.getPixels();
        cameraFrame = cv::Mat(grabber.getHeight(), grabber.getWidth(), CV_8UC3, pixels.getData());
        cv::cvtColor(cameraFrame, cameraFrame, cv::COLOR_RGB2BGR);

        auto inputBlob = dnn::blobFromImage(cameraFrame, 1.0, cv::Size(400, 300), cv::Scalar(104.0, 177.0, 123.0));
        net.setInput(inputBlob);
        auto detection = net.forward("detection_out");
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

        std::vector<std::vector<int>> boxes;
        float confidenceThreshold = 0.2;
        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);
            if (confidence > confidenceThreshold) {
                int xLeftTop = static_cast<int>(detectionMat.at<float>(i, 3) * cameraFrame.cols);
                int yLeftTop = static_cast<int>(detectionMat.at<float>(i, 4) * cameraFrame.rows);
                int xRightBottom = static_cast<int>(detectionMat.at<float>(i, 5) * cameraFrame.cols);
                int yRightBottom = static_cast<int>(detectionMat.at<float>(i, 6) * cameraFrame.rows);
                boxes.push_back({xLeftTop, yLeftTop, xRightBottom, yRightBottom});
            }
        }

        objects = centroidTracker->update(boxes);

        displayFrame = cameraFrame.clone();
        for (const auto& box : boxes) {
            cv::rectangle(displayFrame, cv::Rect(box[0], box[1], box[2]-box[0], box[3]-box[1]), cv::Scalar(0,255,0), 2);
        }
        if (!objects.empty()) {
            for (auto obj: objects) {
                cv::circle(displayFrame, cv::Point(obj.second.first, obj.second.second), 4, cv::Scalar(255, 0, 0), -1);
                std::string ID = std::to_string(obj.first);
                cv::putText(displayFrame, ID, cv::Point(obj.second.first - 10, obj.second.second - 10),
                            cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
            }
            //drawing the path
            for (auto obj: objects) {
                int k = 1;
                for (int i = 1; i < centroidTracker->path_keeper[obj.first].size(); i++) {
                    int thickness = int(std::sqrt(20 / float(k + 1) * 2.5));
                    cv::line(displayFrame,
                             cv::Point(centroidTracker->path_keeper[obj.first][i - 1].first, centroidTracker->path_keeper[obj.first][i - 1].second),
                             cv::Point(centroidTracker->path_keeper[obj.first][i].first, centroidTracker->path_keeper[obj.first][i].second),
                             cv::Scalar(0, 0, 255), thickness);
                    k += 1;
                }
            }
        }

        cv::cvtColor(displayFrame, displayFrame, cv::COLOR_BGR2RGB);
        if (!displayImage.isAllocated() || displayImage.getWidth() != displayFrame.cols || displayImage.getHeight() != displayFrame.rows) {
            displayImage.allocate(displayFrame.cols, displayFrame.rows, OF_IMAGE_COLOR);
        }
        displayImage.setFromPixels(displayFrame.ptr<unsigned char>(), displayFrame.cols, displayFrame.rows, OF_IMAGE_COLOR);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (displayImage.isAllocated()) {
        displayImage.draw(0, 0);
    } else {
        ofDrawBitmapString("No camera image", 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}