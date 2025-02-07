# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detecta o ambiente do usuário para compatibilidade com o VS Code Extension
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# Define a placa como Raspberry Pi Pico W
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

# Define o projeto
project(tarefa C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável e os arquivos fonte
add_executable(tarefa 
    tarefa.c
    ws2812.c
    ssd1306.c
)

# Gera o cabeçalho para PIO
pico_generate_pio_header(tarefa ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# Habilita saída via UART (para Serial Monitor)
pico_enable_stdio_uart(tarefa 1)
pico_enable_stdio_usb(tarefa 0)

# Liga as bibliotecas padrão e periféricos necessários
target_link_libraries(
    tarefa
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_uart
)

# Adiciona diretórios de inclusão
target_include_directories(tarefa PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Gera arquivos extras (UF2, binário, etc.)
pico_add_extra_outputs(tarefa)
