#pragma once
#include <vector>
#include "h2bParser.h"
using namespace std::chrono;

struct SCENE_DATA
{
	GW::MATH::GVECTORF lightDirection;
	GW::MATH::GVECTORF lightColor;
	GW::MATH::GVECTORF lightAmbient;
	GW::MATH::GVECTORF lightPos;
	GW::MATH::GMATRIXF gwViewMatrix;
	GW::MATH::GMATRIXF gwProjectionMatrix;
	GW::MATH::GVECTORF padding[4];
};
struct MESH_DATA
{
	GW::MATH::GMATRIXF gwWorldMatrix;
	GW::MATH::GMATRIXF gwWorldMatrixTwo;
	H2B::ATTRIBUTES material;
	unsigned padding[12];
};
//struct ModelLocation
//{
//	std::vector<GW::MATH::GMATRIXF> gwWorldMatrix;
//};
//struct ModelMaterial
//{
//	std::vector<H2B::ATTRIBUTES> material;	// material information 
//};
//struct SceneLighting
//{
//	GW::MATH::GVECTORF lightDirection;
//	GW::MATH::GVECTORF lightColor;
//	GW::MATH::GVECTORF lightAmbient;
//	GW::MATH::GVECTORF lightPos;
//};

struct ModelInfo
{
	std::string modelNames;							// file path
	GW::MATH::GMATRIXF modelLocations;				// world matrix of model
	std::vector<H2B::MESH> modelMeshes;				// mesh information of model
	std::vector<H2B::ATTRIBUTES> modelMaterial;		// material information
	std::vector<H2B::VERTEX> modelVertex;			// vertex information
	std::vector<unsigned> modelIndecies;			// index information

	unsigned int meshDataStartLocation;				// meshCount at the time this model was made
	unsigned int vertexStartLocation;				// what vertex location to start at

	//unsigned int vertexBufferStart;				// keep track of models vertex buffer
	//unsigned int indexBufferStart;				// keep track of models index buffer
};

const char* vertexShader = "../VertexShader.hlsl";	// vertex shader path
const char* pixelShader = "../PixelShader.hlsl";	// pixel shader path

std::vector<ModelInfo> modelInformation;			// keep track of every models information
std::vector<H2B::VERTEX> vertexInfo;				// keep track of all vertecies
std::vector<unsigned> indexInfo;					// keep track of all indexes
std::vector<MESH_DATA> meshDataModels;				// keep track of all models MESH_DATA
SCENE_DATA sceneData;								// keep track of SCENE_DATA
unsigned int meshCount;								// keep track of all meshes in scene

GW::INPUT::GInput gInput;

float mouseYLast = 0.0f;							// last mouse Y position
float mouseXLast = 0.0f;							// last mouse X position

// DECLARE MATRIX INFORMATION
GW::MATH::GMATRIXF gwCopyMatrix;
GW::MATH::GMATRIXF gwWorldMatrix;
GW::MATH::GMATRIXF gwViewMatrix;
GW::MATH::GMATRIXF gwProjectionMatrix;
GW::MATH::GVECTORF lightDirection;
GW::MATH::GVECTORF lightColor;
GW::MATH::GVECTORF lightAmbient;
GW::MATH::GVECTORF lightPos;
unsigned int tempHeight = 0;
unsigned int tempWidth = 0;
float aspectRatio = 0.0f;

//ModelLocation modelLocations;					// instance of ModelLocation struct
//ModelMaterial modelMaterials;					// instance of ModelMaterial struct

//MESH_DATA meshDataLogo;

