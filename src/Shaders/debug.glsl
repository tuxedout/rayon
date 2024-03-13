#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

uniform vec2 u_resolution;
uniform vec2 u_mouse;

//max count of reflections/refractions
const float MAX_STEP = 30.0;
//maximum distance ray traveling
const float MAX_DISTANCE = 4.5;

const float EPSILON = 0.00001;

//samples per pixed
const float SAMPLES = 96.0;

const float PI =     3.1415926535897932384626433832795;
const float TWO_PI = 6.283185307179586476925286766559;

//these both seems useles now
const float BIAS = 0.01;
const float MAX_DEPTH = 10.0;

//global mouse position
vec2  m;

const vec3 BLACK = vec3(0.0,0.0,0.0);
const vec3 WHITE = vec3(1.0,1.0,1.0);

void main(){
    vec2 uv = fragTexCoord;
    uv -= 0.5;
    uv *= 2.0;

    // Output to screen
    finalColor = vec4(uv.x,uv.y, 0.0, 1.0);
}