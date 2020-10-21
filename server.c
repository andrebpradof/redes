#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORTA 8888

int main(){	//Descricao do funcionamento do servidor

	int sockfd;	//Socket do servidor
	int ret;
	struct sockaddr_in serverAddr;
	time_t rawtime;
	int newSocket;	//Novo socket
	struct sockaddr_in newAddr;
	struct tm * timeinfo;	
	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//Socket do servidor
	if(sockfd < 0){	//Tratamento de erro para falha na criacao do socket
		printf("Erro na conexão.\n");
		exit(1);
	}
	printf("Socket do servidor foi criado.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));	//Setar a variavel serverAddr com \0
	//Definicao do endereco de destino
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORTA);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));	//Associa o socket criado a porta local do sistema operacional
	if(ret < 0){	//Tratamento para caso de erro no bind
		printf("Erro em bind.\n");
		exit(1);
	}
	printf("Bind para a porta %d\n", 8888);	//Atribuicao do bind para a porta requerida

	if(listen(sockfd, 10) == 0){	//Confere se o servidor esta ouvindo as requisicoes dos clientes
		printf("Ouvindo....\n");
	}else{
		printf("Erro em binding.\n");
	}

	while(1){	//Loop para a criacao de novos sockets para os novos clientes
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);	//Criacao do novo socket
		if(newSocket < 0){	//Tratamento em caso de erro na criacao do socket
			exit(1);
		}
		printf("Conexão recebida de %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));	//Validacao do cliente que estabeleceu a conexao

		if((childpid = fork()) == 0){	//Cria um processo filho
			close(sockfd);

			recv(newSocket, buffer,1024,0);	//Recebe a mensagem do socket (ID do cliente)
			
			
			char id[1024];	//ID do cliente (que pode ser o nome do paciente)
			char nome_arquivo[1024];  //nome_arquivo
			strcpy(id,buffer);	//Variavel ID recebe o identificador do cliente que esta contido no buffer
			strcpy(nome_arquivo,buffer);	//O ID do cliente se torna o nome do arquivo
			strcat(nome_arquivo,".txt");	
    		
			bzero(buffer, sizeof(buffer));	//Limpa o buffer para receber novos dados do cliente (batimentos por minuto)

			while(1){
				bzero(buffer, sizeof(buffer));	//Limpa o buffer
				recv(newSocket, buffer, 1024, 0);	//Buffer recebe a mensagem do cliente (numero de batimentos por minuto)
				if(strcmp(buffer, ":sair") == 0){	//Se o cliente enviar um pedido de saida, ele eh desconectado do servidor
					printf("Desconectado de %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{	
					//Se o cliente enviou dados validos, o servidor coleta o horario instantaneo
					time ( &rawtime );
					timeinfo = localtime ( &rawtime );
					//strcpy(buffer, asctime (timeinfo));
					printf("Cliente: %s - Batimento: %s\n", id, buffer);					
					if((atoi(buffer) > 60) && (atoi(buffer) < 120)){	//Tratamento para os dados de batimento: faixa estavel (60-120) e de risco (demais valores)
						send(newSocket, "O paciente está estável", strlen("O paciente está estável"), 0);
					}
					else{
						send(newSocket, "O paciente requer atendimento!", strlen("O paciente requer atendimento!"), 0);
					}
					
					strcat(buffer," - Hora: ");	//Numero de batimentos eh concatenado com o texto "Hora"
					strcat(buffer,asctime (timeinfo));	//Buffer recebe o horario instantaneo coletado e o transforma em uma string

					FILE *file = fopen(nome_arquivo,"at+");	//Criacao de um arquivo para o cliente
					fprintf(file, "%s", buffer);	//Escrita do numero de batimentos e horario instantaneo no arquivo
					fclose(file);	//Fechamento do arquivo
					bzero(buffer, sizeof(buffer));	//Limpeza do buffer
				}
			}
		}

	}
	close(newSocket);	//Encerramento da criacao de novos sockets
	return 0;
}