#include "pch.h"
#include "OpenGLAPI.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Framebuffer.h"

#include "GLFW/glfw3.h"
#include "GL/glew.h"

namespace DuskEngine :: OpenGLAPI 
{
	int GetUniformLocation(Shader& shader, const std::string& name);
	uint32_t CompileShader(GLenum type, const std::string& source);
	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
#if DUSK_DEBUG
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
		GLsizei length, const char* message, const void* userParam);
#endif

	void CreateContext(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK)
			FATAL("Failed to initialize glew!");

		TRACE("OpenGL Context created.");

#if DUSK_DEBUG
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			TRACE("OpenGL Debug Output enabled.");
		}
#endif

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

	void SwapBuffers(GLFWwindow* window)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void SetBufferData(OpenGLBuffer& buffer, void* data)
	{
		glGenBuffers(1, &buffer.ResourceID);
		glBindBuffer((GLenum)buffer.BufferType, buffer.ResourceID);
		glBufferData((GLenum)buffer.BufferType, buffer.Size, data, (GLenum)buffer.UsageType);
	}

	void BindBuffer(const OpenGLBuffer& buffer)
	{
		glBindBuffer((GLenum)buffer.BufferType, buffer.ResourceID);
	}

	void FreeBuffer(OpenGLBuffer& buffer)
	{
		glDeleteBuffers(1, &buffer.ResourceID);
	}

	void GenVertexArrays(VertexArray& array)
	{
		glGenVertexArrays(1, &array.ResourceID);
	}

	void BindVertexArray(const VertexArray& array)
	{
		glBindVertexArray(array.ResourceID);
	}

	void UnbindVertexArray()
	{
		glBindVertexArray(0);
	}

	void SetVertexAttribPointer(VertexBuffer& vb)
	{
		const auto& elements = vb.Layout.Elements;

		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) 
		{
			const auto& element = elements[i];
			glVertexAttribPointer(i, element.Count, ShaderDataTypeToOpenGLBaseType(element.ShaderType), element.Normalized,
				vb.Layout.Stride, (void*)(size_t)offset);
			offset += element.Count * ShaderDataTypeSize(element.ShaderType);
			glEnableVertexAttribArray(i);
		}
	}

	void FreeVertexArray(VertexArray& va)
	{
		glDeleteVertexArrays(1, &va.ResourceID);
		FreeBuffer(va.VB);
		FreeBuffer(va.IB);
	}

	void SetTextureData(Texture& texture, const unsigned char* data, bool offset)
	{
		glGenTextures(1, &texture.ResourceID);
		glBindTexture((GLenum)texture.Type, texture.ResourceID);

		glTexParameteri((GLenum)texture.Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri((GLenum)texture.Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri((GLenum)texture.Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri((GLenum)texture.Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D((GLenum)texture.Type, 0, (GLenum)texture.Format, texture.Width, texture.Height, 0,
			(GLenum)texture.Format, GL_UNSIGNED_BYTE, data + (offset ? sizeof(TextureData) : 0));

		if(texture.Mipmaps)
			glGenerateMipmap((GLenum)texture.Type);
	}

	void BindTexture(const Texture& texture, uint32_t slot)
	{
		glBindTextureUnit(slot, texture.ResourceID);
	}

	void FreeTexture(Texture& texture)
	{
		glDeleteTextures(1, &texture.ResourceID);
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

	void UseProgram(const Shader& shader)
	{
		glUseProgram(shader.ResourceID);
	}

	void DeleteProgram(Shader& shader)
	{
		glDeleteProgram(shader.ResourceID);
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

	void ResizeFramebuffer(Framebuffer& buffer)
	{
		if (buffer.ResourceID)
		{
			glDeleteFramebuffers(1, &buffer.ResourceID);
			glDeleteTextures(1, &buffer.ColorAttachment);
			glDeleteTextures(1, &buffer.DepthAttachment);
		}

		glGenFramebuffers(1, &buffer.ResourceID);
		glBindFramebuffer(GL_FRAMEBUFFER, buffer.ResourceID);

		glGenTextures(1, &buffer.ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, buffer.ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, buffer.Width, buffer.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.ColorAttachment, 0);

		glGenTextures(1, &buffer.DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, buffer.DepthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, buffer.Width, buffer.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, buffer.DepthAttachment, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			ERR("Error in the Framebuffer");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void BindFramebuffer(const Framebuffer& buffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, buffer.ResourceID);
		glViewport(0, 0, buffer.Width, buffer.Height);
	}

	void UnbindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FreeFramebuffer(Framebuffer& buffer)
	{
		glDeleteFramebuffers(1, &buffer.ResourceID);
		glDeleteTextures(1, &buffer.ColorAttachment);
		glDeleteTextures(1, &buffer.DepthAttachment);
	}

	void DrawIndexed(const VertexArray& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray.IB.Count, GL_UNSIGNED_INT, nullptr);
	}

	void DrawArrays(unsigned int start, unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, start, count);
	}

	// #############################################################
	// HELPER FUNCS
	// #############################################################

	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::FLOAT:    return GL_FLOAT;
		case ShaderDataType::FLOAT2:   return GL_FLOAT;
		case ShaderDataType::FLOAT3:   return GL_FLOAT;
		case ShaderDataType::FLOAT4:   return GL_FLOAT;
		case ShaderDataType::MAT3:     return GL_FLOAT;
		case ShaderDataType::MAT4:     return GL_FLOAT;
		case ShaderDataType::INT:      return GL_INT;
		case ShaderDataType::INT2:     return GL_INT;
		case ShaderDataType::INT3:     return GL_INT;
		case ShaderDataType::INT4:     return GL_INT;
		case ShaderDataType::BOOL:     return GL_BOOL;
		}
		return 0;
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

#if DUSK_DEBUG
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
		const char* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string sourceMessage;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             sourceMessage = "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceMessage = "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceMessage = "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceMessage = "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     sourceMessage = "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           sourceMessage = "Source: Other"; break;
		}

		std::string typeMessage;
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               typeMessage = "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeMessage = "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeMessage = "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         typeMessage = "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         typeMessage = "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              typeMessage = "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          typeMessage = "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           typeMessage = "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               typeMessage = "Type: Other"; break;
		}

		std::string severityMessage;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         severityMessage = "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       severityMessage = "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          severityMessage = "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severityMessage = "Severity: notification"; break;
		}

		std::string errorMessage = "Error code: " + std::to_string(id) + ": " + message + "\n"
			+ sourceMessage + "\n" + typeMessage + "\n" + severityMessage;

		ERR(errorMessage);
	}
#endif
}