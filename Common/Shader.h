#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader
{
public:
	unsigned int ProgramID;
	Shader(const string & _vertex, const string & _fragment, const string & _geometry = "", bool isFile = true) {
		string vertexStrSource;
		string fragmentStrSource;
		string geometryStrSource;
		if (!isFile) {
			vertexStrSource = _vertex;
			fragmentStrSource = _fragment;
			geometryStrSource = _geometry;
		}
		else {
			ifstream vertexShaderFile;
			ifstream fragmentShaderFile;
			ifstream geometryShaderFile;
			vertexShaderFile.open(_vertex);
			fragmentShaderFile.open(_fragment);
			geometryShaderFile.open(_geometry);
			if (!vertexShaderFile.is_open() || !fragmentShaderFile.is_open() || (!_geometry.empty() && !geometryShaderFile.is_open()))
				cout << "Open shader files failed." << endl;
			else {
				stringstream stringVertexStream;
				stringstream stringFragmentStream;
				stringstream stringGeometryStream;
				stringVertexStream << vertexShaderFile.rdbuf();
				stringFragmentStream << fragmentShaderFile.rdbuf();
				stringGeometryStream << geometryShaderFile.rdbuf();
				vertexStrSource = stringVertexStream.str();
				fragmentStrSource = stringFragmentStream.str();
				geometryStrSource = stringGeometryStream.str();
			}
			if (vertexShaderFile.is_open())
				vertexShaderFile.close();
			if (fragmentShaderFile.is_open())
				fragmentShaderFile.close();
			if (geometryShaderFile.is_open())
				geometryShaderFile.close();
		}

		const char * vertexSource = vertexStrSource.c_str();
		const char * fragmentSource = fragmentStrSource.c_str();
		const char * geometrySource = geometryStrSource.c_str();

		unsigned int vertexShader, fragmentShader, geometryShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		CheckCompileErrors(vertexShader, GL_COMPILE_STATUS, "compile vertexShader failed.", glGetShaderiv, glGetShaderInfoLog);

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		CheckCompileErrors(fragmentShader, GL_COMPILE_STATUS, "compile fragmentShader failed.", glGetShaderiv, glGetShaderInfoLog);

		if (!geometryStrSource.empty()) {
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &geometrySource, NULL);
			glCompileShader(geometryShader);
			CheckCompileErrors(geometryShader, GL_COMPILE_STATUS, "compile geometryShader failed.", glGetShaderiv, glGetShaderInfoLog);
		}

		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vertexShader);
		glAttachShader(ProgramID, fragmentShader);
		if (!geometryStrSource.empty())
			glAttachShader(ProgramID, geometryShader);
		glLinkProgram(ProgramID);

		CheckCompileErrors(ProgramID, GL_LINK_STATUS, "link program failed.", glGetProgramiv, glGetProgramInfoLog);
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
		if (!geometryStrSource.empty())
			glDeleteShader(geometryShader);
	}
	void Use() {
		glUseProgram(ProgramID);
	}
	void Set1UniformValue(const string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
	}
	void Set1UniformValue(const string &name, int value) const {
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
	}
	void Set1UniformValue(const string &name, float value) const {
		glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
	}
    void Set2UniformValue(const string &name, float value1, float value2) const {
        glUniform2f(glGetUniformLocation(ProgramID, name.c_str()), value1, value2);
    }
	void Set4UniformValue(const string &name, float value0, float value1, float value2, float value3 = 1.f) const {
		glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), value0, value1, value2, value3);
	}
    void SetMat4(const string &name, const glm::mat4 & value) const {
		glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
	virtual ~Shader() {

	}
private:
	void CheckCompileErrors(unsigned int target, unsigned int type, const string & errorMsg, function<void(unsigned int, unsigned int, int*)> getResult, function<void(unsigned int, int, int*, char*)> getLog) {
		int isSuccess;
		char infoLog[1024];
		memset(infoLog, 0, sizeof(infoLog));

		getResult(target, type, &isSuccess);
        if (!isSuccess){
            getLog(target, sizeof(infoLog), NULL, infoLog);
			cout << errorMsg << " : " << infoLog << ". " << target << " : " << type << endl;
        }
	}
};

#endif // !SHADER_H
