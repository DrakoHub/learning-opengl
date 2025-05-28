#version 330 core
in vec3 MyColour; //Variavel Recebida de Cor

out vec4 FragColor;


void main() {
    FragColor = vec4(MyColour, 1.0);
}