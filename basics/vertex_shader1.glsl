#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
    // vertexColor = vec4(190/255.f, 195/255.f, 30/255.f, 255/255.f);
}