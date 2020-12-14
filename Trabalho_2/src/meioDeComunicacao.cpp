#include "../include/meioDeComunicacao.hpp"

meioDeComunicacao::meioDeComunicacao(int *quadro_inicial, int quadro_tamanho){
    this->quadro = quadro_inicial;
    this->quadro_tamanho = quadro_tamanho;
}

meioDeComunicacao::~meioDeComunicacao(){
    free(this->quadro);
    this->quadro = nullptr;
}

void meioDeComunicacao::MeioDeComunicacao(){

    cout << endl << "---- REALIZANDO TRANSMISSAO PONTO A PARA PONTO B ----" << endl;
    int porcentagem_erros = 0; // Porcentagem de erros da transmissão
    int *fluxoBrutoDeBitsPontoA;
    int *fluxoBrutoDeBitsPontoB;

    fluxoBrutoDeBitsPontoA = this->quadro;
    fluxoBrutoDeBitsPontoB = (int *)malloc(sizeof(int) * this->quadro_tamanho);
    for (int i = 0; i < this->quadro_tamanho; i++){

        if ((rand() % 100 + 1) >= porcentagem_erros){
            fluxoBrutoDeBitsPontoB[i] = fluxoBrutoDeBitsPontoA[i];
        } else{
            fluxoBrutoDeBitsPontoB[i] = fluxoBrutoDeBitsPontoA[i] == 0 ? 1 : 0;
        }
        
    }
    free(fluxoBrutoDeBitsPontoA);
    fluxoBrutoDeBitsPontoA = nullptr;
    this->quadro = fluxoBrutoDeBitsPontoB;
}






