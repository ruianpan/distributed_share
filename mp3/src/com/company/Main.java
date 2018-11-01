package com.company;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Vector;

/*
main function
handles the two threads
offers them empty mebership list to the constructor
 */

public class Main {
    public static void main(String[] args) throws IOException{
        /* delete dir storing the file */
        Runtime rt = Runtime.getRuntime();
        File directory = new File("./SDFS");
        if (!directory.exists()){
            directory.mkdir();
            // If you require it to make the entire directory path including parents,
            // use directory.mkdirs(); here instead.
        }
        else{
            String[] entries = directory.list();
            for(String s: entries){
                File currentFile = new File(directory.getPath(),s);
                //System.out.println(s);
                if(currentFile.isDirectory()){
                    String[] subdir = currentFile.list();
                    for(String f: subdir){
                        File currentsubFile = new File(currentFile.getPath(),f);
                        currentsubFile.delete();
                    }
                    currentFile.delete();
                }
                else
                    currentFile.delete();
            }
        }
        Vector member_list = new Vector();
        Vector ts_list = new Vector();
        String filename = "vm" + Integer.toString(Integer.parseInt(args[0]) + 1) + ".log";
        File f = new File(filename);
        FileWriter fw = new FileWriter(f);
        Hashtable<String, ArrayList<Integer>> file_info = new Hashtable<>();
        Hashtable<String, ArrayList<Integer>> version_dict = new Hashtable<>();
        try{
            Server_thread s1 = new Server_thread(3490, member_list, ts_list, fw, file_info, version_dict);
            Client_thread c1 = new Client_thread(member_list, ts_list, file_info, version_dict);
            receiver_thread r1 = new receiver_thread("", 3495, version_dict);
            Failure_detector_thread fd = new Failure_detector_thread(200, member_list, ts_list, fw);
            r1.start();
            s1.start();
            c1.start();
            fd.start();
        }
        catch(IOException e){}

    }
}
