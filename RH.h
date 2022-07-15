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
	GW::MATH::GVECTORF camPos;
	GW::MATH::GMATRIXF gwViewMatrix;
	GW::MATH::GMATRIXF gwProjectionMatrix;
	GW::MATH::GVECTORF padding[3];
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
	std::string modelNames;						// file path
	GW::MATH::GMATRIXF modelLocations;			// world matrix of model
	std::vector<H2B::MESH> modelMeshes;			// mesh information of model
	std::vector<H2B::MATERIAL> modelMaterial;	// material information
	//std::vector<H2B::VERTEX> modelVertex;		// vertex information
	//std::vector<unsigned> modelIndecies;		// index information

	unsigned int vertexBufferStart;				// keep track of models vertex buffer
	unsigned int indexBufferStart;				// keep track of models index buffer
};

std::vector<ModelInfo> modelInformation;		// keep track of every models information
std::vector<H2B::VERTEX> vertexInfo;			// keep track of all vertecies
std::vector<unsigned> indexInfo;				// keep track of all indexes

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
					tempModelInfo.modelMaterial.push_back(gameLevelParse.materials[v]);
				}
				// Get mesh information
				for (int v = 0; v < gameLevelParse.meshCount; v++)
				{
					tempModelInfo.modelMeshes.push_back(gameLevelParse.meshes[v]);
				}
				// Get vertecies
				tempModelInfo.vertexBufferStart = vertexInfo.size();
				for (int v = 0; v < gameLevelParse.vertexCount; v++)
				{
					//tempModelInfo.modelVertex.push_back(gameLevelParse.vertices[v]);
					vertexInfo.push_back(gameLevelParse.vertices[v]);
				}
				// Get Indecies
				tempModelInfo.indexBufferStart = indexInfo.size();
				for (int i = 0; i < gameLevelParse.indexCount; i++)
				{
					//tempModelInfo.modelIndecies.push_back(gameLevelParse.indices[i]);
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

float DegreeToRadians(float degree)
{
	return degree * (3.14 / 180);
};

high_resolution_clock::time_point timeEnd = high_resolution_clock::now();
float rotationAmount = 0.0f;

GW::MATH::GMATRIXF UpdateMatrix(GW::MATH::GMATRIXF inMatrix)
{

	high_resolution_clock::time_point timeStart = high_resolution_clock::now();
	duration<double> timePassed = duration_cast<duration<double>>(timeStart - timeEnd);

	GW::MATH::GMatrix gwGmatrix;
	gwGmatrix.Create();
	const float rotationSpeed = 0.4f;

	rotationAmount += rotationSpeed * (timePassed.count() + 0.5f);

	gwGmatrix.RotateYGlobalF(inMatrix, DegreeToRadians(rotationAmount), inMatrix);

	if (rotationAmount >= 360.0f)
	{
		rotationAmount -= 360.0f;
	}

	timeEnd = high_resolution_clock::now();

	return inMatrix;
};