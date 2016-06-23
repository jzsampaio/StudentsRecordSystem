# StudentsRecordSystem

### Universidade de Brasília 1/2016

autor: Juarez Aires Sampaio Filho  contato: juarez.asf@gmail.com

matrícula UnB: 11/0032829

Este projeto está disponível online [aqui](https://github.com/JuarezASF/StudentsRecordSystem "github do autor")


#### Abstract

System developed for the class of **File Organization**.
The system keeps records of students in secondary memory and creates persistent indexes
to allow a operation such as adding, removing,
searching and printing a system report. We keep students' grades for
two classes. Records of each class are stored on separated files.
When asked to print a system report, files must be processed in a 
co-sequential fashion.


#### Resumo

Sistema desenvolvido para a matéria de 'Organização de Arquivos' que
mantém registro de alunos e suas notas em duas disciplinas. Os dados
estão em memória secundária e devem ser acessados sequencialmente.
Criam-se índices para permitir emissão de relatórios e buscas. Os
dados das duas disciplinas estão em arquivos separados e a criação do
relatório do sistema deve agregar esses dados de forma co-sequencial.
O sistema deve permitir operações de adição, remoção e busca sobre os
dados.

#### Instruções para Compilação

##### Estrutura assumida do projeto

Os comandos aqui contidos assumem a seguinte estrutura desse projeto:

    /projectDir
        /build
        /data
        /src
        README.md

notar que projectDir é o diretório contendo este arquivo README.md


##### Linguagem utilizada

    c++ com suporte a c++11

##### Compilador utilizado para desenvovlimento

    g++ (Ubuntu 4.9.3-8ubuntu2~14.04) 4.9.3


#### Comando para Compilação com cmake

    mkdir build
    cd build
    cmake ..
    make

#### Comando para Compilação com make

//TODO
    
#### Comando para Compilação manual

//TODO

#### Instruções para Execução

