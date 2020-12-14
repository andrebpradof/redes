#include "../include/wrapper.hpp"

void wrapper::AplicacaoTransmissora(){
    string entrada_usuario;
    cout << "Digite uma mensagem: ";
    getline(cin, entrada_usuario);
    this->mensagem = entrada_usuario;
    CamadaDeAplicacaoTransmissora();
}

void wrapper::CamadaDeAplicacaoTransmissora(){
    cout << endl <<  "CAMADA APLICACAO TRANSMISSORA" << endl;
    cout << endl <<  "REALIZANDO CONVERSAO PARA BINARIO" << endl;
    cout << endl
         << "BINARIO: ";
    string binario;
    for (size_t i = 0; i < this->mensagem.size(); i++){
        bitset<8> bits(this->mensagem.c_str()[i]);
        binario += bits.to_string();
    }
    int *binario_array = (int *)malloc(binario.size() * sizeof(int));
    for (size_t i = 0; i < binario.size(); i++){
        binario_array[i] = (int)binario.at(i) - 48;
        cout << binario_array[i];
    }
    cout << endl;
    this->quadro = binario_array;
    this->quadro_tamanho = binario.size();
    binario_array = nullptr;

    this->camada_enlace = new enlace(this->quadro, this->quadro_tamanho);
    this->camada_enlace->CamadaEnlaceDadosTransmissora();

    //Recuperando quadro da camada de enlace dados transmissora
    this->meio_de_comunicacao = new meioDeComunicacao(this->camada_enlace->quadro, this->camada_enlace->quadro_tamanho);
    this->meio_de_comunicacao->MeioDeComunicacao();

    //Recuperando quadro da camada meioDeComunicacao para recepçãp
    this->camada_enlace->quadro = this->meio_de_comunicacao->quadro;
    this->camada_enlace->quadro_tamanho = this->meio_de_comunicacao->quadro_tamanho;
    this->camada_enlace->CamadaEnlaceDadosReceptora();

    //Recuperando quadro da camada de enlace de recepção para passar para aplicação
    this->quadro = this->camada_enlace->quadro;
    this->quadro_tamanho = this->camada_enlace->quadro_tamanho;

    CamadaDeAplicacaoReceptora();
}

void wrapper::AplicacaoReceptora(){
    cout << endl <<  ">> A mensagem recebida foi: " << this->mensagem << endl;
}

void wrapper::CamadaDeAplicacaoReceptora(){
    cout << endl <<  "---- CAMADA APLICACAO RECEPTORA ----" << endl;
    string bit_string = "";
    string mensagem_final = "";
    for (int i = 0; i < this->quadro_tamanho; i++){
        bit_string += (char)(this->quadro[i] + 48);
    }
    stringstream sstream(bit_string);
    while(sstream.good()) {
        bitset<8> bits;
        sstream >> bits;
        char caractere = char(bits.to_ulong());
        mensagem_final += caractere;
    }
    this->mensagem = mensagem_final;
    AplicacaoReceptora();
}

wrapper::wrapper(){
}

wrapper::~wrapper(){
    delete this->meio_de_comunicacao;
    delete this->camada_enlace;
}