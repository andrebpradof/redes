//////////////////////////////////////////////////////////////
//  SSC0641 - Redes de Computadores (2020)                  //
//  Trabalho 1							                    //
//                                                          //
//  Alunos:                                                 //
//  André Baconcelo Prado Furlanetti - Nº USP: 10748305     //
//	Daniel Bernardes Pozzan 		 - Nº USP: 10716608		//
//  Diego da Silva Parra             - Nº USP: 10716550     //
//  Mateus Fernandes Doimo           - Nº USP: 10691971     //
//	Otávio Cury Pontes 				 - Nº USP: 10716525		//
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORTA 8888	//Definicao da porta utilizada para conexao
 
int random_num(int ini, int fim){	//Funcao que gera um numero inteiro e aleatorio
    int random_number;
    srand((unsigned)time(NULL));
    random_number = (ini + (rand()%fim));
    return random_number;
}

int main(){	//Descricao do funcionamento do cliente (captador de batimentos cardiacos)

	int clientSocket;	//Socket do cliente
	int ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	char bat[4];	//Variavel que armazena o numero de batimentos

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);	//Criacao do socket do cliente
	if(clientSocket < 0){	//Tratamento para caso de erro na criacao do socket do cliente
		printf("Erro na conexão.\n");
		exit(1);
	}
	printf("Socket do cliente foi criado.\n");	//Validacao da criacao do socket

	memset(&serverAddr, '\0', sizeof(serverAddr));	//Seta a variavel serverAddr com \0
	//Definicao do endereco de destino
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORTA);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	printf("\nNome do paciente: ");
	scanf("%s", buffer);	//Buffer armazena um identificador do cliente (que pode ser um nome)

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));	//Inicia uma conexao socket do lado do cliente
	if(ret < 0){	//Tratamento para caso de erro na conexao do socket
		printf("Erro na conexão.\n");
		exit(1);
	}
	printf("Conectado com o servidor.\n");	//Validacao da conexao do cliente com o servidor

	send(clientSocket, buffer, strlen(buffer), 0);	//Envia a mensagem do cliente (seu identificador) para o servidor

	while(1){	//Loop para o envio de novas informacoes ao servidor	
		int bat_num = random_num(40,160);	//Batimento cardiaco recebe um numero aleatorio entre 40 e 160
		sprintf(bat,"%d",bat_num);	//Transformar o numero inteiro (batimentos) em uma string
		sleep(random_num(3,8));	//Funcao de pausa 
		strcpy(buffer,bat);	//Buffer recebe o numero de batimentos em formato string
		send(clientSocket, buffer, strlen(buffer), 0);	//Envio do buffer (numero de batimentos) para o servidor

		if(strcmp(buffer, ":sair") == 0){	//Se o cliente desejar sair, ele eh desconectado do servidor
			close(clientSocket);
			printf("Desconectado do servidor.\n");
			exit(1);
		}
		bzero(buffer, sizeof(buffer));	//Limpeza do buffer
		if(recv(clientSocket, buffer, 1024, 0) < 0){	//Tratamento para caso de erro no recebimento de dados por parte do servidor
			printf("Erro no recebimento de dados.\n");
		}else{
			printf("Status: \t%s\n", buffer);
		}
	}
}
