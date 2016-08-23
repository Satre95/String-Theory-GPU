#version 400 core
//--------------------------------------------------------------------------------
//MARK: Uniforms and In/Out params.
//The data textures
//Need to store 2 vectors (6 vals), so two textures are needed.
uniform sampler2D agentData;
uniform sampler2D agentVelocityData;
uniform sampler2D emitterData;
uniform sampler2D attractorData;

//Other parameters
uniform int screenWidth;
uniform int screenHeight;
uniform int screenDepth;
uniform int numberOfAttractors;
uniform int numberOfEmitters;
uniform float ageStep = 0.1f;

uniform float evenHorizon; //The absorption threshold.
uniform float gravityConstant; //The Newtonian gravity constant G
uniform float attractorMass; //The mass of the attractors.
uniform float agentMass; //The mass of the agents;

in vec2 sampleCoord;

layout (location = 0) out vec4 newPosAndAge;
layout (location = 1) out vec3 newVelocity;

//--------------------------------------------------------------------------------
//MARK: Function Declarations
vec3 calculateNetForce( vec3 agentPos);
void applyNetForce( vec3 netForce, out vec3 agentPos, out vec3 velocity);

//--------------------------------------------------------------------------------
//MARK: Main shader body
void main(){
    //Get the current position of the particle from the agent tex.
    vec4 agentPosAndAge = texture(agentData, sampleCoord);
    
    vec3 agentPos = agentPosAndAge.xyz;
    float age = agentPosAndAge.w;
    vec3 velocity = texture(agentVelocityData, sampleCoord).xyz;
    
    
    vec3 netForce = calculateNetForce( agentPos);
    
    //After this call, the agentPos and velocity vars will have been updated.
    applyNetForce(netForce, agentPos, velocity);
    
    //Update the age
    age += ageStep;
    
    //Write out the new age ang position data.
    newPosAndAge = vec4(agentPos.x, agentPos.y, agentPos.z, age);
    //Write out the new velocity data.
    newVelocity = velocity;
}

/**
 *  Loops over all attractors and to calculate the net force.
 *
 *  @param agentPos The current position of the agent
 *
 *  @return The net force vector.
 */
vec3 calculateNetForce( vec3 agentPos) {
    float attractorsDimen = sqrt(numberOfAttractors);
    
    //In each texture, dimension of square texture is sqrt(numberOf_)
    //Since tex coords are normalized, the step size is the 1.0 / sqrt(numberOf_)
    float attractorStepSize = 1.0f / attractorsDimen;
    float emitterStepSize = 1.0f / sqrt(numberOfEmitters);
    
    
    //loop over the attractors and use that to determine net force.
    vec3 netForce = vec3(0);
    float yStep = 0;
    while (yStep <= attractorsDimen) {
        
        float xStep = 0;
        while (xStep <= attractorsDimen) {
            vec4 anAttractor = texture(attractorData, vec2(xStep, yStep));
            vec3 anAttractorPos = anAttractor.xyz;
            
            //Newton's equation of gravity F = - (G * M * m ) / (r^2)
            float forceScalar = (-1.0f * gravityConstant * attractorMass * agentMass) / pow(distance(agentPos, anAttractorPos), 2.0f);
            //unit vector point from agent to attractor.
            vec3 forceDir = normalize(anAttractorPos - agentPos);
            //The final force vector.
            vec3 forceVector = forceScalar * forceDir;
            
            //Add to net force
            netForce += forceVector;
            
            xStep += attractorStepSize;
        }
        
        yStep += attractorStepSize;
    }
    
    return netForce;
}

/**
 *  Applies the given net force to the given position of the agent.
 *
 *  @param netForce The force vector.
 *  @param agentPos The current position. After func executes, this will hold the updated position.
 */
void applyNetForce( vec3 netForce, out vec3 agentPos, out vec3 velocity) {
    //calculate the acceleration (F = m*a)
    vec3 acceleration = netForce / agentMass;
    //Update the velocity
    velocity += acceleration;
    //new position is based on updated velocity.
    agentPos += velocity;
}