#include "EngineMesh.h"

#include "Globals.h"
#include "Application.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "SDL/include/SDL.h"
#include "MathGeoLib.h"
#include "ModuleCamera.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

EngineMesh::EngineMesh()
{
}

EngineMesh::~EngineMesh()
{
	glDeleteBuffers(1, &vbo);
}

void EngineMesh::LoadVBO(const tinygltf::Model& inModel, const tinygltf::Mesh& inMesh, const tinygltf::Primitive& inPrimitive)
{
	const auto& positionIterator = inPrimitive.attributes.find("POSITION");
	
	if (positionIterator != inPrimitive.attributes.end())
	{
		const tinygltf::Accessor& positionAccessor = inModel.accessors[positionIterator->second];
		
		SDL_assert(positionAccessor.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(positionAccessor.componentType == GL_FLOAT);

		vertexCount = positionAccessor.count;

		const tinygltf::BufferView& positionBufferView = inModel.bufferViews[positionAccessor.bufferView];
		const tinygltf::Buffer& positionBuffer = inModel.buffers[positionBufferView.buffer];

		const unsigned char* bufferStart = &(positionBuffer.data[positionAccessor.byteOffset + positionBufferView.byteOffset]);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * positionAccessor.count, nullptr, GL_STATIC_DRAW);
		
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		for (size_t i = 0; i < positionAccessor.count; ++i)
		{
			ptr[i] = *reinterpret_cast<const float3*>(bufferStart);

			// bufferView.byteStride == 0 -> Only positions inside buffer, which then the stride becomes space between vertices -> sizeof(float) * 3.
			bufferStart += positionBufferView.byteStride == 0 ? sizeof(float) * 3 : positionBufferView.byteStride;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}

void EngineMesh::LoadEBO(const tinygltf::Model& inModel, const tinygltf::Mesh& inMesh, const tinygltf::Primitive& inPrimitive)
{
	if (inPrimitive.indices >= 0)
	{
		const tinygltf::Accessor& indexAccessor = inModel.accessors[inPrimitive.indices];
		const tinygltf::BufferView& indexBufferView = inModel.bufferViews[indexAccessor.bufferView];
		const tinygltf::Buffer& indexBuffer = inModel.buffers[indexBufferView.buffer];

		indexCount = indexAccessor.count;

		const unsigned char* bufferStart = &(indexBuffer.data[indexAccessor.byteOffset + indexBufferView.byteOffset]);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		
		if (indexAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const unsigned int* bufferInd = reinterpret_cast<const unsigned int*>(bufferStart);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		if (indexAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const unsigned short* bufferInd = reinterpret_cast<const unsigned short*>(bufferStart);
			for (unsigned short i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void EngineMesh::Render(int program)
{
	float4x4 proj, view, model;
	
	proj = App->GetCamera()->GetProjectionMatrix();
	view = App->GetCamera()->GetViewMatrix();
	
	model = float4x4::FromTRS(
				float3(2.0f, 0.0f, 0.0f),
				float4x4::RotateZ(pi / 4.0f),
				float3(2.0f, 1.0f, 1.0f));
	
	glUseProgram(program);

	glUniformMatrix4fv(0, 1, GL_TRUE, &proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);

	if (indexCount > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
}
