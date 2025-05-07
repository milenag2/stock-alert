#include "../headers/config_reader.hpp"
#include <fstream>
#include <iostream>

using namespace std;

map<string, string> readFile(const string& filename){
  map<string, string> configData;
  ifstream file(filename);

  if (!file.is_open()) {
      cerr << "Erro ao abrir o arquivo de configuração." << endl;
      return configData;
  }

  string line;
  while (getline(file, line)) {
      if (line.empty() || line[0] == '#') {
          continue;
      }

      size_t delimiterPos = line.find('=');
      if (delimiterPos != string::npos) {
          string key = line.substr(0, delimiterPos);
          string value = line.substr(delimiterPos + 1);

          key.erase(key.find_last_not_of(" \t") + 1);
          value.erase(value.find_last_not_of(" \t") + 1);

          configData[key] = value;
      }
  }
  file.close();
  return configData;
}
