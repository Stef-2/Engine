#version 460 core

struct PointLight {
    vec4 position;
    vec4 color;
    float intensity;
    float padding[3];
};

struct SpotLight {
    vec4 position;
    vec4 orientation;
    vec4 color;
    float intensity;
    float angle;
    float sharpness;
    float padding[1];
};

struct DirectionalLight {
    vec4 position;
    vec4 orientation;
    vec4 color;
    float intensity;
    float padding[3];
};

struct AmbientLight {
    vec4 position;
    vec4 color;
    float intensity;
    float padding[3];
};

layout (binding = 0, std140) uniform mvpMatrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

in vertexOutput
{
    vec3 position;
    vec3 viewDirection;
    vec3 rayDirection;
};

layout (binding = 1, std430) buffer PointLights {
  PointLight pointLights[];
};

layout (binding = 2, std430) buffer SpotLights {
  SpotLight spotLights[];
};

layout (binding = 3, std430) buffer DirectionalLights {
  DirectionalLight directionalLights[];
};

layout (binding = 4, std430) buffer AmbientLights {
  AmbientLight ambientLights[];
};

out vec4 fragColor;

const mat3 m3  = mat3( 0.00,  0.80,  0.60,
                      -0.80,  0.36, -0.48,
                      -0.60, -0.48,  0.64 );
const mat3 m3i = mat3( 0.00, -0.80, -0.60,
                       0.80,  0.36, -0.48,
                       0.60, -0.48,  0.64 );
const mat2 m2 = mat2(  0.80,  0.60,
                      -0.60,  0.80 );
const mat2 m2i = mat2( 0.80, -0.60,
                       0.60,  0.80 );
float hash1( float n )
{
    return fract( n*17.0*fract( n*0.3183099 ) );
}
                       vec4 noised( in vec3 x )
{
    vec3 p = floor(x);
    vec3 w = fract(x);
    #if 1
    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);
    #else
    vec3 u = w*w*(3.0-2.0*w);
    vec3 du = 6.0*w*(1.0-w);
    #endif

    float n = p.x + 317.0*p.y + 157.0*p.z;
    
    float a = hash1(n+0.0);
    float b = hash1(n+1.0);
    float c = hash1(n+317.0);
    float d = hash1(n+318.0);
    float e = hash1(n+157.0);
	float f = hash1(n+158.0);
    float g = hash1(n+474.0);
    float h = hash1(n+475.0);

    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec4( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z), 
                      2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                      k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                      k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}

float fbmd_8( in vec3 x )
{
    float f = 2.0;
    float s = 0.65;
    float a = 0.0;
    float b = 0.5;
    vec3  d = vec3(0.0);
    mat3  m = mat3(1.0,0.0,0.0,
                   0.0,1.0,0.0,
                   0.0,0.0,1.0);
    for( int i=0; i<8; i++ )
    {
        vec4 n = noised(x);
        a += b*n.x;          // accumulate values	
        
        //if( i<4 )
        d += b*m*n.yzw;      // accumulate derivatives
        b *= s;
        x = f*m3*x;
        m = f*m3i*m;
    }
	return a;
}

const float lightnessSteps = 4.0;

float dither8x8(vec2 position, float brightness) {
  int x = int(mod(position.x, 8.0));
  int y = int(mod(position.y, 8.0));
  int index = x + y * 8;
  float limit = 0.0;

  if (x < 8) {
    if (index == 0) limit = 0.015625;
    if (index == 1) limit = 0.515625;
    if (index == 2) limit = 0.140625;
    if (index == 3) limit = 0.640625;
    if (index == 4) limit = 0.046875;
    if (index == 5) limit = 0.546875;
    if (index == 6) limit = 0.171875;
    if (index == 7) limit = 0.671875;
    if (index == 8) limit = 0.765625;
    if (index == 9) limit = 0.265625;
    if (index == 10) limit = 0.890625;
    if (index == 11) limit = 0.390625;
    if (index == 12) limit = 0.796875;
    if (index == 13) limit = 0.296875;
    if (index == 14) limit = 0.921875;
    if (index == 15) limit = 0.421875;
    if (index == 16) limit = 0.203125;
    if (index == 17) limit = 0.703125;
    if (index == 18) limit = 0.078125;
    if (index == 19) limit = 0.578125;
    if (index == 20) limit = 0.234375;
    if (index == 21) limit = 0.734375;
    if (index == 22) limit = 0.109375;
    if (index == 23) limit = 0.609375;
    if (index == 24) limit = 0.953125;
    if (index == 25) limit = 0.453125;
    if (index == 26) limit = 0.828125;
    if (index == 27) limit = 0.328125;
    if (index == 28) limit = 0.984375;
    if (index == 29) limit = 0.484375;
    if (index == 30) limit = 0.859375;
    if (index == 31) limit = 0.359375;
    if (index == 32) limit = 0.0625;
    if (index == 33) limit = 0.5625;
    if (index == 34) limit = 0.1875;
    if (index == 35) limit = 0.6875;
    if (index == 36) limit = 0.03125;
    if (index == 37) limit = 0.53125;
    if (index == 38) limit = 0.15625;
    if (index == 39) limit = 0.65625;
    if (index == 40) limit = 0.8125;
    if (index == 41) limit = 0.3125;
    if (index == 42) limit = 0.9375;
    if (index == 43) limit = 0.4375;
    if (index == 44) limit = 0.78125;
    if (index == 45) limit = 0.28125;
    if (index == 46) limit = 0.90625;
    if (index == 47) limit = 0.40625;
    if (index == 48) limit = 0.25;
    if (index == 49) limit = 0.75;
    if (index == 50) limit = 0.125;
    if (index == 51) limit = 0.625;
    if (index == 52) limit = 0.21875;
    if (index == 53) limit = 0.71875;
    if (index == 54) limit = 0.09375;
    if (index == 55) limit = 0.59375;
    if (index == 56) limit = 1.0;
    if (index == 57) limit = 0.5;
    if (index == 58) limit = 0.875;
    if (index == 59) limit = 0.375;
    if (index == 60) limit = 0.96875;
    if (index == 61) limit = 0.46875;
    if (index == 62) limit = 0.84375;
    if (index == 63) limit = 0.34375;
  }

  return brightness < limit ? 0.0 : 1.0;
}

