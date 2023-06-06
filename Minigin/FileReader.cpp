#include "FileReader.h"
#include <algorithm>
#include <sstream>
#include <vector>
#include <iostream>
#include <type_traits>
#include <cassert>
#define _SILENCE_CXX20_IS_POD_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

dae::FileReader::FileReader(const std::string& filePath)
	: m_File{}
	, m_FileName{ filePath }
{}

dae::FileReader::~FileReader()
{
	m_File.close();
}

void dae::FileReader::Update()
{
}

void dae::FileReader::FixedUpdate()
{
}

void dae::FileReader::Render() const
{
}
void dae::FileReader::ExtractLevel(std::string levelName) {
	auto grid{ std::any_cast<std::string>(ObjectList[levelName])};
	std::vector<std::vector<std::string>> griditems;
	std::vector<std::string> splitStrings2;
	std::string line2;
	std::stringstream ss2(grid);
	while (std::getline(ss2, line2, ',')) {
		std::stringstream ss3(line2);
		splitStrings2.push_back(line2);

		std::vector<std::string> splitStrings3;
		std::string line3;
		while (std::getline(ss3, line3, '|')) {
			splitStrings3.push_back(line3);
		}
		griditems.push_back(splitStrings3);
	}
	ObjectList[levelName] = griditems;
}

std::map<std::string, std::any> dae::FileReader::ParseJson(std::string contents, char seperator) {

	std::vector<std::string> splitStrings;
	std::string line;
	if (m_File.is_open()) {
		while (std::getline(m_File, line, seperator)) {
			std::stringstream ss(line);
			splitStrings.push_back(line);
		}
	}

	for (auto string : splitStrings) {
		std::stringstream ss(string);
		std::string token;
		size_t delimiterPos = string.find(':');
		if (delimiterPos != std::string::npos) {
			string.erase(std::remove(string.begin(), string.end(), '\n'), string.end());

			std::string key = string.substr(0, delimiterPos);
			key.erase(std::remove(key.begin(), key.end(), ':'), key.end());
			std::string value = string.substr(delimiterPos + 1);
			ObjectList.insert(std::pair<std::string, std::string>(key, value));
		}
	}

	ExtractLevel("Test");
	ExtractLevel("Stage 1");
	ExtractLevel("Stage 2");
	ExtractLevel("Stage 3");

	Close();
	return ObjectList;
}

std::string dae::FileReader::ReadJsonFile() {
	Open();
	std::ifstream inputFile(m_FileName);
	if (!inputFile) {
		std::cerr << "Failed to open the input file." << std::endl;
		return "";
	}

	std::string content;
	std::string line;

	while (std::getline(inputFile, line)) {
		content += line;
	}

	inputFile.close();
	return content;
}
void dae::FileReader::ReadFile()
{	
	char newlineSeperator{ '\n' };
	char spaceSeperator2{ ' ' };

	std::string line;
	if (m_File.is_open()) {
		while (std::getline(m_File, line)) {
			std::stringstream ss(line);
			if (std::getline(ss, line, newlineSeperator)) {

				std::string line2{ line };
				std::vector<std::string> lineWords;
				std::stringstream ss2(line2);
				while (std::getline(ss2, line2, spaceSeperator2)) {
					lineWords.push_back(line2);
				}

				if (lineWords[0]  == "v") {
					StoreValue(lineWords, m_Verts, "v");
				}
				if (lineWords[0] == "vn") {
					StoreValue(lineWords, m_Vertex, "vn");
				}
				if (lineWords[0] == "f") {
					StoreValue(lineWords, m_Faces, "f");
				}
				if (lineWords[0] == "#") {
					m_Comments.push_back(line);
				}
			}
		}
	}
}

void dae::FileReader::StoreValue(std::vector<std::string>& stringArray, std::vector<std::vector<float>>& outFloat, std::string attribute)
{
	std::vector<float> floats;
	for (auto string : stringArray) {
		if (string != attribute) {
			floats.push_back(std::stof(string));
		}
	}
	outFloat.push_back(floats);
}

void dae::FileReader::WriteToBinary()
{
	std::string pod{ std::is_standard_layout<BinaryStructure>::value == 1 ? "yes" : "no" };
	std::cout << "Is the Float3 struct a pod: " << pod << "\n";

	std::ofstream temp;
	temp.open("../Data/bunny.bobj", std::ios::binary);

	if (temp.is_open()) {
		//for (const auto vertList : m_Comments) {
		//	temp.write(reinterpret_cast<const char*>(&vertList), sizeof(vertList));
		//}	
		for (const auto vertList : m_Verts) {
			BinaryStructure writer;
			writer.one = 'v';
			writer.x = vertList[0];
			writer.y = vertList[1];
			writer.z = vertList[2];
			writer.terminator = 0;
			temp.write(reinterpret_cast<const char*>(&writer), sizeof(writer));
		}
		for (const auto vertexList : m_Vertex) {
			BinaryStructure writer;
			writer.one = 'n';
			writer.x = vertexList[0];
			writer.y = vertexList[1];
			writer.z = vertexList[2];
			writer.terminator = 0;
			temp.write(reinterpret_cast<const char*>(&writer), sizeof(writer));
		}
		for (const auto facesList : m_Faces) {
			BinaryStructure writer;
			writer.one = 'f';
			writer.x = facesList[0];
			writer.y = facesList[1];
			writer.z = facesList[2];
			writer.terminator = 0;
			temp.write(reinterpret_cast<const char*>(&writer), sizeof(writer));
		}
	}
	temp.close();
}

void dae::FileReader::WriteToObj() {
	std::vector<BinaryStructure> toReadVec;
	std::ifstream input;

	input.open("../Data/bunny.bobj", std::ios::binary);
	std::string line;
	if (input.is_open()) {
		while (std::getline(input, line, '\0')) {
			std::stringstream ss(line);
			BinaryStructure toRead;
			ss.read((char*)&toRead, sizeof(toRead));
			toReadVec.push_back(toRead);
		}
	}
	input.close();

	std::ofstream temp;
	temp.open("../Data/convertedBunny.obj", std::ios::in);
	if (temp.is_open()) {
		for (auto item : toReadVec) {
			std::string s;
			s.push_back(item.one);
			std::string writer{ s + " " + std::to_string(item.x) + " " + std::to_string(item.y) + " " + std::to_string(item.z) + "\n" };
			temp << writer;
		}
	}
	temp.close();
}

void dae::FileReader::Open()
{
	m_File.open(m_FileName, std::ios::in);
}

void dae::FileReader::Close()
{
	m_File.close();
}