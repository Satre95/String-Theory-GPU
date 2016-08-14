#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    int agentsTexSize = (int)sqrt(numberOfParticles);
    int attractorsTexSize = (int)sqrt(numberOfAttractors);
    int emittersTexSize = (int)sqrt(numberOfEmitters);
    
    //Setup the various ping pong buffers.
    setupAgentsPingPongBuffer(agentsTexSize);
    setupAttractorsPingPongBuffer(attractorsTexSize);
    setupEmittersPingPongBuffer(emittersTexSize);
    
    //setup the shaders.
    agentUpdateShader.load("Shaders/ParticleEngine/ParticleUpdate");
    attractorsUpdateShader.load("Shaders/ParticleEngine/AttractorUpdate");
    emittersUpdateShader.load("Shaders/ParticleEngine/EmitterUpdate");
    
    drawShader.load("Shaders/ParticleEngine/ParticleDraw");
    
    //Init the particle positions and age.
    initParticleData(agentsTexSize);
}

//--------------------------------------------------------------
void ofApp::update(){
    updateEmitters();
    updateAttractors();
    updateAgents();
}

//--------------------------------------------------------------
void ofApp::draw(){
    auto rect = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
//    agentsPingPongBuffer.src->draw(rect);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

void ofApp::updateEmitters() {
    //Disable blending so that data is written as is to FBO
    ofEnableBlendMode( OF_BLENDMODE_DISABLED );
    
    emittersPingPongBuffer.dst->begin();
        emittersPingPongBuffer.dst->activateAllDrawBuffers();
        
        emittersUpdateShader.begin();
        
            emittersUpdateShader.setUniformTexture("emittersPos", emittersPingPongBuffer.src->getTexture(0), 0);
            emittersUpdateShader.setUniform1f("noiseScale", noiseScale);
            emittersUpdateShader.setUniform1f("noiseStrength", noiseStrength);
        
            emittersPingPongBuffer.src->draw(0,0);
    
        emittersUpdateShader.end();
    
    emittersPingPongBuffer.dst->end();
    
    emittersPingPongBuffer.swap();
}

void ofApp::updateAttractors() {
    //Disable blending so that data is written as is to FBO
    ofEnableBlendMode( OF_BLENDMODE_DISABLED );
    
    attractorsPingPongBuffer.dst->begin();
        attractorsPingPongBuffer.dst->activateAllDrawBuffers();
    
        attractorsUpdateShader.begin();
    
            attractorsUpdateShader.setUniformTexture("attractorPos", attractorsPingPongBuffer.src->getTexture(0), 0);
            attractorsUpdateShader.setUniform1f("noiseScale", noiseScale);
            attractorsUpdateShader.setUniform1f("noiseStrength", noiseStrength);
    
    
            attractorsPingPongBuffer.src->draw(0,0);
    
        attractorsUpdateShader.end();
    
    attractorsPingPongBuffer.dst->end();
    
    attractorsPingPongBuffer.swap();
}

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

void ofApp::initParticleData(int agentsTexSize) {
    //Init the particle positions and age.
    vector<ofVec4f> particlePosAndAge(numberOfParticles);
    int index = 0;
    for (int y = 0; y < agentsTexSize; y++) {
        for (int x = 0; x < agentsTexSize ; x++) {
            ofVec3f randomPos = randomPointOnSphere() * ofRandom(5.0f);
            float randomAge = ofRandom(maxParticleAge);
            particlePosAndAge[index] = ofVec4f(randomPos.x, randomPos.y, randomPos.z, randomAge);
            
            
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