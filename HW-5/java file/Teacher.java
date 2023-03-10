import java.io.*;
import java.net.*;
import java.util.*;

public class Teacher {

    public static void main(String[] args) throws IOException {

        int port = 9000;
        ServerSocket serverSocket = new ServerSocket(port);
        System.out.println("Teacher is started");

        int numbStudents = Integer.parseInt(args[0]);
        int counter = 0;
        while (counter < numbStudents) {
            Socket socket1 = serverSocket.accept();
            System.out.println("Student connected " + socket1);
            DataInputStream dataInputStream1 = new DataInputStream(socket1.getInputStream());
            System.out.println("student " + dataInputStream1.readInt() + " is connected to server");

            Socket socket2 = serverSocket.accept();
            System.out.println("Student connected " + socket2);
            DataInputStream dataInputStream2 = new DataInputStream(socket2.getInputStream());
            System.out.println("student " + dataInputStream2.readInt() +
                    " is connected to server");

            counter += 1;

        }
    }
}