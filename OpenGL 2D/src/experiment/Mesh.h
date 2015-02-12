#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/ProgressHandler.hpp>

#include "..\gl\Buffer.hpp"

#include "..\math\Vector.h"

#include "..\rendering\Vertex.hpp"
#include "..\rendering\Drawable.hpp"
#include "..\rendering\Transform.hpp"
#include "..\rendering\RenderTarget2D.h"

#include <vector>
#include <string>

class Mesh : public Drawable, public Transform
{
public:

	Mesh(const std::string& filename, Color c = Color::White)
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
		std::cout << "Attempting to load: " << filename << std::endl;

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile("./res/models/" + filename,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace);

		if (!scene)
		{
			std::cout << importer.GetErrorString() << std::endl;
			assert(0 == 0);
		}

		std::cout << "Succesfully loaded: " << filename << std::endl;

		const aiMesh* model = scene->mMeshes[0];

		m_vertices.reserve(model->mNumVertices);
		m_verticesOutline.reserve(model->mNumVertices);

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		for (unsigned int i = 0; i < model->mNumVertices; i++)
		{
			const aiVector3D pos = model->mVertices[i];

			m_vertices.push_back(Vertex(Vector3f(pos.x, pos.y, pos.z), c));
			m_verticesOutline.push_back(Vertex(Vector3f(pos.x, pos.y, pos.z), Color::Black));
		}

		for (unsigned int i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			m_indices.push_back(face.mIndices[0]);
			m_indices.push_back(face.mIndices[1]);
			m_indices.push_back(face.mIndices[2]);
		}

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

		setScale(0.1, 0.1, 0.1);
		rotate(Vector3f(0, 1, 0), ToRadians(-90));
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();

		target.draw(&m_vertices[0], m_vertices.size(), Triangles, states, false);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		target.draw(&m_verticesOutline[0], m_verticesOutline.size(), Triangles, states, false);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();
	}

private:

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;

	std::vector<Vertex> m_verticesOutline;
};