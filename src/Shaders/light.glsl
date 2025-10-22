#version 450

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

uniform vec2 u_resolution;

//max count of reflections/refractions
const float MAX_STEP = 20.00;
//maximum distance ray traveling
const float MAX_DISTANCE = 1.0;

const float EPSILON = 0.001;

//samples per pixed
const float SAMPLES = 64.0;

const float PI =     3.1415926535897932384626433832795;
const float TWO_PI = 6.283185307179586476925286766559;

//these both seems useles now
const float BIAS = 0.000001;
const float MAX_DEPTH = 3.0;

uniform sampler2D sdf_texture;
uniform sampler2D noise_texture;

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
struct ssDot{
    float sd; //signed distance
    int mat_idx; //material index
};

const float step_rad = TWO_PI / SAMPLES;

float getSDF(vec2 p) {
    vec4 color = texture(sdf_texture, p);

    return color.x;
}

vec2 getNoise(vec2 p) {
    vec2 tmp_p = p;

//    tmp_p /= 2.0;
//    tmp_p.x /= u_resolution.x / u_resolution.y;
//    tmp_p += 0.5;

    vec4 color = texture2D(noise_texture, tmp_p);

    vec2 res = ((color.xy) - 0.5)* 2.0;

    return res;
}

float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
    vec2(12.9898,78.233)))*
    43758.5453123);
}

vec2 calculateNormal(vec2 p) {
    const float h = 0.0007; // Малое значение для смещения
    const vec2 dx = vec2(h, 0.0);
    const vec2 dy = vec2(0.0, h);
    const vec2 dxy = vec2(h, h);

    // Вычисляем градиенты, используя центральные разности
    float sd = getSDF(p);

    // Градиенты по X и Y
    float gx = (getSDF(p + dx) - getSDF(p - dx)) / (2.0 * h);
    float gy = (getSDF(p + dy) - getSDF(p - dy)) / (2.0 * h);

    // Диагональные градиенты
    float gxy = (getSDF(p + dxy) - getSDF(p - dxy)) / (2.0 * sqrt(2.0) * h);
    float gyx = (getSDF(vec2(p.x + dx.x, p.y - dy.y)) - getSDF(vec2(p.x - dx.x, p.y + dy.y))) / (2.0 * sqrt(2.0) * h);

    // Среднее значение градиентов для более плавного результата
    vec2 n = normalize(vec2(gx + (gxy + gyx) / 2.0, gy + (gxy + gyx) / 2.0));

    // Инвертируем нормаль, если точка находится внутри объекта
    return sd > 0.0 ? n : -n;
}

vec3 new_trace(vec2 p, vec2 dir) {
    vec3 sum = vec3(0.0);

    float traveled = 0.0;

    vec2 tmp_p = p;

//    tmp_p /= 2.0;
//    tmp_p.x /= u_resolution.x / u_resolution.y;
//    tmp_p += 0.5;

    float color_divider = 0.0;

    vec4 res = texture(sdf_texture,tmp_p);

    res.x /= u_resolution.x;

    for (float i = 0.0; i < MAX_STEP && traveled < MAX_DISTANCE; i++) {

        if (abs(res.x) < EPSILON) {

            int index = int(res.y);

            sum += mats[index].emission * mats[index].reflectivity;

            return sum;

            vec2 normal = calculateNormal(p);

            dir = reflect(dir, normal);

            color_divider++;
        }

        traveled += abs(res.x);

        //tmp_dir.y *= resolution.x/resolution.y;

        p = p + dir*abs(res.x);

        vec2 tmp_p = p;

//        tmp_p /= 2.0;
//        tmp_p.x /= u_resolution.x / u_resolution.y;
//        tmp_p += 0.5;

        //tmp_p = (p + 1.0) / 2.0;


        res = texture(sdf_texture, tmp_p);

        res.x /= u_resolution.x;
    }

    color_divider = max(1.0, color_divider);

    return sum / color_divider;
}

float angle_step = TWO_PI / SAMPLES;

vec3 sampleFun(vec2 p) {
    vec3 sum = BLACK;

    float a = 0.0;

    vec2 tmp_p = p;

//    tmp_p /= 2.0;
//    tmp_p.x /= u_resolution.x / u_resolution.y;
//    tmp_p += 0.5;


    vec4 res = texture(sdf_texture,tmp_p);

//    if (res.x < EPSILON){
//        int index = int(res.y);
//
//        return  mats[index].emission;
//    }

    for (float i = 0.0; i < SAMPLES; i++) {
        vec2 rr = getNoise(p);

        vec2 dir = normalize(vec2(cos( a + rr.x), sin( a + rr.x)));

        //dir += rr;

        dir = normalize(dir);

        sum = sum + new_trace(p,  dir);

//        float sd = getSDF(tmp_p);
//
//        if (sd < EPSILON){
//            sum.xy = calculateNormal(tmp_p);
//        }else{
//            sum = BLACK;
//        }
//
//        return sum;

        a += angle_step;
    }

    return sum/SAMPLES;

}

void main(){
    //vec4 color = texture(sdf_texture, fragTexCoord);
    vec2 uv = fragTexCoord;

    vec4 color = vec4(sampleFun(uv), 1.0);

    finalColor = color;
}