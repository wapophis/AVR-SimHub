# Soporte para Encoders Rotatorios MCP23017 en AVR-SimHub

Este documento explica cómo configurar y usar encoders rotatorios conectados a través de módulos MCP23017 en el proyecto AVR-SimHub.

## 📋 Tabla de Contenidos

- [Descripción General](#descripción-general)
- [Requisitos de Hardware](#requisitos-de-hardware)
- [Configuración del Software](#configuración-del-software)
- [Cableado](#cableado)
- [Ejemplos de Configuración](#ejemplos-de-configuración)
- [Solución de Problemas](#solución-de-problemas)

## 🎯 Descripción General

El proyecto AVR-SimHub ahora soporta **encoders rotatorios conectados a través de módulos MCP23017**, permitiendo expandir la capacidad de encoders de tu joystick hasta **64 encoders** (8 por módulo MCP23017). Cada encoder puede tener configuración independiente de dirección, pasos y botón.

### Características Principales

- ✅ **Hasta 64 encoders MCP23017** (8 por módulo MCP23017)
- ✅ **Configuración independiente** por encoder
- ✅ **Soporte para botones** en cada encoder
- ✅ **Dirección reversible** por encoder
- ✅ **Modo de pasos completos/medios** por encoder
- ✅ **Pull-up interno** configurable
- ✅ **Compatibilidad total** con el sistema de encoders existente
- ✅ **Debounce integrado** para botones y encoders

## 🔧 Requisitos de Hardware

### Componentes Necesarios

- **Módulos MCP23017** (hasta 8)
- **Encoders rotatorios KY-040** (hasta 8 por módulo)
- **Botones** (opcional, para cada encoder)
- **Cables de conexión**

### Conexiones I2C

```
Arduino Micro    MCP23017
     SDA    →    SDA
     SCL    →    SCL
     VCC    →    VCC
     GND    →    GND
```

## ⚙️ Configuración del Software

### 1. Habilitar Encoders MCP23017

En el archivo `DisplayClientV2.cpp`, configura:

```cpp
#define MCP23017_TOTAL_ENCODERS_COUNT 4  // Número de encoders MCP23017 a usar
```

### 2. Configurar Encoders Individuales

Para cada encoder, configura:

```cpp
// Encoder 1
#define MCP23017_ENCODER1_MCP_INDEX 0    // Índice del MCP23017 (0-7)
#define MCP23017_ENCODER1_CLK_PIN 0      // Pin CLK en el MCP23017 (0-15)
#define MCP23017_ENCODER1_DT_PIN 1       // Pin DT en el MCP23017 (0-15)
#define MCP23017_ENCODER1_BUTTON_PIN 2   // Pin del botón (0-15, -1 si no hay)
#define MCP23017_ENCODER1_REVERSE_DIRECTION 0 // Invertir dirección
#define MCP23017_ENCODER1_ENABLE_PULLUP 1     // Habilitar pull-up interno
#define MCP23017_ENCODER1_ENABLE_HALFSTEPS 0  // Modo de pasos (0=completos, 1=medios)
```

### 3. Configurar Módulos MCP23017

Asegúrate de que los módulos MCP23017 estén configurados:

```cpp
#define MCP23017_CHIPS_COUNT 1     // Número de módulos MCP23017
#define MCP23017_I2C_ADDRESS_1 0x20 // Dirección I2C del primer módulo
```

## 🔌 Cableado

### Conexión del Encoder KY-040

```
Encoder KY-040    MCP23017
     CLK    →     Pin CLK (ej: pin 0)
     DT     →     Pin DT (ej: pin 1)
     SW     →     Pin Button (ej: pin 2)
     VCC    →     VCC
     GND    →     GND
```

### Distribución de Pines por Módulo MCP23017

Cada módulo MCP23017 tiene 16 pines (0-15). Puedes conectar hasta 8 encoders por módulo:

- **Encoder 1**: Pines 0, 1, 2 (CLK, DT, Button)
- **Encoder 2**: Pines 3, 4, 5 (CLK, DT, Button)
- **Encoder 3**: Pines 6, 7, 8 (CLK, DT, Button)
- **Encoder 4**: Pines 9, 10, 11 (CLK, DT, Button)
- **Encoder 5**: Pines 12, 13, 14 (CLK, DT, Button)
- **Encoder 6**: Pines 15, 0, 1 (CLK, DT, Button) - *Nota: usar otro módulo*
- etc.

## 📝 Ejemplos de Configuración

### Ejemplo 1: Un Encoder en el Primer Módulo

```cpp
#define MCP23017_TOTAL_ENCODERS_COUNT 1
#define MCP23017_CHIPS_COUNT 1
#define MCP23017_I2C_ADDRESS_1 0x20

#define MCP23017_ENCODER1_MCP_INDEX 0
#define MCP23017_ENCODER1_CLK_PIN 0
#define MCP23017_ENCODER1_DT_PIN 1
#define MCP23017_ENCODER1_BUTTON_PIN 2
#define MCP23017_ENCODER1_REVERSE_DIRECTION 0
#define MCP23017_ENCODER1_ENABLE_PULLUP 1
#define MCP23017_ENCODER1_ENABLE_HALFSTEPS 0
```

### Ejemplo 2: Cuatro Encoders en el Primer Módulo

```cpp
#define MCP23017_TOTAL_ENCODERS_COUNT 4
#define MCP23017_CHIPS_COUNT 1
#define MCP23017_I2C_ADDRESS_1 0x20

// Encoder 1: Pines 0, 1, 2
#define MCP23017_ENCODER1_MCP_INDEX 0
#define MCP23017_ENCODER1_CLK_PIN 0
#define MCP23017_ENCODER1_DT_PIN 1
#define MCP23017_ENCODER1_BUTTON_PIN 2

// Encoder 2: Pines 3, 4, 5
#define MCP23017_ENCODER2_MCP_INDEX 0
#define MCP23017_ENCODER2_CLK_PIN 3
#define MCP23017_ENCODER2_DT_PIN 4
#define MCP23017_ENCODER2_BUTTON_PIN 5

// Encoder 3: Pines 6, 7, 8
#define MCP23017_ENCODER3_MCP_INDEX 0
#define MCP23017_ENCODER3_CLK_PIN 6
#define MCP23017_ENCODER3_DT_PIN 7
#define MCP23017_ENCODER3_BUTTON_PIN 8

// Encoder 4: Pines 9, 10, 11
#define MCP23017_ENCODER4_MCP_INDEX 0
#define MCP23017_ENCODER4_CLK_PIN 9
#define MCP23017_ENCODER4_DT_PIN 10
#define MCP23017_ENCODER4_BUTTON_PIN 11
```

### Ejemplo 3: Encoders en Múltiples Módulos

```cpp
#define MCP23017_TOTAL_ENCODERS_COUNT 8
#define MCP23017_CHIPS_COUNT 2
#define MCP23017_I2C_ADDRESS_1 0x20
#define MCP23017_I2C_ADDRESS_2 0x21

// Encoders 1-4 en el primer módulo (0x20)
#define MCP23017_ENCODER1_MCP_INDEX 0  // Módulo 1
#define MCP23017_ENCODER2_MCP_INDEX 0  // Módulo 1
#define MCP23017_ENCODER3_MCP_INDEX 0  // Módulo 1
#define MCP23017_ENCODER4_MCP_INDEX 0  // Módulo 1

// Encoders 5-8 en el segundo módulo (0x21)
#define MCP23017_ENCODER5_MCP_INDEX 1  // Módulo 2
#define MCP23017_ENCODER6_MCP_INDEX 1  // Módulo 2
#define MCP23017_ENCODER7_MCP_INDEX 1  // Módulo 2
#define MCP23017_ENCODER8_MCP_INDEX 1  // Módulo 2
```

## 🔧 Solución de Problemas

### Problema: Los encoders no responden

**Soluciones:**
1. Verifica las conexiones I2C (SDA, SCL)
2. Confirma que la dirección I2C del MCP23017 es correcta
3. Verifica que `MCP23017_TOTAL_ENCODERS_COUNT` sea mayor que 0
4. Asegúrate de que los pines CLK y DT estén correctamente asignados

### Problema: Dirección del encoder invertida

**Solución:**
```cpp
#define MCP23017_ENCODER1_REVERSE_DIRECTION 1  // Cambiar a 1
```

### Problema: Encoder muy sensible o poco sensible

**Soluciones:**
1. Para mayor precisión: `MCP23017_ENCODER1_ENABLE_HALFSTEPS 1`
2. Para menos sensibilidad: `MCP23017_ENCODER1_ENABLE_HALFSTEPS 0`

### Problema: Botón del encoder no funciona

**Soluciones:**
1. Verifica que `MCP23017_ENCODER1_BUTTON_PIN` esté configurado correctamente
2. Usa `-1` si no hay botón: `MCP23017_ENCODER1_BUTTON_PIN -1`
3. Confirma que `MCP23017_ENCODER1_ENABLE_PULLUP 1`

## 📊 IDs de Encoders

Los encoders MCP23017 se asignan IDs secuenciales después de los encoders regulares:

- **Encoders regulares**: IDs 1, 2, 3, 4, 5, 6, 7, 8
- **Encoders MCP23017**: IDs 9, 10, 11, 12, 13, 14, 15, 16, etc.

## 🎮 Compatibilidad con Gamepad

Los encoders MCP23017 son completamente compatibles con el sistema de gamepad de SimHub y se integran automáticamente con el joystick virtual.

## 📚 Referencias

- [Documentación MCP23017](https://www.microchip.com/wwwproducts/en/MCP23017)
- [Encoder KY-040](https://www.dx.com/p/ky-040-rotary-encoder-module-brick-sensor-development-for-arduino-avr-pic-420429)
- [SimHub Documentation](https://www.simhubdash.com/)
