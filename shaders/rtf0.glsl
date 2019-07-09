//#version 410

#define PBR

/**********************************************************************
** @macros
**********************************************************************/

#define saturate(x) clamp(x, 0.0, 1.0)

/**********************************************************************
** @uniforms
**********************************************************************/

uniform vec3 ray_origin;

uniform vec2 rot;
uniform vec2 time;
uniform vec4 mouse;
uniform float aspect;

uniform samplerCube skybox;
uniform sampler2D noise;

/**********************************************************************
** scene's public data
**********************************************************************/

struct Material {
	vec3 kd;
	vec3 ks;
	float roughness;
	float metallic;
};

struct Shape {
	Material m;
	float t;
	float sd;
	vec3 p;
	vec3 a;
	vec3 b;
	vec3 c;
	vec3 d;
};

// Base materials
vec3 ambient_light = vec3(.5);
const Material black = Material(vec3(0.),vec3(0.),1.,0.);
const Material red = Material(vec3(1.,0.,0.),vec3(0.05),0.,0.);
const Material yellow = Material(vec3(1.,1.,0.),vec3(0.),1.,0.);
const Material green = Material(vec3(0.,1.,0.),vec3(0.),1.,0.);
const Material cyan = Material(vec3(0.,1.,1.),vec3(0.),1.,0.);
const Material blue = Material(vec3(0.,0.,1.),vec3(0.1),0.,0.);
const Material magenta = Material(vec3(1.,0.,1.),vec3(0.),1.,0.);
const Material grey = Material(vec3(.5),vec3(0.5),0.,.5);
const Material white = Material(vec3(1.),vec3(0.5),1.0,0.0);

Material sin_noise_()
{
	return Material(
	vec3(sin(gl_FragCoord.x),sin(gl_FragCoord.y),5.),
	vec3(0.),
	1.,
	0.);
}
Material sin_noise = sin_noise_();

Material sandbox_()
{
	vec2 position = (gl_FragCoord.xy / vec2(640.,360.)) + mouse.xy / 4.0;

	float time_ = time.x / 10;
	float color = 0.0;
	color += sin(position.x * cos(time_ / 15.0) * 80.0) + cos(position.y * cos(time_ / 15.0) * 10.0);
	color += sin(position.y * sin(time_ / 10.0) * 40.0) + cos(position.x * sin(time_ / 25.0) * 40.0);
	color += sin(position.x * sin(time_ / 5.0) * 10.0) + sin(position.y * sin(time_ / 35.0) * 80.0);
	color *= sin(time_ / 10.0) * 0.5;

	vec3 kd = vec3(vec3(color,color * 0.5, sin(color + time_ / 3.0) * 0.75));
	return Material(kd, vec3(0.), 1., 0.);
}
Material sandbox = sandbox_();


Material perlin_noise_()
{
	vec4 f = vec4(0.);
	vec2 u = gl_FragCoord.xy/ 8. * cos(time.x / 10.);
	vec2 m;
	vec2 r;
	float l = 1.;
	float s = 1.;

#define h(n) fract(sin(n+vec2(0,157))*57.) 
#define N m=fract(u*s); l=dot(u*s-m,vec2(1,157));s+=s; m*=m*(3.-m-m); r= mix(h(l),h(++l),m.x); f+= mix(r.x,r.y,m.y)/s;

	N N N N

#undef h
#undef N
	return Material(f.xyz * .5,vec3(0.),1.,0.);
}
Material perlin_noise = perlin_noise_();

// 10,12

// Constructors
void Light(Material m, vec3 p);
Shape Null(void);  // 0
Shape Sphere(Material m, vec3 p, float s);  // 1
Shape Box(Material m, vec3 p, vec3 b);  // 2
Shape RoundBox(Material m, vec3 p, vec3 b, float r);  // 2.5
Shape Torus(Material m, vec3 p, vec2 t);  // 3
Shape CappedTorus(Material m, vec3 p, vec2 sc, float ra, float rb);  // 3.5
Shape Cylinder(Material m, vec3 p, vec3 a, vec3 b, float r);  // 4
Shape Cone(Material m, vec3 p, vec3 a, vec3 b, float ra, float rb);  // 5
Shape Plane(Material m, vec3 p, vec4 n);  // 6
Shape HexagonalPrism(Material m, vec3 p, vec2 h);  // 7
Shape TriangularPrism(Material m, vec3 p, vec2 h);  // 8
Shape Capsule(Material m, vec3 p, vec3 a, vec3 b, float r);  // 9
Shape Ellipsoid(Material m, vec3 p, vec3 r);  // 10
Shape Octahedron(Material m, vec3 p, float s);  // 11
Shape Triangle(Material m, vec3 p, vec3 a, vec3 b, vec3 c);  // 12
Shape Quad(Material m, vec3 p, vec3 a, vec3 b, vec3 c, vec3 d);  // 13
Shape Mandelbulb(Material m, vec3 p);  // 14