void ParseFiles()
{
	/// <summary>
		/// READING FILE START
		/// </summary>

		// run h2b file parser on the meshes you read in
		// array of matrix
		// parse gamelevel.txt
		//1.) Use either std::ifstream or FILE * to open the file.
	std::ifstream reader;
	std::string lineRead;
	std::string mFileName = "../GameLevel.txt";
	reader.open(mFileName);

	//int modelsReadIn = 0;							// keep track of how many models
	//bool sameModelName = false;						// bool to keep track of if a model name has been read in already or not

	//2.) Enter a infinite loop that stops when there are no more lines to read from the file.

	if (reader.is_open())
	{
		std::cout << "File is open - time to read" << std::endl;

		while (true)
		{
			if (reader.eof())
				break;
			//3.) Read the current line of text from the file up to the newline "\n". (can use std::getline)
			//	3b.) Exit the loop if nothing to read.
			std::getline(reader, lineRead);
			//4.) String compare for the name of what you want. (intially this will be "MESH")[std::strcmp]
			if (std::strcmp(lineRead.c_str(), "MESH") == 0)
			{
				std::vector<std::string> fileNames; // vector of saved file names
				ModelInfo tempModelInfo;			// Hold temporary model information
				//GW::MATH::GMATRIXF theNewMatrix;	// a matrix to store the global position

				//5.) Read the next line, this is the name of the .h2b file to load in. (duplicates?)
				std::cout << "FOUND A MESH" << std::endl;
				std::getline(reader, lineRead);

				// separate the .001 ect. from the string
				std::stringstream findName(lineRead);
				while (std::getline(findName, lineRead, '.'))
				{
					lineRead = "../models/" + lineRead + ".h2b"; // convert to correct file path
					fileNames.push_back(lineRead);
				}

				std::cout << "Model Name: " << fileNames[0] << std::endl; // for debugging
				tempModelInfo.modelNames = fileNames[0];

				//6.) Read the next 4 lines.Each line contains a row of the 4x4 matrix of where the model is.
				//	6b.) You can walk through the stringand use std::atof / std::stof to grab the floats.
				//	6c.) Alternatley you could use std::sscanf or just std::string helpers to pull the data.
				for (int i = 0; i < 4; i++)
				{
					std::vector<std::string> lineStrings;	// for separating out the strings
					std::vector<float> lineFloats;			// for separating out the floats

					std::getline(reader, lineRead);
					// breaking the matrix into 1.000, 1.000, 1.000, 1.000)>
					std::stringstream findParen(lineRead);
					while (std::getline(findParen, lineRead, '('))
					{
						lineStrings.push_back(lineRead);
					}
					// breaking the matrix into 1.000, 1.000, 1.000, 1.000)
					std::stringstream findCarrot(lineStrings[1]);
					while (std::getline(findCarrot, lineRead, '>'))
					{
						lineStrings.push_back(lineRead);
					}
					// breaking the matrix into 1.000, 1.000, 1.000, 1.000
					std::stringstream find2ndParen(lineStrings[2]);
					while (std::getline(find2ndParen, lineRead, ')'))
					{
						lineStrings.push_back(lineRead);
					}
					// breaking the matrix into 1.000 1.000 1.000 1.000
					std::stringstream findFloats(lineStrings[3]);
					int counter = 4;
					while (std::getline(findFloats, lineRead, ','))
					{
						lineStrings.push_back(lineRead);
						// convert the strings into a float
						lineFloats.push_back(std::stof(lineStrings[counter]));

						counter++;
					}
					// deposit the information into a new matrix
					switch (i)
					{
					case 0:
						tempModelInfo.modelLocations.row1.x = lineFloats[0];
						tempModelInfo.modelLocations.row1.y = lineFloats[1];
						tempModelInfo.modelLocations.row1.z = lineFloats[2];
						tempModelInfo.modelLocations.row1.w = lineFloats[3];
						break;
					case 1:
						tempModelInfo.modelLocations.row2.x = lineFloats[0];
						tempModelInfo.modelLocations.row2.y = lineFloats[1];
						tempModelInfo.modelLocations.row2.z = lineFloats[2];
						tempModelInfo.modelLocations.row2.w = lineFloats[3];
						break;
					case 2:
						tempModelInfo.modelLocations.row3.x = lineFloats[0];
						tempModelInfo.modelLocations.row3.y = lineFloats[1];
						tempModelInfo.modelLocations.row3.z = lineFloats[2];
						tempModelInfo.modelLocations.row3.w = lineFloats[3];
						break;
					case 3:
						tempModelInfo.modelLocations.row4.x = lineFloats[0];
						tempModelInfo.modelLocations.row4.y = lineFloats[1];
						tempModelInfo.modelLocations.row4.z = lineFloats[2];
						tempModelInfo.modelLocations.row4.w = lineFloats[3];
						//modelLocations.gwWorldMatrix.push_back(tempModelInfo.modelLocations);
						std::cout << "Saved MESH matrix to modelLocations vector!" << std::endl;
						break;
					}

				}
				//7.) After the loop exists you should have collected all the level data, now time to transfer to GPU.  

				H2B::Parser gameLevelParse;
				gameLevelParse.Parse(tempModelInfo.modelNames.c_str());

				// Get material information
				for (int v = 0; v < gameLevelParse.materialCount; v++)
				{
					tempModelInfo.modelMaterial.push_back(gameLevelParse.materials[v].attrib);
					//modelMaterials.material.push_back(gameLevelParse.materials[v].attrib);
				}
				// set the meshDataStartLocation
				tempModelInfo.meshDataStartLocation = meshCount;
				// Get mesh information
				for (int v = 0; v < gameLevelParse.meshCount; v++)
				{
					// push back the mesh
					tempModelInfo.modelMeshes.push_back(gameLevelParse.meshes[v]);
					// now change the mesh BATCH (draw) information, this will ensure it works
					tempModelInfo.modelMeshes[v].drawInfo.indexOffset  +=  indexInfo.size();
					// set the MESH_DATA
					MESH_DATA tempData;
					tempData.material = tempModelInfo.modelMaterial[v];
					tempData.gwWorldMatrix = tempModelInfo.modelLocations;
					meshDataModels.push_back(tempData);
					// add to the total mesh count
					meshCount++;
				}
				// Get vertecies
				//tempModelInfo.vertexBufferStart = vertexInfo.size();
				tempModelInfo.vertexStartLocation = vertexInfo.size();
				for (int v = 0; v < gameLevelParse.vertexCount; v++)
				{
					tempModelInfo.modelVertex.push_back(gameLevelParse.vertices[v]);
					vertexInfo.push_back(gameLevelParse.vertices[v]);
				}
				// Get Indecies
				//tempModelInfo.indexBufferStart = indexInfo.size();
				for (int i = 0; i < gameLevelParse.indexCount; i++)
				{
					tempModelInfo.modelIndecies.push_back(gameLevelParse.indices[i]);
					indexInfo.push_back(gameLevelParse.indices[i]);
				}
				modelInformation.push_back(tempModelInfo);
			}
		}
		std::cout << "File is open - time to close" << std::endl;
	}
	else
	{
		std::cout << "Could not find file name" << std::endl;
	}

	// 8. Close the file when complete
	reader.close();

	/// <summary>
	/// READING FILE END
	/// </summary>   
};