float luma(vec3 color) {
  return dot(color, vec3(0.299, 0.587, 0.114));
}

float luma(vec4 color) {
  return dot(color.rgb, vec3(0.299, 0.587, 0.114));
}

vec3 dither8x8(vec2 position, vec3 color) {
  return color * dither8x8(position, luma(color));
}

vec4 dither8x8(vec2 position, vec4 color) {
  return vec4(color.rgb * dither8x8(position, luma(color)), 1.0);
}

vec3 random3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}

const float F3 =  0.3333333;
const float G3 =  0.1666667;
float snoise(vec3 p) {

	vec3 s = floor(p + dot(p, vec3(F3)));
	vec3 x = p - s + dot(s, vec3(G3));
	 
	vec3 e = step(vec3(0.0), x - x.yzx);
	vec3 i1 = e*(1.0 - e.zxy);
	vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	 	
	vec3 x1 = x - i1 + G3;
	vec3 x2 = x - i2 + 2.0*G3;
	vec3 x3 = x - 1.0 + 3.0*G3;
	 
	vec4 w, d;
	 
	w.x = dot(x, x);
	w.y = dot(x1, x1);
	w.z = dot(x2, x2);
	w.w = dot(x3, x3);
	 
	w = max(0.6 - w, 0.0);
	 
	d.x = dot(random3(s), x);
	d.y = dot(random3(s + i1), x1);
	d.z = dot(random3(s + i2), x2);
	d.w = dot(random3(s + 1.0), x3);
	 
	w *= w;
	w *= w;
	d *= w;
	 
	return dot(d, vec4(52.0));
}

float fbm(vec3 x) {
	float v = 0.0;
	float a = 0.5;
	vec3 shift = vec3(100);
	for (int i = 0; i < 8; ++i) {
		v += a * snoise(x);
		x = x * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}

dvec2 NearAndFarClipValues()
{
    // element[2][2] is (far + near) / (far - near)
    double element22 = -projection[2][2];

    // element[3][2] is -(2 * near * far) / (far - near)
    double element32 = -projection[3][2];

    double far = ((2.0f * element32) / (2.0f * element22 - 2.0f));
    double near = ((element22 - 1.0f) * far) / (element22 + 1.0);

    return dvec2(near, far);
}

float LinearizeDepth() 
{
    dvec2 nearFar = NearAndFarClipValues();
    float z = gl_FragCoord.z * 2.0 - 1.0;

    float near = float(nearFar[0]);
    float far = float(nearFar[1]);
    
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
const float ddistance = 11.0f;

void main()
{
    vec3 spherePos = vec3(-100.0f, 100.0f, 0.0f);
    float density = 0;
    vec3 samplePos = position;
    float flatColor;
    vec3 color = vec3(0.4f);
    vec3 rayDir = (vec3(view[0][3], view[1][3], view[2][3]));

    int loop = 50;

    for (unsigned int i = 0; i < loop; i++)
    {
        density += fbm(samplePos * 0.005) * 0.5f;
        samplePos += rayDirection * 5f;

        if (density >= 1.0f)
            break;
            
        /*if (samplePos.y >= 150)
            break;*/
        
        color += 0.015;
    }
    /*
    vec3 sunDirection = vec3(0.0f, 1.0f, 0.0f); //normalize(directionalLights[0].position.xyz - samplePos);

    float thickness = 0.0f;

    for (unsigned int i = 0; i < loop; i++)
    {
        samplePos += sunDirection * 5f;
        thickness += fbmd_8(samplePos * 0.01).x * 0.05f;

        if (thickness >= 1.0f)
            break;

        if (samplePos.y >= 150)
            break;
    }
    */

    //if (density < 0.1) discard;
    if (density < 0.01) discard;
    //flatColor = distance(spherePos, samplePos);
    fragColor = vec4(vec3(color), density);
}
