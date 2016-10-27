public class UnixSocketTest {
    final static String [] output = {
        "Try this string",
        "Maybe this one",
        "Have fun",
        "Bye bye"
    };

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Error: Destination socket wasn't sepecified");
            return;
        }

        UnixSocket socket;
        try {
            socket = new UnixSocket(args[0]);
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return;
        }

        try {
            for (String s : output) {
                socket.write(s);
                String resp = socket.read();
                System.out.println("Got response: " + resp);
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        socket.close();
    }
}
