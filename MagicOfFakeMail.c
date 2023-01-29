#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <math.h>

#define ERROR -1
#define PORT 25

int Mensagem(int Socket);

int main(int argc, char *argv[]){
    char Recv[1024], Send[1024]; // variáveis para receber e enviar
    int Socket, R;
    struct hostent *host; // variável host
    struct sockaddr_in alvo; // variável alvo

    if(argc < 4){ // Há menos de 4 parâmetros
        printf("Envia Mails!\n\n"); // Mensagem
        printf("Uso: %s [host_alvo] [usuario_alvo] [rementente_forjado]\n\n",argv[0]);
        exit(ERROR); // Sai do programa caso haja erro
    }

    host = gethostbyname(argv[1]); // vereficiar o host
    if(host == NULL){
        fprintf(stderr,"Host Desconhecido!\n");
        exit(ERROR); // sair do programa caso haja erro
    }

    alvo.sin_family = host->h_addrtype;
    alvo.sin_port = htons(PORT);
    alvo.sin_addr = *((struct in_addr *)host->_addr);
    bzero(&(alvo.sin_zero), 8);

    Socket = socket(AF_INET, SOCK_STREAM,0); // criar o socket
    if(Socket < 0){
        perror("Socket");
        exit(ERROR); // sair do programa
    }

    if(connect(Socket,(struct sockaddr * )& alvo, sizeof(struct sockaddr)) == ERROR){ // conectar ao socket
        perror("Connect");
        exit(ERROR);
    }

    R = recv(Socket, Recv, sizeof(Recv), 0);
    Recv[R] = '\0';
    printf("%s", Recv);

    sprintf(Send, "HELO %s\r\n", argv[1]);
    send(Socket, Send, strlen(Send), 0);
    R = recv(Socket, Recv, sizeof(Recv), 0);
    Recv[R] = '\0';
    printf("%s", Recv);

    sprintf(Send, "MAIL FROM: <%s>\r\n", argv[3]);
    send(Socket, Send, strlen(Send), 0);
    R = recv(Socket, Recv, sizeof(Recv), 0);
    Recv[R] = '\0'; printf("%s", Recv);

    sprintf(Send, "RCPT TO: <%s>\r\n", argv[2]);
    send(Socket, Send, strlen(Send), 0);
    R = recv(Socket, Recv, sizeof(Recv), 0);
    Recv[R] = '\0';
    printf("%s", Recv);

    sprintf(Send, "DATA\r\n");
    send(Socket, Send, strlen(Send), 0);
    R = recv(Socket, Recv, sizeof(Recv), 0);
    Recv[R] = '\0';
    printf("%s", Recv);

    Mensagem(Socket);

    sprintf(Send, ".\r\n");
    send(Socket, Send, strlen(Send), 0);
    R = recv(Socket, Recv, sizeof(Recv), 0);
    Recv[R] = '\0';
    printf("%s", Recv);

    sprintf(Send, "QUIT\r\n");
    send(Socket, Send, strlen(Send), 0);
    R = recv(Socket, Recv, sizeof(Recv), 0);
    Recv[R] = '\0';
    printf("%s", Recv);

    close(Socket);
return 0;

}

int Mensagem(int Socket){
char Send[1024];
printf("Assunto: ");
fgets(Send, sizeof(Send), stdin);
send(Socket, Send, strlen(Send), 0);
printf("Mensagem: ");
while(fgets(Send, sizeof(Send), stdin) != NULL){
send(Socket, Send, strlen(Send), 0);
}
return 0;
}
