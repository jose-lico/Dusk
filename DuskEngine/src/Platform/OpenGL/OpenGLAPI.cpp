#include "pch.h"
#include "OpenGLAPI.h"

#include "GLCommon.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"
#include "Shader.h"

namespace DuskEngine :: OpenGLAPI 
{
	int GetUniformLocation(Shader& shader, const std::string& name);
	uint32_t CompileShader(GLenum type, const std::string& source);

	void Init()
	{
		TRACE("OpenGL API Created");
	}

	void SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void SetBufferData(uint32_t& id, BufferType buffer, UsageType usage, void* data, size_t size)
	{
		glGenBuffers(1, &id);
		glBindBuffer((GLenum)buffer, id);
		glBufferData((GLenum)buffer, size, data, (GLenum)usage);
	}

	void BindBuffer(BufferType BufferType, uint32_t id)
	{
		glBindBuffer((GLenum)BufferType, id);
	}

	void FreeBuffer(uint32_t& id)
	{
		glDeleteBuffers(1, &id);
	}

	void SetTextureData(Texture& textureData, const unsigned char* data, bool offset)
	{
		glGenTextures(1, &textureData.ResourceID);
		glBindTexture((GLenum)textureData.Type, textureData.ResourceID);

		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri((GLenum)textureData.Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D((GLenum)textureData.Type, 0, (GLenum)textureData.Format, textureData.Width, textureData.Height, 0, 
			(GLenum)textureData.Format, GL_UNSIGNED_BYTE, data + (offset ? sizeof(TextureData) : 0));

		if(textureData.Mipmaps)
			glGenerateMipmap((GLenum)textureData.Type);
	}

	void BindTexture(uint32_t slot, uint32_t id)
	{
		glBindTextureUnit(slot, id);
	}

	void FreeTexture(uint32_t& id)
	{
		glDeleteTextures(1, &id);
	}

	void CreateProgram(Shader& shader ,const std::string& vertexShader, const std::string& fragmentShader)
	{
		shader.ResourceID = glCreateProgram();
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(shader.ResourceID, vs);
		glAttachShader(shader.ResourceID, fs);
		glLinkProgram(shader.ResourceID);
		glValidateProgram(shader.ResourceID);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void UseProgram(uint32_t id)
	{
		glUseProgram(id);
	}

	void DeleteProgram(uint32_t id)
	{
		glDeleteProgram(id);
	}

	void SetUniformInt(Shader& shader, const std::string& name, int i)
	{
		glUniform1i(GetUniformLocation(shader, name), i);
	}

	void SetUniformFloat(Shader& shader, const std::string& name, float f)
	{
		glUniform1f(GetUniformLocation(shader, name), f);
	}

	void SetUniformVec2(Shader& shader, const std::string& name, const glm::vec2& v)
	{
		glUniform2f(GetUniformLocation(shader, name), v.x, v.y);
	}

	void SetUniformVec3(Shader& shader, const std::string& name, const glm::vec3& v)
	{
		glUniform3f(GetUniformLocation(shader, name), v.x, v.y, v.z);
	}

	void SetUniformVec4(Shader& shader, const std::string& name, const glm::vec4& v)
	{
		glUniform4f(GetUniformLocation(shader, name), v.x, v.y, v.z, v.w);
	}

	void SetUniformMat3(Shader& shader, const std::string& name, const glm::mat3& m)
	{
		glUniformMatrix3fv(GetUniformLocation(shader, name), 1, GL_FALSE, &m[0][0]);
	}

	void SetUniformMat4(Shader& shader, const std::string& name, const glm::mat4& m)
	{
		glUniformMatrix4fv(GetUniformLocation(shader, name), 1, GL_FALSE, &m[0][0]);
	}

	void UnbindVAO()
	{
		glBindVertexArray(0);
	}

	void DrawIndexed(const VertexArray& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray.GetIndexBuffer().Count, GL_UNSIGNED_INT, nullptr);
	}

	void DrawArrays(unsigned int start, unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, start, count);
	}

	int GetUniformLocation(Shader& shader, const std::string& name)
	{
		if (shader.UniformLocations.find(name) != shader.UniformLocations.end())
			return shader.UniformLocations[name];

		int location = glGetUniformLocation(shader.ResourceID, name.c_str());

		shader.UniformLocations[name] = location;

		return location;
	}

	uint32_t CompileShader(GLenum type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int success;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			WARN("ERROR::SHADER::COMPILATION_FAILED");
			WARN(infoLog);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}
}