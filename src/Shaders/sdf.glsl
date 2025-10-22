#version 450

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

uniform vec2 u_resolution;
uniform vec2 u_mouse;

//global mouse position
vec2  m;

//global mouse position
const int MATERIALS_COUNT = 8;

struct Material {
    float reflectivity;
    float eta;
    vec3 emission;
    vec3 absorption;
};

const Material red = Material(0.4, 0.7, vec3(1.0, 0.0, 0.0), vec3(0.0,1.0,1.0));
const Material green = Material(0.4, 1.2,  vec3(0.0, 1.0, 0.0), vec3(1.0,0.0,1.0));
const Material blue = Material(0.4, 1.2,  vec3(0.0, 0.0, 1.0), vec3(1.0,1.0,0.0));
const Material white = Material(0.4, 1.2,  vec3(1.0, 1.0, 1.0), vec3(0.0,0.0,0.0));
const Material black = Material(0.4, 1.2,  vec3(0.0, 0.0, 0.0), vec3(1.0,1.0,1.0));
const Material yellow = Material(0.7, 1.2,  vec3(1.0, 1.0, 0.0), vec3(0.0,0.0,0.0));
const Material cyan = Material(0.8, 1.2,  vec3(0.0, 1.0, 1.0), vec3(0.0,0.0,0.0));
const Material last = Material(0.8, 1.2,  vec3(1.0, 0.0, 1.0), vec3(0.0,0.0,0.0));

const Material mats[MATERIALS_COUNT] = {red, green, blue, white, black, yellow, cyan, last};

const int RED_IDX   = 0;
const int GREEN_IDX = 1;
const int BLUE_IDX  = 2;
const int WHITE_IDX = 3;
const int BLACK_IDX = 4;
const int YELLOW_IDX = 5;
const int CYAN_IDX = 6;
const int LAST_IDX = 7;


const vec3 BLACK = vec3(0.0,0.0,0.0);
const vec3 WHITE = vec3(1.0,1.0,1.0);

//result of scene calculation
//result of scene calculation
struct ssDot{
    float sd; //signed distance
    int mat_idx; //material index
};

ssDot unionOp(ssDot a, ssDot b){
    if (a.sd < b.sd){
        return a;
    }else{
        return b;
    }
}

float box(float x, float y, float cx, float cy, vec2 size) {
    vec2 p = vec2(x-cx,y-cy);

    //size -= vec2(radius);
    vec2 d = abs(p) - size;
    return max(-0.01, min(max(d.x, d.y), 0.0) + length(max(d, 0.0))) ;
}

float circle(float x,float y,float cx,float cy,float r){
    return length(vec2(x-cx,y-cy))-r;
}

ssDot r1 = ssDot(0.0, RED_IDX);
ssDot r2 = ssDot(0.0, GREEN_IDX);//
ssDot r3 = ssDot(0.0, BLUE_IDX);
ssDot r4 = ssDot(0.0, WHITE_IDX);
ssDot r5 = ssDot(0.0, BLACK_IDX);
ssDot r6 = ssDot(0.0, YELLOW_IDX);
ssDot r7 = ssDot(0.0, CYAN_IDX);
ssDot r8 = ssDot(0.0, LAST_IDX);
ssDot r9 = ssDot(0.0, RED_IDX);

void scene_update(float x,float y){
    r1.sd = circle(x, y, 0.5, 0.5, 0.125);
    //ssDot r2 = ssDot(circle(x,y,m.x,m.y, 0.25),  2.0, 1.2,vec3(0.7, 0.1, 0.5), vec3(0.8,0.6,0.0));
    r2.sd = circle(x,y,m.x,m.y, 0.1);
    r3.sd = circle(x,y,-0.3,0.0, 0.1);
    r4.sd = box(x,y,-1.0,-0.1,vec2(0.2,0.4));
    r5.sd = circle(x,y,-0.3,0.4, 0.1);
    r6.sd = circle(x,y, 0.4,-0.8, 0.1);
    r7.sd = circle(x,y, 0.4,-0.5, 0.1);
    r8.sd = box(x,y,1.0,0.1,vec2(0.1,0.2));
    r9.sd = box(x,y,0.1,0.1,vec2(0.2,0.1));
}


ssDot scene(float x,float y){
    scene_update(x,y);
    ssDot r = unionOp(r1,r2);
    //r = unionOp(r, r3);
    r = unionOp(r, r4);
    //r = unionOp(r, r5);
    //r = unionOp(r, r6);
    //r = unionOp(r, r7);
    //r  = unionOp(r, r8);
    r  = unionOp(r, r9);
    return r;
}

float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
    vec2(12.9898,78.233)))*
    43758.5453123);
}

vec2 calculateNormal(vec2 p) {
    const float h = 0.01; // Малое значение для смещения
    const vec2 dx = vec2(h, 0.0);
    const vec2 dy = vec2(0.0, h);

    // Вычисляем градиент SDF, используя центральные разности
    float sd = scene(p.x, p.y).sd;
    float gx = scene(p.x + dx.x, p.y).sd - scene(p.x - dx.x, p.y).sd;
    float gy = scene(p.x, p.y + dy.y).sd - scene(p.x, p.y - dy.y).sd;

    // Нормализуем градиент, чтобы получить нормаль
    vec2 n = normalize(vec2(gx, gy));

    // Инвертируем нормаль, если точка находится внутри объекта
    return sd > 0.0 ? n : -n;
}


vec3 sampleFun(vec2 p) {
    vec3 sum = BLACK;

    ssDot r = scene(p.x, p.y);

    return sum;
}

void main(){
    vec2 uv = fragTexCoord;
    uv -= 0.5;
    uv *= 2.0;
    uv.x *= u_resolution.x/u_resolution.y;// / u_resolution.xy;

    //uv *= 2.0;

    //uv -= 1.0;

    // Calculate derivatives of UV coordinates
    vec2 ddxUV = dFdx(uv);
    vec2 ddyUV = dFdy(uv);

    // Use derivatives to adjust UV coordinates for antialiasing
    vec2 aaUV = uv + (ddxUV + ddyUV);

    m  = (u_mouse.xy) / u_resolution.xy;

    m -= 0.5;
    m *= 2.0;
    m.x *= u_resolution.x/u_resolution.y;

    m.y = -m.y;

    ssDot sv = scene(uv.x, uv.y);

    //vec2 n = calculateNormal(uv);
    vec3 col = vec3(sv.sd, sv.mat_idx, 0.0);

    finalColor = vec4(col, 1.0);
}