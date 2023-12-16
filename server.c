#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>

int main()
{
    // Création du socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET : domain, permet de spécifier la famille de protocole de communication
    // AF_INET est pour les pour protocoles utilsés pour Internet IPv4
    //
    // SOCK_STREAM : type, TCP socket qui est un protocole utilisé par http
    // En comparaison avec UDP, https://www.geeksforgeeks.org/differences-between-tcp-and-udp/ 

    // fonction bind, relier une adresse à un socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080); // port 8080   
    addr.sin_addr = 0; // l'adresse ip 0.0.0.0, le serveur écoute toutes les interfaces réseau

    bind(socket_fd, &addr, sizeof(addr));
    
    listen(socket_fd, 10);

    int client_fd = accept(socket_fd, 0, 0);
    
    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);

    // dans le buffer, il y aura une requête http
    // de la forme : GET /file.html ......
    // On ne veut que le nom du fichier donc on va prendre buffer + 5 et mettre NULL sur l'espace après file.html
    
    char* f = buffer + 5;
    // renvoie un pointeur de la première occurence d'un char 
    *strchr(f, ' ') = 0; // NULL terminator
    
    int opened_fd = open(f, O_RDONLY);
    
    sendfile(client_fd, opened_fd, 0, 256);
    close(opened_fd);
    close(socket_fd);
}
