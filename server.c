#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h> 

#include <arpa/inet.h>    //permite que use in_port_t e in_add_t do netinet
#include <netinet/in.h>

#include <pthread.h>

void *Processor(void *arg)
{
    // Cast do ponteiro
    long int port = *(long int *)arg;
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
    char id[32];

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
		printf("Falha ao criar Socket\n");
		pthread_exit(NULL);
	}
    
    serverAddr.sin_family      = AF_INET;
	serverAddr.sin_port        = htons(6881+port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(&serverAddr.sin_zero,0,sizeof(serverAddr.sin_zero));

	if(bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1){
		printf("Erro em bind()\n");
		pthread_exit(NULL);
	}    

    if(listen(sockfd,1) == -1){
		printf("Erro em listen()\n");
		pthread_exit(NULL);
	}
    
    printf("Aguardando cliente...\n");

	clienteSockfd = accept(sockfd,0,0);

	if(clienteSockfd == -1){
		printf("Erro em accept()\n");
		pthread_exit(NULL);
	}

    data_size = recv(clienteSockfd, id, 32,0);
    sensor_datas[data_size] = '\0';

    printf("%s",id);

    strcat(id,".txt");

    FILE *file = fopen(id,"a");
 
    // Loop "infinito"
    printf("\nEsperando dados: ");
    while(1){
        // Le o que vem do cliente
        data_size = recv(clienteSockfd, sensor_datas, 128,0);
        sensor_datas[data_size] = '\0';
        printf("\nCliente: %s", sensor_datas);
        fprintf(file,"%s", sensor_datas);
        send(clienteSockfd, "Dado recebido", strlen("Dado recebido"),0);
    }

    fclose(file);
    /*Encerra o descritor*/
    close(clienteSockfd);
    /*Encerra a thread*/
    pthread_exit((void*) 0);
}

int main(){
    int clienteSockfd;
    int sockfd;
    // Declaracao da estrutura
    struct sockaddr_in serverAddr;
    int flag = 1;
    char answer[32];
    long int num_sensor = 0;
    int tam_send = 0;

    pthread_t thread;
	pthread_attr_t attr;

    //Cria o socket
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("\nErro no Socket");
        exit(1);
    }

    serverAddr.sin_family      = AF_INET;
	serverAddr.sin_port        = htons(6881);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	memset(&serverAddr.sin_zero,0,sizeof(serverAddr.sin_zero));

    //Faz a bindagem (cola, gruda, conecta seja o que for)
    if (bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0)
    {
        printf("\nErro no Socket");
        exit(1);
    }
    // Fica na escuta de ate 5 clientes
    if (listen(sockfd, 1) < 0)
    {
        printf("\nErro no Socket");
        exit(1);
    }

    // Loop de espera dos clientes
    while(flag != -1)
    {
       printf("\nEsperando conexão");
        
        if((clienteSockfd = accept(sockfd,0,0)) < 0){
            printf("\nErro ao aceitar conexão");
            exit(0);
        }
        
        sprintf(answer, "%ld", num_sensor);
		tam_send = send(clienteSockfd,answer,strlen(answer),0);

		//Cria a thread de servidor para a sonda
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_create(&thread, &attr, Processor, &num_sensor);

		close(clienteSockfd);
    }

    close(clienteSockfd);
    close(sockfd);

    exit(0);
}