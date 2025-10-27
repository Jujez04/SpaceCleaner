#pragma once

#include <glad/glad.h>

namespace vrtx {

#define Vertex float;

	class VertexArray {
	private:
		unsigned int id;
	public:
		VertexArray() {
			glGenVertexArrays(1, &id);
		}

		void bind() const {
			glBindVertexArray(id);
		}

		void unbind() const { 
			glBindVertexArray(0);
		}

		unsigned int getID() const {
			return id;
		}
	};

	class VertexBuffer {
	private:
		unsigned int id;
	public:
		VertexBuffer(const void* data, unsigned int size) {
			glGenBuffers(1, &id);
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		void bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, id);
		}

		void unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		unsigned int getID() const {
			return id;
		}

		void setData(const void* data, unsigned int size) {
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}
	};


}