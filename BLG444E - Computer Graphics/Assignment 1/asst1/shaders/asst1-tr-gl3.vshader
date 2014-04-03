#version 130

uniform int uWidth;
uniform int uHeight;

in vec2 aPosition;
in vec2 aTexCoord;
in vec3 acolor;

out vec2 vTexCoord;
out vec3 vcolor;

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
			gl_Position = vec4(ratio * sign, aPosition.y, 0, 1);
		}
	}
	else{
		ratio = (w/h)*abs(aPosition.y);
		if(aPosition.y == 0)
			gl_Position = vec4(aPosition.x, 0, 0, 1);
		else{
			sign = aPosition.y / abs(aPosition.y);
			gl_Position = vec4(aPosition.x, ratio * sign, 0, 1);
		}
	}

	//gl_Position = vec4(aPosition.x, aPosition.y, 0, 1);

    vTexCoord = aTexCoord;
	vcolor = acolor;
}
