#include "../headers/stock_fetcher.hpp"
#include "../include/nlohmann/json.hpp"
#include <curl/curl.h>
#include <iostream>
#include <stdexcept>

using namespace std;
using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
  output->append((char*)contents, size * nmemb);
  return size * nmemb;
}

optional<float> fetchStockPrice(const string& ticker, int interval) {
  const string api_key = "MLAEF77A8B77N7NG";
  const string url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" + ticker + "&interval=" + to_string(interval) + "min&apikey=" + api_key;

  CURL* curl;
  CURLcode res;
  string readBuffer;
  optional<float> quotePrice; 

  curl = curl_easy_init();
  if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);

      if (res == CURLE_OK) {
        try {
          json j = json::parse(readBuffer);
          cout << "JSON recebido:\n" << j.dump(4) << endl;

          if (!j.contains("Time Series (1min)")) {
            cerr << "Erro: JSON não contém 'Time Series (1min)'. Verifique o ticker ou o limite da API.\n";
            return nullopt;
          }
          
          auto time_series = j["Time Series (1min)"];
          auto it = time_series.begin();
          if (it == time_series.end()) {
              cerr << "Erro: Time series está vazio.\n";
              return nullopt;
          }
          string last_time = it.key();
        
          string close_price_str = time_series[last_time]["4. close"];

          cout << "Último preço de fechamento: " << close_price_str << endl;
          try {
            quotePrice = stof(close_price_str);
          } catch (const invalid_argument& e) {
              cerr << "Erro: valor inválido para conversão." << endl;
              exit(1);
          } catch (const out_of_range& e) {
              cerr << "Erro: número fora do intervalo." << endl;
              exit(1);
          }
      }
      catch (const exception& e) {
          cerr << "Erro ao processar o JSON: " << e.what() << endl;
          exit(1);
      }
    }
  }
  return quotePrice;
}
