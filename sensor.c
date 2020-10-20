#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h> 

#include <arpa/inet.h>    //permite que use in_port_t e in_add_t do netinet
#include <netinet/in.h>

#include <pthread.h>
#include <time.h>

int done = 1;

int random_num(){
    int random_number;
    srand(time(0));
    random_number = rand() % 10;
    return random_number;
}

int clientConfig()
{
    // Descritor do cliente
    int clienteSockfd;
    // Estrutura do Cliente
    struct sockaddr_in serv_addr;
    // Tamanho da estrutura
    socklen_t addrlen = sizeof(serv_addr);

    // Socket familia TCP declarado SOCK_STREAM e criado
    clienteSockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd < 0)
    {
        printf("\nErro no Socket");
        exit(1);
    }
    // Zera a estrutura
    bzero((char *)&serv_addr, sizeof(serv_addr));
    // Seta a familia
    serv_addr.sin_family = AF_INET;
    // Define o IP nesse caso o localhost
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Define a porta de conexao
    serv_addr.sin_port = htons(6881);
    // Faz a conexao com o servidor
    if (connect(clienteSockfd, (struct sockaddr *)&serv_addr, addrlen) < 0)
    {
        printf("\nErro no Socket");
        exit(1);
    }
    else{
        printf("\nConexão estabelecida");
    }
    return clienteSockfd;
}


int main()
{
    long int port;
    int clienteSockfd;
    int sockfd;
    struct sockaddr_in serverAddr;

    long int num_sensor = 0;
    int tam_send;
    int tam_received;
    // Buffer de entrada (armazena buffer do cliente)
    char sensor_datas[128];
    char answer[32];
    int data_size;
    char str_port[64];
    char id[32];
    int flag = 1;
    time_t rawtime;
    struct tm * timeinfo;

    sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd == -1){
		printf("Falha ao criar socket\n");
		exit(0);
	}

	//Funções do <netinet.h>
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(6881);   //Porta de conexao
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//fim

	memset(&(serverAddr.sin_zero),0,sizeof(serverAddr.sin_zero));

	printf("Tentando se conectar a base...\n");

	if(connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1)  {
		printf("Erro em connect()\n");
		exit(0);
	}
	printf("Conectado\n\n");
    
    tam_received = recv(answer, str_port, 32,0);
    answer[tam_received] = '\0';

    port = strtol(answer, NULL, 32);
	printf("Porta: %ld\n", port);
    
    sleep(5);

    close(sockfd);         
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd == -1){
		printf("Falha ao criar socket\n");
		exit(0);
	}

    //Conecta na porta fornecida pela base
	//Funções do <netinet.h>
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(6881+port);   //Porta de conexao
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//fim

	memset(&(serverAddr.sin_zero),0,sizeof(serverAddr.sin_zero));

	printf("Tentando se conectar a base...\n");

	if(connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1)  {
		printf("Erro em connect()\n");
		exit(0);
	}
	printf("Conectado\n\n");

    while(flag != -1){
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        strcpy(sensor_datas, asctime (timeinfo));

        tam_send = send(sockfd,sensor_datas,strlen(sensor_datas),0);

        tam_received = recv(sockfd,answer,64,0);
        answer[tam_received] = '\0';

        printf("\nServer: %s", answer);
    }

    close(sockfd);         
	exit(0);
}