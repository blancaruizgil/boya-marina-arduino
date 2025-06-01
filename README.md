# boya-marina-arduino
Boya marina con sensores climáticos de medicion, control y actuacion con Wokwi.

## Qué hace el sistema?
- Mide la temperatura y humedad del aire con un sensor DHT22.
- Mide la temperatura de la bateria un un DS18B20.
- Mide la luz ambiental con un LDR.
- Mueve un Servo que simula una valvula de refrigeracion segun la temparatura de la bateria.
- En funcion de la luz ambiente, enciende entre 0 y 8 LEDs para iluminacion artificial.
- Si la bateria esta muy fria, enciende un LED de alerta.
- Visualiza ciertos parametros en una pantalla LCD con I2C.
