#version 410

uniform sampler2D   renderedTexture;
in   vec2 vertexPassThrough;
out  vec4 outColor;
void main()
{
     vec2 texCoord = vertexPassThrough * 0.5 + 0.5;
     vec4 inColor = texture(renderedTexture, texCoord);
     //sepia tone.
     //outColor.r = min((inColor.r * .393) + (inColor.g *.769) + (inColor.b * .189), 1.0);
     //outColor.g = min((inColor.r * .349) + (inColor.g *.686) + (inColor.b * .168), 1.0);
     //outColor.b = min((inColor.r * .272) + (inColor.g *.534) + (inColor.b * .131), 1.0);
     //outColor.a = 1.0;
     outColor = inColor;
}
