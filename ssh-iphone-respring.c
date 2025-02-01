#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

#define DEFAULT_HOST "127.0.0.1"  // Primary iOS IP.
#define DEFAULT_USER "mobile" // Primary user of iPhone ##Defualt dopamine jb is "mobile"
#define DEFAULT_COMMAND "killall -9 SpringBoard"  // Fucking aggressive respring command !!!beware before using this shit!!!

// function for SSH command
int execute_ssh_command(ssh_session session, const char *command) {
   // Create SSH channel 
    ssh_channel channel = ssh_channel_new(session);
    if (channel == NULL) return SSH_ERROR;

    if (ssh_channel_open_session(channel) != SSH_OK) {
        ssh_channel_free(channel);
        return SSH_ERROR;
    }

  // Open SSH and checking for error
        if (ssh_channel_request_exec(channel, command) != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return SSH_ERROR;
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_OK;
}

int main(int argc, char *argv[]) {
    ssh_session my_ssh_session = ssh_new();
    if (my_ssh_session == NULL) {
        fprintf(stderr, "Error creating SSH session.\n");
        exit(EXIT_FAILURE);
    }

    const char *host = DEFAULT_HOST;
    const char *user = DEFAULT_USER;
    const char *command = DEFAULT_COMMAND;
    unsigned int port = 22;

    // Allow command-line arguments to override defaults
    if (argc > 1) host = argv[1];
    if (argc > 2) user = argv[2];
    if (argc > 3) command = argv[3];
    if (argc > 4) port = atoi(argv[4]);

    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, user);

    // Set timeout for connection
    int timeout = 10;
    ssh_options_set(my_ssh_session, SSH_OPTIONS_TIMEOUT, &timeout);

    if (ssh_connect(my_ssh_session) != SSH_OK) {
        fprintf(stderr, "Error connecting to host: %s\n", ssh_get_error(my_ssh_session));
        ssh_free(my_ssh_session);
        exit(EXIT_FAILURE);
    }

    // Verify host key using the modern method
    enum ssh_known_hosts_e known_hosts = ssh_session_is_known_server(my_ssh_session);
    switch (known_hosts) {
        case SSH_KNOWN_HOSTS_OK:
            break; // Host is known and key matches
        case SSH_KNOWN_HOSTS_CHANGED:
            fprintf(stderr, "Host key for server changed. Possible man-in-the-middle attack!\n");
            ssh_disconnect(my_ssh_session);
            ssh_free(my_ssh_session);
            exit(EXIT_FAILURE);
        case SSH_KNOWN_HOSTS_OTHER:
            fprintf(stderr, "Host key for server not found in known hosts.\n");
            ssh_disconnect(my_ssh_session);
            ssh_free(my_ssh_session);
            exit(EXIT_FAILURE);
        case SSH_KNOWN_HOSTS_UNKNOWN:
            fprintf(stderr, "Host key for server is unknown. Do you want to add it to known hosts? (y/n)\n");
            char response = getchar();
            if (response != 'y' && response != 'Y') {
                ssh_disconnect(my_ssh_session);
                ssh_free(my_ssh_session);
                exit(EXIT_FAILURE);
            }
            if (ssh_session_update_known_hosts(my_ssh_session) != SSH_OK) {
                fprintf(stderr, "Failed to update known hosts file.\n");
                ssh_disconnect(my_ssh_session);
                ssh_free(my_ssh_session);
                exit(EXIT_FAILURE);
            }
            break;
        case SSH_KNOWN_HOSTS_ERROR:
            fprintf(stderr, "Error verifying host key: %s\n", ssh_get_error(my_ssh_session));
            ssh_disconnect(my_ssh_session);
            ssh_free(my_ssh_session);
            exit(EXIT_FAILURE);
    }

    char *password = getenv("SSH_PASS");  // ใช environment variable แทน hardcoded password
    if (!password) {
        fprintf(stderr, "Error: SSH_PASS environment variable is not set.\n");
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(EXIT_FAILURE);
    }

    if (ssh_userauth_password(my_ssh_session, user, password) != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(my_ssh_session));
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(EXIT_FAILURE);
    }

    printf("Connected! Sending respring command...\n");
    if (execute_ssh_command(my_ssh_session, command) != SSH_OK) {
        fprintf(stderr, "Error executing command.\n");
    } else {
        printf("Respring command sent successfully!\n");
    }

    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    return 0;
}
   