void SetMESH_DATA()
{
	for (int i = 0; i < modelInformation.size(); i++)
	{
		for (int j = 0; j < modelInformation[i].modelMaterial.size(); j++)
		{

		}
	}
};

float DegreeToRadians(float degree)
{
	return degree * (3.14 / 180);
};


//float rotationAmount = 0.0f;
//
//GW::MATH::GMATRIXF UpdateMatrix(GW::MATH::GMATRIXF inMatrix)
//{
//
//	high_resolution_clock::time_point timeStart = high_resolution_clock::now();
//	duration<double> timePassed = duration_cast<duration<double>>(timeStart - timeEnd);
//
//	GW::MATH::GMatrix gwGmatrix;
//	gwGmatrix.Create();
//	const float rotationSpeed = 0.4f;
//
//	rotationAmount += rotationSpeed * (timePassed.count() + 0.5f);
//
//	gwGmatrix.RotateYGlobalF(inMatrix, DegreeToRadians(rotationAmount), inMatrix);
//
//	if (rotationAmount >= 360.0f)
//	{
//		rotationAmount -= 360.0f;
//	}
//
//	timeEnd = high_resolution_clock::now();
//
//	return inMatrix;
//};

// Load a shader file as a string of characters.
std::string ShaderAsString(const char* shaderFilePath) {
	std::string output;
	unsigned int stringLength = 0;
	GW::SYSTEM::GFile file; file.Create();
	file.GetFileSize(shaderFilePath, stringLength);
	if (stringLength && +file.OpenBinaryRead(shaderFilePath)) {
		output.resize(stringLength);
		file.Read(&output[0], stringLength);
	}
	else
		std::cout << "ERROR: Shader Source File "" << shaderFilePath << "" Not Found!" << std::endl;
	return output;
}

