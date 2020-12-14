#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include "./enlace.hpp"
#include "./meioDeComunicacao.hpp"

using namespace std;

class wrapper{
private:
    //Atributo que guarda mensagem inicial ou final da simulação
    string mensagem;
    //Atributo que guarda referencia do meioDeComunicacao
    meioDeComunicacao *meio_de_comunicacao;
    //Atributo que guarda referencia da camada de enlace
    enlace *camada_enlace;

public:
    //Guarda quadro inicial antes dos processos de codificação
    int *quadro;
    //Guarda tamanho do quadro inicial antes dos processos de codificação
    int quadro_tamanho;

    /**
     *  Simula recebimento de mensagem pela camada de aplicação
     */
    void AplicacaoTransmissora();

    /**
     *  Transforma mensagem de ASCII chars para binário
     */
    void CamadaDeAplicacaoTransmissora();

    /**
     *  Recebe mensagem ao final de todo processo de transmissão,
     *  simulando aplicação receptora
     */
    void AplicacaoReceptora();

    /**
     *  Transforma o fluxo de binário ao final da transmissão,
     *  para string novamente.
     */
    void CamadaDeAplicacaoReceptora();

    /**
     *  Construtora da classe wrapper.
     */
    wrapper();


    /**
     *  Destrutor da classe wrapper, desreferencia e desaloca as memorias
     *  utilizadas.
     */
    ~wrapper();
};