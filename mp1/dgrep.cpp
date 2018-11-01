#include "dgrep.h"
#include "test.h"

#define TEST_INFREQUENT 0
#define TEST_FREQUENT 0
#define VM_NUM 10
#define RECV_BUF_SIZE 500
#define SEND_BUF_SIZE 100000
#define COMMAND_SIZE 100

int line_count[VM_NUM];
int vm_id = 0;

using namespace std;

/*
 * [recv_all description: Since the function recv sometimes don't
 * receive all messages in the buffer, this function make sure that
 * all the bytes in the buffer are received]
 * @param  sockfd [description: socket file descriptor]
 * @param  buf    [description: buffer to receive the messages]
 * @param  length [description: number of bytes to be received]
 * @return        [description: number of bytes actually received]
 */
int recv_all(int sockfd, char *buf, int length){
    int byte_read = 0;
    int cnt = 0;
    while(byte_read < length){
        if((cnt = read(sockfd, buf, length - byte_read)) < 0)
            return -1;
        byte_read += cnt;
        buf += cnt;
        if(cnt == 0)    // If no bytes are read, no further bytes can be read
            break;
    }
    return byte_read;
}

/*
 * [send_all description: Since the function send sometimes don't
 * send all messages in the buffer, this function make sure that all the
 * bytes in the buffer are sent]
 * @param  sockfd [description: socket file descriptor]
 * @param  buf    [description: buffer containning the messages to be sent]
 * @param  length [description: number of bytes to be sent]
 * @return        [description: number of bytes actually sent]
 * @return        [description: number of bytes actually sent]
 */
int send_all(int sockfd, char *buf, int length){
    int byte_sent = 0;
    int byte_left = length;
    int cnt = 0;
    while(byte_sent < length){
        if((cnt = write(sockfd, buf, length - byte_sent)) < 0)
            return -1;
        byte_sent += cnt;
        buf += cnt;
        if(cnt == 0)    // If no bytes are sent, no further bytes can be sent
            break;
    }
    return byte_sent;
}

/*
 * [client description: This function identifies the server to send
 * command to, connect with it, send the command and receive messages
 * from the server]
 * @param  target  [description: vm ID of the target server]
 * @param  kbInput [description: keyboard input containing command]
 * @return         [description:  0 if no error]
 */
int client(int target, char *kbInput){
    int socketfd;
    struct addrinfo hints, *serverinfo, *curr_node;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(addrs[target], PORT, &hints, &serverinfo) != 0) {
        cout<<"Failed to get address information";
        return 1;
    }

    // connect to the first valid one
    for(curr_node = serverinfo; curr_node != NULL; curr_node = curr_node->ai_next) {
        if((socketfd = socket(curr_node->ai_family, curr_node->ai_socktype, curr_node->ai_protocol)) == -1) {
            continue;
        }
        setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, NULL, (socklen_t)sizeof(int));
        if(connect(socketfd, curr_node->ai_addr, curr_node->ai_addrlen) == -1) {
            close(socketfd);
            continue;
        }
        break;
    }

    if (curr_node == NULL) {
        cout<<"client: failed to connect\n";
        return 2;
    }

    freeaddrinfo(serverinfo);

    // Send the command from keyboard to the server
    if (send_all(socketfd, kbInput, COMMAND_SIZE - 1) == -1) {
        perror("input");
        exit(1);
    }

    char response[RECV_BUF_SIZE];
    memset(response, 0, sizeof(char)*RECV_BUF_SIZE);

    // In case of testing, output the result to a file
    FILE *grep_res = fopen("grepresult.log", "w");

    // Receive all messages from the serve
    int num_recved = RECV_BUF_SIZE - 1;
    while(num_recved == RECV_BUF_SIZE - 1){
        memset(response, 0, sizeof(char)*RECV_BUF_SIZE);
        num_recved =recv_all(socketfd, response, RECV_BUF_SIZE - 1);
        cout<<response;
        // Save the result to grepresult.log
        fprintf(grep_res, response);
    }

    close(socketfd);
    fclose(grep_res);

    // Get line count from terminal using the wc command
    FILE* lcf;
    lcf = popen("wc -l grepresult.log", "r");
    char c = fgetc(lcf);
    char temp[VM_NUM];
    int index = 0;
    while(c!=' '){
        temp[index] = c;
        c = fgetc(lcf);
        index++;
    }
    temp[index] = '\0';
    int a = atoi(temp);
    cout<<a<<endl;
    line_count[target] = a;

    return 0;
}

/*
 * [client_thread description: This function serves as the thread
 * to take keyboard input and try to get grep information from all
 * possible servers]
 * @param  arg [description: none]
 * @return     [description: none]
 */
