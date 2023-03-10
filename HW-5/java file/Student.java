import java.io.*;
import java.net.*;
import java.util.*;

public class Student {

    // write a method that connects to a server here.
    int studentId;
    int partnerId;

    public Student(int id) {
        studentId = id;
    }

    void findPartner() throws UnknownHostException, IOException {
        Socket socket = new Socket("localhost", 9000);
        System.out.println("Student " + studentId + " connected to server");

        DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
        dataOutputStream.writeInt(studentId);
        dataOutputStream.flush();

        DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());
        // partnerId = dataInputStream.readInt();

        System.out.println("Student " + studentId + " is partnered with " +
                partnerId);

        dataInputStream.close();
        dataOutputStream.close();
        socket.close();

    }

    public static void main(String[] args) throws UnknownHostException, IOException {
        int numbStudents = Integer.parseInt(args[0]);
        for (int i = 1; i <= numbStudents; i++) {
            Student student = new Student(i);
            student.findPartner();
        }
    }
}
