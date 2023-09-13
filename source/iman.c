#include "../headers/headers.h"

int PrintDesc(char* Buffer, int BufferLength, int flag) {
    int parity = 0;
    int i=0;
    if (!flag) {
        for (i=0; i<BufferLength && Buffer[i] != '\0'; i++) {
            if (Buffer[i] == '<') 
                parity = 1;
            else if (Buffer[i] == '>')
                parity = 0;
            else if (!parity && i + 4 < BufferLength && Buffer[i] == 'N') {
                if (Buffer[i+1] == 'A' && Buffer[i+2] == 'M' && Buffer[i+3] == 'E') {
                    flag = 1;
                    break;
                }
            }
        }
    }
    int breakflag = 0;
    int breakindex;
    for (int j=i; j+6<BufferLength; j++) {
        if (Buffer[j] == '\n') {
            if (j+1 < BufferLength && (Buffer[j+1] == ' ' || Buffer[j+1] == '\n'))
                printf("Yes\n");
            else if (j+1 < BufferLength)
                printf("%c\n",Buffer[j+1]);
        }
        char temp = Buffer[j+6];
        Buffer[j+6] = '\0';
        if (strcmp(&Buffer[j], "AUTHOR") == 0) {
            breakflag = 1;
            breakindex = j;
            Buffer[j] = '\0';
            break;
        }
        Buffer[j+6] = temp;
    }
    if (breakflag) {
        write(STDOUT_FILENO, &Buffer[i], breakindex-i);
        return -1;
    }
    else {
        write(STDOUT_FILENO, &Buffer[i], BufferLength-i);
    }
    return flag;
}

int iman(char* Arg) {
    if (Arg == NULL) {
        PrintError("Need 1 Argument\n");
        return 1;
    }
    char buffer[BUFSIZ];
    enum CONSTEXPR { MAX_REQUEST_LEN = 1024};
    char request[MAX_REQUEST_LEN];
    char request_template[] = "GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\n\r\n";
    struct protoent *protoent;
    char* hostname = "man.he.net";
    in_addr_t in_addr;
    int request_len;
    int socket_file_descriptor;
    ssize_t nbytes_total, nbytes_last;
    struct hostent *hostent;
    struct sockaddr_in sockaddr_in;
    unsigned short server_port = 80;

    // if (argc > 1)
    //     hostname = argv[1];

    request_len = snprintf(request, MAX_REQUEST_LEN, request_template, Arg ,hostname);
    if (request_len >= MAX_REQUEST_LEN) {
        PrintError("request length too large: %d\n", request_len);
        exit(EXIT_FAILURE);
    }

    /* Build the socket. */
    protoent = getprotobyname("tcp");
    if (protoent == NULL) {
        PrintError("Unable to get proto by name");
        exit(EXIT_FAILURE);
    }
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, protoent->p_proto);
    if (socket_file_descriptor == -1) {
        PrintError("Unable to open socket\n");
        exit(EXIT_FAILURE);
    }

    /* Build the address. */
    hostent = gethostbyname(hostname);
    if (hostent == NULL) {
        PrintError("Unable to resolve hostname (\"%s\")\n", hostname);
        return -4;
    }
    in_addr = inet_addr(inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list)));
    if (in_addr == (in_addr_t)-1) {
        PrintError("inet_addr(\"%s\")\n", *(hostent->h_addr_list));
        return -3;
    }
    sockaddr_in.sin_addr.s_addr = in_addr;
    sockaddr_in.sin_family = AF_INET;
    sockaddr_in.sin_port = htons(server_port);

    /* Actually connect. */
    if (connect(socket_file_descriptor, (struct sockaddr*)&sockaddr_in, sizeof(sockaddr_in)) == -1) {
        PrintError("Not able to connect!\n");
        return -2;
    }

    /* Send HTTP request. */
    nbytes_total = 0;
    while (nbytes_total < request_len) {
        nbytes_last = write(socket_file_descriptor, request + nbytes_total, request_len - nbytes_total);
        if (nbytes_last == -1) {
            perror("write");
            return -1;
        }
        nbytes_total += nbytes_last;
    }

    /* Read the response. */
    int writeflag = 0;
    int parity = 0;
    int i =0;
    int breakflag = 0;
    int breakindex;
    int countheads = 1;
    while ((nbytes_total = read(socket_file_descriptor, buffer, BUFSIZ)) > 0) {
        // write(STDOUT_FILENO, buffer, nbytes_total);
        // writeflag = PrintDesc(buffer, nbytes_total, writeflag);
        i = 0;
        if (!writeflag) {
            for (i=0; i<nbytes_total && buffer[i] != '\0'; i++) {
                if (buffer[i] == '<') 
                    parity = 1;
                else if (buffer[i] == '>')
                    parity = 0;
                else if (!parity && i + 4 < nbytes_total && buffer[i] == 'N') {
                    if (buffer[i+1] == 'A' && buffer[i+2] == 'M' && buffer[i+3] == 'E') {
                        writeflag = 1;
                        break;
                    }
                }
            }
        }
        for (int j=i; j+6<nbytes_total; j++) {
            if (buffer[j] == '\n') {
                if (j+1 < nbytes_total && !((buffer[j+1] == ' ' || buffer[j+1] == '\n'))) {
                    countheads++;
                    if (countheads > 3) {
                        breakflag = 1;
                        breakindex = j+1;
                        buffer[j+1] = '\0';
                        break;
                    }
                }
            }
        }
        if (breakflag) {
            for (int k = i; k < breakindex; k++) {
                if (buffer[k] == '<')
                    parity = 1;
                else if (buffer[k] == '>')
                    parity = 0;
                if (parity == 0) {
                    putchar(buffer[k]);
                }
            }
            writeflag = -1;
        }
        else {
            for (int k = i; k < nbytes_total; k++) {
                if (buffer[k] == '<')
                    parity = 1;
                else if (buffer[k] == '>')
                    parity = 0;
                if (parity == 0) {
                    putchar(buffer[k]);
                }
            }
        }
        if (countheads > 3)
            break;
        if (writeflag == -1)
            break;
    }
    if (nbytes_total == -1) {
        perror("read");
        return -1;
    }
    if (countheads == 1) {
        PrintError("No Page Found!\n");
    }

    close(socket_file_descriptor);
    return 0;
}