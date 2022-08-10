#version 460 core

const float epsilon = 0.01f;

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
	vec4 position;
	flat vec3 viewDirection;
	vec4 rayDirection;

	vec4 WSpos;
	flat vec4 WSviewPos;

	flat mat4 pw;
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

uniform sampler3D volumeMap;

out vec4 fragColor;

struct Box
{
	vec3 frontBottomLeft;
	vec3 backTopRight;
};

struct Sphere
{
	vec3 position;
	float radius;
};

bool PointInsideBox(in vec3 point, in Box box)
{
	return point.x >= box.frontBottomLeft.x && point.x <= box.backTopRight.x
		&& point.y >= box.frontBottomLeft.y && point.y <= box.backTopRight.y
		&& point.z >= box.frontBottomLeft.z && point.z <= box.backTopRight.z;
}

bool PointInsideSphere(in vec3 point, in Sphere sphere)
{
	return distance(point, sphere.position) <= sphere.radius;
}

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

float rand(vec2 co){
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float snoiseFractal(vec3 m) {
	return   0.5333333* snoise(m)
				+0.2666667* snoise(2.0*m)
				+0.1333333* snoise(4.0*m)
				+0.0666667* snoise(8.0*m);
}

vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}
vec3 fade(vec3 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}

float cnoise(vec3 P){
  vec3 Pi0 = floor(P); // Integer part for indexing
  vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  Pi0 = mod(Pi0, 289.0);
  Pi1 = mod(Pi1, 289.0);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 / 7.0;
  vec4 gy0 = fract(floor(gx0) / 7.0) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 / 7.0;
  vec4 gy1 = fract(floor(gx1) / 7.0) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}

float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec3 x) {
	const vec3 step = vec3(110, 241, 171);

	vec3 i = floor(x);
	vec3 f = fract(x);
 
	// For performance, compute the base input to a 1D hash from the integer part of the argument and the 
	// incremental change to the 1D based on the 3D -> 1D wrapping
	float n = dot(i, step);

	vec3 u = f * f * (3.0 - 2.0 * f);
	return mix(mix(mix( hash(n + dot(step, vec3(0, 0, 0))), hash(n + dot(step, vec3(1, 0, 0))), u.x),
				   mix( hash(n + dot(step, vec3(0, 1, 0))), hash(n + dot(step, vec3(1, 1, 0))), u.x), u.y),
			   mix(mix( hash(n + dot(step, vec3(0, 0, 1))), hash(n + dot(step, vec3(1, 0, 1))), u.x),
				   mix( hash(n + dot(step, vec3(0, 1, 1))), hash(n + dot(step, vec3(1, 1, 1))), u.x), u.y), u.z);
}

float RGBToFloat(vec3 x)
{
	return (x.r + x.g + x.b) / 3;
}

void main()
{
	int steps = 0;
	mat3 invmvp = inverse(mat3(view));
	float linearDepth = LinearizeDepth();

	vec3 spherePos = vec3(0.0f, 0.0f, 0.0f);

	Sphere sphere = {{0.0f, 0.0f, 0.0f}, 1.0f};

	Box box = {{-50.0f - epsilon, -5.0f - epsilon, -50.0f - epsilon},
				{50.0f + epsilon, 5.0f + epsilon, 50.0f + epsilon}};
	
	float density = 0;
	vec3 samplePos = WSpos.xyz;
	float flatColor;
	vec3 color = vec3(1.0f);
	vec3 rayDir = normalize(WSpos.xyz - WSviewPos.xyz);
	float dist = 0;
	const unsigned int loop = 50;

	vec4 sunDirection = {0.0f, 1.0f, 0.0f, 0.0f}; //normalize(directionalLights[0].position.xyz - samplePos);
	vec3 toLight;
	float stepSize = 0.01f;
	float noiseScale = 1.0f;
	vec3 fragViewPos = -view[3].xyz * mat3(view);
	//vec3 ray = normalize(fragViewPos - position);

	while (true)
	{
		//density += max(fbm((samplePos).xyz * noiseScale), 0.0f) * 0.3f;
		density += (texture(volumeMap, (samplePos.xyz / 16)).x) * 0.2;
		//color = mix(vec3(1.0f), vec3(0.0f), snoise(samplePos) > 0);

		toLight = samplePos;
		
		while (true)
		{
			//color += -max(fbm(toLight * noiseScale), 0.0f) * 0.01f;
			color += -texture(volumeMap, (toLight.xyz / 16)).xyz * 0.01;
			toLight += sunDirection.xyz * (stepSize * 4);
			if (color.r <= 0.0f || !PointInsideSphere(toLight.xyz, sphere))
				break;
		}
		//dist = distance(samplePos.xyz, spherePos);

		//color = (samplePos).xyz;
		//color = vec3(dist);
		//color += -0.015;

		if (density >= 1.0f || !PointInsideSphere(samplePos.xyz, sphere))
			break;

		samplePos += (rayDir * stepSize);
	}

	//color = max(color, 0.0f);
	//density = max(density, 0.0f);


	float thickness = 0.0f;
	/*
	for (unsigned int i = 0; i < loop; i++)
	{
		thickness += max(0.0f, fbm(samplePos.xyz) * 0.05f);
		samplePos += vec4(sunDirection, 0.0f) * 0.02f;

		dist = distance(samplePos.xyz, spherePos);

		if (thickness >= 1.0f)
			break;

		if (dist >= 1.0f)
			break;
	}
	*/

	//if (density < 0.1) discard;
	//if (density < 0.1) discard;
	//flatColor = distance(spherePos, samplePos);

	//fragColor = vec4((projection * vec4(rayDir,1)).xyz, 1);
	fragColor = vec4(color , density);
}
