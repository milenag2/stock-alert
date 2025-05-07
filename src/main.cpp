#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <thread>
#include <chrono>
#include "../headers/email_sender.hpp"
#include "../headers/config_reader.hpp"
#include "../headers/stock_fetcher.hpp"

using namespace std;

int main(){
  Alert alert;
  string ticker;
  int interval=1;
  float sellLimit, buyLimit;
  optional<float> quotePrice;
  map<string, string> config = readFile("./files/config.txt");
  if (config.empty()) {
    cerr << "Erro: arquivo de configuração vazio ou não encontrado. Encerrando o programa." << endl;
    return 1;
  }
  string senderEmail = config["sender_email"];
  string recipientEmail = config["recipient_email"];
  string senderPassword = config["sender_password"];
  string smtpServer = config["smtp_server"];
  string smtpPort = config["smtp_port"];
  string smtpUrl = "smtps://" + smtpServer + ":" + smtpPort;

  cin>>ticker>>sellLimit>>buyLimit;
  
  while(true){
    try {
      quotePrice = fetchStockPrice(ticker, interval);
    }
    catch (const exception& e) {
        cerr << "Erro ao obter o preço de cotação: " << e.what() << endl;
        quotePrice = nullopt;
        exit(1);
    }
    if(quotePrice.has_value()){
      if(quotePrice.value()>sellLimit){
        alert = Alert::Sell;
        sendEmail(senderEmail, recipientEmail, senderPassword, ticker, alert, smtpUrl);
      }
      else if(quotePrice.value()<buyLimit){
        alert = Alert::Buy;
        sendEmail(senderEmail, recipientEmail, senderPassword, ticker, alert, smtpUrl);
      }
    }
    this_thread::sleep_for(chrono::minutes(1));
  }
  return 0;
}
