#include "config.h"

#define MAXWIDTH 800
#define MAXHEIGHT 600

float vertices[] = {
    // Posição          // Cor              //Coordenadas da Textura
     0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,   0.5f, 0.5f, // V0
    // Hexagono
     0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   0.5f, 1.0f,// V1
     1.0f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.75f,// V2
     1.0f,-0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 0.25f,// V3
     0.0f,-1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   0.5f, 0.0f,// V4
    -1.0f,-0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.25f,// V5
    -1.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.75f// V6
}; 

unsigned int indices[] = {
    // Triângulos a partir do centro (V0)
    0, 1, 2,  // Centro, Topo, Topo-direita
    0, 2, 3,  // Centro, Topo-direita, Base-direita
    0, 3, 4,  // Centro, Base-direita, Base
    0, 4, 5,  // Centro, Base, Base-esquerda
    0, 5, 6,  // Centro, Base-esquerda, Topo-esquerda
    0, 6, 1   // Centro, Topo-esquerda, Topo
};

//Função para rodar o shader
std::string readShaderFile(const char* filePath){
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    //Verificando se o aruivo foi aberto corretamente
    if (!fileStream.is_open()) {
        std::cerr << "Could not open file " << filePath << ". File does not exist." << std::endl;
        return "";
    }
    //Lendo o arquivo linha por linha
    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    //Fechando o arquivo
    fileStream.close();
    return content;
}

// Compilando o shader
unsigned int compileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    // Verificando se houve erro na compilação do shader
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetShaderInfoLog(id, length, &length, &message[0]);
        std::cerr << "Failed to compile shader: " << &message[0] << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

// Criando o shader
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    // Verificando se houve erro na ligação do programa
    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetProgramInfoLog(program, length, &length, &message[0]);
        std::cerr << "Failed to link program: " << &message[0] << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(MAXWIDTH, MAXHEIGHT, "OPENGL", NULL, NULL);

    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Lendo os shaders
    std::string vertexShaderSource = readShaderFile("../src/shader/vertex.glsl");
    std::string fragmentShaderSource = readShaderFile("../src/shader/fragment.glsl");

    int indexCount = sizeof(indices) / sizeof(indices[0]); // Contando o número de índices
    
    if(vertexShaderSource.empty() || fragmentShaderSource.empty()) {
        std::cerr << "Error reading shader files." << std::endl;
        glfwTerminate();
        return -1;
    }
    // Criando o shader
    unsigned int shader = createShader(vertexShaderSource, fragmentShaderSource);

    // setando os dados do vertex e buffers
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Atributo Vertice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Atributo Cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Desligando o buffer
    glBindVertexArray(0); // Desligando o VAO

    // Loop principal
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); 

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // Desenhando o triângulo

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}