#version 450

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

uniform vec2 u_resolution;
uniform vec2 u_mouse;

//max count of reflections/refractions
const float MAX_STEP = 25.0;
//maximum distance ray traveling
const float MAX_DISTANCE = 5.5;

const float EPSILON = 0.0001;

//samples per pixed
const float SAMPLES = 64.0;

const float PI =     3.1415926535897932384626433832795;
const float TWO_PI = 6.283185307179586476925286766559;

//these both seems useles now
const float BIAS = 0.000001;
const float MAX_DEPTH = 10.0;

//global mouse position
vec2  m;


const vec3 BLACK = vec3(0.0,0.0,0.0);
const vec3 WHITE = vec3(1.0,1.0,1.0);

//result of scene calculation
struct ssDot{
    float sd; //signed distance
    float reflectivity; //reflectivity coef.
    float eta; //refractivity coef.
    vec3 emission; //emission color and strength
    vec3 absorption; //absorption color and strength
};

ssDot unionOp(ssDot a, ssDot b){
    if (a.sd < b.sd){
        return a;
    }else{
        return b;
    }
}

float fresnel(float cosi, float cost, float etai,float etat) {
    float rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
    float rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
    return (rs * rs + rp * rp) * 0.5f;
}

vec3 beerLambert(vec3 a, float d) {
    return vec3(exp(-a.r * d), exp(-a.g * d), exp(-a.b * d));
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

ssDot r1 = ssDot(0.0, 0.0,  0.31, vec3(0.0, 0.0, 0.0), vec3(0.8,0.6,0.0));
ssDot r2 = ssDot(0.0, 1.0,  0.0,  vec3(1.0,0.0,0.0),   vec3(0.8,0.6,0.0));//
ssDot r3 = ssDot(0.0, 0.10, 0.0,  vec3((0.0)),         vec3(1.0,1.0,1.0));
ssDot r4 = ssDot(0.0, 0.50, 0.20, vec3(0.0,0.0,0.0),   vec3(1.0,1.0,1.0));
ssDot r5 = ssDot(0.0, 0.50, 0.20, vec3(0.0,0.0,0.0),   vec3(1.0,1.0,1.0));
ssDot r6 = ssDot(0.0, 0.50, 0.70, vec3(1.0,0.0,0.0),   vec3(1.0,1.0,1.0));
ssDot r7 = ssDot(0.0, 0.50, 5.20, vec3(0.0,0.0,0.0),   vec3(1.0,1.0,1.0));
ssDot r8 = ssDot(0.0, 0.50, 0.0, vec3(1.0,1.0,0.0),   vec3(1.0,1.0,1.0));
ssDot r9 = ssDot(0.0, 0.50, 0.0, vec3(0.50,0.50,1.0),   vec3(1.0,1.0,1.0));

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
    r = unionOp(r, r3);
    r = unionOp(r, r4);
    r = unionOp(r, r5);
    r = unionOp(r, r6);
    r = unionOp(r, r7);
    r  = unionOp(r, r8);
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

vec3 trace(vec2 op, vec2 dp, float depth) {
    float t = 0.0;

    vec3 sum = vec3(0.0);

    ssDot r;

    ssDot r0 = scene(op.x, op.y);

    r = r0;

    for (float i = 0.0; i < MAX_STEP && t < MAX_DISTANCE; i++) {

        op = op + dp*(r.sd);


        r = scene(op.x, op.y);

        t += abs(r.sd);

        if (abs(r.sd) < EPSILON) {
            return r.emission;
        }
    }

    return (sum);
}


// test
float angle_step = TWO_PI / SAMPLES;

vec3 sampleFun(vec2 p) {
    vec3 sum = BLACK;

    float a = 0.0;

    vec2 dir;

    //ssDot r = scene(p.x, p.y);

    vec3 bonus = BLACK;

    float i = 0.0;


    for (i = 0.0; i < SAMPLES; i++) {
        float rand_a = random((p+i)) - 0.5;

        float ddXa = dFdx(a);
        float ddYa = dFdy(a);

        //rand_a  += (ddXa+ddYa) * 0.5;

        dir = normalize(vec2(cos(a + rand_a * 0.5), sin(a + rand_a * 0.5)));

        //dir = normalize(vec2(cos(a), sin(a)));

        sum = sum + trace(p, dir , 0.0);

        a += angle_step;

        // if( length(sum.xyz) > TWO_PI) break;
    }

    return sum/(i + 1.0);
}

void main(){
    vec2 uv = fragTexCoord;
    uv -= 0.5;
    uv.x *= u_resolution.x/u_resolution.y;// / u_resolution.xy;
    uv *= 2.0;

    //uv *= 2.0;

    //uv -= 1.0;

    // Calculate derivatives of UV coordinates
    vec2 ddxUV = dFdx(uv);
    vec2 ddyUV = dFdy(uv);

    // Use derivatives to adjust UV coordinates for antialiasing
    vec2 aaUV = uv + (ddxUV + ddyUV);

    m  = (u_mouse.xy) / u_resolution.xy;

    m -= 0.5;
    m.x *= u_resolution.x/u_resolution.y;
    m *= 2.0;

    // scene pixel color
    //vec3 col = (sampleFun(aaUV));

    float prox = length(uv - m);

    ssDot sv = scene(uv.x, uv.y);

    vec3 col = (sampleFun(uv));
//    vec3 col = vec3(sv.sd);
//
//    if (sv.sd < EPSILON){
//        col.r = 1.0;
//    }


    //col = step(0.5 - prox, col);

    // Output to screen
    finalColor = vec4(col, 1.0);
}