#include "pch.h"
#include "PrimitiveMesh.h"

#include "Core/Assets/Assets/Mesh.h"

namespace DuskEngine
{
	const char* PrimitiveMesh::m_Primitives[] = { "Quad", "Cube" };

	Mesh PrimitiveMesh::Quad()
    {
		float vertices[] =
		{
			// positions        // tex		// normals
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
		};

		unsigned int indices[] =
		{
			3, 1, 0,   // first triangle
			3, 2, 1    // second triangle
		};

		LOG("Loading Quad");

        return Mesh(vertices, (unsigned int)sizeof(vertices),
			indices, (unsigned int)(sizeof(indices) / sizeof(unsigned int)), MeshType::Quad);
    }

	// This is terrible, redo in the future

	Mesh PrimitiveMesh::Cube()
	{
		float vertices[] =
		{
			 // positions       // tex		// normals

			 //FRONT
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
			 //RIGHT
			 0.5f,  0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 
			 0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 //TOP
			  0.5f,  0.5f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			  0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			  -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			  -0.5f, 0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 //LEFT
			 -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			 -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			 -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
			 -0.5f, 0.5f, -1.0f, 0.0f, 1.0f,-1.0f, 0.0f, 0.0f,
			 //BOTTOM
			 0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			 -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			 -0.5f, -0.5f,-1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			 //BACK
			  0.5f,  0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 -0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 -0.5f,  0.5f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		};

		unsigned int indices[] =
		{
			//FRONT
			3, 1, 0,
			3, 2, 1,
			//RIGHT
			7, 5, 4,
			7, 6, 5,
			//TOP
			11, 9, 8,
			11, 10, 9,
			//LEFT
			15,13,12,
			15,14,13,
			//BOTTOM
			17,19,16,
			17,18,19,
			//BACK
			21,23,20,
			21,22,23
		};

		LOG("Loading Cube");

		return Mesh(vertices, (unsigned int)sizeof(vertices),
			indices, (unsigned int)(sizeof(indices) / sizeof(unsigned int)), MeshType::Cube);
	}
}

