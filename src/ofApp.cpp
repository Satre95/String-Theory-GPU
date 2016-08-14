#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    int texSize = (int)sqrt(numberOfParticles);
    
    //setup the ping-pong buffers.
    ofFbo::Settings settings;
    settings.width = texSize;
    settings.height = texSize;
    settings.useDepth = false;
    settings.internalformat = GL_RGBA32F;
    settings.textureTarget  = GL_TEXTURE_2D;
    settings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    settings.wrapModeVertical   = GL_CLAMP_TO_EDGE;
    settings.minFilter = GL_NEAREST;					// No interpolation, that would mess up data reads later!
    settings.maxFilter = GL_NEAREST;

    swapBuffer.allocate(settings);
    
    //Init the particle positions and age.
    vector<ofVec4f> particlePosAndAge(numberOfParticles);
    int index = 0;
    for (int y = 0; y < texSize; y++) {
        for (int x = 0; x < texSize ; x++) {
            ofVec3f randomPos = randomPointOnSphere() * ofRandom(5.0f);
            float randomAge = ofRandom(maxParticleAge);
            particlePosAndAge[index] = ofVec4f(randomPos.x, randomPos.y, randomPos.z, randomAge);
            
            
            ofVec2f texCoord;
            texCoord.x = ofMap( x + 0.5f,	0, texSize,	0.0f, 1.0f ); // the original source has a  '+ 0.5' in it, to get the ceil?
            texCoord.y = ofMap( y + 0.5f,	0, texSize,	0.0f, 1.0f );
            particlePoints.addTexCoord(texCoord);
            particlePoints.addVertex(randomPos);
            
            index++;
        }
        
    }
    
    //upload source data to ping-pong buffer source.
    swapBuffer.src->getTexture(0).loadData((float *)&particlePosAndAge[0].x, texSize, texSize, GL_RGBA32F);
    
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    auto rect = ofRectangle(100, 100, 500, 500);
    particleData.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

ofVec3f ofApp::randomPointOnSphere()
{
    float lambda = ofRandom(1.0f);
    float u = ofRandom(-1.0f, 1.0f);
    float phi = ofRandom( 2.0 * PI );
    
    ofVec3f p;
    p.x = pow(lambda, 1/3) * sqrt(1.0 - u * u) * cos(phi);
    p.y = pow(lambda, 1/3) * sqrt(1.0 - u * u) * sin(phi);
    p.z = pow(lambda, 1/3) * u;
    
    return p;
}