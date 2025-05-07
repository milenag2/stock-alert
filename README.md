Stock Quote Alert:
O programa Stock Quote Alert é uma aplicação de console que monitora a cotação de um ativo e envia alertas via e-mail quando a cotação do ativo atinge níveis específicos. O sistema permite que o usuário defina dois preços de referência (para compra e para venda). O programa utiliza a API da Alpha Vantage para buscar as cotações e o CURL para enviar os e-mails via SMTP.

-> Funcionalidades:
- Monitoramento contínuo da cotação do ativo em intervalos de 1 minuto.
- Envio de alertas por e-mail quando o preço do ativo atinge o preço de referência para compra ou venda.
- Configuração de parâmetros (e-mail de destino, SMTP, etc.) via arquivo de configuração.

-> Pré-requisitos:
- C++ Compiler.
- CURL instalado no sistema.
- nlohmann/json para manipulação de JSON (disponível em https://github.com/nlohmann/json).
- Alpha Vantage API Key (cadastre-se para obter uma chave em https://www.alphavantage.co/support/#api-key).

-> Arquivo de Configuração:
O programa utiliza um arquivo de configuração (config.txt) para armazenar informações sensíveis como os dados de e-mail e o servidor SMTP.

# Arquivo de configuração para o Stock Quote Alert
Prepare o arquivo de configuração (config.txt) conforme o modelo:
sender_email=seu-email@example.com
recipient_email=email-destino@example.com
sender_password=sua-senha
smtp_server=smtp.exemplo.com
smtp_port=465

-> Observações:
- Intervalo de Consulta: A cada 1 minuto, o programa consulta a cotação do ativo. Isso pode ser ajustado no código (alterando o valor do intervalo na função fetchStockPrice).
- API Limitada: A API da Alpha Vantage tem limites de requisições por minuto. Caso o limite seja ultrapassado, o programa pode falhar temporariamente até que o limite seja reiniciado.
