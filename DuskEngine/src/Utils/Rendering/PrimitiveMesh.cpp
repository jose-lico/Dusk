#include "pch.h"
#include "PrimitiveMesh.h"

#include "Core/Resources/Resources/Mesh.h"

namespace DuskEngine
{
	const char* PrimitiveMesh::m_Primitives[] = { "Quad", "Cube" };

    Ref<Mesh> PrimitiveMesh::Quad()
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
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		Ref<Mesh> mesh = MakeRef<Mesh>(vertices, (unsigned int)sizeof(vertices),
			indices, (unsigned int)(sizeof(indices) / sizeof(unsigned int)), MeshType::Quad);

        return mesh;
    }

	// This is terrible, redo in the future

	Ref<Mesh> PrimitiveMesh::Cube()
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
			0, 1, 3,
			1, 2, 3,
			//RIGHT
			4, 5, 7,
			5, 6, 7,
			//TOP
			8, 9, 11,
			9, 10, 11,
			//LEFT
			12,13,15,
			13,14,15,
			//BOTTOM
			16,19,17,
			19,18,17,
			//BACK
			20,23,21,
			23,22,21
		};

		Ref<Mesh> mesh = MakeRef<Mesh>(vertices, (unsigned int)sizeof(vertices),
			indices, (unsigned int)(sizeof(indices) / sizeof(unsigned int)), MeshType::Cube);

		return mesh;
	}
}

