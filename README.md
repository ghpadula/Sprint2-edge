# Monitoramento de Temperatura com MQTT

Este projeto demonstra como usar um microcontrolador ESP32 para ler dados de temperatura de um sensor DS18B20 e publicar esses dados em um broker MQTT. Além disso, as mensagens são exibidas em um display(LCD) e o sistema é capaz de receber mensagens do broker.

## Componentes Necessários

- Microcontrolador ESP32
- Sensor de Temperatura DS18B20
- Display LCD (I2C)
- Fios Jumper
- Placa de Protótipo (opcional)

## Bibliotecas Utilizadas

- `OneWire.h` para comunicação com o sensor DS18B20
- `DallasTemperature.h` para leituras de temperatura
- `WiFi.h` para conectividade Wi-Fi
- `PubSubClient.h` para comunicação MQTT
- `ArduinoJson.h` para manipulação de JSON
- `LiquidCrystal_I2C.h` para controle do LCD

## Instalação

1. **Configuração do Arduino IDE**: Certifique-se de ter o Arduino IDE instalado.
2. **Instalação das Bibliotecas**:
   - Vá em `Sketch` > `Incluir Biblioteca` > `Gerenciar Bibliotecas`.
   - Instale as bibliotecas necessárias listadas acima.
3. **Credenciais Wi-Fi**: Atualize o `SSID` e a `PASSWORD` no código com suas credenciais de Wi-Fi.

## Visão Geral do Código

### Variáveis Globais

```cpp
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = ""; 
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "techadvanced_mqt";
const char *TOPIC_MSG = "fiap/techadvanced/msg";
const char *TOPIC_PUBLISH_TEMP = "fiap/techadvanced/temp";
```

- **Credenciais Wi-Fi**: Configure seu SSID e senha.
- **Detalhes do Broker MQTT**: Configure o endereço e a porta do broker.

### Funções

1. **`initWiFi()`**: Conecta-se à rede Wi-Fi e imprime o endereço IP local.
2. **`initMQTT()`**: Inicializa o cliente MQTT e define a função de callback para mensagens recebidas.
3. **`callbackMQTT()`**: Manipula mensagens recebidas do tópico MQTT inscrito.
4. **`reconnectMQTT()`**: Tenta reconectar ao broker MQTT se a conexão for perdida.
5. **`checkWiFIAndMQTT()`**: Verifica o status da conexão Wi-Fi e MQTT, reconectando se necessário.
6. **`reconnectWiFi()`**: Reconnecta ao Wi-Fi se estiver desconectado.

### Setup e Loop

- **`setup()`**: Inicializa a comunicação serial, LCD, Wi-Fi e MQTT. Também detecta o sensor DS18B20.
- **`loop()`**: Verifica o status da conexão, lê dados de temperatura, publica no tópico MQTT e atualiza o display LCD.

### Serialização em JSON

Os dados de temperatura são serializados em formato JSON antes da publicação:

```cpp
StaticJsonDocument<TAMANHO> json;
json["temperatura"] = temp;
```

## Uso

1. Faça o upload do código para o ESP32 usando o Arduino IDE.
2. Abra o Monitor Serial para visualizar o status da conexão e as leituras de temperatura.
3. O LCD exibirá a temperatura atual.
4. Qualquer mensagem enviada para o tópico `fiap/techadvanced/msg` será exibida no LCD.

## Integração com Node-RED

No repositorio, você encontrará os fluxos do Node-RED para receber mensagens e gerar gráficos. Quando a temperatura estiver irregular, o Node-RED enviará uma mensagem ao broker, que será recebida pelo ESP32, fazendo com que uma mensagem apareça no LCD.

## Solução de Problemas

- Verifique se o sensor DS18B20 está corretamente conectado.
- Confira a conexão Wi-Fi se o ESP32 não conectar.
- Verifique o status do broker MQTT se houver problemas ao publicar mensagens.

## Conclusão

Este projeto serve como uma base para aplicações de IoT envolvendo monitoramento de temperatura e comunicação MQTT. Sinta-se à vontade para expandi-lo adicionando mais sensores ou funcionalidades!

## Licença

Este projeto é de código aberto e está disponível sob a Licença MIT.
