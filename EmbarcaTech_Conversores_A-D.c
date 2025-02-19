#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "Bibliotecas/ssd1306.h"
#include "pico/bootrom.h"

// Define os pinos dos LED's
#define pin_red 13
#define pin_blue 12
#define pin_green 11

// Define os pinos dos botões
#define botaoA 5
#define botaoB 6
#define botaoJ 22

// Define o barramento do I2C
#define I2C_PORT i2c1
#define ADDR 0x3c
#define I2C_SDA 14
#define I2C_SCL 15

// Define os pinos do joystick
#define JoyX 27 
#define JoyY 26

// Define wrap para PWM
#define wrap 4096

// Define o limite de movimento do joystick
static const uint minX = 9;
static const uint maxX = 110;  
static const uint minY = 8;
static const uint maxY = 46;

// Variáveis globais
volatile bool estado_pwm = true;
volatile uint cont = 1; 
ssd1306_t ssd;
bool cor = true;
uint16_t adc_value_x, adc_value_y;
uint posX, posY; 

// Declaração das funções utilizadas
void setup();
void button_isr(uint gpio, uint32_t events);
void pwm_setup(uint gpio);
void desenhar_borda(uint cont);

int main(){
    stdio_init_all();
    setup();
    pwm_setup(pin_red);
    pwm_setup(pin_blue);
    
    while (true) { 
        ssd1306_fill(&ssd, false); // Limpa o display 
        
        // Lê o valor do joystick
        adc_select_input(1);
        adc_value_x = adc_read();
        printf("X: %d\n", adc_value_x);
        adc_select_input(0);
        adc_value_y = adc_read();
        printf("Y: %d\n", adc_value_y);
        
        //  Verifica se o joystick está na posição central
        if(adc_value_x > 1700 && adc_value_x < 2100 && adc_value_y > 1700 && adc_value_y < 2100){
            // Centraliza as coordenadas do quadrado 
            posX = 60; 
            posY = 28;
        }else{ 
            posX = (adc_value_x * (maxX-minX) / 4095) + minX;   // Mapeia o movimento horizontal do joystick
            posY = maxY - ((adc_value_y * (maxY- minY)) / 4095);   // Mapeia o movimento vertical do joystick
        }
        
        desenhar_borda(cont);   // Seleciona a borda 
        ssd1306_rect(&ssd, posY, posX, 8, 8, cor, cor); // Move o quadrado de 8x8 no display
        ssd1306_send_data(&ssd);    // Atualiza o display

        //  Controla a aitvação e intensidade dos LED's     
        if(estado_pwm){
            //  Verifica se o joystick está na posição central
            if (adc_value_x > 1500 && adc_value_x < 2300 && adc_value_y > 1500 && adc_value_y < 2300){
                // Desliga os LEDs se o joystick estiver centralizado
                pwm_set_gpio_level(pin_red, 0);
                pwm_set_gpio_level(pin_blue, 0);
            }else{
                // Ajusta o brilho dos LEDs com base nos valores do joystick
                uint16_t pwm_value_x = abs(adc_value_x - 2048) * 2;
                uint16_t pwm_value_y = abs(adc_value_y - 2048) * 2;  

                pwm_set_gpio_level(pin_red, pwm_value_x);
                pwm_set_gpio_level(pin_blue, pwm_value_y);
            }
        }
        sleep_ms(10);
    }
}

// Configura o PWM para os LEDS
void pwm_setup(uint gpio){
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true);  
}

// Inicialização e Configurações
void setup(){
    // Inicializa o LED verde
    gpio_init(pin_green);
    gpio_set_dir(pin_green, GPIO_OUT);
    gpio_put(pin_green, false);

    // Inicializa e configura o callback dos botões
    gpio_init(botaoA);
    gpio_set_dir(botaoA, GPIO_IN);
    gpio_pull_up(botaoA);
    gpio_set_irq_enabled_with_callback(botaoA, GPIO_IRQ_EDGE_FALL, true, button_isr);

    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, button_isr);

    gpio_init(botaoJ);
    gpio_set_dir(botaoJ, GPIO_IN);
    gpio_pull_up(botaoJ);
    gpio_set_irq_enabled_with_callback(botaoJ, GPIO_IRQ_EDGE_FALL, true, button_isr);

    // Inicializa o I2C para comunicação com o display 
    i2c_init(I2C_PORT, 400 * 1000); // Configura a comunicação I2C com velocidade de 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);

    // Inicializa e configura o Display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDR, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicializa o ADC
    adc_init();
    adc_gpio_init(JoyX);
    adc_gpio_init(JoyY);
}

// Função de interrupção para detectar o pressionamento dos botões com debounce
void button_isr(uint gpio, uint32_t events){
    
    static absolute_time_t last_press = 0;  // Armazena o tempo do último pressionamento
    absolute_time_t now = get_absolute_time();  // Obtém o tempo atual
    // Debounce de 500 ms
    if(absolute_time_diff_us(last_press, now) > 500000){
        if(!gpio_get(botaoA)){  // Verifica presionamento do Botão A
            estado_pwm = !estado_pwm;   // Controla a ativação dos LED's
        }else   if(!gpio_get(botaoJ)){  // Verifica presionamento do Botão J
            cont++;     
            if(cont > 4) cont = 1;  
            gpio_put(pin_green, !gpio_get(pin_green)); 
        }else   if(!gpio_get(botaoB)){ // Verifica presionamento do Botão B
            reset_usb_boot(0,0);    // Ativa o modo bootsel
        }   
    }
    last_press = now;   // Atualiza o tempo do último pressionamento
}

// Desenha diferente esilos de borda no display
void desenhar_borda(uint cont){
    switch (cont){
    case 1:
        ssd1306_rect(&ssd, 0,0, 127, 63, cor,!cor);
        ssd1306_rect(&ssd, 1,1, 125, 61, cor,!cor);
        ssd1306_rect(&ssd, 2,2, 123, 59, cor,!cor);
        ssd1306_rect(&ssd, 3,3, 121, 57, cor,!cor);
        ssd1306_rect(&ssd, 4,4, 119, 55, cor,!cor);
        break;
    case 2:
        ssd1306_rect(&ssd, 0,0, 127, 63, cor,!cor);
        ssd1306_rect(&ssd, 2,2, 123, 59, cor,!cor);
        ssd1306_rect(&ssd, 4,4, 119, 55, cor,!cor);
        break; 
    case 3:
        ssd1306_rect(&ssd, 0,0, 127, 63, cor,!cor);
        ssd1306_rect(&ssd, 4,4, 119, 55, cor,!cor);
        break;
    case 4:
        ssd1306_rect(&ssd, 0,0, 127, 63, cor,!cor);
        break;
    }
}