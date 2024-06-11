#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/bpf.h>
#include <bpf/libbpf.h>

#define MAP_PATH "/sys/fs/bpf/drop_port_map"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number\n");
        return 1;
    }

    int map_fd = bpf_obj_get(MAP_PATH);
    if (map_fd < 0) {
        perror("bpf_obj_get");
        return 1;
    }

    __u32 key = 0;
    __u16 value = (__u16)port;

    if (bpf_map_update_elem(map_fd, &key, &value, BPF_ANY) != 0) {
        perror("bpf_map_update_elem");
        close(map_fd);
        return 1;
    }

    close(map_fd);
    return 0;
}

