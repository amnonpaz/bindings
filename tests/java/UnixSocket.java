import java.lang.Exception.*;

public class UnixSocket {
    native private int open(String socketName);
    native private void close(int socket);
    native private String read(int socket);
    native private int write(int socket, String out);

    int fd;

    static {
        try {
            System.loadLibrary("unixsockets");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Error: UnixSockets JNI Library failed to load\n" + e);
            System.exit(1);
        }
    }

    public UnixSocket(String socketName) throws Exception {
        fd = open(socketName);
    }

    public void close() {
        close(fd);
    }

    public String read() throws Exception {
        return read(fd);
    }

    public void write(String out) throws Exception {
        write(fd, out);
    }
} 
