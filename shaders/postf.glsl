#version 410


uniform sampler2D   renderedTexture;
in   vec2 vertexPassThrough;
out  vec4 outColor;

#define edge_thres .2
#define edge_thres2 5.0
#define HueLevCount 6
#define SatLevCount 7
#define ValLevCount 4
float[HueLevCount] HueLevels = float[] (0.0,140.0,160.0,240.0,240.0,360.0);
float[SatLevCount] SatLevels = float[] (0.0,0.15,0.3,0.45,0.6,0.8,1.0);
float[ValLevCount] ValLevels = float[] (0.0,0.3,0.6,1.0);


vec3 RGBtoHSV( float r, float g, float b) 
{
	float minv, maxv, delta;
	vec3 res;

	minv = min(min(r, g), b);
	maxv = max(max(r, g), b);
	res.z = maxv;

	delta = maxv - minv;

	if (maxv != 0.0)
		res.y = delta / maxv;
   	else {
		res.y = 0.0;
		res.x = -1.0;
		return res;
	}

	if( r == maxv )
		res.x = ( g - b ) / delta;      // between yellow & magenta
	else if( g == maxv )
		res.x = 2.0 + ( b - r ) / delta;   // between cyan & yellow
	else
		res.x = 4.0 + ( r - g ) / delta;   // between magenta & cyan

	res.x *= 60.0;            // degrees
	res.x += -180.0 * (sign(res.x) - 1);
		
	return res;
}

vec3 HSVtoRGB(float h, float s, float v ) 
{
   int i;
   float f, p, q, t;
   vec3 res;

   if( s == 0.0 ) 
   {
      // achromatic (grey)
      res.x = v;
      res.y = v;
      res.z = v;
      return res;
   }

   h /= 60.0;         // sector 0 to 5
   i = int(floor( h ));
   f = h - float(i);         // factorial part of h
   p = v * ( 1.0 - s );
   q = v * ( 1.0 - s * f );
   t = v * ( 1.0 - s * ( 1.0 - f ) );

   switch(i) 
   {
      case 0:
         res.x = v;
         res.y = t;
         res.z = p;
         break;
      case 1:
         res.x = q;
         res.y = v;
         res.z = p;
         break;
      case 2:
         res.x = p;
         res.y = v;
         res.z = t;
         break;
      case 3:
         res.x = p;
         res.y = q;
         res.z = v;
         break;
      case 4:
         res.x = t;
         res.y = p;
         res.z = v;
         break;
      default:      // case 5:
         res.x = v;
         res.y = p;
         res.z = q;
         break;
   }
   return res;
}

float nearestLevel(float col, int mode) 
{
	int levCount;
	if (mode == 0) levCount = HueLevCount;
	if (mode == 1) levCount = SatLevCount;
	if (mode == 2) levCount = ValLevCount;

	for (int i =0; i<levCount-1; i++ ) {
		if (mode == 0) {
			if (col >= HueLevels[i] && col <= HueLevels[i + 1]) {
				return HueLevels[i + 1];
			}
		}
		if (mode == 1) {
			if (col >= SatLevels[i] && col <= SatLevels[i + 1]) {
				return SatLevels[i + 1];
			}
		}
		if (mode == 2) {
			if (col >= ValLevels[i] && col <= ValLevels[i + 1]) {
				return ValLevels[i + 1];
			}
		}
	}
}

// averaged pixel intensity from 3 color channels
float avg_intensity(vec4 pix) 
{
	return (pix.r + pix.g + pix.b) / 3.;
}

vec4 get_pixel(vec2 coords, float dx, float dy) 
{
	return texture(renderedTexture, coords + vec2(dx, dy));
}

