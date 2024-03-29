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

struct ModelInfo
{
	std::string modelNames;							// file path
	GW::MATH::GMATRIXF modelLocations;				// world matrix of model
	std::vector<H2B::MESH> modelMeshes;				// mesh information of model
	std::vector<H2B::ATTRIBUTES> modelMaterial;		// material information
	std::vector<H2B::VERTEX> modelVertex;			// vertex information
	std::vector<unsigned> modelIndecies;			// index information

	unsigned int meshDataStartLocation = 0;			// meshCount at the time this model was made
	unsigned int vertexStartLocation = 0;			// what vertex location to start at

	bool haschildren = false;						// lets renderer know if there are children to this object
	unsigned int parentLocation = 0;				// what index parent mesh is located at in the childObjects vector

	//unsigned int vertexBufferStart;				// keep track of models vertex buffer
	//unsigned int indexBufferStart;				// keep track of models index buffer
};

struct Child
{
	std::vector<ModelInfo> childObjects;			// child object 0 will always be parent
};

const char* vertexShader = "../VertexShader.hlsl";	// vertex shader path
const char* pixelShader = "../PixelShader.hlsl";	// pixel shader path

std::vector<ModelInfo> modelInformation;			// keep track of every models information
std::vector<H2B::VERTEX> vertexInfo;				// keep track of all vertecies
std::vector<unsigned> indexInfo;					// keep track of all indexes
std::vector<MESH_DATA> meshDataModels;				// keep track of all models MESH_DATA
std::vector<GW::MATH::GMATRIXF> cameraLocations;	// keep track of camera locations
std::vector<Child> childObjects;					// keep track of all parent / children relationships
SCENE_DATA sceneData;								// keep track of SCENE_DATA
SCENE_DATA sceneDataMM;								// minimap SCENE_DATA
unsigned int meshCount;								// keep track of all meshes in scene
unsigned int cameraLocation = 1;					// current camera index

GW::INPUT::GInput gInput;							// keyboard and mouse input
std::string mFileName;								// GameLevel File to load in
bool currentLevel = false;							// The current level you are on
bool swappingLevel = false;							// if game is swapping level
bool makeNewRenderer = false;						// if we are done reading files and ready to re-populate renderer
bool couldBeParent = true;							// keep track of what objects could be a parent

bool fileLoaded = false;							// if one window already loaded a file, do not load again

float levelSwapCooldown = 0.0f;						// level swap cooldown to avoid double clicks

float mouseYLast = 0.0f;							// last mouse Y position
float mouseXLast = 0.0f;							// last mouse X position

float rotationSpeed = 0.4f;
float rotationAmount = 0.0f;

float tempX = 0;

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
	if (fileLoaded == false)
	{
		// run h2b file parser on the meshes you read in
		// array of matrix
		// parse gamelevel.txt
		//1.) Use either std::ifstream or FILE * to open the file.
		std::ifstream reader;
		std::string lineRead;
		if (currentLevel)
		{
			mFileName = "../GameLevel.txt";
		}
		else
		{
			mFileName = "../GameLevel1.txt";
		}
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
					if (std::strcmp(lineRead.c_str(), "MESH") == 0 || std::strcmp(lineRead.c_str(), "  MESH") == 0)
					{
						std::vector<std::string> fileNames; // vector of saved file names
						ModelInfo tempModelInfo;			// Hold temporary model information
						Child tempChildInfo;				// keep track of potential child information
						//GW::MATH::GMATRIXF theNewMatrix;	// a matrix to store the global position

						//5.) Read the next line, this is the name of the .h2b file to load in. (duplicates?)

						if (std::strcmp(lineRead.c_str(), "MESH") == 0)
						{
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

							couldBeParent = true;
						}
						else
						{
							std::cout << "FOUND A CHILD MESH" << std::endl;
							Child parentCopy;
							// push back last read in mesh
							parentCopy.childObjects.push_back(modelInformation[modelInformation.size() - 1]);
							// put that mesh into childObects[0]
							if (couldBeParent)
							{
								childObjects.push_back(parentCopy);
							}
							//else
							//{
							//	//childObjects[childObjects.size() - 1].childObjects.push_back(parentCopy);
							//}
							std::cout << "PARENT IS: " << childObjects[childObjects.size() - 1].childObjects[0].modelNames << std::endl;
							// set parent location in the childObjects array
							modelInformation[modelInformation.size() - 1].parentLocation = childObjects.size() - 1;

							std::getline(reader, lineRead);
							std::string fileName;
							for (int i = 2; i < lineRead.size(); i++)
							{
								fileName += lineRead[i];
							}

							// separate the .001 ect. from the string
							std::stringstream findName(fileName);
							while (std::getline(findName, fileName, '.'))
							{
								fileName = "../models/" + fileName + ".h2b"; // convert to correct file path
								fileNames.push_back(fileName);
							}

							std::cout << "Model Name: " << fileNames[0] << std::endl; // for debugging
							tempModelInfo.modelNames = fileNames[0];

							couldBeParent = false;
						}

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
							tempModelInfo.modelMeshes[v].drawInfo.indexOffset += indexInfo.size();
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

						if (couldBeParent == false)
						{
							childObjects[childObjects.size() - 1].childObjects.push_back(tempModelInfo);
						}
					}
					else if (std::strcmp(lineRead.c_str(), "CAMERA") == 0)
					{
						std::cout << "FOUND A CAMERA" << std::endl;
						std::getline(reader, lineRead);
						std::cout << "Do not need " << lineRead << " information for camera..." << std::endl;

						GW::MATH::GMATRIXF tempMatrix;

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
								tempMatrix.row1.x = lineFloats[0];
								tempMatrix.row1.y = lineFloats[1];
								tempMatrix.row1.z = lineFloats[2];
								tempMatrix.row1.w = lineFloats[3];
								break;
							case 1:
								tempMatrix.row2.x = lineFloats[0];
								tempMatrix.row2.y = lineFloats[1];
								tempMatrix.row2.z = lineFloats[2];
								tempMatrix.row2.w = lineFloats[3];
								break;
							case 2:
								tempMatrix.row3.x = lineFloats[0];
								tempMatrix.row3.y = lineFloats[1];
								tempMatrix.row3.z = lineFloats[2];
								tempMatrix.row3.w = lineFloats[3];
								break;
							case 3:
								tempMatrix.row4.x = lineFloats[0];
								tempMatrix.row4.y = lineFloats[1];
								tempMatrix.row4.z = lineFloats[2];
								tempMatrix.row4.w = lineFloats[3];
								//modelLocations.gwWorldMatrix.push_back(tempModelInfo.modelLocations);
								std::cout << "Saved MESH matrix to modelLocations vector!" << std::endl;
								break;
							}
						}
						GW::MATH::GMatrix gwGmatrix;
						gwGmatrix.Create();
						gwGmatrix.InverseF(tempMatrix, tempMatrix);
						cameraLocations.push_back(tempMatrix);
						std::cout << "CAMERA PUSHED BACK" << std::endl;
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
	}

	if (swappingLevel == true)
	{
		swappingLevel = false;
	}

	fileLoaded = true;
	//else
	//{
	//	swappingLevel = false;
	//}

	/// <summary>
	/// READING FILE END
	/// </summary>   
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

