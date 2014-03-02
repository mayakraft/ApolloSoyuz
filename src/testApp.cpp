#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    apolloSoyuz.setup(0);
}

//--------------------------------------------------------------
void testApp::update(){
    apolloSoyuz.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    apolloSoyuz.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    apolloSoyuz.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}