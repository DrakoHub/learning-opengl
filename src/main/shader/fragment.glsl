#version 330 core
in vec3 MyColour; //Variavel Recebida de Cor
in vec2 TextureCoordinate // Variavel de Textura

out vec4 FragColor;

uniform sampler2D MyTexture

void main() {
    FragColor = texture(MyTexture, TextureCoordinate) * vec4(MyColour, 1.0);
}