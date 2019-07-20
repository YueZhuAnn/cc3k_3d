#include "drawpotion.hpp"

using namespace std;
using namespace glm;

DrawPotion::DrawPotion(){
    m_mode = GL_TRIANGLES;
    m_size = 3*8;
    m_type = GL_UNSIGNED_INT;
}

DrawPotion::~DrawPotion(){}

void DrawPotion::registerUniform(){
    setUniformPos("P");
    setUniformPos("V");
    setUniformPos("M");
    setUniformPos("colour");
}

void DrawPotion::uploadModel(){
    float verts[6*3] = {
        -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, - 0.5f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    // upload the indices
    unsigned indices[] = {
        0, 2, 4, 0, 2, 5,
        1, 2, 4, 1, 2, 5,
        1, 3, 4, 1, 3, 5,
        0, 3, 4, 0, 3, 5,
    };

    // Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_vao );
	glBindVertexArray( m_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(verts),
		verts, GL_STATIC_DRAW );

    // Create the cube elements buffer
    glGenBuffers( 1, &m_ebo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), 
        indices, GL_STATIC_DRAW );

    // Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}