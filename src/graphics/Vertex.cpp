/**
 * @file Vertex.cpp
 * @brief Implementazione delle classi per la gestione dei buffer di vertici e indici.
 */

#include "graphics/Vertex.h"

namespace vrtx {

    // ======================== Vertex ========================

    Vertex::Vertex(const glm::vec2& pos, const glm::vec3& col)
        : position(pos), color(col) {
    }

    // =================== VertexBufferElement ===================

    VertexBufferElement::VertexBufferElement(unsigned int type, unsigned int count, bool normalized)
        : type(type), count(count), normalized(normalized) {
    }

    unsigned int VertexBufferElement::getSizeOfType(unsigned int type) {
        switch (type) {
        case GL_FLOAT:         return sizeof(GLfloat);
        case GL_UNSIGNED_INT:  return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        default:               return 0;
        }
    }

    // =================== VertexBufferLayout ===================

    template<> void VertexBufferLayout::push<float>(int count) {
        elements.emplace_back(GL_FLOAT, count, GL_FALSE);
        stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    template<> void VertexBufferLayout::push<unsigned int>(int count) {
        elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
        stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    template<> void VertexBufferLayout::push<unsigned char>(int count) {
        elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_FALSE);
        stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    unsigned int VertexBufferLayout::getStride() const { return stride; }
    const std::vector<VertexBufferElement>& VertexBufferLayout::getElements() const { return elements; }

    // ======================== VertexBuffer ========================

    VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
        glGenBuffers(1, &rendererId);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &rendererId);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, rendererId);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // ======================== VertexArray ========================

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &id);
    }

    void VertexArray::bind() const {
        glBindVertexArray(id);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl) {
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

    // ======================== IndexBuffer ========================

    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
        : count(count)
    {
        glGenBuffers(1, &rendererId);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &rendererId);
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
    }

    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // namespace vrtx
