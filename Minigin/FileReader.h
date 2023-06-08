#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include <fstream>
#include <any>
#include <map>

namespace dae {
	class FileReader final : public Component
	{
	public:

		struct BinaryStructure {
			char one;
			float x, y, z;
			int terminator;
		};

		FileReader(const std::string& filePath);
		~FileReader();
		FileReader(const FileReader& other) = delete;
		FileReader(FileReader&& other) = delete;
		FileReader& operator=(const FileReader& other) = delete;
		FileReader& operator=(FileReader&& other) = delete;

		void Update() override;
		virtual void FixedUpdate() override;
		void Render() const override;

		std::string ReadGameDataFile();
		std::map<std::string, std::any> ParseData(std::string contents, char seperator);
		std::map<std::string, std::any> ParseDataSimple(std::string contents, char seperator);

		void WriteData(std::string dataKey, std::string data);

		void Open();
		void Close();

		//void ReadFile();
	/*	void WriteToBinary();
		void WriteToObj();*/

	private:
		std::ifstream m_File;
		std::string m_FileName;

		std::vector<std::vector<float>> m_Verts;
		std::vector<std::vector<float>> m_Vertex;
		std::vector<std::vector<float>> m_Faces;
		std::vector<std::string> m_Comments;
		
		std::vector<std::vector<float>> m_ConvVerts;
		std::vector<std::vector<float>> m_ConvVertex;
		std::vector<std::vector<float>> m_ConvFaces;
		std::vector<std::string> m_ConvComments;

		std::map<std::string, std::any> ObjectList{};

		//void StoreValue(std::vector<std::string>& stringArray, std::vector<std::vector<float>>& outFloat, std::string attribute);
		void ExtractLevel(std::string levelName);
	};
}
