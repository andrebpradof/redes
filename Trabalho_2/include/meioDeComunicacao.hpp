#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

class meioDeComunicacao{
public:
    //Atributo que guarda referencia do quadro a ser utilizado em cada passo.
    int* quadro;
    //Atributo que guarda tamanho do quadro, para que seja possivel iterar sobre este
    int quadro_tamanho;



    /**
     * Simula a comunicação entre um ponto A e B. Por questão
     * de simplificação, a comunicação é realizada dentro da própria
     * classe instanciada, realizando chamadas de métodos
     */
    void MeioDeComunicacao();

    /**
     *  Construtor da classe meioDeComunicacao, recebe o quadro incial como 
     *  parametro
     */
    meioDeComunicacao(int* quadro, int quadro_tamanho);

    /**
     *  Destrutor da classe meioDeComunicacao, libera espaços de memória alocados
     *  ao decorrer do processo
     */
    ~meioDeComunicacao();
};

