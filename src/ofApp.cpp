#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    screenDepth = ofGetWidth();
    
    numberOfEmitters = 4;
    numberOfAttractors = 4;
    numberOfParticles = 100;
    
    agentsTexSize = (int)sqrt(numberOfParticles);
    attractorsTexSize = (int)sqrt(numberOfAttractors);
    emittersTexSize = (int)sqrt(numberOfEmitters);
    
    ofDisableArbTex();
    
    //Setup the various ping pong buffers.
    setupAgentsPingPongBuffer(agentsTexSize);
    setupAttractorsPingPongBuffer(attractorsTexSize);
    setupEmittersPingPongBuffer(emittersTexSize);
    
    //setup the shaders.
    agentUpdateShader.load("Shaders/ParticleEngine/ParticleUpdate");
    agentsDrawShader.load("Shaders/ParticleEngine/ParticleDraw");
    attractorsUpdateShader.load("Shaders/ParticleEngine/AttractorUpdate");
    attractorsDrawShader.load("Shaders/ParticleEngine/AttractorDraw");
    emittersUpdateShader.load("Shaders/ParticleEngine/EmitterUpdate");
    emittersDrawShader.load("Shaders/ParticleEngine/EmitterDraw");
    
    noiseScale = 533.0f;
    noiseStrength = 77.0f;
    
    //Init the particle positions and age.
    initParticleData(agentsTexSize);
    initAttractorData(attractorsTexSize);
    initEmitterData(emittersTexSize);
    
    //Setup the GUI
    ofxDatGui::setAssetPath("/Users/satre/Developer/OF/addons/ofxDatGui/");
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    
    noiseStrengthSlider = gui->addSlider("Noise Strength", 1, 200, noiseStrength);
    noiseScaleSlider = gui->addSlider("Noise Scale", 1, 1000, noiseScale);
    
    framerateLabel =  gui->addLabel("Framerate: ");
    gui->setTheme(new ofxDatGuiThemeWireframe());
    gui->onSliderEvent(this, &ofApp::noiseChanged);
    
    //setup the camera
    camera.setDistance(400);
    camera.enableMouseInput();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    updateEmitters();
    updateAttractors();
    updateAgents();
    
    framerateLabel->setLabel("Framerate: " + ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofBackground(0, 0, 0);
    camera.begin();
        ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
        drawAgents();
    camera.end();
    
    gui->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'f') {
        ofToggleFullscreen();
    } else if( key == 'g') {
        gui->setVisible(!gui->getVisible());
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::updateEmitters() {
    //Disable blending so that data is written as is to FBO
    ofEnableBlendMode( OF_BLENDMODE_DISABLED );
    
    ofFbo * src = emittersPingPongBuffer.src;
    ofFbo * dst = emittersPingPongBuffer.dst;
    
    dst->begin();
        dst->activateAllDrawBuffers();
        
        emittersUpdateShader.begin();
        
            emittersUpdateShader.setUniformTexture("emitterData", src->getTexture(0), 0);
            updateCommonNoiseParams(emittersUpdateShader);
    
            src->draw(0,0);
    
        emittersUpdateShader.end();
    
    dst->end();
    
    emittersPingPongBuffer.swap();
}

//--------------------------------------------------------------
void ofApp::updateAttractors() {
    //Disable blending so that data is written as is to FBO
    ofEnableBlendMode( OF_BLENDMODE_DISABLED );
    
    ofFbo * src = attractorsPingPongBuffer.src;
    ofFbo * dst = attractorsPingPongBuffer.dst;
    
    dst->begin();
        dst->activateAllDrawBuffers();
    
        attractorsUpdateShader.begin();
    
            attractorsUpdateShader.setUniformTexture("attractorData", src->getTexture(), 0);
            attractorsUpdateShader.setUniform1f("maxSpeed", maxAttractorSpeed);
            updateCommonNoiseParams(attractorsUpdateShader);
    
            src->draw(0,0);
    
        attractorsUpdateShader.end();
    
    dst->end();
    
    attractorsPingPongBuffer.swap();
}

//--------------------------------------------------------------
void ofApp::updateAgents() {
    //Disable blending so that data is written as is to FBO
    ofEnableBlendMode( OF_BLENDMODE_DISABLED );
    
    agentsPingPongBuffer.dst->begin();
        agentsPingPongBuffer.dst->activateAllDrawBuffers();
        
        agentUpdateShader.begin();
        
            agentUpdateShader.setUniformTexture("agentPosData", agentsPingPongBuffer.src->getTexture(0), 0);
            agentUpdateShader.setUniformTexture("agentVelocityData", agentsPingPongBuffer.src->getTexture(1), 1);
            agentUpdateShader.setUniformTexture("emitterData", emittersPingPongBuffer.src->getTexture(0), 1);
            agentUpdateShader.setUniformTexture("attractorData", attractorsPingPongBuffer.src->getTexture(0), 2);
            agentUpdateShader.setUniform1f("maxParticleAge", maxParticleAge);
            updateCommonNoiseParams(agentUpdateShader);
    
            agentsPingPongBuffer.src->draw(0,0);
    
        agentUpdateShader.end();
    
    agentsPingPongBuffer.dst->end();
    
    agentsPingPongBuffer.swap();
}

//--------------------------------------------------------------
void ofApp::setupAttractorsPingPongBuffer(int attractorsTexSize) {
    //setup the ping-pong buffers.
    ofFbo::Settings settings;
    settings.width = attractorsTexSize;
    settings.height = attractorsTexSize;
    settings.useDepth = false;
    settings.internalformat = GL_RGBA32F;
    settings.textureTarget  = GL_TEXTURE_2D;
    settings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    settings.wrapModeVertical   = GL_CLAMP_TO_EDGE;
    settings.minFilter = GL_NEAREST;					// No interpolation, that would mess up data reads later!
    settings.maxFilter = GL_NEAREST;
    
    attractorsPingPongBuffer.allocate(settings);
}

//--------------------------------------------------------------
void ofApp::setupEmittersPingPongBuffer( int emittersTexSize) {
    //setup the ping-pong buffers.
    ofFbo::Settings settings;
    settings.width = emittersTexSize;
    settings.height = emittersTexSize;
    settings.useDepth = false;
    settings.internalformat = GL_RGBA32F;
    settings.textureTarget  = GL_TEXTURE_2D;
    settings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    settings.wrapModeVertical   = GL_CLAMP_TO_EDGE;
    settings.minFilter = GL_NEAREST;					// No interpolation, that would mess up data reads later!
    settings.maxFilter = GL_NEAREST;
    
    emittersPingPongBuffer.allocate(settings);
}

//--------------------------------------------------------------
void ofApp::setupAgentsPingPongBuffer(int agentsTexSize) {
    //setup the ping-pong buffers.
    ofFbo::Settings settings;
    settings.width = agentsTexSize;
    settings.height = agentsTexSize;
    settings.useDepth = false;
    settings.internalformat = GL_RGBA32F;
    settings.textureTarget  = GL_TEXTURE_2D;
    settings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    settings.wrapModeVertical   = GL_CLAMP_TO_EDGE;
    settings.minFilter = GL_NEAREST;					// No interpolation, that would mess up data reads later!
    settings.maxFilter = GL_NEAREST;
    
    agentsPingPongBuffer.allocate(settings);
    
    //Allocate the shaders for agent pos+age and velocity
    agentsPingPongBuffer.src->createAndAttachTexture(GL_RGBA32F, 0);
    agentsPingPongBuffer.src->createAndAttachTexture(GL_RGB32F, 1); //Velocity only has 3 components.
    agentsPingPongBuffer.dst->createAndAttachTexture(GL_RGBA32F, 0);
    agentsPingPongBuffer.dst->createAndAttachTexture(GL_RGB32F, 1); //Velocity only has 3 components.
}

//--------------------------------------------------------------
void ofApp::initParticleData(int agentsTexSize) {
    //Init the particle positions and age.
    vector<ofVec4f> particlePosAndAge(numberOfParticles);
    vector<ofVec3f> particleVelocities(numberOfParticles);
    
    particlePoints.setMode(OF_PRIMITIVE_POINTS);
    
    int index = 0;
    for (int y = 0; y < agentsTexSize; y++) {
        for (int x = 0; x < agentsTexSize ; x++) {
            ofVec3f randomPos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(screenDepth));
            float randomAge = ofRandom(maxParticleAge);
            particlePosAndAge[index] = ofVec4f(randomPos.x, randomPos.y, randomPos.z, randomAge);
            
            
            ofVec2f texCoord;
            texCoord.x = ofMap( x + 0.5f,	0, agentsTexSize,	0.0f, 1.0f ); // the original source has a  '+ 0.5' in it, to get the ceil?
            texCoord.y = ofMap( y + 0.5f,	0, agentsTexSize,	0.0f, 1.0f );
            particlePoints.addTexCoord(texCoord);
            particlePoints.addVertex(randomPos);
            
            particleVelocities[index] = ofVec3f(ofRandom(2.0f)), ofRandom(2.0f), ofRandom(2.0f);
            
            index++;
        }
        
    }
    
    //upload source data to ping-pong buffer source.
    agentsPingPongBuffer.src->getTexture(0).loadData((float *)&particlePosAndAge[0].x, agentsTexSize, agentsTexSize, GL_RGBA);
    agentsPingPongBuffer.dst->getTexture(1).loadData((float *)&particleVelocities[0].x, agentsTexSize, agentsTexSize, GL_RGB);

}