// Base Operations
Shape Union(Shape a, Shape b);
Shape Subtraction(Shape a, Shape b);
Shape Intersection(Shape a, Shape b);
Shape SmoothUnion(Shape a, Shape b, float k);
Shape SmoothSubtraction(Shape a, Shape b, float k);
Shape SmoothIntersection(Shape a, Shape b, float k);

// Extra Operations
Shape Translate(vec3 p, Shape s);


void Scene(void)
{
        SCENE;
}

/*
********************************************************************************
** @output
********************************************************************************
*/

out vec4 out_color;

/**********************************************************************
** @constants
**********************************************************************/

const float FOV = 90.0;
const float MIN_DISTANCE = 1.;
const float MAX_DISTANCE = 256.;
const int MAX_STEPS = 64;
const float EPSILON = 0.0005;
const float EPSMOD = 0.0105;
const int MAX_AO = 4;
const float MAX_SS = 2.;
const float MAX_SSK = 16.;
const float METALLIC = 0.0;
float ROUGHNESS = 0;
const float MINIMUM_ROUGHNESS = 0.03;
const float MAXIMUM_ROUGHNESS = 0.97;
const float PI = 3.14159265359;

/**********************************************************************
** @main's prototypes
**********************************************************************/

vec4 render();

/**********************************************************************
** @main
**********************************************************************/

void main()
{
	// GLSL standard removes unused variables
	vec4 _mouse = mouse;
	vec2 _time = time;
	out_color = render();
}

/**********************************************************************
** @scene's private data
**********************************************************************/

// Lights
const int MAX_LIGHTS = 256;
struct Light_ {
	Material m;
	vec3 p;
} lights[MAX_LIGHTS];
int light_count;

void Light(Material m, vec3 p)
{
	Light_ light = Light_(m, p);
	for (int i = 0; i < MAX_LIGHTS; ++i) {
		if (i == light_count) {
			lights[i] = light;
			break ;
		}
	}
	++light_count;
}

// Shapes
Shape Null(void)
{
	Shape shape;
	shape.t = 0;
	shape.sd = MAX_DISTANCE;
	shape.m = black;
	shape.p = vec3(0.);
	return shape;
}
Shape Sphere(Material m, vec3 p, float s)
{
	Shape shape;
	shape.t = 1;
	shape.m = m;
	shape.p = -p;
	shape.a.x = s;
	return shape;
}
Shape Box(Material m, vec3 p, vec3 b)
{
	Shape shape;
	shape.t = 2;
	shape.m = m;
	shape.p = -p;
	shape.a = b;
	return shape;
}
Shape RoundBox(Material m, vec3 p, vec3 b, float r)
{
	Shape shape;
	shape.t = 2.5;
	shape.m = m;
	shape.p = -p;
	shape.a.xyz = b;
	shape.b.x = r;
	return shape;
}
Shape Torus(Material m, vec3 p, vec2 t)
{
	Shape shape;
	shape.t = 3;
	shape.m = m;
	shape.p = -p;
	shape.a.xy = t;
	return shape;
}
Shape CappedTorus(Material m, vec3 p, vec2 sc, float ra, float rb)
{
	Shape shape;
	shape.t = 3.5;
	shape.m = m;
	shape.p = -p;
	shape.a = vec3(sc, ra);
	shape.b.x = rb;
	return shape;
}
Shape Cylinder(Material m, vec3 p, vec3 a, vec3 b, float r)
{
	Shape shape;
	shape.t = 4;
	shape.m = m;
	shape.p = -p;
	shape.a = a;
	shape.b = b;
	shape.c.x = r;
	return shape;
}
Shape Cone(Material m, vec3 p, vec3 a, vec3 b, float ra, float rb)
{
	Shape shape;
	shape.t = 5;
	shape.m = m;
	shape.p = -p;
	shape.a = a;
	shape.b = b;
	shape.c.xy = vec2(ra, rb);
	return shape;
}
Shape Plane(Material m, vec3 p, vec4 n)
{
	Shape shape;
	shape.t = 6;
	shape.m = m;
	shape.p = -p;
	shape.a = n.xyz;
	shape.b.x = n.z;
	return shape;
}
Shape HexagonalPrism(Material m, vec3 p, vec2 h)
{
	Shape shape;
	shape.t = 7;
	shape.m = m;
	shape.p = -p;
	shape.a.xy = h;
	return shape;
}
Shape TriangularPrism(Material m, vec3 p, vec2 h)
{
	Shape shape;
	shape.t = 8;
	shape.m = m;
	shape.p = -p;
	shape.a.xy = h;
	return shape;
}
Shape Capsule(Material m, vec3 p, vec3 a, vec3 b, float r)
{
	Shape shape;
	shape.t = 9;
	shape.m = m;
	shape.p = -p;
	shape.a = a;
	shape.b = b;
	shape.c.x = r;
	return shape;
}
Shape Ellipsoid(Material m, vec3 p, vec3 r)
{
	Shape shape;
	shape.t = 10;
	shape.m = m;
	shape.p = -p;
	shape.a = r;
	return shape;
}
Shape Octahedron(Material m, vec3 p, float s)
{
	Shape shape;
	shape.t = 11;
	shape.m = m;
	shape.p = -p;
	shape.a.x = s;
	return shape;
}
Shape Triangle(Material m, vec3 p, vec3 a, vec3 b, vec3 c)
{
	Shape shape;
	shape.t = 12;
	shape.m = m;
	shape.p = -p;
	shape.a = a;
	shape.b = b;
	shape.c = c;
	return shape;
}
Shape Quad(Material m, vec3 p, vec3 a, vec3 b, vec3 c, vec3 d)
{
	Shape shape;
	shape.t = 13;
	shape.m = m;
	shape.p = -p;
	shape.a = a;
	shape.b = b;
	shape.c = c;
	shape.d = d;
	return shape;
}
Shape Mandelbulb(Material m, vec3 p)
{
	Shape shape;
	shape.t = 14;
	shape.m = m;
	shape.p = -p;
	return shape;
}

