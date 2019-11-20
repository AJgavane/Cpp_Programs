/************************************************************************
 * This code converts the input Obj file and outputs a .cpp file
 * required for the PCSS code by NVIDIA.
 * the vertices in CPP file are stored in the following fashion
 * vertices[NumOfVertices] = {
 * {{position}, {color}, {normal}, {texcoord}},
 * {	"			"		"			"} , ...
 * };
 * https://github.com/NVIDIAGameWorks/GraphicsSamples/blob/master/samples/es3aep-kepler/SoftShadows/KnightModel.cpp
 *  
 **************************************************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "ParseObjFile.h"


void WriteDotHFile(const std::string& class_name, const std::string& once_only_header);
std::string UpperCase(const std::string& str);
void WriteCppFile(const std::string& class_name, const OBJModel& obj_model);
void WriteCppFile(const std::string& class_name, const IndexedModel& indexed_model);

int main(int argc, char* argv[])
{
	std::string onceOnlyHeader = "";
	
	std::string objFileName = "bunny.obj";
	std::string model = "Model";
	
	std::string className = objFileName.substr(0, objFileName.length() - 4);
	className[0] = className[0] - 32;

	onceOnlyHeader = UpperCase(className) + "_" +  UpperCase(model) + "_H";
	std::cout << onceOnlyHeader << std::endl;
	className += model;
	
	// Create the output .h and .cpp file
	//1. Create objModel.h file
	WriteDotHFile(className, onceOnlyHeader);

	//2. Read the obj file
	OBJModel ObjModel(objFileName);
	WriteCppFile(className, ObjModel);

	//IndexedModel IdxModel = ObjModel.ToIndexedModel();
	//WriteCppFile(className, IdxModel);

	std::cout << "Press Enter to Continue" << std::endl;
	std::cin.get();
}

void WriteCppFile(const std::string& class_name, const IndexedModel& indexed_model)
{
	std::ofstream dotCppFile;
	dotCppFile.open(class_name + ".cpp");

	dotCppFile << "#include \"SoftShadowsCommon.h\" " << std::endl;
	dotCppFile << "#include \"" + class_name + ".h\" \n" << std::endl;

	dotCppFile << "const uint32_t " + class_name + "::numVertices = " + std::to_string(indexed_model.positions.size()) << ";" << std::endl;
	dotCppFile << "const uint32_t " + class_name + "::numIndices = " + std::to_string(indexed_model.indices.size()) << ";" << std::endl;
	dotCppFile << std::endl;

	dotCppFile << "const ModelVertex " + class_name + "::vertices[" + std::to_string(indexed_model.positions.size()) + "] = {"
		<< std::endl;
	for (int i = 0; i < indexed_model.positions.size(); i++)
	{
		dotCppFile << "{ ";
		// Position
		dotCppFile << "{" + std::to_string(indexed_model.positions[i].x) + ", " + std::to_string(indexed_model.positions[i].y) + ", " + std::to_string(indexed_model.positions[i].z) + "}, ";

		// Normal
		dotCppFile << "{" + std::to_string(indexed_model.normals[i].x) + ", " + std::to_string(indexed_model.normals[i].y) + ", " + std::to_string(indexed_model.normals[i].z) + "}, ";
		// Color
		dotCppFile << "{" + std::to_string(indexed_model.normals[i].x) + ", " + std::to_string(indexed_model.normals[i].y) + ", " + std::to_string(indexed_model.normals[i].z) + "}, ";

		// Tex Coord
			dotCppFile << "{" + std::to_string(indexed_model.texCoords[i].x) + ", " + std::to_string(indexed_model.texCoords[i].y) + "} ";

		dotCppFile << "}, " << std::endl;
	}
	dotCppFile << "};" << std::endl;

	dotCppFile << "const uint16_t " + class_name + "::indices[" + std::to_string(indexed_model.indices.size()) + "] = {";
	for (int j = 0; j < indexed_model.indices.size(); j++)
	{
		if (j % 3 == 0)
			dotCppFile << std::endl;
		dotCppFile << indexed_model.indices[j] << ", ";
	}
	dotCppFile << "\n};" << std::endl;
	dotCppFile.close();
}

void WriteCppFile(const std::string& class_name, const OBJModel& obj_model)
{
	std::ofstream dotCppFile;
	dotCppFile.open(class_name + ".cpp");

	dotCppFile << "#include \"SoftShadowsCommon.h\" " << std::endl;
	dotCppFile << "#include \"" + class_name + ".h\" \n"<< std::endl;

	dotCppFile << "const uint32_t " + class_name + "::numVertices = " + std::to_string(obj_model.vertices.size()) << ";" << std::endl;
	dotCppFile << "const uint32_t " + class_name + "::numIndices = " + std::to_string(obj_model.OBJIndices.size()) << ";" << std::endl;
	dotCppFile << std::endl;

	dotCppFile << "const ModelVertex " + class_name + "::vertices["+ std::to_string(obj_model.vertices.size()) +"] = {"
	<< std::endl;
	for (int i = 0; i < obj_model.vertices.size(); i++)
	{
		dotCppFile << "{ ";
		// Position
		dotCppFile << "{" + std::to_string(obj_model.vertices[i].x) + ", " + std::to_string(obj_model.vertices[i].y) + ", " + std::to_string(obj_model.vertices[i].z) + "}, ";
		
		if(obj_model.hasNormals){
			// Normal
			dotCppFile << "{" + std::to_string(obj_model.normals[i].x) + ", " + std::to_string(obj_model.normals[i].y) + ", " + std::to_string(obj_model.normals[i].z) + "}, ";
			// Color
			dotCppFile << "{" + std::to_string(obj_model.normals[i].x) + ", " + std::to_string(obj_model.normals[i].y) + ", " + std::to_string(obj_model.normals[i].z) + "}, ";
		} else	{
			dotCppFile << "{}, ";	// Normals
			dotCppFile << "{}, ";	// Colors
		}
		
		// Tex Coord
		if(obj_model.hasUVs)
			dotCppFile << "{" + std::to_string(obj_model.uvs[i].x) + ", " + std::to_string(obj_model.uvs[i].y) + "} ";
		else
			dotCppFile << "{} ";

		dotCppFile << "}, " << std::endl;
	}
	dotCppFile << "};" << std::endl;

	dotCppFile << "const uint16_t " + class_name + "::indices[" + std::to_string(obj_model.OBJIndices.size()) + "] = {";
	for(int j = 0; j < obj_model.OBJIndices.size(); j++)
	{
		if(j%3 == 0)
			dotCppFile << std::endl;
		dotCppFile << obj_model.OBJIndices[j].vertexIndex << ", ";
	}
	dotCppFile << "\n};" << std::endl;
	dotCppFile.close();
}

void WriteDotHFile(const std::string& class_name, const std::string& once_only_header)
{
	// ofstream: output file stream
	std::ofstream dotHFile;
	dotHFile.open(class_name + ".h");
	dotHFile << "#pragma once\n";
	dotHFile << "#ifndef " + once_only_header + "\n";
	dotHFile << "#define " + once_only_header + "\n\n";
	dotHFile << "class " + class_name + "\n";
	dotHFile << R"({
public:

	static const uint32_t numVertices;
	static const ModelVertex vertices[];

	static const uint32_t numIndices;
	static const uint16_t indices[];
};

#endif )";
	dotHFile.close();
}


std::string UpperCase(const std::string& str)
{
	std::string temp = str;
	std::for_each(temp.begin(), temp.end(), [](char & c) {
		c = ::toupper(c);
	});
	return temp;
}
