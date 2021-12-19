#include"shader.h"

std::string get_file_contents(const char* filename) {
  std::ifstream in(filename, std::ios::binary);
  if (in) {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
  std::string vertexStdStr = get_file_contents(vertexFile);
  std::string fragmentStdStr = get_file_contents(fragmentFile);

  const char* vertexCode = vertexStdStr.c_str();
  const char* fragmentCode = fragmentStdStr.c_str();

  // ========== Load Shader ==========
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // 1 means 1 screen for the shader
  glShaderSource(vertexShader, 1, &vertexCode, NULL);
  glCompileShader(vertexShader);
  compileErrors(vertexShader, "VERTEX");

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // 1 means 1 screen for the shader
  glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
  glCompileShader(fragmentShader);
  compileErrors(fragmentShader, "FRAGMENT");

  // attach shader to object called shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  // clear memory
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Activate() {
  glUseProgram(ID);
}

void Shader::Delete() {
  glDeleteProgram(ID);
}

void Shader::compileErrors(GLuint shader, const char* type) {
  GLint compiled;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_CLINKING_ERROR for:" << type << "\n" << std::endl;
    }
  }
}