high_resolution_clock::time_point timeEnd = high_resolution_clock::now();
void UpdateCamera()
{
	high_resolution_clock::time_point timeStart = high_resolution_clock::now();
	duration<double> timePassed = duration_cast<duration<double>>(timeStart - timeEnd);


	GW::MATH::GMatrix gwGmatrix;// = gwGmatrix.Create();
	gwGmatrix.Create();
	gwGmatrix.InverseF(sceneData.gwViewMatrix, sceneData.gwViewMatrix);
	//unsigned int controllerIndex = 0;
	GW::MATH::GMATRIXF tempMatrix;
	gwGmatrix.IdentityF(tempMatrix);

	const float cameraVertSpeed = 0.5f;
	float changeYPos, spaceOutput, shiftOutput,
		wOutput, sOutput, dOutput, aOutput,
		mouseDeltaX, mouseDeltaY, frameSpeed;
	bool controllerCheck;

	// CAMERA UP / DOWN CODE
	gInput.GetState(G_KEY_SPACE, spaceOutput);

	gInput.GetState(G_KEY_LEFTSHIFT, shiftOutput);

	changeYPos = spaceOutput - shiftOutput;

	sceneData.gwViewMatrix.row4.y += (changeYPos * cameraVertSpeed) * timePassed.count();
	// CAMERA UP / DOWN CODE

	// CAMERA STRAFE CODE
	gInput.GetState(G_KEY_W, wOutput);
	gInput.GetState(G_KEY_S, sOutput);
	gInput.GetState(G_KEY_D, dOutput);
	gInput.GetState(G_KEY_A, aOutput);

	frameSpeed = cameraVertSpeed * timePassed.count();
	float zChange;
	zChange = wOutput - sOutput;
	float xChange;
	xChange = dOutput - aOutput;

	GW::MATH::GVECTORF matrixTransformer;
	matrixTransformer.x = xChange * timePassed.count();
	matrixTransformer.y = 0.0f;
	matrixTransformer.z = zChange * timePassed.count();
	matrixTransformer.w = 1.0f;
	gwGmatrix.TranslateLocalF(sceneData.gwViewMatrix, matrixTransformer, sceneData.gwViewMatrix);
	// CAMERA STRAFE CODE

	// CAMERA TILT CODE
	float mouseSpeed = 3.14 * timePassed.count();
	gInput.GetMouseDelta(mouseDeltaX, mouseDeltaY);
	//mouseDeltaX = mouseDeltaX - mouseXLast;
	//mouseDeltaY = mouseDeltaY - mouseYLast;
	if (mouseDeltaX != mouseXLast && mouseDeltaY != mouseYLast)
	{
		float pitch = 65 * mouseDeltaY / tempHeight * mouseSpeed;
		float yaw = 65 * aspectRatio * mouseDeltaX / tempWidth * mouseSpeed;
		gwGmatrix.IdentityF(tempMatrix);
		gwGmatrix.RotationYawPitchRollF(0, pitch, 0, tempMatrix);
		gwGmatrix.MultiplyMatrixF(tempMatrix, sceneData.gwViewMatrix, sceneData.gwViewMatrix);
		gwGmatrix.IdentityF(tempMatrix);
		gwGmatrix.RotationYawPitchRollF(yaw, 0, 0, tempMatrix);
		// SAVE CAMERA POS
		matrixTransformer.x = sceneData.gwViewMatrix.row4.x;
		matrixTransformer.y = sceneData.gwViewMatrix.row4.y;
		matrixTransformer.z = sceneData.gwViewMatrix.row4.z;
		matrixTransformer.w = sceneData.gwViewMatrix.row4.w;
		gwGmatrix.MultiplyMatrixF(sceneData.gwViewMatrix, tempMatrix, sceneData.gwViewMatrix);
		// RESTORE CAMERA POS
		sceneData.gwViewMatrix.row4.x = matrixTransformer.x;
		sceneData.gwViewMatrix.row4.y = matrixTransformer.y;
		sceneData.gwViewMatrix.row4.z = matrixTransformer.z;
		sceneData.gwViewMatrix.row4.w = matrixTransformer.w;
		//gwViewMatrix = gwViewMatrixCopy;
	}
	//gwViewMatrixCopy = gwViewMatrix;
	mouseXLast = mouseDeltaX;
	mouseYLast = mouseDeltaY;
	// CAMERA TILT CODE

	timeEnd = high_resolution_clock::now();
	gwGmatrix.InverseF(sceneData.gwViewMatrix, sceneData.gwViewMatrix);
}