#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace vrtx {

	struct Vertex {
		glm::vec2 position;
		glm::vec2 textureCoor;
		glm::vec4 color;

		Vertex(const glm::vec2& pos = glm::vec2(0.0f),
			const glm::vec2& tex = glm::vec2(0.0f),
			const glm::vec4& col = glm::vec4(1.0f))
			: position(pos), textureCoor(tex), color(col) {
		}
	};

	struct VertexBufferElement {
		unsigned int type;
		unsigned int count;
		bool normalized;

		VertexBufferElement(unsigned int type, unsigned int count, bool normalized)
			: type(type), count(count), normalized(normalized) {
		}

		static unsigned int getSizeOfType(unsigned int type) {
			switch (type) {
			case GL_FLOAT:			return sizeof(GLfloat);
			case GL_UNSIGNED_INT:	return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:	return sizeof(GLubyte);
			}
			return 0;
		}
	};

	class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride = 0;
	public:
		VertexBufferLayout() = default;
		~VertexBufferLayout() = default;

		template<typename T>
		void push(int count);

		template<>
		void push<float>(int count) {
			elements.emplace_back(GL_FLOAT, count, GL_FALSE);
			stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
		}

		template<>
		void push<unsigned int>(int count) {
			elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
			stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void push<unsigned char>(int count) {
			elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_FALSE);
			stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline unsigned int getStride() const { return stride; }
		inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	};


	class VertexBuffer {
	private:
		unsigned int rendererId;
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer() {
			glDeleteBuffers(1, &rendererId);
		}

		void bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		}

		void unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	};

	class VertexArray {
	private:
		unsigned int id;
	public:
		VertexArray() {
			glGenVertexArrays(1, &id);
		}
		~VertexArray() {
			glDeleteVertexArrays(1, &id);
		}

		void bind() const {
			glBindVertexArray(id);
		}

		void unbind() const { 
			glBindVertexArray(0);
		}

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl) {
			vb.bind();
			const auto& elements = vbl.getElements();
			unsigned int offset = 0;
			for (unsigned int i = 0; i < elements.size(); i++) {
				const auto& element = elements[i];
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(
					i,
					element.count,
					element.type,
					element.normalized ? GL_TRUE : GL_FALSE,
					static_cast<GLsizei>(vbl.getStride()),
					reinterpret_cast<const void*>(static_cast<uintptr_t>(offset))
				);
				offset += element.count * VertexBufferElement::getSizeOfType(element.type);
			}
		}

	};



	class IndexBuffer {
	private:
		unsigned int rendererId;
		unsigned int count;
	public:
		IndexBuffer(const unsigned int* data, const unsigned int count) 
			:count(count)
		{
			glGenBuffers(1, &rendererId);
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
		}
		~IndexBuffer() {
			glDeleteBuffers(1, &rendererId);
		}

		void bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
		}

		void unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	};

}