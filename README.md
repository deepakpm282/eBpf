# eBpf

Problem statement 1:

        The program includes various header files required for eBPF programming, such as <linux/bpf.h>, <linux/in.h>, <linux/if_ether.h>, <linux/ip.h>, <linux/tcp.h>, and <bpf/bpf_helpers.h>. A macro DEFAULT_DROP_PORT is defined with the value 4040. This will be used as the default port number to drop if no other port is specified. A BPF map called drop_port_map is defined as an array map with a single entry. The key is a 32-bit unsigned integer (__u32), and the value is a 16-bit unsigned integer (__u16). This map will be used to store the TCP port number to drop. The main function drop_tcp_port is defined with the SEC("xdp") attribute, indicating that it's an XDP (eXpress Data Path) program.
        
        Inside the drop_tcp_port function, pointers data_end and data are initialized to point to the end and start of the packet data, respectively. A pointer eth is assigned the value of data, treating the packet data as starting with an Ethernet header. A bounds check is performed to ensure that the entire Ethernet header is present within the packet data. If the header extends beyond the packet data, the function returns XDP_PASS, allowing the packet to continue through the networking stack. The program checks if the Ethernet protocol type is IPv4 (ETH_P_IP). If not, it returns XDP_PASS. A pointer ip is assigned the address immediately after the Ethernet header, assuming it's the start of the IP header. Another bounds check is performed to ensure that the entire IP header is present within the packet data. If the header extends beyond the packet data, the function returns XDP_PASS. The program checks if the IP protocol is TCP (IPPROTO_TCP). If not, it returns XDP_PASS. A pointer tcp is assigned the address immediately after the IP header, assuming it's the start of the TCP header. Another bounds check is performed to ensure that the entire TCP header is present within the packet data. If the header extends beyond the packet data, the function returns XDP_PASS. A key with the value 0 is used to look up the drop_port_map. If a port number is found in the map (drop_port is not NULL), and the destination port of the TCP segment (tcp->dest) matches the port number stored in the map (*drop_port), the function returns XDP_DROP, indicating that the packet should be dropped. If the packet does not match the drop criteria, the function returns XDP_PASS, allowing the packet to continue through the networking stack.

Problem statement 2:

Explaining how the highlighted constructs work:

        cnp := make(chan func(), 10): This creates a channel cnp that can hold functions as values. The make(chan func(), 10) part initializes a buffered channel with a capacity of 10, meaning it can hold up to 10 function values before blocking.
        
        for i := 0; i < 4; i++: This is a regular for loop that iterates 4 times.
        
        go func() { ... }(): This is an anonymous Go routine that starts executing concurrently. The func() { ... } part defines an anonymous function, and the () at the end immediately invokes the function.
        
        for f := range cnp: This is a for loop that iterates over the values received from the cnp channel. It will block until a value is received from the channel or the channel is closed.
        
        f(): This line executes the function f that was received from the channel.
        
        cnp <- func() { ... }: This sends an anonymous function value into the cnp channel.

Use cases for these constructs:

        Channels and Go routines are used for concurrent programming in Go, enabling communication and coordination between multiple concurrent processes. Buffered channels, like the one created with make(chan func(), 10), are useful when you want to decouple the sender and receiver, allowing the sender to send values without immediately blocking if the receiver is not ready. Spawning multiple Go routines and using channels for communication can be helpful in scenarios like parallel processing, task distribution, and asynchronous programming. Anonymous functions are commonly used as callbacks, event handlers, or for creating closures with specific data captured from the outer scope.

Significance of the for loop with 4 iterations:

        The for loop with 4 iterations creates 4 separate Go routines that will run concurrently. Each Go routine continuously reads from the cnp channel and executes the received function values. Having multiple Go routines listening on the same channel can help distribute the workload and process functions in parallel.

Significance of make(chan func(), 10):

        make(chan func(), 10) creates a buffered channel that can hold up to 10 function values. The buffer size of 10 allows the sender to send up to 10 function values without blocking, even if no receiver is immediately ready. Without a buffer (e.g., make(chan func())), the sender would block after sending the first function value if no receiver is ready.

Why "HERE1" is not getting printed:

        The reason "HERE1" is not getting printed is that the program exits before the Go routine has a chance to execute the function sent to the channel. In the provided code, the main Go routine sends a single function value to the cnp channel and then immediately exits without waiting for the Go routines to complete. To see "HERE1" printed, you would need to keep the main Go routine running until the sent function has been processed by one of the Go routines listening on the cnp channel.
