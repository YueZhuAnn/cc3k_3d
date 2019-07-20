#include "draw/wall.hpp"

using namespace std;
using namespace glm;

Wall::Wall(){
    m_mode = GL_TRIANGLES;
    m_size = 6*2*3;
    m_type = GL_UNSIGNED_INT;
}

Wall::~Wall(){}

void Wall::registerUniform(){
    setUniformPos("P");
    setUniformPos("V");
    setUniformPos("M");
    setUniformPos("colour");
}

void Wall::uploadModel(){
    float verts[8*3];

    unsigned index = 0;
    // upload the vertices
    for(unsigned x = 0; x < 2; x++){
        for(unsigned y = 0; y < 2; y++){
            for(unsigned z = 0; z < 2; z++){
                verts[index*3] = x*1-0.5f;
                verts[index*3+1] = y*1;
                verts[index*3+2] = z*1-0.5f;
                index++;
            }
        }
    }

    // upload the indices
    unsigned indices[] = {
        0, 1, 2, 1, 2, 3, 
        1, 3, 5, 3, 5, 7,
        4, 5, 6, 5, 6, 7,
        0, 2, 4, 2, 4, 6,
        0, 1, 4, 1, 4, 5,
        2, 3, 6, 3, 6, 7
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