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

#include "..\window\Clock.h"

#include <vector>
#include <string>

class Mesh : public Drawable, public Transform
{
public:

	Mesh(const std::string& filename, Color c = Color::Black, bool flag = false)
		:
		m_verticesBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		m_indicesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
		Clock clock;

		std::cout << "Attempting to load: " << filename << std::endl;

		Assimp::Importer importer;

		std::string file("./res/models/");

		if (!flag)
		{
			file += filename;
		}
		else
		{
			file = filename;
		}

		const aiScene* scene = importer.ReadFile(file,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace);

		if (!scene)
		{
			std::cout << importer.GetErrorString() << std::endl;
			assert(0 == 0);
		}

		const aiMesh* model = scene->mMeshes[0];
		
		m_vertices.reserve(model->mNumVertices);
		m_wireframe.reserve(model->mNumVertices);

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		 
		bool hasNormals = model->HasNormals();
		bool hasTexCoords = model->HasTextureCoords(0);

		for (unsigned int i = 0; i < model->mNumVertices; i++)
		{
			const aiVector3D pos = model->mVertices[i];
			const aiVector3D normal = hasNormals ? model->mNormals[i] : aiZeroVector;
			const aiVector3D texCoord = hasTexCoords ? model->mTextureCoords[0][i] : aiZeroVector;
		
			m_vertices.emplace_back(Vector3f(pos.x, pos.y, pos.z), c, Vector2f(texCoord.x, texCoord.y), Vector3f(normal.x, normal.y, normal.z));
			m_wireframe.emplace_back(Vector3f(pos.x, pos.y, pos.z), Color::Black);

		}

		if (!hasNormals)
		{
			calcNormals();
		}

		m_indices.reserve(model->mNumFaces*3);

		for (unsigned int i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			m_indices.push_back(face.mIndices[0]);
			m_indices.push_back(face.mIndices[1]);
			m_indices.push_back(face.mIndices[2]);
		}

		setScale(0.1, 0.1, 0.1);
		rotate(Vector3f(0, 1, 0), ToRadians(-90));

		m_verticesBuffer.data(m_vertices.size() * sizeof(Vertex), m_vertices.data());
		m_indicesBuffer.data(m_indices.size() * sizeof(GLuint), m_indices.data());

		std::cout << "Succesfully loaded : " << filename << " in " << clock.getElapsedTime().asMilliseconds() << "ms" << std::endl;
	}

	Mesh(Mesh&& other) :
		Drawable(std::move(other)),
		Transform(std::move(other)),
		m_verticesBuffer(std::move(other.m_verticesBuffer)),
		m_indicesBuffer(std::move(other.m_indicesBuffer)),
		m_vertices(std::move(other.m_vertices)),
		m_wireframe(std::move(other.m_wireframe)),
		m_indices(std::move(other.m_indices))
	{

	}

	Mesh& operator=(Mesh&& other)
	{
		if (this != &other)
		{
			m_verticesBuffer = std::move(other.m_verticesBuffer);
			m_indicesBuffer = std::move(other.m_indicesBuffer);
			m_vertices = std::move(other.m_vertices);
			m_wireframe = std::move(other.m_wireframe);
			m_indices = std::move(other.m_indices);
		}

		return *this;
	}


	void calcNormals()
	{
		std::vector<Vector3f> m_normals;
		m_normals.reserve(m_vertices.size());

		for (unsigned int i = 0; i < m_vertices.size(); i++)
			m_normals.push_back(Vector3f(0, 0, 0));

		for (unsigned int i = 0; i < m_indices.size(); i += 3)
		{
			int i0 = m_indices[i];
			int i1 = m_indices[i + 1];
			int i2 = m_indices[i + 2];

			Vector3f v1 = m_vertices[i1].position - m_vertices[i0].position;
			Vector3f v2 = m_vertices[i2].position - m_vertices[i0].position;

			Vector3f normal = v1.Cross(v2).Normalized();

			m_normals[i0] = m_normals[i0] + normal;
			m_normals[i1] = m_normals[i1] + normal;
			m_normals[i2] = m_normals[i2] + normal;
		}

		for (unsigned int i = 0; i < m_normals.size(); i++)
		{
			//m_normals[i] = m_normals[i].Normalized();
			m_vertices[i].normals = m_normals[i].Normalized();
		}
	}

	virtual void draw(RenderTarget2D& target, RenderStates states) const
	{
		m_verticesBuffer.bind();
		m_indicesBuffer.bind();

		states.transform *= getTransform();

		target.drawDeferred(&m_vertices[0], m_vertices.size(), Triangles, states);

		m_verticesBuffer.unbind();
		m_indicesBuffer.unbind();
	}

private:

	Buffer m_verticesBuffer;
	Buffer m_indicesBuffer;

	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;

	std::vector<Vertex> m_wireframe;
};