#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    agentsTexSize = (int)sqrt(numberOfParticles);
    attractorsTexSize = (int)sqrt(numberOfAttractors);
    emittersTexSize = (int)sqrt(numberOfEmitters);
    
    ofDisableArbTex();
    
    //Setup the various ping pong buffers.
    setupAgentsPingPongBuffer(agentsTexSize);
    setupAttractorsPingPongBuffer(attractorsTexSize);
    setupEmittersPingPongBuffer(emittersTexSize);
    
    //setup the shaders.
//    agentUpdateShader.load("Shaders/ParticleEngine/ParticleUpdate");
    attractorsUpdateShader.load("Shaders/ParticleEngine/AttractorUpdate");
    attractorsDrawShader.load("Shaders/ParticleEngine/AttractorDraw");
//    emittersUpdateShader.load("Shaders/ParticleEngine/EmitterUpdate");
    
//    agentsDrawShader.load("Shaders/ParticleEngine/ParticleDraw");
    
//    noiseScale = 533.0f;
//    noiseStrength = 77.0f;
    noiseScale = 1.0f;
    noiseStrength = 1.0f;
    
    //Init the particle positions and age.
    initParticleData(agentsTexSize);
    initAttractorData(attractorsTexSize);
    initEmitterData(emittersTexSize);
}

//--------------------------------------------------------------
void ofApp::update(){
    updateEmitters();
    updateAttractors();
    updateAgents();
}

//--------------------------------------------------------------
void ofApp::draw(){
    drawAttractorDebugData();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::updateEmitters() {
    //Disable blending so that data is written as is to FBO
    ofEnableBlendMode( OF_BLENDMODE_DISABLED );
    
    emittersPingPongBuffer.dst->begin();
        emittersPingPongBuffer.dst->activateAllDrawBuffers();
        
        emittersUpdateShader.begin();
        
            emittersUpdateShader.setUniformTexture("emittersPos", emittersPingPongBuffer.src->getTexture(0), 0);
            updateCommonNoiseParams(emittersUpdateShader);
    
            emittersPingPongBuffer.src->draw(0,0);
    
        emittersUpdateShader.end();
    
    emittersPingPongBuffer.dst->end();
    
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
        
            agentUpdateShader.setUniformTexture("agentsPosAndAge", agentsPingPongBuffer.src->getTexture(0), 0);
            agentUpdateShader.setUniformTexture("emitterPositions", emittersPingPongBuffer.src->getTexture(0), 1);
            agentUpdateShader.setUniformTexture("attractorPositions", attractorsPingPongBuffer.src->getTexture(0), 2);
            agentUpdateShader.setUniform1f("maxParticleAge", maxParticleAge);
            
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
}

//--------------------------------------------------------------
void ofApp::initParticleData(int agentsTexSize) {
    //Init the particle positions and age.
    vector<ofVec4f> particlePosAndAge(numberOfParticles);
    
    particlePoints.setMode(OF_PRIMITIVE_POINTS);
    
    int index = 0;
    for (int y = 0; y < agentsTexSize; y++) {
        for (int x = 0; x < agentsTexSize ; x++) {
            ofVec2f randomPos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
            float randomAge = ofRandom(maxParticleAge);
            particlePosAndAge[index] = ofVec4f(randomPos.x, randomPos.y, 0, randomAge);
            
            
            ofVec2f texCoord;
            texCoord.x = ofMap( x + 0.5f,	0, agentsTexSize,	0.0f, 1.0f ); // the original source has a  '+ 0.5' in it, to get the ceil?
            texCoord.y = ofMap( y + 0.5f,	0, agentsTexSize,	0.0f, 1.0f );
            particlePoints.addTexCoord(texCoord);
            particlePoints.addVertex(randomPos);
            
            index++;
        }
        
    }
    
    //upload source data to ping-pong buffer source.
    agentsPingPongBuffer.src->getTexture(0).loadData((float *)&particlePosAndAge[0].x, agentsTexSize, agentsTexSize, GL_RGBA32F);
}

//--------------------------------------------------------------
void ofApp::initAttractorData( int attractorsTexSize) {
    //Init the particle positions and age.
    vector<ofVec4f> attractorsPosAndSpeed(numberOfAttractors);
    
    attractorPoints.setMode(OF_PRIMITIVE_LINES);
    
    int index = 0;
    for (int y = 0; y < attractorsTexSize; y++) {
        for (int x = 0; x < attractorsTexSize ; x++) {
            ofVec2f randomPos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
            
            float randomSpeed = maxAttractorSpeed;
            attractorsPosAndSpeed[index] = ofVec4f(randomPos.x, randomPos.y, 0, randomSpeed);
            
            
            ofVec2f texCoord;
            texCoord.x = ofMap( x + 0.5f,	0, attractorsTexSize,	0.0f, 1.0f ); // the original source has a  '+ 0.5' in it, to get the ceil?
            texCoord.y = ofMap( y + 0.5f,	0, attractorsTexSize,	0.0f, 1.0f );
            attractorPoints.addTexCoord(texCoord);
            attractorPoints.addVertex(randomPos);
            
            index++;
        }
        
    }
    
    //upload source data to ping-pong buffer source.
    attractorsPingPongBuffer.src->getTexture(0).loadData((float *)&attractorsPosAndSpeed[0].x, attractorsTexSize, attractorsTexSize, GL_RGBA32F);
}


//--------------------------------------------------------------
void ofApp::initEmitterData( int emitterTexSize) {
    
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
    
//    shader.setUniformMatrix4f("MVP", camera.getModelViewProjectionMatrix());
    
}

void ofApp::drawAttractorDebugData() {
    attractorsDrawShader.begin();
    
    attractorsDrawShader.setUniformTexture("tex0", attractorsPingPongBuffer.src->getTexture(), 0);
    attractorPoints.draw();
    
    attractorsDrawShader.end();
}