// Scene data
vec3 scene_p_;
float scene_sd_;
Material scene_m_;
float sdf_shape(Shape shape);
Shape Union(Shape a, Shape b)
{
	Shape shape;
	a.sd = sdf_shape(a);
	b.sd = sdf_shape(b);
	if (a.sd < b.sd) {
		shape = a;
	} else {
		shape = b;
	}
	shape.sd = min(a.sd, b.sd);
	if (shape.sd < scene_sd_) {
		scene_sd_ = shape.sd;
		scene_m_ = shape.m;
	}
	shape.t = -1;
	return shape;
}
Shape Subtraction(Shape a, Shape b)
{
	Shape shape;
	a.sd = sdf_shape(a);
	b.sd = sdf_shape(b);
	if (a.sd < b.sd) {
		shape = a;
	} else {
		shape = b;
	}
	shape.sd = max(-a.sd, b.sd);
	if (shape.sd < scene_sd_) {
		scene_sd_ = shape.sd;
		scene_m_ = shape.m;
	}
	shape.t = -1;
	return shape;
}
Shape Intersection(Shape a, Shape b)
{
	Shape shape;
	a.sd = sdf_shape(a);
	b.sd = sdf_shape(b);
	if (a.sd < b.sd) {
		shape = a;
	} else {
		shape = b;
	}
	shape.sd = max(a.sd, b.sd);
	if (shape.sd < scene_sd_) {
		scene_sd_ = shape.sd;
		scene_m_ = shape.m;
	}
	shape.t = -1;
	return shape;
}

Shape SmoothUnion(Shape a, Shape b, float k)
{
	Shape shape;
	a.sd = sdf_shape(a);
	b.sd = sdf_shape(b);
	if (a.sd < b.sd) {
		shape = a;
	} else {
		shape = b;
	}
	float h = clamp(0.5 + 0.5*(b.sd-a.sd)/k, 0.0, 1.0);
	shape.sd = mix(b.sd,a.sd,h) - k*h*(1.0-h);
	if (shape.sd < scene_sd_) {
		scene_sd_ = shape.sd;
		scene_m_ = shape.m;
	}
	shape.t = -1;
	return shape;
}
Shape SmoothSubtraction(Shape a, Shape b, float k)
{
	Shape shape;
	a.sd = sdf_shape(a);
	b.sd = sdf_shape(b);
	if (a.sd < b.sd) {
		shape = a;
	} else {
		shape = b;
	}
	float h = clamp(0.5 - 0.5*(b.sd+a.sd)/k, 0.0, 1.0);
	shape.sd = mix(b.sd,-a.sd,h) + k*h*(1.0-h);
	if (shape.sd < scene_sd_) {
		scene_sd_ = shape.sd;
		scene_m_ = shape.m;
	}
	shape.t = -1;
	return shape;
}
Shape SmoothIntersection(Shape a, Shape b, float k)
{
	Shape shape;
	a.sd = sdf_shape(a);
	b.sd = sdf_shape(b);
	if (a.sd < b.sd) {
		shape = a;
	} else {
		shape = b;
	}
	float h = clamp(0.5 - 0.5*(b.sd-a.sd)/k, 0.0, 1.0);
	shape.sd = mix(b.sd,a.sd,h) + k*h*(1.0-h);
	if (shape.sd < scene_sd_) {
		scene_sd_ = shape.sd;
		scene_m_ = shape.m;
	}
	shape.t = -1;
	return shape;
}

Shape Translate(vec3 p, Shape s)
{
	Shape shape = s;
	shape.p = -p;
	return shape;
}

/**********************************************************************
** prototypes
**********************************************************************/