void *client_thread(void *arg){
    while(1){
        char kbInput[128];
        // wait for input from keyboard
        cout<<"Type in your command: ";
        cin.getline(kbInput, sizeof(kbInput));
        // Poll information from all 10 vms, including the client itself
        for(int vm_num = 1; vm_num <= VM_NUM; vm_num++){
            if(vm_num!=vm_id){
                client(vm_num-1, kbInput);
            }
            else{
                string updated(kbInput);
                #if TEST_FREQUENT
                    updated += " testvmfreq.log";
                #elif TEST_INFREQUENT
                    updated += testlogs[vm_id - 1];
                #else
                    updated += logs[vm_id - 1];
                #endif
                system(&updated[0]);
                updated += "> grepresultclient.log";
                system(&updated[0]);    // store result to file
                // Get line count
                FILE* lcf;
                lcf =popen("wc -l grepresultclient.log", "r");
                char c = fgetc(lcf);
                char temp[VM_NUM];
                int index = 0;
                while(c!=' '){
                    temp[index] = c;
                    c = fgetc(lcf);
                    index++;
                }
                temp[index] = '\0';
                int a = atoi(temp);
                cout<< a<<endl;
                line_count[vm_id-1] = a;
            }
        }
        int total = 0;
        for(int a = 0; a < VM_NUM; a++){
            if(&line_count[a]!=NULL){
                total+=line_count[a];
            }
        }
        cout<<total<<endl;
        memset(line_count, 0, sizeof(int)*VM_NUM);
    }
}

/*
 * [server_thread description: This function serves as the thread to listen
 * connection from clients, grep info locally and send everything back to the
 * client. ]
 * @param  arg [description: none]
 * @return     [description: none]
 */
void *server_thread(void *arg){
    int socketfd;
    struct addrinfo hints, *serverinfo, *curr_node;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;

    char buf[COMMAND_SIZE];
    FILE * in  = NULL;
    FILE *inc = NULL;
    char send_buf[SEND_BUF_SIZE];
    int index = 0;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if (getaddrinfo(addrs[vm_id-1], PORT, &hints, &serverinfo) != 0) {
        cout<<"Failed to get address information";
        return 0;
    }

    // loop through all the results and bind to the first we can
    for(curr_node = serverinfo; curr_node != NULL; curr_node = curr_node->ai_next) {
        if ((socketfd = socket(curr_node->ai_family, curr_node->ai_socktype, curr_node->ai_protocol)) == -1) {
            continue;
        }
        int opt = 1;
        setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(int));

        if (bind(socketfd, curr_node->ai_addr, curr_node->ai_addrlen) == -1) {
            close(socketfd);
            continue;
        }

        break;
    }

    freeaddrinfo(serverinfo); // all done with this structure

    if (curr_node == NULL)  {
        cout<<"Failed to bind\n";
        exit(1);
    }

    if (listen(socketfd, BACKLOG) == -1) {
        cout<<"Failed to listen\n";
        exit(1);
    }

    while(1){
        sin_size = sizeof their_addr;
        int new_fd = accept(socketfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
        recv_all(new_fd, buf, COMMAND_SIZE - 1);

        if(buf[0] == 'g' && buf[1] == 'r' && buf[2] == 'e' &&buf[3] == 'p'){
            //Complete the command to execute
            string updated(buf);
            #if TEST_INFREQUENT
                updated += testlogs[vm_id-1];
            #elif TEST_FREQUENT
                updated += " testvmfreq.log";
            #else
                updated += logs[vm_id-1];
            #endif
            //cout<<updated<<endl;
            string count = updated + " -c";
            in = popen(&updated[0], "r");
            inc = popen(&count[0], "r");
            char c = fgetc(in);
            index = 0;
            int num_sent = SEND_BUF_SIZE;
            //memset(send_buf, 0, sizeof(char)*50);
            //Send all messages
            while(num_sent == SEND_BUF_SIZE){
                memset(send_buf, 0, sizeof(char)*SEND_BUF_SIZE);
                while(c!=EOF){
                    if(index == SEND_BUF_SIZE){break;}
                    send_buf[index] = c;
                    c = fgetc(in);
                    index++;
                }
                //cout<<send_buf;
                //num_sent = index;
                num_sent = send_all(new_fd, send_buf, index);

                index = 0;
            }
        }
        close(new_fd);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Take in the second argument as the id for the vm
    if(argc!=2){
        cout<<"no vm num arg"<<endl;
        return 0;
    }
    int arg = (int)(*argv[1]) - ASC_ZERO;
    if(arg == 1){
        if(*(argv[1] + 1) == ASC_ZERO){
            arg = 10;
        }
        else{
            arg = 1;
        }
    }
    vm_id = arg;
    cout<<vm_id<<endl;
    // In case of testing, generate some log files for testing
    #if TEST_INFREQUENT
        test_generates_logs(vm_id);
    #endif
    // Two threads created: one for client one for server
    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1, NULL, server_thread, NULL);
    pthread_create(&tid2, NULL, client_thread, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // In case of testing, remove log files for testing
    #if TEST_INFREQUENT
        remove_test_file(vm_id);
    #elif TEST_FREQUENT
        remove("testvmfreq.log");
    #endif

    return 0;
}