//--------------------------------------------------------------
void ofApp::initAttractorData( int attractorsTexSize) {
    //Init the particle positions and age.
    vector<ofVec4f> attractorsPosAndSpeed(numberOfAttractors);
    
    attractorPoints.setMode(OF_PRIMITIVE_POINTS);
    
    int index = 0;
    for (int y = 0; y < attractorsTexSize; y++) {
        for (int x = 0; x < attractorsTexSize ; x++) {
            //Use the height as a depth value as well.
            ofVec3f randomPos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(screenDepth));
            
            float randomSpeed = ofRandom(maxAttractorSpeed);
            attractorsPosAndSpeed[index] = ofVec4f(randomPos.x, randomPos.y, randomPos.z, randomSpeed);
            
            
            ofVec2f texCoord;
            texCoord.x = ofMap( x + 0.5f,	0, attractorsTexSize,	0.0f, 1.0f ); // the original source has a  '+ 0.5' in it, to get the ceil?
            texCoord.y = ofMap( y + 0.5f,	0, attractorsTexSize,	0.0f, 1.0f );
            attractorPoints.addTexCoord(texCoord);
            attractorPoints.addVertex(randomPos);
            
            index++;
        }
        
    }
    
    //upload source data to ping-pong buffer source.
    attractorsPingPongBuffer.src->getTexture(0).loadData((float *)&attractorsPosAndSpeed[0].x, attractorsTexSize, attractorsTexSize, GL_RGBA);
}


