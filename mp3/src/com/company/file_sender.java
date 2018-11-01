package com.company;

import java.net.*;
import java.io.*;
import java.util.ArrayList;

public class file_sender extends Thread {
    private Socket socket;
    private File tosend;
    private  String fn;
    private Long ts;
    public int flag = 0;
    ArrayList<String> filelist;
    public file_sender(InetAddress b,  int port, ArrayList<String> as) throws IOException{
        socket = new Socket(b, port);
        filelist = as;
    }

    public file_sender(InetAddress b, String localname, String newname, int port) throws IOException{
        socket = new Socket(b, port);
        fn = newname;
        System.out.println("sending file "+localname + " to " + b);
        tosend = new File(localname);
    }

    public file_sender(InetAddress b, String localname, String newname, int port, Long timestamp) throws IOException{
        socket = new Socket(b, port);
        fn = newname;
        System.out.println("sending file "+localname + " to " + b);
        tosend = new File(localname);
        ts = timestamp;
    }

    public void sendfiles(ArrayList<String> list)throws IOException{
        for(int i = 0; i<list.size();i++){
            tosend = new File(list.get(i));
            byte[] mybytearray = new byte[(int) tosend.length()];
            FileInputStream fis = new FileInputStream(tosend);
            BufferedInputStream bis = new BufferedInputStream(fis);
            bis.read(mybytearray, 0, mybytearray.length);


            OutputStream os = socket.getOutputStream();

            String delimiter = "Version " + Integer.toString(i + 1) + ":\n";
            os.write(delimiter.getBytes());
            os.write(mybytearray, 0, mybytearray.length);
            os.flush();
            System.out.println("File sent");
        }
        socket.close();
        System.out.println("All Done");

    }

    public void sendts() throws IOException{
        //System.out.println("sending file " + tosend.getName());
        //fn+='\n';
        byte[] mybytearray = new byte[(int) tosend.length()];

        FileInputStream fis = new FileInputStream(tosend);
        BufferedInputStream bis = new BufferedInputStream(fis);
        bis.read(mybytearray, 0, mybytearray.length);

        OutputStream os = socket.getOutputStream();
        DataOutputStream dos = new DataOutputStream(os);
        dos.writeUTF(fn);
        dos.writeLong(ts);
        dos.flush();
        os.write(mybytearray, 0, mybytearray.length);
        os.flush();

        fis.close();
        bis.close();
        os.close();
        dos.close();
        socket.close();
        System.out.println("File sent");
    }


    public void send() throws IOException{
        //System.out.println("sending file " + tosend.getName());
        //fn+='\n';
        byte[] mybytearray = new byte[(int) tosend.length()];

        FileInputStream fis = new FileInputStream(tosend);
        BufferedInputStream bis = new BufferedInputStream(fis);
        bis.read(mybytearray, 0, mybytearray.length);


        OutputStream os = socket.getOutputStream();
        DataOutputStream dos = new DataOutputStream(os);

        dos.writeUTF(fn);
        dos.flush();

        os.write(mybytearray, 0, mybytearray.length);
        os.flush();

        fis.close();
        bis.close();
        os.close();
        dos.close();
        socket.close();
        System.out.println("File sent");
    }

    public void run() {
        if (flag == 0) {
            System.out.println("run flag 0");
            try {
                send();
            } catch (IOException e) {
            }
        }
        else{
            try {
                System.out.println("run flag 1");
                sendfiles(filelist);
            } catch (IOException e) {
            }

        }
    }
}
