# 🔬 Laboratório Zephyr RTOS — ESP32-S3

Repositório dedicado ao desenvolvimento e à experimentação com sistemas embarcados utilizando **Zephyr RTOS** e a placa **ESP32-S3 DevKitC**.

O projeto tem como objetivo estudar o desenvolvimento de aplicações embarcadas em C, explorar mecanismos de concorrência e tratamento de eventos e comparar diferentes estratégias de execução, incluindo polling, interrupções, threads e workqueues.

A organização do repositório separa os experimentos principais de aplicações independentes, facilitando a manutenção, a compilação e a evolução do código.

---

## 📑 Sumário

- [Visão geral](#-visão-geral)
- [Estrutura do repositório](#-estrutura-do-repositório)
- [Projetos](#-projetos)
- [Tecnologias utilizadas](#-tecnologias-utilizadas)
- [Requisitos](#-requisitos)
- [Configuração do ambiente](#-configuração-do-ambiente)
- [Compilação](#-compilação)
- [Gravação na placa](#-gravação-na-placa)
- [Configuração de hardware](#-configuração-de-hardware)
- [Organização do código](#-organização-do-código)
- [Fluxo de desenvolvimento](#-fluxo-de-desenvolvimento)
- [Objetivos de aprendizagem](#-objetivos-de-aprendizagem)

---

## 🎯 Visão geral

O Zephyr RTOS é um sistema operacional de tempo real voltado para dispositivos embarcados e aplicações com recursos limitados.

Neste repositório, são desenvolvidas aplicações para explorar funcionalidades do sistema operacional e compreender como diferentes mecanismos de execução influenciam o comportamento de um sistema embarcado.

Os principais objetivos são:

- Desenvolver aplicações embarcadas utilizando a linguagem C.
- Compreender a configuração de dispositivos por meio do DeviceTree.
- Trabalhar com GPIOs e periféricos da ESP32-S3.
- Estudar interrupções e mecanismos de sincronização.
- Utilizar threads e workqueues.
- Explorar estratégias de tratamento de eventos.
- Estruturar aplicações independentes dentro de um mesmo repositório.
- Utilizar ferramentas de compilação e gerenciamento de projetos do ecossistema Zephyr.

---

## 📁 Estrutura do repositório

O repositório está organizado em dois projetos independentes:

```text
zephyr-esp32-lab/
│
├── experimentos/
│   ├── CMakeLists.txt
│   ├── Kconfig
│   ├── prj.conf
│   ├── west.yml
│   ├── esp32s3_test.overlay
│   │
│   ├── overlays/
│   │   ├── app.overlay
│   │   ├── flash16.overlay
│   │   └── usb_console.overlay
│   │
│   └── src/
│       ├── main.c
│       ├── bench.c
│       ├── bench.h
│       ├── mode_polling.c
│       ├── mode_isr_only.c
│       ├── mode_irq_thread.c
│       └── mode_workqueue.c
│
└── blink-test/
    ├── CMakeLists.txt
    ├── app.overlay
    ├── prj.conf
    │
    └── src/
        └── main.c
```

### `experimentos/`

Contém a aplicação principal, os módulos de experimentação, as configurações do Zephyr e os overlays necessários para configurar os dispositivos utilizados.

### `blink-test/`

Contém uma aplicação independente para controlar um LED RGB endereçável compatível com WS2812.

Cada projeto possui seu próprio `CMakeLists.txt`, arquivo `prj.conf` e configuração de hardware. Isso permite compilar as aplicações separadamente.

---

## 🧪 Projetos

### 1. Experimentos de execução e tratamento de eventos

**Diretório:** `experimentos/`

Este projeto reúne diferentes abordagens para tratar eventos em sistemas embarcados.

A estrutura permite estudar estratégias de execução e comparar suas características em termos de organização do código, utilização de recursos e comportamento temporal.

#### Polling

Arquivo: `src/mode_polling.c`

O polling consiste em consultar periodicamente o estado de uma entrada ou condição.

Nesse modelo, o programa verifica repetidamente se existe algum evento a ser tratado.

**Características:**
- Implementação simples.
- Controle explícito da frequência de consulta.
- Possível consumo desnecessário de processamento quando não há eventos.
- Tempo de resposta relacionado ao intervalo entre consultas.

#### ISR — Interrupt Service Routine

Arquivo: `src/mode_isr_only.c`

Uma ISR é uma rotina executada em resposta a uma interrupção.

Em vez de consultar continuamente uma entrada, o sistema pode reagir a um evento sinalizado pelo hardware.

**Características:**
- Resposta orientada a eventos.
- Possibilidade de reduzir a necessidade de consultas periódicas.
- Necessidade de manter a rotina de interrupção curta.
- Restrições quanto às operações permitidas no contexto de interrupção.

#### Interrupção com thread

Arquivo: `src/mode_irq_thread.c`

Nesse modelo, uma interrupção sinaliza a ocorrência de um evento, enquanto uma thread realiza o processamento associado.

Essa separação permite manter a rotina de interrupção curta e transferir o trabalho para um contexto de execução apropriado.

**Características:**
- Separação entre detecção e processamento.
- Possibilidade de executar operações que não são apropriadas dentro de uma ISR.
- Necessidade de sincronização entre os contextos.
- Comportamento dependente das prioridades e do escalonamento das threads.

#### Workqueue

Arquivo: `src/mode_workqueue.c`

Uma workqueue permite encaminhar tarefas para processamento posterior por uma fila de trabalho.

Esse mecanismo é útil quando uma interrupção ou outro contexto precisa solicitar uma operação sem executar todo o processamento imediatamente.

**Características:**
- Separação entre solicitação e execução do trabalho.
- Reutilização dos mecanismos de processamento do Zephyr.
- Necessidade de considerar o contexto e o tempo de execução das tarefas.
- Possibilidade de comparar o comportamento dessa abordagem com threads dedicadas.

#### Benchmark

Arquivos:
- `src/bench.c`
- `src/bench.h`

Esses arquivos dão suporte à organização das medições e comparações dos experimentos.

As métricas efetivamente disponíveis dependem da implementação do código e da configuração utilizada em cada experimento.

---

### 2. Teste de LED RGB WS2812

**Diretório:** `blink-test/`

Esta aplicação é independente do projeto principal e tem como objetivo controlar um LED RGB endereçável compatível com WS2812.

O programa foi estruturado para alternar entre quatro estados:

1. Vermelho.
2. Verde.
3. Azul.
4. Apagado.

Cada estado permanece ativo por aproximadamente um segundo.

#### Arquivos principais

| Arquivo | Responsabilidade |
|---|---|
| `CMakeLists.txt` | Define a aplicação para o sistema de compilação do Zephyr. |
| `prj.conf` | Habilita as opções de configuração necessárias. |
| `app.overlay` | Configura o dispositivo RGB e os recursos de hardware utilizados. |
| `src/main.c` | Implementa a sequência de cores. |

O overlay configura a interface RMT para gerar os sinais utilizados pelo LED, com saída associada ao GPIO48.

A compatibilidade elétrica, a pinagem física e a configuração do dispositivo devem ser verificadas de acordo com o hardware utilizado.

---

## 🛠️ Tecnologias utilizadas

| Tecnologia | Aplicação |
|---|---|
| C | Desenvolvimento das aplicações embarcadas. |
| Zephyr RTOS | Sistema operacional de tempo real. |
| ESP32-S3 | Plataforma de hardware. |
| GPIO | Leitura e controle de sinais digitais. |
| Interrupções | Detecção de eventos de hardware. |
| Threads | Execução de tarefas concorrentes. |
| Workqueues | Processamento assíncrono de tarefas. |
| DeviceTree | Descrição e configuração dos dispositivos. |
| CMake | Configuração do sistema de compilação. |
| Ninja | Execução das etapas de compilação. |
| west | Gerenciamento do workspace e das operações de build e flash. |
| Git | Controle de versão. |
| GitHub | Hospedagem do repositório e colaboração. |

---

## 💻 Requisitos

Para utilizar os projetos, é necessário ter:

- Linux.
- Placa ESP32-S3 DevKitC compatível com o alvo selecionado.
- Zephyr RTOS instalado e configurado.
- Python e ambiente virtual com as dependências necessárias.
- Ferramenta `west`.
- CMake.
- Ninja.
- Toolchain compatível com o Zephyr.
- Cabo USB adequado para conectar a placa ao computador.

As instruções a seguir pressupõem que o workspace Zephyr já esteja configurado no diretório `~/zephyrproject`.

---

## ⚙️ Configuração do ambiente

Entre na pasta do repositório:

```bash
cd ~/zephyrproject/zephyr-esp32-lab-main
```

Ative o ambiente virtual utilizado no desenvolvimento:

```bash
source ~/zephyrproject/.venv/bin/activate
```

Verifique se o `west` está disponível:

```bash
west --version
```

Verifique se a variável de ambiente `ZEPHYR_BASE` está configurada:

```bash
echo "$ZEPHYR_BASE"
```

Caso o workspace tenha sido configurado de outra forma, ajuste os caminhos de acordo com a instalação local.

---

## 🏗️ Compilação

O alvo utilizado nos comandos deste repositório é:

`esp32s3_devkitc/esp32s3/procpu`

Os projetos devem ser compilados separadamente, cada um com seu próprio diretório de build.

### Compilar os experimentos

Na raiz do repositório, execute:

```bash
west build -p always \
  -b esp32s3_devkitc/esp32s3/procpu \
  experimentos \
  -d build-experimentos \
  -- -DDTC_OVERLAY_FILE="$PWD/experimentos/esp32s3_test.overlay"
```

O comando seleciona o projeto `experimentos`, define o alvo da ESP32-S3 e informa explicitamente o overlay principal.

### Compilar o teste RGB

Execute:

```bash
west build -p always \
  -b esp32s3_devkitc/esp32s3/procpu \
  blink-test \
  -d build-blink
```

A aplicação `blink-test` possui seu próprio overlay, localizado em `blink-test/app.overlay`.

### Por que utilizar diretórios de build separados?

Os diretórios `build-experimentos` e `build-blink` mantêm separados os arquivos gerados durante a configuração e a compilação.

Essa organização evita misturar artefatos de aplicações diferentes e facilita a manutenção dos projetos.

> Os comandos de compilação documentam o procedimento esperado. A compilação de cada projeto deve ser considerada validada somente quando executada com sucesso no ambiente correspondente.

---

## 🔌 Gravação na placa

Depois de compilar a aplicação desejada, conecte a placa ao computador e verifique se o runner de gravação está configurado corretamente.

### Gravar os experimentos

```bash
west flash -d build-experimentos
```

### Gravar o teste RGB

```bash
west flash -d build-blink
```

A gravação depende da configuração do runner, da conexão USB e das condições da placa.

A compilação e a gravação são etapas distintas: uma compilação bem-sucedida não garante, por si só, que a aplicação foi gravada ou executada no hardware.

---

## 🔧 Configuração de hardware

### GPIOs dos experimentos

O overlay `experimentos/esp32s3_test.overlay` declara os seguintes sinais:

| Sinal | GPIO | Função prevista |
|---|---:|---|
| LED de saída | GPIO2 | Sinalização de saída. |
| Botão/entrada | GPIO4 | Entrada para eventos. |
| Estímulo | GPIO18 | Geração de estímulos para os experimentos. |

A função efetiva de cada sinal depende da implementação da aplicação e da configuração de hardware.

### LED RGB

No projeto `blink-test`, o overlay configura a saída RMT associada ao GPIO48 para controlar o LED RGB.

Antes de conectar periféricos externos, confira a pinagem da placa, a alimentação, o aterramento comum e os requisitos elétricos do componente.

---

## 🗂️ Organização do código

A estrutura do repositório segue o princípio de manter aplicações independentes e configurações específicas por projeto.

### `CMakeLists.txt`

Define como o projeto é integrado ao sistema de compilação do Zephyr e quais arquivos de código-fonte fazem parte da aplicação.

### `prj.conf`

Contém as opções de configuração do Zephyr habilitadas para a aplicação.

### `Kconfig`

Permite definir opções de configuração específicas do projeto principal.

### Overlays

Os arquivos `.overlay` complementam a descrição de hardware fornecida pelo DeviceTree, habilitando ou modificando dispositivos e suas propriedades.

### Código-fonte

Os arquivos `.c` contêm as implementações das aplicações e dos módulos experimentais.

O arquivo `bench.h` declara interfaces compartilhadas com o módulo de benchmark.

---

## 🔄 Fluxo de desenvolvimento

O fluxo recomendado para trabalhar no repositório é:

1. Selecionar o projeto que será modificado.
2. Alterar o código ou os arquivos de configuração correspondentes.
3. Compilar o projeto em seu diretório de build.
4. Analisar eventuais erros de compilação ou configuração.
5. Gravar a aplicação na placa quando necessário.
6. Registrar as alterações no Git.
7. Enviar as alterações para uma branch no GitHub.
8. Documentar novas funcionalidades e mudanças relevantes.

Para manter a organização, evite compartilhar o mesmo diretório de build entre aplicações independentes.

---

## 📚 Objetivos de aprendizagem

Este laboratório busca desenvolver conhecimentos práticos sobre:

- Programação em C para sistemas embarcados.
- Fundamentos de sistemas operacionais de tempo real.
- Tratamento de eventos e interrupções.
- Concorrência e escalonamento.
- Comunicação entre contextos de execução.
- Configuração de periféricos por DeviceTree.
- Integração de aplicações com o Zephyr RTOS.
- Compilação e gravação de firmware.
- Organização de projetos e controle de versão.
- Análise comparativa de estratégias de execução.

---

## 👩‍💻 Autoria

Projeto de estudos e experimentação com sistemas embarcados, desenvolvido com foco no Zephyr RTOS e na plataforma ESP32-S3.

**Repositório:** `zephyr-esp32-lab`