float intersect(vec3 ro, vec3 rd, float max_dist);
vec3 get_shading(vec3 v, vec3 n, vec3 p, Material m);
float scene_sdf(vec3 p);
Material scene_m(vec3 p);
float ambient_occlusion(vec3 p, vec3 n);
float soft_shadows(vec3 ro, vec3 rd, float light_dist);
vec3 get_normal(vec3 p);

float dot2(vec3 v) {return dot(v,v);}

float sdf_light(vec3 p);  // 0
float sdf_sphere(vec3 p, float s);  // 1
float sdf_box(vec3 p, vec3 b);  // 2
float sdf_roundbox(vec3 p, vec3 b, float r);  // 2.5
float sdf_torus(vec3 p, vec2 t);  // 3
float sdf_cappedtorus(vec3 p, vec2 sc, float ra, float rb);
float sdf_cylinder(vec3 p, vec3 a, vec3 b, float r);  // 4
float sdf_cone(vec3 p, vec3 a, vec3 b, float ra, float rb);  // 5
float sdf_plane(vec3 p, vec4 n);  // 6
float sdf_hex_prism(vec3 p, vec2 h);  // 7
float sdf_tri_prism(vec3 p, vec2 h);  // 8
float sdf_capsule(vec3 p, vec3 a, vec3 b, float r);  // 9
float sdf_ellipsoid(vec3 p, vec3 r);  // 10
float sdf_octahedron(vec3 p, float s);  // 11
float udf_triangle(vec3 p, vec3 a, vec3 b, vec3 c);  // 12
float udf_quad(vec3 p, vec3 a, vec3 b, vec3 c, vec3 d);  // 13
float sdf_mandelbulb(vec3 p);  // 14

/**********************************************************************
** PBR formulas
**********************************************************************/

float V_SmithGGXCorrelated(float roughness, float NoV, float NoL);
vec3 F_Fresnel( vec3 SpecularColor, float VoH);
float D_GGX(float roughness, float NoH, const vec3 h);
float D_Beckmann( float roughness, float NoH );
vec3 Diffuse_OrenNayar( vec3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH );
vec3 EnvBRDFApprox(vec3 SpecularColor, float Roughness, float NoV );

/**********************************************************************
** Sampling formulas
**********************************************************************/

vec3 randomSpherePoint(vec2 rand);
vec3 randomHemispherePoint(vec2 rand, vec3 n);
vec3 randomCosineWeightedHemispherePoint(vec3 rand, vec3 n);
vec3 ImportanceSampleGGX(vec2 Xi, float Roughness, vec3 N );
vec4 CosineSampleHemisphere( vec2 E, vec3 N );
vec2 Hammersley(uint Index, uint NumSamples, ivec2 Random);

/**********************************************************************
** definitions
**********************************************************************/

mat2 rd_rotate(float angle)
{
	float s = sin(angle);
	float c = cos(angle);
	return mat2(c, -s, s, c);
}

in vec2 vertexPassThrough;

