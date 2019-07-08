#version 410


uniform sampler2D   renderedTexture;
in   vec2 vertexPassThrough;
out  vec4 outColor;

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
    vec4 inColor = texture(renderedTexture, texCoord);

    //outColor = sepia(inColor);

	outColor = fxaa();
}