// returns pixel color
float IsEdge(in vec2 coords)
{
	float dxtex = 1.0 / float(textureSize(renderedTexture, 0)) ;
	float dytex = 1.0 / float(textureSize(renderedTexture, 0));
	float pix[9];
	int k = -1;
	float delta;

	for (int i=-1; i<2; i++) {
		for(int j=-1; j<2; j++) {
			k++;
			pix[k] = avg_intensity(get_pixel(coords,float(i)*dxtex,
												float(j)*dytex));
		}
	}

	delta = (abs(pix[1]-pix[7])+
			abs(pix[5]-pix[3]) +
			abs(pix[0]-pix[8]) +
			abs(pix[2]-pix[6])
			) / 4.;

	return clamp(edge_thres2 * delta, 0.0, 1.0);
}

vec4 cartoon(vec4 colorOrg)
{
	vec2 uv = vertexPassThrough * .5 + .5;
	vec4 tc = vec4(1.0, 0.0, 0.0, 1.0);

	vec3 vHSV = RGBtoHSV(colorOrg.r, colorOrg.g, colorOrg.b);
	vHSV.x = nearestLevel(vHSV.x, 0);
	vHSV.y = nearestLevel(vHSV.y, 1);
	vHSV.z = nearestLevel(vHSV.z, 2);
	float edge = IsEdge(uv);
	vec3 vRGB = (edge > .2) ? 
	vec3(0.0, 0.0, 0.0):HSVtoRGB(vHSV.x, vHSV.y, vHSV.z);
	return vec4(vRGB, 1.0);
}

vec4 sepia(vec4 inColor)
{
	vec4 outSepia;

	outSepia.r = min((inColor.r * .393) + (inColor.g *.769) + (inColor.b * .189), 1.0);
	outSepia.g = min((inColor.r * .349) + (inColor.g *.686) + (inColor.b * .168), 1.0);
	outSepia.b = min((inColor.r * .272) + (inColor.g *.534) + (inColor.b * .131), 1.0);
	outSepia.a = 1.0;
	return outSepia;
}

vec4 fxaa()
{
	const float FXAA_SPAN_MAX = 8.0;
	const float FXAA_REDUCE_MUL = 1.0/8.0;
	const float FXAA_REDUCE_MIN = 1.0/128.0;
	vec2 res = vec2(640., 360.);
	vec2 offset = 1.0/res.xy;
	vec2 uv = vertexPassThrough.xy * .5 + .5;

	vec3 nw = texture(renderedTexture, uv + vec2(-1., 1.) * offset).rgb;
	vec3 ne = texture(renderedTexture, uv + vec2(1., 1.) * offset).rgb;
	vec3 sw = texture(renderedTexture, uv + vec2(-1., -1.) * offset).rgb;
	vec3 se = texture(renderedTexture, uv + vec2(1., -1.) * offset).rgb;
	vec3 m = texture(renderedTexture, uv).rgb;

	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lumaNW = dot(nw, luma);
	float lumaNE = dot(ne, luma);
	float lumaSW = dot(sw, luma);
	float lumaSE = dot(se, luma);
	float lumaM = dot(m, luma);

	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	vec2 dir = vec2(
        -((lumaNW + lumaNE) - (lumaSW + lumaSE)),
        ((lumaNW + lumaSW) - (lumaNE + lumaSE)));
	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
	dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * rcpDirMin)) * offset;

	vec3 rgbA = .5 * (texture(renderedTexture, uv + dir * (1.0 / 3.0 - 0.5)).xyz + texture(renderedTexture, uv + dir * (2.0 / 3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (texture(renderedTexture, uv + dir * -0.5).xyz + texture(renderedTexture, uv + dir * 0.5).xyz);
    float lumaB = dot(rgbB, luma);
	if (lumaB < lumaMin || lumaB > lumaMax) {
		return vec4(rgbA, 1.0);
	} else {
		return vec4(rgbB, 1.0);
	}
}



void main()
{
	vec2 texCoord = vertexPassThrough * 0.5 + 0.5;
    outColor = texture(renderedTexture, texCoord);

    //outColor = fxaa();

    //outColor = cartoon(outColor);

    //outColor = sepia(inColor);
	//outColor = fxaa();
}
