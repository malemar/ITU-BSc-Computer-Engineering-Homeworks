#version 130

uniform sampler2D uTex;

in vec2 vTexCoord;
in vec3 vcolor;

out vec4 fragColor;

void main(void) {
  vec4 color = vec4(vcolor, 1);

  // The texture(...) calls always return a vec4. Data comes out of a texture in RGBA format
  vec4 texColor = texture(uTex, vTexCoord);
  
  fragColor = texColor*color;
}
