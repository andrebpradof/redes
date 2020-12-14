#include "../include/enlace.hpp"

enlace::enlace(int *quadro_inicial, int quadro_tamanho){
    this->quadro = quadro_inicial;
    this->quadro_tamanho = quadro_tamanho;
}

enlace::~enlace(){
    free(this->quadro);
    this->quadro = nullptr;
}

void enlace::CamadaEnlaceDadosTransmissora(){
    cout << endl
         << "---- CAMADA ENLACE TRANSMISSORA ----" << endl;
    CamadaEnlaceDadosTransmissoraControleDeErro();
}

void enlace::CamadaEnlaceDadosReceptora(){
    cout << endl
         << "---- CAMADA ENLACE RECEPTORA ----" << endl;
    CamadaEnlaceDadosReceptoraControleDeErro();
}

void enlace::CamadaEnlaceDadosTransmissoraControleDeErro(){
    int tipoDeControleDeErro = 2; // Alterar de acordo com o teste
    switch (tipoDeControleDeErro){
    case 0:
        CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar();
        break;
    case 1:
        CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar();
        break;
    case 2:
        CamadaEnlaceDadosTransmissoraControleDeErroCRC();
        break;
    }
}

void enlace::CamadaEnlaceDadosReceptoraControleDeErro(){

    int tipoDeControleDeErro = 2; // Alterar de acordo com o teste
    switch (tipoDeControleDeErro)
    {
    case 0:
        CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadePar();
        break;
    case 1:
        CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadeImpar();
        break;
    case 2:
        CamadaEnlaceDadosReceptoraControleDeErroCRC();
        break;
    }
}

void enlace::CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(){
    cout << endl
         << "---- CONTROLE PARIDADE PAR ----" << endl;

    int novo_tamanho = this->quadro_tamanho + 1;
    bool paridade = false;
    int *quadro_codificado = (int *)malloc(sizeof(int) * novo_tamanho);
    cout << endl
         << ">> QUADRO: ";
    for (int i = 0; i < this->quadro_tamanho; i++){
        if (this->quadro[i] == 1){
            paridade = !paridade;
        }
        quadro_codificado[i] = this->quadro[i];
    }
    quadro_codificado[this->quadro_tamanho] = int(paridade);
    for (int i = 0; i < novo_tamanho; i++){
        cout << quadro_codificado[i];
    }
    cout << endl;
    free(this->quadro);
    this->quadro = quadro_codificado;
    this->quadro_tamanho = novo_tamanho;
}

void enlace::CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(){
    cout << endl
         << "---- CONTROLE PARIDADE IMPAR ----" << endl;

    int novo_tamanho = this->quadro_tamanho + 1;
    bool paridade = true;
    int *quadro_codificado = (int *)malloc(sizeof(int) * novo_tamanho);
    cout << endl
         << ">> QUADRO: ";
    for (int i = 0; i < this->quadro_tamanho; i++){
        if (this->quadro[i] == 1){
            paridade = !paridade;
        }
        quadro_codificado[i] = this->quadro[i];
    }
    quadro_codificado[this->quadro_tamanho] = int(paridade);
    for (int i = 0; i < novo_tamanho; i++){
        cout << quadro_codificado[i];
    }
    cout << endl;
    free(this->quadro);
    this->quadro = quadro_codificado;
    this->quadro_tamanho = novo_tamanho;
}

void enlace::CamadaEnlaceDadosTransmissoraControleDeErroCRC(){
    cout << endl
         << ">> CONTROLE CRC 32" << endl;
    // 0x04C11DB7
    // x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
    // 0000 0100 1100 0001 0001 1101 1011 0111
    cout << endl
         << ">> QUADRO: ";
    for (int i = 0; i < this->quadro_tamanho; i++){
        cout << this->quadro[i];
    }
    // Removi os 0's da frente pra facilitar na divisao
    int polinomio[27] = {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};
    cout << endl
         << ">> POLINOMIO: ";
    for (int i = 0; i < 27; i++){
        cout << polinomio[i];
    }

    // Padding de tamanho (grau do polinomio-1)
    int novo_tamanho = this->quadro_tamanho + 31;
    int *quadro_crc = (int *)malloc(sizeof(int) * novo_tamanho);
    for (int i = 0; i < novo_tamanho; i++){
        if (i < this->quadro_tamanho)
            quadro_crc[i] = this->quadro[i];
        else
            quadro_crc[i] = 0;
    }
    cout << endl;

    for (int i = 0; i < this->quadro_tamanho; i++){
        if (quadro_crc[i] == 1)
        {
            //Faz o XOR com todos os elementos do polinomio
            for (int j = 0; j < 27; j++)
            {
                quadro_crc[j + i] ^= polinomio[j];
            }
        }
    }

    for (int i = 0; i < this->quadro_tamanho; i++){
        quadro_crc[i] = this->quadro[i];
    }
    cout << ">> CRC-32: ";
    for (int i = 0; i < novo_tamanho; i++){
        cout << quadro_crc[i];
    }
    cout << endl;

    free(this->quadro);
    this->quadro_tamanho = novo_tamanho;
    this->quadro = quadro_crc;
}


