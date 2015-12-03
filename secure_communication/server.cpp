#include <iostream>
#include <bitset>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"

#define BACKLOG 10

struct Trivium{

    std::string read_file(char file_name[]){

        std::ifstream in(file_name);
        std::string return_value;
        char c;

        if (in.is_open()){
            getline (in, return_value);
            in.close();
            return return_value;
        }
    }

    std::string keystr;
    std::string ivstr;

    Trivium(){
        keystr = read_file("trivium_key.txt");
        ivstr = read_file("trivium_iv.txt");
    }


    void setup(std::bitset<288> &s, std::bitset<3> &t, std::bitset<80> &KEY, std::bitset<80> &INITIAL_VALUE){
        for (int i = 0; i < 288; ++i){
            s[i] = 0;
        }
        t[0] = t[1] = t[2] = 0;
        for(int i=0; i<80; i++)
        {
            s[i] = KEY[i];
        }
        for(int i=0; i<80; i++)
        {
            s[i+93] = INITIAL_VALUE[i];
        }
        s[285] = 1;
        s[286] = 1;
        s[287] = 1;
        for(int j=0; j<4; j++)
        {
            for(int i=0; i<288; i++)
            {
                t[0] = s[65]  ^ s[92];
                t[1] = s[161] ^ s[176];
                t[2] = s[242] ^ s[287];
                t[0] = t[0] ^ (s[90]  & s[91] ) ^ s[170];
                t[1] = t[1] ^ (s[174] & s[175]) ^ s[263];
                t[2] = t[2] ^ (s[285] & s[286]) ^ s[68];
                s <<= 1;
                s[0] = t[2];
                s[93] = t[0];
                s[177] = t[1];
            }
        }

        return;
    }

    std::string encrypt(std::string plaintext){
        std::string ciphertext = plaintext;

        std::bitset<288> s;
        std::bitset<3> t;
        std::bitset<80> key(keystr);
        std::bitset<80> iv(ivstr);


        setup(s, t, key, iv);

        int z = 0;
        int i,x;
        for(int x=0; x<plaintext.size(); ++x)
        {
            for(i=0; i<8; ++i)
            {
                t[0] = s[65]  ^ s[92];
                t[1] = s[161] ^ s[176];
                t[2] = s[242] ^ s[287];
                z |= (t[0] ^ t[1] ^ t[2]) << (i&0b111);

                t[0] = t[0] ^ (s[90]  & s[91] ) ^ s[170];
                t[1] = t[1] ^ (s[174] & s[175]) ^ s[263];
                t[2] = t[2] ^ (s[285] & s[286]) ^ s[68];
                s <<= 1;
                s[0] = t[2];
                s[93] = t[0];
                s[177] = t[1];
            }
            ciphertext[x] ^= char(z);
            z = 0;
        }

        return ciphertext;
    }
};

void sigchld_handler(int s)
{
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{

    Trivium trivium;
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    char msg[64];
    int msg_len;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);

        printf("server: got connection from %s\n", s);
        printf("server: enter a message: ");

	fgets(msg, sizeof(msg), stdin);
	msg_len = strlen(msg);
	msg[--msg_len] = '\0';
    strcpy(msg, trivium.encrypt(msg));

        if (!fork()) {
            close(sockfd);
            if (send(new_fd, msg, msg_len, 0) == -1)
                perror("send");
	    printf("server: message send\n");
	    close(new_fd);
            exit(0);
        }
        close(new_fd);
    }

    return 0;
}
