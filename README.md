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

#### Sobre o Código

O código deve acessar o arquivo de dados registro a registro sem nunca carregar o arquivo inteiro em memória.

Assume-se que todos os indexes gerados cabem inteiramente na memória. Quando o index está em memória ele é 
carregado em um map<string, int> para fácil manipulação. Alterações no index em memória devem ser persistidas
em alterações no index em disco.


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

#### Sobre a construção

Os seguintes arquivos são criados por gera_index:

* index_lista1.txt
* sec_lista1.txt
* index_lista2.txt
* index_lista3.txt

Os últimos dois arquivos são indexes da seguinte forma:

  chave primária -> linha no arquivo de dados correspondente onde se encontra o registro
   correspondente a essa chave primária.
   
O segundo arquivo é um índice secundário. Para cada nome existente no conjunto de dados, ele
mantém contém o número da linha do arquivo de índice primário onde você encontrará uma entrada
no índice primário referente a uma chave primária com aquele nome. Essa entrada no índice 
primário(primeiro arquivo da lista) contém os seguintes campos:
Chave primária, linha no arquivo de dados correspondente onde se encontra o registro 
com essa chave primária, linha deste arquivo de índice primário onde se encontra a próxima 
entrada do índice com a chave primária de uma registro que tem o mesmo nome que este.

Tudo isso para dizer que: utiliza-se uma lista ligada para implementar o índice secundário
para nome. O primeiro usuário com um certo nome pode ser lido do arquivo sec_lista2.txt. 
A lista ligada em si está na terceira coluna do arquivo index_lista1.txt.

Inicialmente esse trabalho utilizava o endereço (em bytes) do registro correspondente a uma
chave primária. Apesar se facilitar para o computador, dificultava debugar o código. 
Utilizando-se linha o usuário(programador) pode verificar se a lista invertida está correta
por inspeção visual. Por conta disso, é preciso que ao carregar o índice em memória seja feita 
uma passada pelo arquivo de dados para criar um mapa entre o índice da linha e o byte em
que ele se encontra no arquivo de dados.

Nota: o arquivo secundário contém uma informação para a posição relativa dentro do índice 
primário onde encontrar o primeiro registro com um dado nome. Como o índice primário 
index_lista 1 contém o número de registros deletados na primeira linha, para obter a
linha real em que o registro se encontra deve-se somar 2 ao número que aparece no índice 
secundário.(+1 por conta da linha dedicada ao contador + 1 pq começamos a contar com 0 dentro
do código, mas editores de texto começam a contar de 1).

Para mapear a informação dos índices primários linha do arquivo de dados correspondente
basta somar 1(pq contamos a partir de zero e editores de texto a partir de 1).