vec4 render()
{
	// Initialize ray.
	vec3 ro = ray_origin;
	vec2 q = vec2(vertexPassThrough.x * aspect, vertexPassThrough.y);
	vec3 rd = normalize(vec3(q * sin(FOV / 2), -1.0));
	// Make rd follow rot
	ro.xz *= rd_rotate(rot.x);
	rd.xz *= rd_rotate(rot.x);
	ro.yz *= rd_rotate(rot.y);
	rd.yz *= rd_rotate(rot.y);

	float d = intersect(ro, rd, MAX_DISTANCE);
	if (d >= MAX_DISTANCE) {
		// TODO: implement a good background
		//return vec4(0.);
		//return vec4(sin(time.x) / 4., cos(time.x) / 4., atan(time.x) / 4., 1.);
		return texture(skybox, rd);
	}
	vec3 p = ro + (rd * d);
	Material m = scene_m(p);
	vec3 kd = m.kd * ambient_light;  // TODO: fix to use pbr
	vec3 ks = m.ks;
	//common values
	vec3 v = -rd;
	vec3 n = get_normal(p);
	vec3 r = normalize(-reflect(v,n));
	float roughness = clamp(m.roughness, MINIMUM_ROUGHNESS, MAXIMUM_ROUGHNESS);
	vec3 diffuseColor = kd * (1.0 - m.metallic);
	vec3 specularColor = m.ks;
	vec3 spec = get_shading(v, n, p, m);
	float dotrv = dot(r, v);
	float dotnv = dot(n, v);

	//monte carlo integration for global illumination. only skybox for now.
	//replace texture(skybox, l).rgb to actual color calculation.
	//possibly replace randomness to hammersley
	//	specular term
	{
		const int nSample = 4;
		vec3 sampled = vec3(0);
		for (int i = 0; i < nSample; i++) {
			vec3 h = ImportanceSampleGGX(Hammersley(i, nSample, ivec2(gl_FragCoord.xy)), roughness, n);
			vec3 l = 2 * dot(v, h) * h - v;
			float dotln = dot(l, n);
			float dotvh = dot(v, h);
			vec3 brdfSpec = V_SmithGGXCorrelated(roughness, dotnv, dotln) * F_Fresnel(specularColor, dotvh);
			vec3 k;
			float gd = intersect(p, l, MAX_DISTANCE);
			if (gd >= MAX_DISTANCE) {
				k = texture(skybox, l).rgb;
			} else {
				Material lm = scene_m(vec3(0));
				vec3 ln = get_normal(vec3(0));
				vec3 lp = p + (l * gd);
				//lp += ln * EPSILON;
				vec3 lv = -l;
				k = get_shading(lv, ln, lp, lm);
			}
			sampled += brdfSpec * k * saturate(dotln);
		}
		spec += sampled / nSample;
	}

	//diffuse term
	{
		const uint nSample = 4;
		vec3 sampled = vec3(0);
		// vec4 samplePoint = texture(noise, vertexPassThrough);
		for (uint i = 0; i < nSample; i++) {
			//do we have to divide with total pdf?
			vec3 l = CosineSampleHemisphere(Hammersley(i, nSample, ivec2(gl_FragCoord.xy)), n).xyz;
			float dotln = dot(l, n);
			vec3 h = normalize(v + l);
			float dotvh = dot(v, h);

			vec3 brdfDiff = Diffuse_OrenNayar(diffuseColor, roughness, dotnv, dotln, dotvh);
			vec3 k;
			float gd = intersect(p, l, MAX_DISTANCE);
			k = texture(skybox, l).rgb;
			if (gd >= MAX_DISTANCE) {
				k = texture(skybox, l).rgb;
			} else {
				Material lm = scene_m(vec3(0));
				vec3 ln = get_normal(vec3(0));
				vec3 lp = p + (l * gd);
				//lp += ln * EPSILON;
				vec3 lv = -l;
				//k = lm.kd;
				k = get_shading(lv, ln, lp, lm);
			}
			sampled += brdfDiff * k * saturate(dotln);
			//samplePoint = texture(noise, samplePoint.zw);
		}
		spec += sampled / nSample;
	}

//	shadows = .5 + .5 * shadows;
	
	float ao = ambient_occlusion(p, n);
	vec3 color; 
	
	// Light accumulation
	color = spec;
//	color = spec * ao;
//	color = (kd * ao) + spec;
//	color = (kd * ao * shadows) + spec;
	
	// TODO: make flag
	// fog
	color *= exp(-EPSILON * d * d * d);
	
	// gamma-correct
	color = pow(color, vec3(.4545));
	
	return vec4(color, 1.);
}


float intersect(vec3 ro, vec3 rd, float max_dist)
{
	float t = EPSILON * 100.;
	for (int i = 0; i < MAX_STEPS; ++i) {
		float d = scene_sdf(ro + (rd * t));
		if (d < EPSILON || t >= max_dist) {
			return t;
		}
		t += d;
	}
	return t;
}

vec3 get_shading(vec3 v, vec3 n, vec3 p, Material m)
{
	vec3 r = normalize(-reflect(v,n));
	float dotrv = dot(r, v);
	float dotnv = dot(n, v);
	vec3 diffuseColor = m.kd * (1.0 - m.metallic);
	vec3 specularColor = m.ks;
	float roughness = clamp(m.roughness, MINIMUM_ROUGHNESS, MAXIMUM_ROUGHNESS);
	vec3 spec = vec3(0.);
	// Go through local lights
	for (int i = 0; i < MAX_LIGHTS; ++i) {
		if (i == light_count) {
			break ;
		}
		Light_ light = lights[i];
		vec3 pl = light.p - p;
		float light_dist = length(pl);
		vec3 l = pl / light_dist;
		float shadow = soft_shadows(p, l, light_dist);
		float ao = ambient_occlusion(p, n);
		float dotln = dot(l, n);
		float dotrl = dot(r, l);
		vec3 h = normalize(v + l);
		float dotvl = dot(l, v);
		float dotnh = dot(n, h);
		float dotvh = dot(v, h);
		vec3 brdfSpec = D_Beckmann(roughness, dotnh) * V_SmithGGXCorrelated(roughness, dotnv, dotln) * F_Fresnel(specularColor, dotvh);
		vec3 brdfDiff = Diffuse_OrenNayar(diffuseColor, roughness, dotnv, dotln, dotvh);
		vec3 brdf = brdfDiff + brdfSpec;
		spec += brdf * light.m.kd * shadow * ao * saturate(dotln);
	}
	return spec;
}

float scene_sdf(vec3 p)
{
	// TODO: initialize materials and move to outer function
	// Intialize lights
	light_count = 0;
	// Intialize scene data
	scene_p_ = p;
	scene_sd_ = MAX_DISTANCE;
	scene_m_ = black;
	Scene();
	return scene_sd_;
}