void enlace::CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadePar(){
    cout << endl
         << "---- RECEPCAO PARIDADE PAR ----" << endl;

    int novo_tamanho = this->quadro_tamanho - 1;
    int *quadro_decodificado = (int *)malloc(sizeof(int) * novo_tamanho);
    bool paridade = false;

    cout << endl
         << ">> QUADRO: ";
    for (int i = 0; i < novo_tamanho; i++){
        if (this->quadro[i] == 1){
            paridade = !paridade;
        }
        quadro_decodificado[i] = this->quadro[i];
        cout << quadro_decodificado[i];
    }
    if (this->quadro[novo_tamanho] != int(paridade)){
        cout << endl
             << ">> ERRO NA MENSAGEM" << endl;
        exit(EXIT_FAILURE);
    }
    cout << endl;
    this->quadro = quadro_decodificado;
    this->quadro_tamanho = novo_tamanho;
}

void enlace::CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadeImpar(){
    cout << endl
         << "---- RECEPCAO PARIDADE IMPAR ----" << endl;

    int novo_tamanho = this->quadro_tamanho - 1;
    int *quadro_decodificado = (int *)malloc(sizeof(int) * novo_tamanho);
    bool paridade = true;

    cout << endl
         << ">> QUADRO: ";
    for (int i = 0; i < novo_tamanho; i++){
        if (this->quadro[i] == 1){
            paridade = !paridade;
        }
        quadro_decodificado[i] = this->quadro[i];
        cout << quadro_decodificado[i];
    }
    if (this->quadro[novo_tamanho] != int(paridade)){
        cout << endl
             << ">> ERRO NA MENSAGEM" << endl;
        exit(EXIT_FAILURE);
    }
    cout << endl;
    this->quadro = quadro_decodificado;
    this->quadro_tamanho = novo_tamanho;
}

void enlace::CamadaEnlaceDadosReceptoraControleDeErroCRC(){
    cout << endl
         << "---- RECEPCAO CRC 32 ----" << endl;
    cout << endl
         << ">> QUADRO: ";
    bool quadro_valido = true;

    int tamanho_decodificado = this->quadro_tamanho - 31;
    int *quadro_decodificado = (int *)malloc(sizeof(int) * tamanho_decodificado);
    for (int i = 0; i < tamanho_decodificado; i++){
        quadro_decodificado[i] = this->quadro[i];
    }

    for (int i = 0; i < this->quadro_tamanho; i++){
        cout << this->quadro[i];
    }
    int polinomio[27] = {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};

    cout << endl
         << ">> POLINOMIO: ";
    for (int i = 0; i < 27; i++){
        cout << polinomio[i];
    }

    int *quadro_crc = this->quadro;
    int tamanho_mensagem = this->quadro_tamanho - 31;
    cout << endl;
    for (int i = 0; i < tamanho_mensagem; i++){
        if (quadro_crc[i] == 1){
            for (int j = 0; j < 27; j++){
                quadro_crc[j + i] = quadro_crc[j + i] == polinomio[j] ? 0 : 1;
            }
        }
    }

    cout << ">> RESTO: ";
    for (int i = 0; i < this->quadro_tamanho; i++){
        if (quadro_crc[i] != 0){
            quadro_valido = false;
        }
        cout << quadro_crc[i];
    }
    cout << endl;

    if (quadro_valido){
        this->quadro = quadro_decodificado;
        this->quadro_tamanho = tamanho_decodificado;
    }
    else{
        cout << endl
             << ">> ERRO NA MENSAGEM" << endl;
        exit(EXIT_FAILURE);
    }
}
