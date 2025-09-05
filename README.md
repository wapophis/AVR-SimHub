# Configuración de MCP23017 para Botones de Joystick en AVR-SimHub

Este documento explica cómo configurar módulos MCP23017 para expandir la capacidad de botones de tu joystick en el proyecto AVR-SimHub.

## 📋 Tabla de Contenidos

- [Descripción General](#descripción-general)
- [Requisitos de Hardware](#requisitos-de-hardware)
- [Configuración de Direcciones I2C](#configuración-de-direcciones-i2c)
- [Cableado de Botones](#cableado-de-botones)
- [Configuración del Software](#configuración-del-software)
- [Ejemplos de Configuración](#ejemplos-de-configuración)
- [Solución de Problemas](#solución-de-problemas)
- [Referencias](#referencias)

## 🎯 Descripción General

El proyecto AVR-SimHub ahora soporta hasta **8 módulos MCP23017** conectados vía I2C, permitiendo expandir la capacidad de botones de tu joystick hasta **128 botones** (16 por módulo). Cada módulo puede tener una dirección I2C única y los botones se mapean automáticamente al joystick.

### Características Principales

- ✅ **Hasta 8 módulos MCP23017** en paralelo
- ✅ **128 botones totales** (16 por módulo)
- ✅ **Direcciones I2C flexibles** (0x20-0x27)
- ✅ **Validación automática** de configuración
- ✅ **Debounce integrado** (50ms por botón)
- ✅ **Compatibilidad total** con Gamepad/Joystick
- ✅ **Configuración selectiva** de módulos activos

## 🔧 Requisitos de Hardware

### Componentes Necesarios

- **Módulos MCP23017** (hasta 8)
- **Botones** (hasta 16 por módulo)
- **Resistencias pull-up** (opcional, el MCP23017 las incluye internamente)
- **Cables de conexión**

### Conexiones I2C

```
Arduino Micro    MCP23017
     SDA    →    SDA
     SCL    →    SCL
     VCC    →    VCC
     GND    →    GND
```

### 🔌 Cableado Específico para Arduino Micro Pro

#### Pines I2C del Arduino Micro Pro

El Arduino Micro Pro tiene los pines I2C en ubicaciones específicas:

```
Arduino Micro Pro Pinout:
┌─────────────────────────────────┐
│  RAW    VIN    GND   RST   A3   │
│  A0     A1     A2    A3    A4   │ ← A4 = SDA
│  A5     A6     A7    A8    A9   │ ← A5 = SCL
│  A10    A11    A12   A13   A14  │
│  A15    A16    A17   A18   A19  │
└─────────────────────────────────┘
```

#### Conexiones Detalladas

| Arduino Micro Pro | MCP23017 | Descripción |
|------------------|----------|-------------|
| **A4**           | SDA      | Datos I2C (Serial Data) |
| **A5**           | SCL      | Reloj I2C (Serial Clock) |
| **VIN** o **5V** | VCC      | Alimentación 5V |
| **GND**          | GND      | Tierra común |

#### Diagrama de Conexión Múltiple

Para conectar múltiples módulos MCP23017:

```
Arduino Micro Pro
     │
     ├─ A4 (SDA) ──┬─ MCP23017 #1 SDA
     │              ├─ MCP23017 #2 SDA
     │              ├─ MCP23017 #3 SDA
     │              └─ ... (hasta 8 módulos)
     │
     ├─ A5 (SCL) ──┬─ MCP23017 #1 SCL
     │              ├─ MCP23017 #2 SCL
     │              ├─ MCP23017 #3 SCL
     │              └─ ... (hasta 8 módulos)
     │
     ├─ VIN (5V) ──┬─ MCP23017 #1 VCC
     │              ├─ MCP23017 #2 VCC
     │              ├─ MCP23017 #3 VCC
     │              └─ ... (hasta 8 módulos)
     │
     └─ GND ───────┬─ MCP23017 #1 GND
                   ├─ MCP23017 #2 GND
                   ├─ MCP23017 #3 GND
                   └─ ... (hasta 8 módulos)
```

#### Resistencias Pull-up (Opcional)

Para mejorar la estabilidad de la comunicación I2C, puedes añadir resistencias pull-up:

```
Arduino Micro Pro
     │
     ├─ A4 (SDA) ──┬─ Resistor 4.7kΩ ── VIN
     │              └─ MCP23017 SDA
     │
     ├─ A5 (SCL) ──┬─ Resistor 4.7kΩ ── VIN
     │              └─ MCP23017 SCL
     │
     ├─ VIN (5V) ── MCP23017 VCC
     └─ GND ─────── MCP23017 GND
```

**Nota:** Los módulos MCP23017 ya incluyen resistencias pull-up internas, por lo que las externas son opcionales.

#### ⚠️ Consideraciones Especiales para Arduino Micro Pro

##### Alimentación
- **VIN**: Usar VIN para alimentación desde USB o fuente externa
- **5V**: Solo usar si tienes una fuente de alimentación estable de 5V
- **Consumo**: Cada MCP23017 consume ~1mA en reposo, verificar capacidad de alimentación

##### Pines I2C Internos
- **A4 (SDA)**: Pin 18 del microcontrolador ATMEGA32U4
- **A5 (SCL)**: Pin 19 del microcontrolador ATMEGA32U4
- **Frecuencia I2C**: Por defecto 100kHz, compatible con MCP23017

##### Compatibilidad de Voltaje
- **MCP23017**: Funciona con 3.3V y 5V
- **Arduino Micro Pro**: Salida 5V en VIN/VCC
- **Niveles lógicos**: Compatibles sin conversión

##### Limitaciones del Arduino Micro Pro
- **RAM disponible**: ~2.5KB (considerar uso de memoria)
- **Pines digitales**: Limitados, I2C libera pines para otros usos
- **Velocidad de procesamiento**: Suficiente para escaneo de botones

### Pines MCP23017

Cada módulo MCP23017 tiene 16 pines GPIO:
- **GPA0-GPA7**: Pines 0-7 (Puerto A)
- **GPB0-GPB7**: Pines 8-15 (Puerto B)

## 🔌 Configuración de Direcciones I2C

### Pines de Dirección

Cada MCP23017 tiene 3 pines de configuración de dirección:
- **A0**: Bit menos significativo
- **A1**: Bit medio
- **A2**: Bit más significativo

### Tabla de Direcciones

| A2 | A1 | A0 | Dirección | Módulo |
|----|----|----|-----------|--------|
| 0  | 0  | 0  | 0x20      | #1     |
| 0  | 0  | 1  | 0x21      | #2     |
| 0  | 1  | 0  | 0x22      | #3     |
| 0  | 1  | 1  | 0x23      | #4     |
| 1  | 0  | 0  | 0x24      | #5     |
| 1  | 0  | 1  | 0x25      | #6     |
| 1  | 1  | 0  | 0x26      | #7     |
| 1  | 1  | 1  | 0x27      | #8     |

### Ejemplo de Configuración

Para usar 3 módulos con direcciones no consecutivas:
```
Módulo 1: A2=0, A1=0, A0=0 → 0x20
Módulo 2: A2=0, A1=1, A0=1 → 0x23  
Módulo 3: A2=1, A1=1, A0=1 → 0x27
```

## 🔗 Cableado de Botones

### Configuración Recomendada (Pull-up Interno)

```
VCC (3.3V/5V)
    │
    ├─ Resistor Pull-up (opcional)
    │
    ├─ MCP23017 Pin (GPA0-GPB7)
    │
    └─ Botón
        │
        └─ GND
```

### Configuración Alternativa (Pull-down)

```
VCC (3.3V/5V)
    │
    └─ Botón
        │
        └─ MCP23017 Pin (GPA0-GPB7)
            │
            └─ GND
```

### Mapeo de Pines Globales

Los pines se mapean globalmente en el sistema:

```
Módulo 0 (0x20): Pines 0-15   → GPA0-GPA7, GPB0-GPB7
Módulo 1 (0x21): Pines 16-31  → GPA0-GPA7, GPB0-GPB7
Módulo 2 (0x22): Pines 32-47  → GPA0-GPA7, GPB0-GPB7
...
Módulo 7 (0x27): Pines 112-127 → GPA0-GPA7, GPB0-GPB7
```

## ⚙️ Configuración del Software

### 1. Editar DisplayClientV2.cpp

Localiza la sección de configuración MCP23017 (aproximadamente línea 110):

```cpp
#define MCP23017_CHIPS_COUNT 0     // Número de módulos activos (1-8)
#define MCP23017_TOTAL_BUTTONS_COUNT 0  // Total de botones (1-128)

// Direcciones I2C para cada módulo
#define MCP23017_I2C_ADDRESS_1 0x20 // Módulo #1
#define MCP23017_I2C_ADDRESS_2 0x21 // Módulo #2
#define MCP23017_I2C_ADDRESS_3 0x22 // Módulo #3
#define MCP23017_I2C_ADDRESS_4 0x23 // Módulo #4
#define MCP23017_I2C_ADDRESS_5 0x24 // Módulo #5
#define MCP23017_I2C_ADDRESS_6 0x25 // Módulo #6
#define MCP23017_I2C_ADDRESS_7 0x26 // Módulo #7
#define MCP23017_I2C_ADDRESS_8 0x27 // Módulo #8
```

### 2. Configurar Módulos Activos

Ejemplo para 3 módulos:

```cpp
#define MCP23017_CHIPS_COUNT 3
#define MCP23017_TOTAL_BUTTONS_COUNT 24  // 3 módulos × 8 botones cada uno

// Solo configurar las direcciones de los módulos activos
#define MCP23017_I2C_ADDRESS_1 0x20  // Primer módulo
#define MCP23017_I2C_ADDRESS_2 0x23  // Segundo módulo (no consecutivo)
#define MCP23017_I2C_ADDRESS_3 0x27  // Tercer módulo
```

### 3. Habilitar Gamepad

Asegúrate de que el gamepad esté habilitado:

```cpp
#define ENABLE_MICRO_GAMEPAD 1
```

## 📝 Ejemplos de Configuración

### Ejemplo 1: Un Solo Módulo con 8 Botones

```cpp
#define MCP23017_CHIPS_COUNT 1
#define MCP23017_TOTAL_BUTTONS_COUNT 8
#define MCP23017_I2C_ADDRESS_1 0x20
```

**Cableado:**
- Botones conectados a GPA0-GPA7 (pines 0-7)
- Dirección: A2=0, A1=0, A0=0

### Ejemplo 2: Dos Módulos con 16 Botones Total

```cpp
#define MCP23017_CHIPS_COUNT 2
#define MCP23017_TOTAL_BUTTONS_COUNT 16
#define MCP23017_I2C_ADDRESS_1 0x20  // Módulo 1
#define MCP23017_I2C_ADDRESS_2 0x25  // Módulo 2
```

**Cableado:**
- Módulo 1: Botones en GPA0-GPA7 (pines 0-7)
- Módulo 2: Botones en GPA0-GPA7 (pines 8-15)
- Direcciones: 0x20 y 0x25

### Ejemplo 3: Configuración Completa (3 Módulos)

```cpp
#define MCP23017_CHIPS_COUNT 3
#define MCP23017_TOTAL_BUTTONS_COUNT 24
#define MCP23017_I2C_ADDRESS_1 0x20  // Módulo 1
#define MCP23017_I2C_ADDRESS_2 0x23  // Módulo 2  
#define MCP23017_I2C_ADDRESS_3 0x27  // Módulo 3
```

### 🔌 Ejemplo de Cableado para Arduino Micro Pro

#### Configuración: 2 Módulos MCP23017 con 16 Botones

**Hardware necesario:**
- 1x Arduino Micro Pro
- 2x Módulos MCP23017
- 16x Botones
- Cables de conexión

**Cableado paso a paso:**

```
Arduino Micro Pro
     │
     ├─ A4 (SDA) ──┬─ MCP23017 #1 SDA (dirección 0x20)
     │              └─ MCP23017 #2 SDA (dirección 0x23)
     │
     ├─ A5 (SCL) ──┬─ MCP23017 #1 SCL
     │              └─ MCP23017 #2 SCL
     │
     ├─ VIN (5V) ──┬─ MCP23017 #1 VCC
     │              └─ MCP23017 #2 VCC
     │
     └─ GND ───────┬─ MCP23017 #1 GND
                   └─ MCP23017 #2 GND
```

**Configuración de direcciones:**
- **MCP23017 #1**: A2=0, A1=0, A0=0 → 0x20
- **MCP23017 #2**: A2=0, A1=1, A0=1 → 0x23

**Conexión de botones:**
- **Módulo 1**: Botones conectados a GPA0-GPA7 (pines 0-7)
- **Módulo 2**: Botones conectados a GPA0-GPA7 (pines 8-15)

**Configuración software:**
```cpp
#define MCP23017_CHIPS_COUNT 2
#define MCP23017_TOTAL_BUTTONS_COUNT 16
#define MCP23017_I2C_ADDRESS_1 0x20
#define MCP23017_I2C_ADDRESS_2 0x23
```

## 🔍 Mapeo de Botones al Joystick

### IDs de Botones

Los botones se mapean automáticamente al joystick con IDs consecutivos:

```
Botones MCP23017: ID 1 → ID N (donde N = MCP23017_TOTAL_BUTTONS_COUNT)
Botones Locales: ID N+1 → ID N+M (donde M = ENABLED_BUTTONS_COUNT)
Matrices: ID N+M+1 → ID N+M+P (donde P = botones de matriz)
```

### Ejemplo de Mapeo

Con la configuración:
- 24 botones MCP23017
- 4 botones locales
- 1 matriz 3×3

```
MCP23017: Botones 1-24
Locales: Botones 25-28  
Matriz: Botones 29-37
```

## 🚀 Compilación y Uso

### 1. Compilar el Proyecto

```bash
pio run
```

### 2. Verificar Configuración

El sistema mostrará en el Serial Monitor:

```
MCP23017 configuration: 3 chips, 24 total buttons
  Chip #1: 0x20
  Chip #2: 0x23
  Chip #3: 0x27
```

### 3. Probar Botones

Los botones MCP23017 aparecerán automáticamente en tu sistema como botones de joystick adicionales.

### ✅ Verificación Rápida para Arduino Micro Pro

#### Checklist de Conexiones

Antes de compilar, verifica:

- [ ] **A4 (SDA)** conectado a todos los módulos MCP23017
- [ ] **A5 (SCL)** conectado a todos los módulos MCP23017  
- [ ] **VIN** conectado a VCC de todos los módulos
- [ ] **GND** conectado a GND de todos los módulos
- [ ] **Pines A0-A2** configurados correctamente en cada módulo
- [ ] **Botones** conectados a los pines deseados (GPA0-GPB7)

#### Test de Comunicación I2C

Añade este código temporal en `setup()` para verificar conexiones:

```cpp
void testI2CSetup() {
    Serial.println("=== I2C Connection Test ===");
    Wire.begin();
    delay(100);
    
    for (int i = 0; i < MCP23017_CHIPS_COUNT; i++) {
        byte address = MCP23017_ADDRESSES[i];
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.print("✓ MCP23017 #");
            Serial.print(i + 1);
            Serial.print(" found at 0x");
            Serial.println(address, HEX);
        } else {
            Serial.print("✗ MCP23017 #");
            Serial.print(i + 1);
            Serial.print(" NOT found at 0x");
            Serial.println(address, HEX);
        }
    }
    Serial.println("==========================");
}
```

**Llamar en setup():**
```cpp
testI2CSetup(); // Añadir después de Wire.begin()
```

## 🔧 Solución de Problemas

### Error: Dirección I2C Inválida

```
Error: MCP23017 #1 address 0x30 is invalid. Must be between 0x20-0x27
```

**Solución:** Verificar que las direcciones estén entre 0x20-0x27.

### Error: Direcciones Duplicadas

```
Error: Duplicate MCP23017 address 0x20 found
```

**Solución:** Asegurar que cada módulo tenga una dirección única.

### Botones No Responden

**Posibles causas:**
1. Dirección I2C incorrecta
2. Cableado incorrecto
3. Módulo no inicializado

**Solución:**
1. Verificar configuración de pines A0-A2
2. Comprobar conexiones SDA/SCL
3. Revisar Serial Monitor para errores

### Módulo No Detectado

**Diagnóstico:**
```cpp
// Añadir en setup() para debug
Wire.beginTransmission(0x20);
byte error = Wire.endTransmission();
if (error == 0) {
    Serial.println("MCP23017 detected at 0x20");
} else {
    Serial.println("MCP23017 not found at 0x20");
}
```

### 🔧 Problemas Específicos del Arduino Micro Pro

#### Error: "MCP23017 not found"

**Causas comunes:**
1. **Pines I2C incorrectos**: Verificar que uses D2 (SDA) y D3 (SCL)
2. **Alimentación insuficiente**: Usar VIN en lugar de 5V
3. **Cables sueltos**: Verificar conexiones SDA/SCL/VCC/GND

**Solución paso a paso:**
```cpp
// Código de diagnóstico completo
void testI2CConnections() {
    Serial.println("Testing I2C connections...");
    
    for (byte address = 0x20; address <= 0x27; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.print("Device found at 0x");
            Serial.println(address, HEX);
        } else {
            Serial.print("No device at 0x");
            Serial.println(address, HEX);
        }
    }
}
```

#### Problemas de Alimentación

**Síntomas:**
- Módulos se reinician aleatoriamente
- Comunicación I2C intermitente
- Botones no responden consistentemente

**Solución:**
- Usar **VIN** en lugar de **5V** para alimentación
- Verificar que la fuente USB proporcione suficiente corriente
- Considerar alimentación externa para múltiples módulos

#### Conflicto de Pines

**Pines a evitar en Arduino Micro Pro:**
- **D2 y D3**: Reservados para I2C
- **A0-A3**: Usar con precaución (pueden ser analógicos)
- **D0-D1**: Reservados para comunicación serial

**Recomendación:** Usar pines digitales D2-D13 para otros componentes.

## 📚 Referencias

- [Adafruit MCP23017 Library](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library)
- [MCP23017 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf)
- [I2C Address Configuration](https://www.microchip.com/wwwproducts/en/MCP23017)

## 🤝 Contribuciones

Si encuentras problemas o tienes sugerencias de mejora, por favor:

1. Verifica que tu configuración siga este documento
2. Revisa el Serial Monitor para mensajes de error
3. Documenta tu configuración específica
4. Comparte tu experiencia para ayudar a otros

---

**Nota:** Este sistema es compatible con SimHub y otros software de simulación que soporten joysticks HID estándar. 