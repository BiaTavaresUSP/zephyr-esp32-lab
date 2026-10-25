# 🔬 Laboratório Zephyr RTOS — ESP32-S3

Repositório dedicado ao desenvolvimento e à experimentação com sistemas embarcados utilizando **Zephyr RTOS** e a placa **ESP32-S3 DevKitC**.

O projeto tem como objetivo estudar o desenvolvimento de aplicações embarcadas em C, explorar mecanismos de concorrência e tratamento de eventos e comparar diferentes estratégias de execução, incluindo polling, interrupções, threads e workqueues.

O repositório está organizado em dois projetos independentes:

- **`experimentos/`** — aplicação de benchmark e experimentação sem controle do LED RGB.
- **`exp_8/`** — versão independente que preserva o controle do LED RGB endereçável WS2812.

Essa separação permite comparar implementações, testar funcionalidades e manter configurações específicas para cada aplicação.

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
- [Monitor serial](#-monitor-serial)
- [Configuração de hardware](#-configuração-de-hardware)
- [Organização do código](#-organização-do-código)
- [Fluxo de desenvolvimento](#-fluxo-de-desenvolvimento)
- [Objetivos de aprendizagem](#-objetivos-de-aprendizagem)
- [Autoria](#-autoria)

---

## 🎯 Visão geral

O Zephyr RTOS é um sistema operacional de tempo real voltado para dispositivos embarcados, com suporte a diferentes arquiteturas e periféricos.

Neste laboratório, são desenvolvidas aplicações para explorar funcionalidades do sistema operacional e compreender como diferentes mecanismos de execução influenciam o comportamento de sistemas embarcados.

Os principais objetivos são:

- Desenvolver aplicações embarcadas utilizando a linguagem C.
- Compreender a configuração de dispositivos por meio do DeviceTree.
- Trabalhar com GPIOs e periféricos da ESP32-S3.
- Estudar interrupções e mecanismos de sincronização.
- Utilizar threads e workqueues.
- Explorar diferentes estratégias de tratamento de eventos.
- Realizar benchmarks para analisar o comportamento das aplicações.
- Comparar implementações com diferentes configurações de hardware.
- Utilizar ferramentas de compilação e gerenciamento de projetos do ecossistema Zephyr.
- Organizar aplicações independentes dentro de um mesmo repositório.

---

## 📁 Estrutura do repositório

O repositório possui dois projetos independentes, cada um com seus próprios arquivos de configuração e código-fonte.

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
└── exp_8/
    ├── CMakeLists.txt
    ├── Kconfig
    ├── prj.conf
    ├── west.yml
    ├── esp32s3_test.overlay
    │
    ├── overlays/
    │   ├── app.overlay
    │   ├── flash16.overlay
    │   └── usb_console.overlay
    │
    └── src/
        ├── main.c
        ├── bench.c
        ├── bench.h
        ├── mode_polling.c
        ├── mode_isr_only.c
        ├── mode_irq_thread.c
        └── mode_workqueue.c
```

*Observação: a árvore acima destaca os arquivos principais conhecidos. Arquivos adicionais podem existir em cada diretório.*

### `experimentos/`

Contém a aplicação utilizada nos experimentos de execução e tratamento de eventos.

Essa versão foi modificada para funcionar sem o controle do LED RGB WS2812, permitindo concentrar a análise no processamento dos eventos e nas métricas do benchmark.

O projeto mantém os módulos de polling, interrupção, thread e workqueue, além dos arquivos de configuração e do overlay de hardware.

### `exp_8/`

Contém uma versão independente da aplicação original, preservando o controle do LED RGB endereçável WS2812.

O projeto mantém os arquivos de configuração necessários para habilitar o periférico e controlar a saída utilizada pelo LED.

Essa versão permite testar o comportamento da aplicação com o controle do LED RGB habilitado, independentemente das modificações realizadas em `experimentos/`.

### Separação dos projetos

Cada aplicação possui seus próprios arquivos de configuração, código-fonte e diretório de compilação.

Essa organização facilita a manutenção, permite compilar os projetos separadamente e reduz o risco de misturar configurações e artefatos gerados durante o desenvolvimento.

---

## 🧪 Projetos

### 1. Experimentos de execução e tratamento de eventos

**Diretório:** `experimentos/`

O projeto reúne diferentes abordagens para tratar eventos em sistemas embarcados.

A proposta é estudar como cada estratégia organiza a execução, responde aos eventos e utiliza os recursos do sistema.

#### Polling

**Arquivo:** `src/mode_polling.c`

O polling consiste em consultar periodicamente o estado de uma entrada ou condição.

Nesse modelo, o programa verifica repetidamente se existe algum evento a ser tratado.

**Características:**

- Implementação relativamente simples.
- Controle explícito da frequência de consulta.
- Possível consumo desnecessário de processamento quando não há eventos.
- Tempo de resposta relacionado ao intervalo entre consultas.

#### ISR — Interrupt Service Routine

**Arquivo:** `src/mode_isr_only.c`

Uma ISR é uma rotina executada em resposta a uma interrupção.

Em vez de consultar continuamente uma entrada, o sistema pode reagir a um evento sinalizado pelo hardware.

**Características:**

- Execução orientada a eventos.
- Possibilidade de reduzir consultas periódicas.
- Necessidade de manter a rotina de interrupção curta.
- Restrições quanto às operações permitidas no contexto de interrupção.

#### Interrupção com thread

**Arquivo:** `src/mode_irq_thread.c`

Nesse modelo, uma interrupção sinaliza a ocorrência de um evento, enquanto uma thread realiza o processamento associado.

Essa separação permite manter a rotina de interrupção curta e transferir o processamento para um contexto de execução apropriado.

**Características:**

- Separação entre detecção e processamento.
- Possibilidade de executar operações que não são apropriadas dentro de uma ISR.
- Necessidade de sincronização entre os contextos.
- Comportamento dependente das prioridades e do escalonamento das threads.

#### Workqueue

**Arquivo:** `src/mode_workqueue.c`

Uma workqueue permite encaminhar tarefas para processamento posterior por uma fila de trabalho.

Esse mecanismo é útil quando uma interrupção ou outro contexto precisa solicitar uma operação sem executar todo o processamento imediatamente.

**Características:**

- Separação entre solicitação e execução do trabalho.
- Utilização dos mecanismos de processamento do Zephyr.
- Necessidade de considerar o contexto e o tempo de execução das tarefas.
- Possibilidade de comparar essa abordagem com o uso de threads dedicadas.

#### Benchmark

**Arquivos:**

- `src/bench.c`
- `src/bench.h`

Esses arquivos dão suporte à organização das medições e dos experimentos.

As métricas disponíveis dependem da implementação e da configuração utilizada. Entre os indicadores que podem ser analisados estão:

- Quantidade de eventos gerados.
- Quantidade de eventos detectados.
- Quantidade de eventos processados.
- Quantidade de eventos perdidos.
- Utilização de CPU, quando medida pela implementação.
- Diferenças de comportamento entre as estratégias de execução.

A interpretação dos resultados deve considerar as condições do experimento, os intervalos de medição e os mecanismos efetivamente implementados.

---

### 2. Aplicação com controle de LED RGB WS2812

**Diretório:** `exp_8/`

Esse projeto preserva a implementação original do controle do LED RGB endereçável WS2812.

O controle é realizado por meio da configuração de hardware do Zephyr, incluindo o periférico RMT da ESP32-S3.

A sequência de cores e os intervalos de atualização dependem da implementação presente no código-fonte.

#### Arquivos principais

| Arquivo | Responsabilidade |
|---|---|
| `CMakeLists.txt` | Define a aplicação para o sistema de compilação do Zephyr. |
| `Kconfig` | Define opções de configuração específicas do projeto, quando utilizadas. |
| `prj.conf` | Contém as opções de configuração do Zephyr. |
| `esp32s3_test.overlay` | Define configurações específicas de hardware. |
| `overlays/app.overlay` | Complementa a configuração de hardware da aplicação. |
| `overlays/flash16.overlay` | Contém configurações relacionadas à memória flash, quando selecionado. |
| `overlays/usb_console.overlay` | Contém configurações relacionadas ao console USB, quando selecionado. |
| `src/main.c` | Implementa a inicialização e o fluxo principal da aplicação. |
| `src/bench.c` | Implementa funcionalidades relacionadas ao benchmark. |
| `src/bench.h` | Declara interfaces compartilhadas do benchmark. |
| `src/mode_polling.c` | Implementação da estratégia de polling. |
| `src/mode_isr_only.c` | Implementação baseada em interrupção. |
| `src/mode_irq_thread.c` | Implementação com interrupção e thread. |
| `src/mode_workqueue.c` | Implementação com workqueue. |

A configuração do LED RGB deve ser consultada nos arquivos de configuração e no código da própria aplicação.

**Importante:** o funcionamento do LED depende da compatibilidade entre a configuração do DeviceTree, o periférico utilizado, a pinagem física e o hardware conectado.

---

## 🛠️ Tecnologias utilizadas

| Tecnologia | Aplicação |
|---|---|
| C | Desenvolvimento das aplicações embarcadas. |
| Zephyr RTOS | Sistema operacional de tempo real. |
| ESP32-S3 | Plataforma de hardware. |
| GPIO | Leitura e controle de sinais digitais. |
| RMT | Geração de sinais para periféricos compatíveis. |
| Interrupções | Detecção de eventos de hardware. |
| Threads | Execução de tarefas concorrentes. |
| Workqueues | Processamento assíncrono de tarefas. |
| DeviceTree | Descrição e configuração dos dispositivos. |
| CMake | Configuração do sistema de compilação. |
| Ninja | Execução das etapas de compilação. |
| west | Gerenciamento do workspace e das operações de build e flash. |
| Python | Suporte às ferramentas do ambiente Zephyr. |
| Git | Controle de versão. |
| GitHub | Hospedagem e gerenciamento do código-fonte. |

---

## 💻 Requisitos

Para compilar e executar os projetos, é necessário ter:

- Linux.
- Placa ESP32-S3 DevKitC compatível com o alvo selecionado.
- Workspace Zephyr configurado.
- Python e ambiente virtual com as dependências necessárias.
- Ferramenta `west`.
- CMake.
- Ninja.
- Toolchain compatível com o Zephyr.
- Cabo USB adequado para conectar a placa ao computador.
- Permissões de acesso à interface USB utilizada pela placa.

Os comandos deste README pressupõem que o workspace esteja configurado no diretório `~/zephyrproject` e que o repositório esteja em:

```text
~/zephyrproject/zephyr-esp32-lab-main
```

Caso a instalação utilize outros caminhos, ajuste os comandos de acordo com o ambiente local.

---

## ⚙️ Configuração do ambiente

### 1. Acessar o repositório

```bash
cd ~/zephyrproject/zephyr-esp32-lab-main
```

### 2. Ativar o ambiente virtual

```bash
source ~/zephyrproject/.venv/bin/activate
```

### 3. Verificar a instalação do west

```bash
west --version
```

### 4. Verificar a variável ZEPHYR_BASE

```bash
echo "$ZEPHYR_BASE"
```

A variável deve apontar para a instalação do Zephyr utilizada pelo workspace.

Se o ambiente já estiver configurado corretamente, será possível executar os comandos de compilação descritos a seguir.

---

## 🏗️ Compilação

O alvo utilizado nos comandos deste repositório é:

```text
esp32s3_devkitc/esp32s3/procpu
```

Cada aplicação deve ser compilada separadamente e utilizar seu próprio diretório de build.

Os comandos abaixo devem ser executados a partir da raiz do repositório.

### 1. Compilar `experimentos/`

```bash
west build -p always \
  -b esp32s3_devkitc/esp32s3/procpu \
  experimentos \
  -d build-experimentos \
  -- -DDTC_OVERLAY_FILE="$PWD/experimentos/esp32s3_test.overlay"
```

Esse comando compila a versão sem controle do LED RGB, utilizando o overlay específico do projeto.

### 2. Compilar `exp_8/`

```bash
west build -p always \
  -b esp32s3_devkitc/esp32s3/procpu \
  exp_8 \
  -d build-exp-8 \
  -- -DDTC_OVERLAY_FILE="$PWD/exp_8/esp32s3_test.overlay"
```

Esse comando compila a versão que preserva o controle do LED RGB.

### Por que utilizar diretórios de build separados?

Os diretórios são organizados da seguinte forma:

| Projeto | Diretório de build |
|---|---|
| `experimentos/` | `build-experimentos/` |
| `exp_8/` | `build-exp-8/` |

A separação evita misturar arquivos gerados durante a configuração e a compilação de aplicações diferentes.

A opção `-p always` solicita uma reconstrução limpa da configuração de build antes da compilação.

**Importante:** a compilação só deve ser considerada bem-sucedida quando o comando terminar sem erros.

---

## 🔌 Gravação na placa

Depois de compilar a aplicação desejada, conecte a ESP32-S3 ao computador.

### Gravar `experimentos/`

```bash
west flash -d build-experimentos
```

### Gravar `exp_8/`

```bash
west flash -d build-exp-8
```

A gravação depende da configuração do runner, da conexão USB e das condições da placa.

A compilação e a gravação são etapas distintas: uma compilação bem-sucedida não garante, por si só, que o firmware foi gravado ou executado corretamente no hardware.

---

## 🖥️ Monitor serial

O monitor serial permite acompanhar as mensagens impressas pela aplicação durante sua execução.

No ambiente utilizado no desenvolvimento, a placa é acessada pela interface `/dev/ttyACM0`, com velocidade de comunicação de 115200 baud.

### Abrir o Minicom

```bash
minicom -D /dev/ttyACM0 -b 115200
```

Depois de abrir o terminal, reinicie a placa, se necessário, para observar as mensagens de inicialização e os resultados dos experimentos.

### Encerrar o Minicom

Pressione:

1. `Ctrl + A`
2. Em seguida, `X`.
3. Confirme a saída quando solicitado.

Se a interface `/dev/ttyACM0` não estiver disponível, verifique a conexão USB, as permissões de acesso e o dispositivo serial associado à placa.

A interface serial pode variar conforme a configuração do console e a forma como a placa está conectada.

---

## 🔧 Configuração de hardware

### GPIOs dos experimentos

O arquivo `experimentos/esp32s3_test.overlay` contém a configuração específica de hardware utilizada pela aplicação.

Os sinais previstos na configuração desenvolvida são:

| Sinal | GPIO | Função prevista |
|---|---:|---|
| Botão/entrada | GPIO4 | Entrada para eventos. |
| Estímulo | GPIO18 | Geração de estímulos para os experimentos. |

A configuração efetiva deve ser conferida no overlay e no código-fonte correspondente.

A versão `experimentos/` foi modificada para não controlar o LED RGB WS2812.

### LED RGB WS2812

No projeto `exp_8/`, a configuração original do LED RGB foi preservada.

A saída RMT está associada ao GPIO48 na configuração utilizada pelo projeto.

Antes de conectar periféricos externos, confira:

- A pinagem da placa.
- A alimentação necessária para o LED.
- O aterramento comum, quando aplicável.
- Os requisitos elétricos do componente.
- A compatibilidade entre a configuração do DeviceTree e o hardware.

O funcionamento depende da configuração completa do projeto e do hardware efetivamente utilizado.

---

## 🗂️ Organização do código

### `CMakeLists.txt`

Define como a aplicação é integrada ao sistema de compilação do Zephyr e quais arquivos de código-fonte fazem parte do projeto.

### `prj.conf`

Contém as opções de configuração do Zephyr habilitadas para a aplicação.

### `Kconfig`

Permite definir opções de configuração específicas do projeto, quando utilizadas.

### Arquivos `.overlay`

Complementam a descrição de hardware fornecida pelo DeviceTree, habilitando ou modificando dispositivos e suas propriedades.

### Arquivos `.c`

Contêm as implementações das aplicações e dos módulos experimentais.

### Arquivo `bench.h`

Declara interfaces compartilhadas com o módulo de benchmark.

### Arquivo `west.yml`

Contém informações de configuração relacionadas ao gerenciamento de dependências e ao workspace, conforme a implementação de cada projeto.

---

## 🔄 Fluxo de desenvolvimento

O fluxo recomendado para trabalhar no repositório é:

1. Selecionar o projeto que será modificado.
2. Alterar o código ou os arquivos de configuração correspondentes.
3. Compilar o projeto em seu próprio diretório de build.
4. Analisar eventuais erros de compilação ou configuração.
5. Gravar a aplicação na placa quando necessário.
6. Acompanhar a execução pelo monitor serial.
7. Analisar os resultados dos experimentos.
8. Registrar as alterações no Git.
9. Enviar as alterações para a branch apropriada no GitHub.
10. Documentar novas funcionalidades e mudanças relevantes.

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
- Utilização de periféricos da ESP32-S3.
- Organização de projetos e controle de versão.
- Análise comparativa de estratégias de execução.
- Monitoramento e interpretação de resultados experimentais.

---

## 👩‍💻 Autoria

Projeto de estudos e experimentação com sistemas embarcados, desenvolvido com foco no Zephyr RTOS e na plataforma ESP32-S3 DevKitC.

**Repositório:** `zephyr-esp32-lab`
