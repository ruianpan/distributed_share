package com.company;

import java.io.IOException;
import java.io.*;
import java.net.*;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Hashtable;

public class receiver_thread extends Thread {
    ServerSocket socket;
    Socket ClientSocket;
    String localname;
    Hashtable<String, ArrayList> vdict;
    public receiver_thread(String input, int port, Hashtable version_dict)throws IOException {
        vdict = version_dict;
        socket = new ServerSocket(port);
        localname = input;
        //System.out.println("Receiver initializing...");
    }

    public void simple_receive() throws IOException{
        ClientSocket = socket.accept();
        int bytesRead;
        InputStream in = ClientSocket.getInputStream();
        DataInputStream clientData = new DataInputStream(in);

        System.out.println("Receiving file: " + localname);
        //fileName = fileName.substring(0,fileName.indexOf('.'))+"_v0"+fileName.substring(fileName.indexOf('.'), fileName.length()-1);
        OutputStream output = new FileOutputStream(localname);
        byte[] buffer = new byte[1024];
        while ((bytesRead = in.read(buffer)) != -1) {
            output.write(buffer, 0, bytesRead);
        }
        //System.out.println("simple receive");
        // Closing the FileOutputStream handle
        in.close();
        clientData.close();
        output.close();
        //ClientSocket.close();
    }

    /* used for reading */
    public void read_receive() throws IOException{
        ClientSocket = socket.accept();
        int bytesRead;
        InputStream in = ClientSocket.getInputStream();
        DataInputStream clientData = new DataInputStream(in);
        String fileName = clientData.readUTF();
        System.out.println("Receiving reading file: " + localname);
        //fileName = fileName.substring(0,fileName.indexOf('.'))+"_v0"+fileName.substring(fileName.indexOf('.'), fileName.length()-1);
        OutputStream output = new FileOutputStream(localname);
        byte[] buffer = new byte[1024];
        while ((bytesRead = in.read(buffer)) != -1) {
            output.write(buffer, 0, bytesRead);
        }
        // Closing the FileOutputStream handle
        //System.out.println("read receive");
        in.close();
        clientData.close();
        output.close();
        //ClientSocket.close();
    }

    public void close() throws IOException{
        socket.close();
    }

    /* used for writing */
    public void run(){
        try {
            while(true){
                //System.out.println("receiver looping");
                ClientSocket = socket.accept();
                int bytesRead;
                InputStream in = ClientSocket.getInputStream();
                DataInputStream clientData = new DataInputStream(in);
                String fileName = clientData.readUTF();
                Long ts = clientData.readLong();
                //System.out.print("receiver rant: ");
                //System.out.println("Timestamp" + ts);
                fileName = fileName.replace("\0", "");
                System.out.println("Receiving writing file: " + fileName);
                String fileNameClean = fileName;


                fileName = "./SDFS/" + fileName;
                //fileName = "./SDFS/" + fileName.substring(0,fileName.indexOf('.'))+"_v0"+fileName.substring(fileName.indexOf('.'), fileName.length()-1);
                OutputStream output = new FileOutputStream(fileName);
                BufferedOutputStream out = new BufferedOutputStream(output);
                byte[] buffer = new byte[1024];
                while ((bytesRead = in.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                    //System.out.println("getting sth");
                }
                out.flush();
                // Closing the FileOutputStream handle
                out.close();
                in.close();
                clientData.close();
                output.close();
                //ClientSocket.close();
                //System.out.println("plain receive");
                if (vdict.containsKey(fileNameClean)){
                    ArrayList oldlist =  vdict.get(fileNameClean);
                    oldlist.add(ts);
                }
                else{
                    File directory = new File("./SDFS/"+fileNameClean+'d');
                    directory.mkdir();
                    //System.out.println(cmdstr);
                    ArrayList list = new ArrayList();
                    list.add(ts);
                    vdict.put(fileNameClean, list);
                }
                String old_path = "./SDFS/"+fileNameClean;
                String new_path = "./SDFS/"+fileNameClean+'d'+'/'+fileNameClean+ts.toString();
                File old = new File(old_path);
                File newf = new File(new_path);
                //System.out.println(new_path);
                Files.copy(old.toPath(), newf.toPath());

            }
        }
        catch(Exception ex){}
    }

}