void SwapLevel()
{
	// swap what level we load
	currentLevel = !currentLevel;
	// destroy old data
	modelInformation.clear();
	vertexInfo.clear();
	indexInfo.clear();
	meshDataModels.clear();
	cameraLocations.clear();
	meshCount = 0;
	cameraLocation = 1;

	fileLoaded = false;
	// create a new renderer
	makeNewRenderer = true;
}

high_resolution_clock::time_point timeEnd = high_resolution_clock::now();
void UpdateCamera()
{
	high_resolution_clock::time_point timeStart = high_resolution_clock::now();
	duration<double> timePassed = duration_cast<duration<double>>(timeStart - timeEnd);
	levelSwapCooldown += timePassed.count();


	GW::MATH::GMatrix gwGmatrix;// = gwGmatrix.Create();
	gwGmatrix.Create();
	gwGmatrix.InverseF(sceneData.gwViewMatrix, sceneData.gwViewMatrix);
	//unsigned int controllerIndex = 0;
	GW::MATH::GMATRIXF tempMatrix;
	gwGmatrix.IdentityF(tempMatrix);

	const float cameraVertSpeed = 0.5f;
	float changeYPos, spaceOutput, shiftOutput,
		wOutput, sOutput, dOutput, aOutput, cameraSwap,
		mouseDeltaX, mouseDeltaY, frameSpeed, levelSwap;
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
	matrixTransformer.x = xChange * 0.05f;
	matrixTransformer.y = 0.0f;
	matrixTransformer.z = zChange * 0.05f;
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

	// TRANSFORM CHILD / PARENT OBJECTS
	if (currentLevel == false)
	{
		rotationAmount = rotationSpeed * timePassed.count()+0.5f;
		GW::MATH::GMATRIXF rotMatrix;
		gwGmatrix.IdentityF(rotMatrix);
		gwGmatrix.RotateYLocalF(rotMatrix, DegreeToRadians(rotationAmount), rotMatrix);
		for (int i = 0; i < childObjects[0].childObjects.size(); i++)
		{
			int meshDataLocator = childObjects[0].childObjects[i].meshDataStartLocation;
			for (int j = 0; j < childObjects[0].childObjects[i].modelMeshes.size(); j++)
			{
				gwGmatrix.MultiplyMatrixF(meshDataModels[meshDataLocator].gwWorldMatrix, rotMatrix, meshDataModels[meshDataLocator].gwWorldMatrix);
				meshDataLocator++;
			}
		}
	}
	// TRANSFORM CHILD / PARENT OBJECTS

	timeEnd = high_resolution_clock::now();
	gwGmatrix.InverseF(sceneData.gwViewMatrix, sceneData.gwViewMatrix);


	// CHECK FOR LEVEL SWAPPING
	gInput.GetState(G_KEY_L, levelSwap);

	if (levelSwapCooldown < 0.2f && levelSwap == 1.0f)
	{
		std::cout << "Please wait before swapping levels again!" << std::endl;
	}
	if (levelSwap == 1.0f && swappingLevel == false && levelSwapCooldown >= 0.2f)
	{
		levelSwapCooldown = 0.0f;
		swappingLevel = true;
		std::cout << "SWAPPING LEVEL" << std::endl;
		SwapLevel();
	}


	// CHECK FOR NEXT CAMERA 
	gInput.GetState(G_KEY_C, cameraSwap);
	if (levelSwapCooldown >= 0.2f && cameraSwap == 1.0f && cameraLocations.size() > 1)
	{
		levelSwapCooldown = 0.0f;

		std::cout << "SWAPPING CAMERA" << std::endl;
		if (cameraLocation < (cameraLocations.size() - 1))
		{
			cameraLocation++;
		}
		else
		{
			cameraLocation = 1;
		}
		std::cout << "Camera Index: " << cameraLocation << std::endl;
		sceneData.gwViewMatrix = cameraLocations[cameraLocation];
		//sceneData.gwViewMatrix = cameraLocations[1];
	}
}