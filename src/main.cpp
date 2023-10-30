#include "base.hpp"


GLFWwindow* window = nullptr;
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
const char* WINDOW_TITLE = "TESTSs";
const char* SHADER_SOURCE = "shaders\\Basic.shader";
GLuint shader_program, vao, vbo, ibo;
// GLuint vcolor;

bool initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Falhou ao tentar inicializar o GLFW." << std::endl;
        return 0;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Falha ao criar a janela a partir do GLFW." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

bool initGLAD() {
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Não foi possível iniciar o contexto OpenGL." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }
    return true;
}

void start() {
    ShaderProgramSource shader_source = parserShader(SHADER_SOURCE);
    shader_program = createProgram(shader_source.vertex_source, shader_source.fragment_source);
    GLCall(glUseProgram(shader_program));

    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    GLfloat vertices[] = {
        // Vértice frontal
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        // Vértice superior
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        // Vértice esquerdo
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        // Vértice inferior
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        // Vértice direito
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        // Vértice traseiro
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f
    };

    for(int i = 0; i < 6*4*3; i++) {
        vertices[i] *= .5;
    }
    
    static const GLuint indices[] = {
        // Face frontal
        0, 1, 2,
        2, 3, 0,
        // Face superior
        4, 5, 6,
        6, 7, 4,
        // Face traseira
        8, 9, 10,
        10, 11, 8,
        // Face inferior
        12, 13, 14,
        14, 15, 12,
        // Face esquerda
        16, 17, 18,
        18, 19, 16,
        // Face direita
        20, 21, 22,
        22, 23, 20
    };
    
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    // especifica o atributo de posição
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

    // criar um buffer de índices
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    // encerra as vinculações
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void render() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    GLCall(glUseProgram(shader_program));

    GLCall(GLuint vcolor = glGetUniformLocation(shader_program, "vcolor"));
    GLCall(glUniform3f(vcolor, 0.3f, 0.2f, 0.7f));

    // Definição da matriz de projeção
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);
    GLCall(GLuint projLoc = glGetUniformLocation(shader_program, "projection"));
    GLCall(glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)));

    // Renderizar apenas as arestas (wireframe)
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    // 6 blocos de 2 por 3 colunas - indices
    GLCall(glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_INT, nullptr));

    // Renderizar apenas as arestas (wireframe)
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    glfwSwapBuffers(window);
}

void loop() {
    start();
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
    }

    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDeleteBuffers(1, &vbo));
    GLCall(glDeleteBuffers(1, &ibo));
    GLCall(glDeleteVertexArrays(1, &vao));
    GLCall(glDeleteProgram(shader_program));

    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    if (!initGLFW()) {
        return EXIT_FAILURE;
    }
    if (!initGLAD()) {
        return EXIT_FAILURE;
    }
    loop();
    return 0;
}