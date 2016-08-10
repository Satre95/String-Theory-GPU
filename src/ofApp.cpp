#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    int texSize = (int)sqrt(numberOfParticles);
    
    particleData.allocate(texSize, texSize, GL_RGBA);
//    ofBuffer shaderBuffer = ofBufferFromFile("Shaders/ParticleEngine.glsl");
    particleData.load("Shaders/ParticleEngine/ParticleEngine");

}

//--------------------------------------------------------------
void ofApp::update(){
    particleData.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    auto rect = ofRectangle(0, 0, 500, 500);
    particleData.draw(rect);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
