import java.io.*;
import java.util.concurrent.*;

class TestCoverage {
    public static boolean[] coverage = new boolean[10000];
}

public class Sample10 {

    public  @interface Test {}

    private static int lineNo = 0;

    private static abstract class DelegatingServerSocketFactory {
        public DelegatingServerSocketFactory(Object o) {
            assert o == ServerSocketFactory.testDefault;
        }

        protected abstract ServerSocket configureServerSocket(ServerSocket s) throws IOException;
    }

    private static class ServerSocketFactory {
        public final static Object testDefault = new Object();

        public static Object getDefault() {
            return testDefault;
        }
    }

    private static class ServerSocket {
        public void setReceiveBufferSize(int x) {
            assert lineNo == 1;
            lineNo++;
            assert x == 4096;
        }

    }

    private static class SocketFactory {
        public final static Object testDefault = new Object();

        public static Object getDefault() {
            return testDefault;
        }
    }

    private static class Socket {
        public void setReceiveBufferSize(int x) {
            assert lineNo == 2;
            lineNo++;
            assert x == 4096;
        }

        public void setSendBufferSize(int x) {
            assert lineNo == 3;
            lineNo++;
            assert x == 4096;
        }
    }

    public abstract static class DelegatingSocketFactory {
        public DelegatingSocketFactory(Object o) {
            assert o == SocketFactory.testDefault;
        }

        protected abstract Socket configureSocket(Socket s) throws IOException;
    }

    private static class UrlFactory {
        public Client client() {
            return new Client();
        }

        public static class Client {
            public Builder1 newBuilder() {
                return new Builder1();
            }
        }

        public static class Builder1 {
            public Builder2 socketFactory(DelegatingSocketFactory f) throws IOException {
                Socket s =new Socket();
                assert f.configureSocket(s) == s;
                return new Builder2();
            }
        }

        public static class Builder2 {
            public Builder3 writeTimeout(int x, TimeUnit t) {
                assert x == 500;
                assert t == TimeUnit.MILLISECONDS;
                return new Builder3();
            }
        }

        public static class Builder3 {
            public FinalClient build() {
                return new FinalClient();
            }
        }

        public static class FinalClient {}

        public void setClient(FinalClient f) {
            assert lineNo == 4;
            lineNo++;
            assert f != null;
        }

        public Connection open(Object url) {
            assert url == testUrl;
            return testConnection;
        }
    }



    private final static UrlFactory urlFactory = new UrlFactory();


    private final static Object testUrl = new Object();

    private static class MockWebServer {
        private void setServerSocketFactory(DelegatingServerSocketFactory d) throws IOException {
            assert lineNo == 0;
            lineNo++;
            ServerSocket s = new ServerSocket();
            assert d.configureServerSocket(s) == s;
        }


        public void start() {
            assert lineNo == 5;
            lineNo++;
        }

        public void enqueue(Object x) {
            assert lineNo == 6;
            lineNo++;
            assert x == testThrottleBodyRes;
        }

        public U url(String s) {
            assert s.equals("/");
            return new U();
        }

        public static class U {
            public Object url() {
                return testUrl;
            }
        }
    }

    private final static Object testThrottleBodyRes = new Object();

    private static class Connection {
        public void setDoOutput(boolean b) {
            assert b == true;
            assert lineNo == 7;
            assert this == testConnection;
            lineNo++;
        }

        public void setChunkedStreamingMode(int x) {
            assert x == 0;
            assert lineNo == 8;
            assert this == testConnection;
            lineNo++;
        }

        public OutputStream getOutputStream() {
            return testOutputStream;
        }
    }

    private static Connection connection;
    private final static Connection testConnection = new Connection();

    private static class OutputStream {
        public void write(byte[] b) throws SocketTimeoutException {
            assert this == testOutputStream;
            assert lineNo == 9;
            lineNo++;
            assert b.length == 2 * 1024 * 1024;
            throw new SocketTimeoutException();
        }
    }

    private final static OutputStream testOutputStream = new OutputStream();

    public static class SocketTimeoutException extends Exception {}

    private static void fail() {
        assert false;
    }

    private static class MockResponse {
        public Object throttleBody(int a, int b, TimeUnit t) {
            assert a == 1;
            assert b == 1;
            assert t == TimeUnit.SECONDS;
            return testThrottleBodyRes;
        }
    }


#START OF SAMPLE    
    /** Confirm that an unacknowledged write times out. */
    @Test public void writeTimeouts() throws IOException {
        MockWebServer server = new MockWebServer();
        // Sockets on some platforms can have large buffers that mean writes do not block when
        // required. These socket factories explicitly set the buffer sizes on sockets created.
        final int SOCKET_BUFFER_SIZE = 4 * 1024;
        server.setServerSocketFactory(
                new DelegatingServerSocketFactory(ServerSocketFactory.getDefault()) {
                    @Override
                    protected ServerSocket configureServerSocket(ServerSocket serverSocket)
                            throws IOException {
                        serverSocket.setReceiveBufferSize(SOCKET_BUFFER_SIZE);
                        return serverSocket;
                    }
                });
        urlFactory.setClient(urlFactory.client().newBuilder()
                .socketFactory(new DelegatingSocketFactory(SocketFactory.getDefault()) {
                    @Override
                    protected Socket configureSocket(Socket socket) throws IOException {
                        socket.setReceiveBufferSize(SOCKET_BUFFER_SIZE);
                        socket.setSendBufferSize(SOCKET_BUFFER_SIZE);
                        return socket;
                    }
                })
                .writeTimeout(500, TimeUnit.MILLISECONDS)
                .build());

        server.start();
        server.enqueue(new MockResponse()
                .throttleBody(1, 1, TimeUnit.SECONDS)); // Prevent the server from reading!

        connection = urlFactory.open(server.url("/").url());
        connection.setDoOutput(true);
        connection.setChunkedStreamingMode(0);
        OutputStream out = connection.getOutputStream();
        try {
            byte[] data = new byte[2 * 1024 * 1024]; // 2 MiB.
            out.write(data);
            fail();
        } catch (SocketTimeoutException expected) {
        }
    }
#END OF SAMPLE    

    public static void main(String[] args) throws Exception {
        new Sample10().writeTimeouts();
        assert lineNo == 10;
    }

}