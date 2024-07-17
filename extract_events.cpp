#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdio.h>
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


struct Run{
	int x[1855] = {};
	int y[1855] = {};
	double npe[1855] = {};
};

using namespace std;

int main(){

	string masses[1] = {"silicon"};
	
	for (const auto& value : masses) {
	
	cout <<  value << endl;
	
	string mass = value;
	string inputPath = "/media/usuario/C7F5-1A2E/decompressed/" + mass + "/";

	std::vector<std::string> arquivos = listarArquivos(inputPath);
	
	string outputFolder = "/home/usuario/Área de Trabalho/Mestrado/dados/image/" + mass + "/";
	
	int fileNumber = 0;
	
	for (const auto& arquivo : arquivos) {
		string outputName = arquivo.substr(0, arquivo.length() - 4);
		fstream arq;
		
		long count = 0;	
		int iter = 0;
	
		string line;
		string array[6];
		
		arq.open(inputPath + arquivo, fstream::in);
		
		if(arq.is_open()){	
			Run runs;
			while(getline(arq, line)){
			
				string title;
				//fstream arqOut;
				
				stringstream ss(line);
				string word;
				vector<string> words;
				
				while (ss >> word) {
					words.push_back(word);
				}
				
				int i = 0;
				for (const auto& w : words) {
					array[i] = w;
					i++;
				}
				
				runs.x[iter] = stoi(array[2])/333;
				runs.y[iter] = stoi(array[3])/192;
				runs.npe[iter] = stof(array[4]);
				
				count++;
				iter++;
				
				if((count % 1855) == 0){
					
					double zeros[60][130] = {};
						
						for(int i = 0; i < 1855; i++){
							zeros[runs.x[i]][runs.y[i]] = runs.npe[i];
						}
					
					string title;
					title = outputName +"_event_" + array[0] + ".txt";
					
					std::ofstream outFile(outputFolder + title);
					
					for (int i = 5; i < 60; ++i) {
						for (int j = 37; j < 130; ++j) {
							outFile << zeros[i][j];
							if (j < 130) {
								outFile << " "; // Adicionar espaço entre os valores, exceto no final da linha
							}
						}
						outFile << "\n"; // Nova linha após cada linha da matriz
					}
					
					outFile.close();
					
					iter = 0;
					Run new_run;
					runs = new_run;
					
				}
			}
			
				
		} else{
			cout << "O arquivo não está aberto" << endl;
		}
		
		arq.close();
		
		fileNumber++;
		
		cout << arquivo << fileNumber << endl;
	}
	}
	return 0;
}
