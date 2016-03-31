#include "Shader.h"

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

void compile_info(const GLuint shader)
{
	GLint status(0);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (1 != status)
	{
		GLint maxLength(0);
		GLint logLength(0);

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar * log = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &logLength, log);

		std::cout << "Compiling shader failed." << std::endl
			<< log << std::endl;
	}
}

void link_info(const GLuint program)
{
	GLint status(0);
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (1 != status)
	{
		GLint maxLength(0);
		GLint logLength(0);

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar * log = new GLchar[maxLength];
		glGetProgramInfoLog(program, maxLength, &logLength, log);

		std::cout << "Linking program failed." << std::endl
			<< log << std::endl;
	}
}

Shader::Shader(ShaderType type, const std::string & sourceFile) :
	m_ShaderType(type),
	m_ShaderSourceFile(sourceFile)
{
	std::ifstream file(m_ShaderSourceFile);
	if (file.is_open())
	{
		m_ShaderSource.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());		
	}	

	m_ShaderID = glCreateShader(static_cast<GLenum>(m_ShaderType));

	const char* shaderSource = m_ShaderSource.data();
	glShaderSource(m_ShaderID, 1, &shaderSource, nullptr);
	glCompileShader(m_ShaderID);
	compile_info(m_ShaderID);
}

GLuint Shader::shaderID()
{
    return m_ShaderID;
}

Program::Program()
{
    m_ProgramID = glCreateProgram();
}

void Program::attach(Shader* shader)
{
    glAttachShader(m_ProgramID, shader->shaderID());
}

void Program::link()
{
    glLinkProgram(m_ProgramID);
    link_info(m_ProgramID);
}

void Program::setUniform(const std::string &name, const glm::mat4 &matrix)
{
    auto loc = glGetUniformLocation(m_ProgramID, name.data());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::setUniform(const std::string& name, const glm::vec3& vector)
{
    auto loc = glGetUniformLocation(m_ProgramID, name.data());
    glUniform3fv(loc, 1, glm::value_ptr(vector));
}

void Program::setUniform(const std::string& name, float value)
{
    auto loc = glGetUniformLocation(m_ProgramID, name.data());
    glUniform1f(loc, value);
}

void Program::bind()
{
    glUseProgram(m_ProgramID);
}

void Program::unbind()
{
    glUseProgram(0);
}
