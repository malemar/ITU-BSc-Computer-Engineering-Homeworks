#version 130

uniform float uVertexScale;
uniform int uWidth;
uniform int uHeight;

in vec2 aPosition;
in vec2 aTexCoord;

out vec2 vTexCoord;
out vec2 vTemp;

void main() {
	float ratio;
	float w = uWidth;
	float h = uHeight;
	float sign;

	if(uWidth > uHeight){
		ratio = (h/w)*abs(aPosition.x);
		if(aPosition.x == 0)
			gl_Position = vec4(0, aPosition.y, 0, 1);
		else{
			sign = aPosition.x / abs(aPosition.x);
			gl_Position = vec4(ratio * sign * uVertexScale, aPosition.y, 0, 1);
		}
	}
	else{
		ratio = (w/h)*abs(aPosition.y);
		if(aPosition.y == 0)
			gl_Position = vec4(aPosition.x * uVertexScale, 0, 0, 1);
		else{
			sign = aPosition.y / abs(aPosition.y);
			gl_Position = vec4(aPosition.x * uVertexScale, ratio * sign, 0, 1);
		}
	}

	//gl_Position = vec4(aPosition.x * uVertexScale, aPosition.y, 0, 1);

    vTexCoord = aTexCoord;
    vTemp = vec2(1, 1);
}