vec3 get_normal(vec3 p) {
	vec2 e = vec2(1.0, -1.0) * EPSILON;
	return normalize(
		e.xyy * scene_sdf(p + e.xyy) +
		e.yyx * scene_sdf(p + e.yyx) +
		e.yxy * scene_sdf(p + e.yxy) +
		e.xxx * scene_sdf(p + e.xxx));
}

Material scene_m(vec3 p)
{
	return scene_m_;
}

float ambient_occlusion(vec3 p, vec3 n)
{
	// TODO: make steps a uniform
	const int steps = 3;
	const float delta = 0.5;

	float a = 0.0;
	float weight = 0.75;
	for(int i = 1; i <= steps; ++i) {
		float d = (float(i) / float(steps)) * delta; 
		a += weight * (d - scene_sdf(p + n * d));
		weight *= 0.5;
	}
	return clamp(1.0 - a, 0.0, 1.0);
}

float soft_shadows(vec3 ro, vec3 rd, float light_dist)
{
    float res = 1.0;
    float ph = MAX_DISTANCE;
	float t = EPSILON * 100;
	while(t < light_dist)
    {	
        float h = scene_sdf(ro + rd * t);
        if( h < EPSILON)
            return 0.0;
        float y = h * h / (2. * ph);
        float d = sqrt(h * h- y * y);
        res = min(res, MAX_SSK * d / max(0.,t - y));
        ph = h;
        t += h;
    }
    return res;
}

float sdf_shape(Shape shape)
{
	if (shape.t == -1) {
		return shape.sd;
	} else if (shape.t == 1) {
		return sdf_sphere(scene_p_ + shape.p, shape.a.x);
	} else if (shape.t == 2) {
		return sdf_box(scene_p_ + shape.p, shape.a);
	} else if (shape.t == 2.5) {
		return sdf_roundbox(scene_p_ + shape.p, shape.a, shape.b.x);
	} else if (shape.t == 3) {
		return sdf_torus(scene_p_ + shape.p, shape.a.xy);
	} else if (shape.t == 3.5) {
		return sdf_cappedtorus(scene_p_ + shape.p, shape.a.xy, shape.a.z,
				shape.b.x);
	} else if (shape.t == 4) {
		return sdf_cylinder(scene_p_ + shape.p, shape.a, shape.b, shape.c.x);
	} else if (shape.t == 5) {
		return sdf_cone(scene_p_ + shape.p,
			shape.a, shape.b, shape.c.x, shape.c.y);
	} else if (shape.t == 6) {
		return sdf_plane(scene_p_ + shape.p, vec4(shape.a, shape.b.x));
	} else if (shape.t == 7) {
		return sdf_hex_prism(scene_p_ + shape.p, shape.a.xy);
	} else if (shape.t == 8) {
		return sdf_tri_prism(scene_p_ + shape.p, shape.a.xy);
	} else if (shape.t == 9) {
		return sdf_capsule(scene_p_ + shape.p, shape.a, shape.b, shape.c.x);
	} else if (shape.t == 10) {
		return sdf_ellipsoid(scene_p_ + shape.p, shape.a);
	} else if (shape.t == 11) {
		return sdf_octahedron(scene_p_ + shape.p, shape.a.x);
	} else if (shape.t == 12) {
		return udf_triangle(scene_p_ + shape.p, shape.a, shape.b, shape.c);
	} else if (shape.t == 13) {
		return udf_quad(scene_p_ + shape.p, shape.a, shape.b, shape.c, shape.d);
	} else if (shape.t == 14) {
		return sdf_mandelbulb(scene_p_ + shape.p);
	} else {
		return MAX_DISTANCE;
	}
}
float sdf_sphere(vec3 p, float s)
{
	return length(p) - s;
}
float sdf_box(vec3 p, vec3 b)
{
	vec3 d = abs(p) - b;
	return length(max(d,0.0)) + min(max(d.x,max(d.y,d.z)),0.0);
}
float sdf_roundbox(vec3 p, vec3 b, float r)
{
	vec3 d = abs(p) - b;
	return length(max(d,0.0)) - r + min(max(d.x,max(d.y,d.z)),0.0);
}
float sdf_torus(vec3 p, vec2 t)
{
	vec2 q = vec2(length(p.xz)-t.x,p.y);
	return length(q)-t.y;
}
float sdf_cappedtorus(vec3 p, vec2 sc, float ra, float rb)
{
	p.x = abs(p.x);
	float k = (sc.y*p.x>sc.x*p.y) ? dot(p.xy,sc) : length(p.xy);
	return sqrt( dot(p,p) + ra*ra - 2.0*ra*k ) - rb;
}
float sdf_cylinder(vec3 p, vec3 a, vec3 b, float r)
{
	// TODO: fix style
	vec3 pa = p - a;
	vec3 ba = b - a;
	float baba = dot(ba,ba);
	float paba = dot(pa,ba);

	float x = length(pa*baba-ba*paba) - r*baba;
	float y = abs(paba-baba*0.5)-baba*0.5;
	float x2 = x*x;
	float y2 = y*y*baba;
	float d = (max(x,y)<0.0)?-min(x2,y2):(((x>0.0)?x2:0.0)+((y>0.0)?y2:0.0));
	return sign(d)*sqrt(abs(d))/baba;
}

