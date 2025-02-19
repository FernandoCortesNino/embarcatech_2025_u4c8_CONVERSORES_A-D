# embarcatech_2025_u4c8_CONVERSORES_A-D
Relatório do Projeto: Controle de LEDs e Exibição Gráfica com Conversor A/D

1. Introdução
Este projeto tem como objetivo consolidar os conhecimentos sobre Conversores Analógico-Digitais (ADC) no RP2040, utilizando a placa de desenvolvimento BitDogLab. Durante a implementação, foram explorados os conceitos de aquisição de sinais analógicos por meio do joystick, controle de LEDs RGB via PWM e representação gráfica no display SSD1306 utilizando o protocolo I2C.

2. Objetivos

Compreender o funcionamento do Conversor A/D (ADC) no RP2040.

Utilizar PWM para controlar a intensidade dos LEDs RGB com base nos valores lidos do joystick.

Representar graficamente a posição do joystick no display SSD1306 por meio de um quadrado móvel.

Aplicar o protocolo de comunicação I2C na integração com o display.

Implementar interrupções (IRQ) para interação com botões.

Aplicar debouncing via software para evitar leituras incorretas dos botões.

Desenvolver um código bem estruturado e comentado para facilitar compreensão e reuso.

3. Materiais Utilizados

Microcontrolador Raspberry Pi Pico W

LED RGB (GPIOs 11, 12 e 13)

Joystick (GPIOs 26 e 27 para eixos X e Y)

Botão do Joystick (GPIO 22)

Botão A (GPIO 5)

Display SSD1306 (I2C - GPIOs 14 e 15)

4. Implementação

4.1 Controle de LEDs RGB com PWM

O LED Azul teve sua intensidade ajustada de acordo com o eixo Y do joystick.

O LED Vermelho seguiu o mesmo princípio, mas baseado no eixo X.

O brilho dos LEDs foi controlado via PWM, permitindo transições suaves de intensidade.

4.2 Representação Gráfica no Display SSD1306

Um quadrado de 8x8 pixels foi renderizado no display.

O movimento do quadrado foi mapeado proporcionalmente à posição do joystick.

4.3 Interação com Botões

Botão do Joystick (GPIO 22):

Alterna o estado do LED Verde.

Modifica o estilo da borda do display para indicar o pressionamento.

Botão A (GPIO 5):

Ativa ou desativa os LEDs PWM.

O tratamento dos botões foi realizado via interrupções (IRQ) e debounce via software.

5. Resultados Obtidos

O Conversor A/D (ADC) do RP2040 funcionou corretamente, permitindo a leitura dos valores do joystick.

A implementação do PWM possibilitou o controle preciso da intensidade dos LEDs RGB.

O quadrado no display SSD1306 acompanhou corretamente os movimentos do joystick.

O botão do joystick e o botão A responderam conforme esperado, alternando os estados dos LEDs e mudando o estilo da borda do display.

A implementação do debounce via software garantiu leituras confiáveis dos botões.

O código foi bem estruturado e comentado, facilitando sua compreensão e manutenção.

6. Conclusão
O projeto permitiu explorar a integração de sensores analógicos (joystick) com microcontroladores, demonstrando a importância do Conversor A/D (ADC) do RP2040. O controle de LEDs RGB via PWM, combinado com a representação gráfica no display SSD1306, mostrou a versatilidade e capacidade de processamento do sistema. O uso de interrupções e debounce via software garantiu interações fluidas e confiáveis. Com isso, a atividade reforçou conceitos essenciais sobre comunicação I2C, PWM e processamento de sinais analógicos.

7. LINK DO VÍDEO: https://drive.google.com/file/d/1_Qv-EhTjgYshXmwmhUJ3rO6hyfu87_eG/view?usp=sharing

8. Referências

Documentação do Pico SDK

Datasheets do RP2040, Display SSD1306 e Joystick

Materiais da disciplina sobre Conversores A/D e PWM
