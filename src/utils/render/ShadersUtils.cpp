#include "ShadersUtils.h"

GLuint ShadersUtils::loadShaders(const char *vertexShaderPath, const char *fragShaderPath) {
    ifstream vertexFile, fragFile;

    // Ensure ifstream objects can throw exceptions
    vertexFile.exceptions(ifstream::failbit | ifstream::badbit);
    fragFile.exceptions(ifstream::failbit | ifstream::badbit);

    // Read the files
    string vertexSourceString, fragSourceString;
    try {
        // Open the files
        vertexFile.open(vertexShaderPath);
        fragFile.open(fragShaderPath);

        // Read the file buffer contents into streams
        stringstream vertexStream, fragStream;
        vertexStream << vertexFile.rdbuf();
        fragStream << fragFile.rdbuf();

        // Close file handlers
        vertexFile.close();
        fragFile.close();

        // Convert the stream into x string
        vertexSourceString = vertexStream.str();
        fragSourceString = fragStream.str();
    } catch (ifstream::failure &e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
    }

    // Get the C-style strings
    const char *vertexSource = vertexSourceString.c_str();
    const char *fragSource = fragSourceString.c_str();

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexSource, nullptr);
    glCompileShader(vertexShaderId);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragSource, nullptr);
    glCompileShader(fragmentShaderId);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);

    return programId;
}

