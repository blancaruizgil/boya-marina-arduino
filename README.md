# boya-marina-arduino
Boya marina con sensores climáticos de medicion, control y actuacion con Wokwi.

## ¿Qué hace el sistema?
- Mide la temperatura y humedad del aire con un sensor DHT22.
- Mide la temperatura de la batería un un DS18B20.
- Mide la luz ambiental con un LDR.
- Mueve un Servo que simula una válvula de refrigeración según la temparatura de la batería.
- En función de la luz ambiente, enciende entre 0 y 8 LEDs para iluminación artificial.
- Si la batería está muy fría, enciende un LED de alerta.
- Visualiza ciertos parámetros en una pantalla LCD con I2C.

En este proyecto la boya no mide solo los parámetros, si no que decide si calentra o enfriar, cuánta luz artificial poner y proteger la batería si esta en mal estado.

## Componentes usados
- Arduino UNO
- Sensor DHT22 (para medir temperatura y humedad)
- Sensor DS18B20 (para medir temperatura de la batería)
- LDR (Sensor de luz ambiental)
- 8 LEDS + Registro 74HC595 (para la iluminación)
- Servo SG90 (Válvula de refrigeración)
- Pantalla LCD 16x2 cpn I2C
- LED de alerta

## Pines usados
- DHT22 (2)
- DS18B20 (3)
- LDR (A1)
- Servo (6)
- LED Alerta (13)
- 74HC595 Latch (12)
- 74HC595 Clock (11)
- 74HC595 Data (10)

## Función del control

- El servo se mueve dependiendo de la temperatura:
    - Se mueve a la posición de 0° si hace mucho frío, para calentar la batería.
    - Se mueve a 180° si hace mucho calor, para enfriar.
    - Se mueve a 90° si la temperatura está dentro de un buen rango.
  
      
- Los 8 LEDs se encienden en función del valor del LDR, cuanto más oscuro, más se encienden.

- Si la temperatura de la batería baja de 0°C, se enciende el LED del pin13 y avisa en la pantalla.

## Visualización en pantalla

Muestra Temperatura y Humedad en la fila de arriba y Luz Ambiental y Temeperatura de la batería en la fila de abajo.