float sdf_cone(vec3 p, vec3 a, vec3 b, float ra, float rb)
{
	// TODO: fix style
	float rba  = rb-ra;
	float baba = dot(b-a,b-a);
	float papa = dot(p-a,p-a);
	float paba = dot(p-a,b-a)/baba;

	float x = sqrt( papa - paba*paba*baba );

	float cax = max(0.0,x-((paba<0.5)?ra:rb));
	float cay = abs(paba-0.5)-0.5;

	float k = rba*rba + baba;
	float f = clamp( (rba*(x-ra)+paba*baba)/k, 0.0, 1.0 );

	float cbx = x-ra - f*rba;
	float cby = paba - f;
	
	float s = (cbx < 0.0 && cay < 0.0) ? -1.0 : 1.0;
	
	return s*sqrt( min(cax*cax + cay*cay*baba,
					   cbx*cbx + cby*cby*baba) );
}

float sdf_plane(vec3 p, vec4 n)
{
	n = normalize(n);  // TODO: is this necessary?
	return dot(p, n.xyz) + n.w;
}
float sdf_hex_prism(vec3 p, vec2 h)
{
	const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
	p = abs(p);
	p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;
	vec2 d = vec2(
			length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x), h.x))*sign(p.y-h.x),
			p.z-h.y );
	return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}
float sdf_tri_prism(vec3 p, vec2 h)
{
	vec3 q = abs(p);
	return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);
}
float sdf_capsule(vec3 p, vec3 a, vec3 b, float r)
{
	vec3 pa = p - a, ba = b - a;
	float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	return length(pa - ba*h) - r;
}
float sdf_ellipsoid(vec3 p, vec3 r)
{
	float k0 = length(p/r);
	float k1 = length(p/(r*r));
	return k0*(k0-1.0)/k1;
}
float sdf_octahedron(vec3 p, float s)
{
	p = abs(p);
	float m = p.x + p.y + p.z - s;
	vec3 q;
	
	if (3. * p.x < m) {
		q = p.xyz;
	} else if (3. * p.y < m) {
		q = p.yzx;
	} else if (3. * p.z < m ) {
		q = p.zxy;
	} else {
		return m * 0.57735027;
	}
	
	float k = clamp(.5 * (q.z - q.y + s), 0., s);
	return length(vec3(q.x, q.y - s + k, q.z - k));
}
float udf_triangle(vec3 p, vec3 a, vec3 b, vec3 c)
{
	vec3 ba = b - a; vec3 pa = p - a;
	vec3 cb = c - b; vec3 pb = p - b;
	vec3 ac = a - c; vec3 pc = p - c;
	vec3 nor = cross( ba, ac );

	return sqrt(
			(sign(dot(cross(ba,nor),pa)) +
			 sign(dot(cross(cb,nor),pb)) +
			 sign(dot(cross(ac,nor),pc))<2.0)
			? min(min(
					dot2(ba*clamp(dot(ba,pa)/dot2(ba),0.0,1.0)-pa),
					dot2(cb*clamp(dot(cb,pb)/dot2(cb),0.0,1.0)-pb)),
				dot2(ac*clamp(dot(ac,pc)/dot2(ac),0.0,1.0)-pc))
			: dot(nor,pa)*dot(nor,pa)/dot2(nor));
}
float udf_quad(vec3 p, vec3 a, vec3 b, vec3 c, vec3 d)
{
	vec3 ba = b - a;
	vec3 pa = p - a;
	vec3 cb = c - b;
	vec3 pb = p - b;
	vec3 dc = d - c;
	vec3 pc = p - c;
	vec3 ad = a - d;
	vec3 pd = p - d;
	vec3 n = cross(ba, ad);

	return sqrt(
		(sign(dot(cross(ba,n),pa))
		 + sign(dot(cross(cb,n),pb))
		 + sign(dot(cross(dc,n),pc))
		 + sign(dot(cross(ad,n),pd)) < 3.)
		? min(min(min(
					dot2(ba * saturate(dot(ba, pa) / dot2(ba)) - pa),
					dot2(cb * saturate(dot(cb, pb) / dot2(cb)) - pb)),
				dot2(dc * saturate(dot(dc, pc) / dot2(dc)) - pc)),
			dot2(ad * saturate(dot(ad,pd) / dot2(ad)) - pd))
		: dot(n, pa) * dot(n, pa) / dot2(n));
}
float sdf_mandelbulb(vec3 p)
{
	vec3 w = p;
	float m = dot(w,w);
	vec4 trap = vec4(abs(w),m);
	float dz = 1.0;

	for (int i = 0; i < 4; ++i) {
		float m2 = m*m;
		float m4 = m2*m2;
		dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

		float x = w.x; float x2 = x*x; float x4 = x2*x2;
		float y = w.y; float y2 = y*y; float y4 = y2*y2;
		float z = w.z; float z2 = z*z; float z4 = z2*z2;

		float k3 = x2 + z2;
		float k2 = inversesqrt(k3*k3*k3*k3*k3*k3*k3);
		float k1 = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
		float k4 = x2 - y2 + z2;

		w.x = p.x +  64.0*x*y*z*(x2-z2)*k4*(x4-6.0*x2*z2+z4)*k1*k2;
		w.y = p.y + -16.0*y2*k3*k4*k4 + k1*k1;
		w.z = p.z +  -8.0*y*k4*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k1*k2;

		trap = min(trap, vec4(abs(w),m));

		m = dot(w,w);
		if (m > 256.0) {
			break;
		}
	}
	return 0.25*log(m)*sqrt(m)/dz;
}

