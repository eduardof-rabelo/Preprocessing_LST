#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

std::vector<std::string> listarArquivos(const std::string& diretorio) {
    std::vector<std::string> nomesArquivos;

    try {
        // Verifica se o diretório é válido
        if (fs::is_directory(diretorio)) {
            // Itera sobre os arquivos dentro do diretório
            for (const auto& entry : fs::directory_iterator(diretorio)) {
                // Verifica se é um arquivo regular
                if (fs::is_regular_file(entry)) {
                    nomesArquivos.push_back(entry.path().filename().string());
                }
            }
        } else {
            std::cerr << "O diretório especificado não é válido.\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Erro ao acessar o sistema de arquivos: " << e.what() << '\n';
    }

    return nomesArquivos;
}

bool decompressGzip(const std::string& inputFile, const std::string& outputFile){
	//std::cout << inputFile.size() << std:: endl;

	gzFile inFile = gzopen(inputFile.c_str(), "rb");
	
	std::ofstream outFile(outputFile, std::ios::out | std::ios::binary);
   
    char buffer[4096];
    int bytesRead = 0;
    while ((bytesRead = gzread(inFile, buffer, sizeof(buffer))) > 0) {
        outFile.write(buffer, bytesRead);
    }

    gzclose(inFile);
    outFile.close();
	
	return true;
}


using namespace std;

int main(){
	//Definimos as variáveis de entrada do tipo string.
	string mass = "nitrogen";
	string inputPath = "/home/usuario/Área de Trabalho/Mestrado/dados/SimTelArray_2022_05/" + mass + "/images_tel/";
	
    std::vector<std::string> arquivos = listarArquivos(inputPath);
	
	string outputPath = "/media/usuario/C7F5-1A2E/decompressed/" + mass + "/";
	
	// Imprime os nomes dos arquivos encontrados
    for (const auto& arquivo : arquivos) {
    	if(arquivo.find(".dt.gz") != std::string::npos){
    		
    		string outputName = arquivo.substr(0, arquivo.length() - 6) + ".txt";
    		
			if (decompressGzip(inputPath + arquivo, outputPath + outputName)) {
				std::cout << "Arquivo descompactado com sucesso: " << outputPath << std::endl;
			} else {
				std::cerr << "Falha ao descompactar o arquivo." << std::endl;
			}
    	}	
    }
	
    //Para executar: g++ -std=c++17 -o decompress decompress_data.cpp -lz
}
