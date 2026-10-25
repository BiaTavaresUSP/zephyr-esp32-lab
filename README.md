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

## 📁 Estr