#ifdef PBR

float D_GGX(float roughness, float NoH, const vec3 h) {
    float oneMinusNoHSquared = 1.0 - NoH * NoH;
    float a = NoH * roughness;
    float k = roughness / (oneMinusNoHSquared + a * a);
    float d = k * k * (1.0 / PI);
    return d;
}

float D_Beckmann( float roughness, float NoH )
{
    float a2 = roughness * roughness;
	float NoH2 = NoH * NoH;
	return exp( (NoH2 - 1) / (a2 * NoH2) ) / ( PI * a2 * NoH2 * NoH2 );
}

float V_SmithGGXCorrelated(float roughness, float NoV, float NoL) {
    float a2 = roughness * roughness;
    float lambdaV = NoL * sqrt((NoV - a2 * NoV) * NoV + a2);
    float lambdaL = NoV * sqrt((NoL - a2 * NoL) * NoL + a2);
    float v = 0.5 / (lambdaV + lambdaL);
    return v;
}

vec3 F_Fresnel( vec3 SpecularColor, float VoH )
{
	vec3 SpecularColorSqrt = sqrt( clamp( vec3(0, 0, 0), vec3(0.99, 0.99, 0.99), SpecularColor ) );
	vec3 n = ( 1 + SpecularColorSqrt ) / ( 1 - SpecularColorSqrt );
	vec3 g = sqrt( n*n + VoH*VoH - 1 );
	vec3 v1 = (g - VoH) / (g + VoH);
	vec3 v2 = ((g+VoH)*VoH - 1) / ((g-VoH)*VoH + 1);
	return 0.5 * v1 * v1 * ( 1 + v2 * v2);
}

vec3 Diffuse_OrenNayar( vec3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH )
{
	float a = Roughness * Roughness;
	float s = a;// / ( 1.29 + 0.5 * a );
	float s2 = s * s;
	float VoL = 2 * VoH * VoH - 1;		// double angle identity
	float Cosri = VoL - NoV * NoL;
	float C1 = 1 - 0.5 * s2 / (s2 + 0.33);
	float C2 = 0.45 * s2 / (s2 + 0.09) * Cosri * ( Cosri >= 0 ? (1/ max( NoL, NoV ) ) : 1 );
	return DiffuseColor / PI * ( C1 + C2 ) * ( 1 + Roughness * 0.5 );
}

vec3 ImportanceSampleGGX(vec2 Xi, float Roughness, vec3 N )
{
	float a = Roughness * Roughness;
	float Phi = 2 * PI * Xi.x;
	float CosTheta = sqrt( (1 - Xi.y) / ( 1 + (a*a - 1) * Xi.y ) );
	float SinTheta = sqrt( 1 - CosTheta * CosTheta );
	vec3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;
	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector, N ) );
	vec3 TangentY = cross( N, TangentX );
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

vec4 UniformSampleSphere( vec2 E )
{
	float Phi = 2 * PI * E.x;
	float CosTheta = 1 - 2 * E.y;
	float SinTheta = sqrt( 1 - CosTheta * CosTheta );

	vec3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;

	float PDF = 1.0 / (4 * PI);

	return vec4( H, PDF );
}

vec4 CosineSampleHemisphere( vec2 E, vec3 N ) 
{
	vec3 H = UniformSampleSphere( E ).xyz;
	H = normalize( N + H );

	float PDF = H.z * (1.0 /  PI);

	return vec4( H, PDF );
}

vec2 Hammersley( uint Index, uint NumSamples, ivec2 Random)
{
	float E1 = fract( float(Index) / NumSamples + float( Random.x & 0xffff ) / (1<<16) );
	float E2 = float( bitfieldReverse(Index) ^ uint(Random.y)) * 2.3283064365386963e-10;
	return vec2( E1, E2 );
}

#endif
