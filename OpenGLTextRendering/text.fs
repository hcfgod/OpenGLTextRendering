#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    float distance = texture(text, TexCoords).r;
    float alpha = smoothstep(0.45, 0.55, distance);
    color = vec4(textColor, alpha);
}