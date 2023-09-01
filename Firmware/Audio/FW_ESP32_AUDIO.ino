/* ****************************************************
 * ****                                           *****
 * ****              FABterl  AUDIO               *****
 * ****        VER: R1  -  by cyX - 3iusS         *****
 * ****                                           *****
 * ****************************************************/


#include "BluetoothA2DPSink.h" //https://github.com/pschatzmann/ESP32-A2DP
#include "string.h"
BluetoothA2DPSink a2dp_sink;

//############# Configuracion #############

/* ************** PINES FIJOS **************
  SCK <== NULL (al menos en el dac usado; haced el puente)
  VIN <== 5V
  GND <== DEBE SER UNA GND LIMPIA DE RUIDO (o el dac se vera afectado generando ruido)
*/


// DAC
#define pin_BCK 26       // DAC
#define pin_DIN 22       // DAC
#define pin_LCK 25       // DAC
#define pin_pico 23      // PICO
#define led_integrada 2  // ENCENDIDO ==> CONECTADO // PARPADEANDO ==> NO CONECTADO

char nombre_bt[] = "FABLAB READY TO PAIN";  // Nombre aki ray

void setup() {
  Serial.begin(9600);
  pinMode(led_integrada, OUTPUT);
  pinMode(pin_pico, OUTPUT);

  i2s_pin_config_t config_pines = {
    .bck_io_num = pin_BCK,
    .ws_io_num = pin_LCK,
    .data_out_num = pin_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  static i2s_config_t config_dac = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = (i2s_bits_per_sample_t)32,  // El dac es de 32bits, tenedlo en cuenta.
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = true,
    .tx_desc_auto_clear = true
  };
  a2dp_sink.set_i2s_config(config_dac);
  a2dp_sink.set_pin_config(config_pines);
  a2dp_sink.start(nombre_bt);
}

void loop() {
  while (a2dp_sink.is_connected() == 0) {
    Serial.print("No debe haber nada conectado ==> is_connected val: ");
    Serial.println(a2dp_sink.is_connected());
    for (int brillo = 0; brillo <= 255; brillo++) {
      analogWrite(led_integrada, brillo);
      delay(2);
    }

    // Atenuamos el LED disminuyendo gradualmente el brillo
    for (int brillo = 255; brillo >= 0; brillo--) {
      analogWrite(led_integrada, brillo);
      delay(2);
    }
    analogWrite(pin_pico, 0);
  }
  Serial.print("Debe haber algo conectado ==> is_connected val: ");
  Serial.println(a2dp_sink.is_connected());
  analogWrite(led_integrada, 255);
  analogWrite(pin_pico, 255);  // Activa las luces al ritmo de la musica (pi pico)
}
