#include "../headers/email_sender.hpp"
#include <iostream>
#include <curl/curl.h>
#include <cstring>

using namespace std;

static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp) {
  string* payload = static_cast<string*>(userp);
  size_t buffer_size = size * nmemb;
  if (payload->empty()) return 0;
  size_t copy_size = min(buffer_size, payload->size());
  memcpy(ptr, payload->c_str(), copy_size);
  payload->erase(0, copy_size);
  return copy_size;
}

int sendEmail(const string& senderEmail,
  const string& recipientEmail,
  const string& senderPassword,
  const string& ticker,
  Alert alert,
  const string& smtpUrl){
  CURL* curl = curl_easy_init();
  if (!curl) {
      cerr << "Erro ao inicializar CURL.\n";
      return 1;
  }
  
  string from = senderEmail;
  string to = recipientEmail;
  string user = senderEmail;
  string pass = senderPassword;

  string dataBuy =
      "To: " + to + "\r\n"
      "From: " + from + "\r\n"
      "Subject: Alerta para compra do ativo!\r\n"
      "Content-Type: text/plain; charset=utf-8\r\n"
      "\r\n"
      "A cotação de " + ticker + " da B3 está abaixo do preço de referência para compra. É aconselhável a compra.\r\n";

  string dataSell =
      "To: " + to + "\r\n"
      "From: " + from + "\r\n"
      "Subject: Alerta para venda do ativo!\r\n"
      "Content-Type: text/plain; charset=utf-8\r\n"
      "\r\n"
      "A cotação de " + ticker + " da B3 está acima do preço de referência para venda. É aconselhável a venda.\r\n";

  struct curl_slist* recipients = nullptr;
  recipients = curl_slist_append(recipients, to.c_str());

  curl_easy_setopt(curl, CURLOPT_URL, smtpUrl.c_str());
  curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
  curl_easy_setopt(curl, CURLOPT_PASSWORD, pass.c_str());
  curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
  curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
  curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
  if(alert == Alert::Buy){
    curl_easy_setopt(curl, CURLOPT_READDATA, &dataBuy);
  }
  else if(alert == Alert::Sell){
    curl_easy_setopt(curl, CURLOPT_READDATA, &dataSell);
  }
  curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
      cerr << "Erro ao enviar e-mail: " << curl_easy_strerror(res) << endl;
      curl_slist_free_all(recipients);
      curl_easy_cleanup(curl);
      return 1;
  }

  cout << "E-mail enviado com sucesso!\n";

  curl_slist_free_all(recipients);
  curl_easy_cleanup(curl);
  return 0;
}
