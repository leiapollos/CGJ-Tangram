#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Constants.h"

namespace engine {

	typedef struct {
		GLfloat x, y, z;
	} Vertex;

	typedef struct {
		GLfloat u, v;
	} Texcoord;

	typedef struct {
		GLfloat nx, ny, nz;
	} Normal;

	class Mesh {
	public:
		GLuint VaoId;
		bool TexcoordsLoaded, NormalsLoaded;
		std::vector <Vertex> Vertices, vertexData;
		std::vector <Texcoord> Texcoords, texcoordData;
		std::vector <Normal> Normals, normalData;

		std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

	public:
		void parseVertex(std::stringstream& sin);
		void parseTexcoord(std::stringstream& sin);
		void parseNormal(std::stringstream& sin);
		void parseFace(std::stringstream& sin);
		void parseLine(std::stringstream& sin);
		void loadMeshData(std::string& filename);
		void processMeshData();
		void freeMeshData();
		const void createMesh(std::string& filename);
		void createBufferObjects();
		void destroyBufferObjects();
		void draw();
	};
}