#pragma once
#include <string>

enum class Alert {
  Buy,
  Sell
};

// Melhor com parâmetros por referência constante
int sendEmail(const std::string& senderEmail,
              const std::string& recipientEmail,
              const std::string& senderPassword,
              const std::string& ticker,
              Alert alert,
              const std::string& smtpUrl);