//--------------------------------------------------------------
void ofApp::initEmitterData( int emitterTexSize) {
    //Init the particle positions and age.
    vector<ofVec4f> emittersPosAndSpeed(numberOfEmitters);
    
    emitterPoints.setMode(OF_PRIMITIVE_POINTS);
    
    int index = 0;
    for (int y = 0; y < emitterTexSize; y++) {
        for (int x = 0; x < emitterTexSize ; x++) {
            //Use the height as a depth value as well.
            ofVec3f randomPos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(screenDepth));
            
            float randomSpeed = ofRandom(maxEmitterSpeed);
            emittersPosAndSpeed[index] = ofVec4f(randomPos.x, randomPos.y, randomPos.z, randomSpeed);
            
            
            ofVec2f texCoord;
            texCoord.x = ofMap( x + 0.5f,	0, emitterTexSize,	0.0f, 1.0f ); // the original source has a  '+ 0.5' in it, to get the ceil?
            texCoord.y = ofMap( y + 0.5f,	0, emitterTexSize,	0.0f, 1.0f );
            emitterPoints.addTexCoord(texCoord);
            emitterPoints.addVertex(randomPos);
            
            index++;
        }
        
    }
    
    //upload source data to ping-pong buffer source.
    emittersPingPongBuffer.src->getTexture(0).loadData((float *)&emittersPosAndSpeed[0].x, emitterTexSize, emitterTexSize, GL_RGBA);
}

//--------------------------------------------------------------
/**
 *  Updates the noise params for the given shader.
 */
void ofApp::updateCommonNoiseParams(ofShader & shader) {
    
    shader.setUniform1f("noiseStrength", noiseStrength);
    shader.setUniform1f("noiseScale", noiseScale);
    shader.setUniform1i("screenWidth", ofGetWidth());
    shader.setUniform1i("screenHeight", ofGetHeight());
    shader.setUniform1i("screenDepth", screenDepth);
}

void ofApp::drawAttractorDebugData() {
    attractorsDrawShader.begin();
    attractorsDrawShader.setUniform1i("screenWidth", ofGetWidth());
    attractorsDrawShader.setUniform1i("screenHeight", ofGetHeight());
    attractorsDrawShader.setUniform1i("screenDepth", screenDepth);
    
    attractorsDrawShader.setUniformTexture("tex0", attractorsPingPongBuffer.src->getTexture(), 0);
    attractorsDrawShader.setUniform1f("maxSpeed", maxAttractorSpeed);
    
//    attractorsPingPongBuffer.src->draw(0,0, ofGetWidth(), ofGetHeight());
    attractorPoints.draw();
    
    attractorsDrawShader.end();
}

void ofApp::drawEmitterDebugData() {
    emittersDrawShader.begin();
    emittersDrawShader.setUniform1i("screenWidth", ofGetWidth());
    emittersDrawShader.setUniform1i("screenHeight", ofGetHeight());
    emittersDrawShader.setUniform1i("screenDepth", screenDepth);
    
    emittersDrawShader.setUniformTexture("tex0", emittersPingPongBuffer.src->getTexture(), 0);
    emittersDrawShader.setUniform1f("maxSpeed", maxEmitterSpeed);
    
    emitterPoints.draw();
    
    emittersDrawShader.end();
}

void ofApp::drawAgents() {
    agentsDrawShader.begin();
    agentsDrawShader.setUniform1i("screenWidth", ofGetWidth());
    agentsDrawShader.setUniform1i("screenHeight", ofGetHeight());
    agentsDrawShader.setUniform1i("screenDepth", screenDepth);
    agentsDrawShader.setUniformTexture("agentsPosData", agentsPingPongBuffer.src->getTexture(0), 0);
    
    particlePoints.draw();
    
    emittersDrawShader.end();
}


void ofApp::noiseChanged( ofxDatGuiSliderEvent e) {
    if( e.target == noiseStrengthSlider) {
        noiseStrength = e.value;
    } else if( e.target == noiseScaleSlider) {
        noiseScale = e.value;
    }
}

void ofApp::windowResized(int w, int h) {
    screenDepth = h;
}
