package com.company;

import java.net.*;
import java.io.*;
import java.time.Clock;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Vector;

/*
this is the client thread
this waits for user input and send messages to other machines
it detects the membership list when client
 */

public class Client_thread extends Thread{
    private Vector<InetAddress> member_list; //membership list
    private Vector<Long> ts_list;            //timestamp list
    private DatagramSocket client;
    private InetAddress serverAddr;
    public String[] ip_array;
    private long timestamp;
    private Hashtable<String, ArrayList<Integer>> file_info;
    private Hashtable<String, ArrayList<Long>> vdict;

    /*
    Client_thread(Vector target, Vector target_ts)
    this is the client thread contructor
    initializes the introducer IP address
     */
    public Client_thread(Vector target, Vector target_ts, Hashtable f_info, Hashtable version_dict) throws IOException{
        file_info = f_info;
        member_list = target;
        ts_list = target_ts;
        timestamp = ts_list.size() > 0 ? ts_list.get(0) : 0;
        vdict = version_dict;
        int port = 3491;
        client = new DatagramSocket(port);
        ip_array = new String[]{"172.22.156.75", "172.22.158.75", "172.22.154.76", "172.22.156.76", "172.22.158.76",
                "172.22.154.77", "172.22.156.77", "172.22.158.77", "172.22.154.78", "172.22.156.78"};
        try{
            serverAddr = InetAddress.getByName("172.22.156.75"); //vm1 ip address
        }
        catch (UnknownHostException ex) {
            System.out.print("unknown exception");
        }
    }

    /*
    run()
    run function of client thread
    detects user command line input
    sends the raw join/leave messages to other vms
     */

    public void run(){
        while(true){
            try{
                BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
                String kbInput = br.readLine();
                byte[] msg = kbInput.getBytes();

                synchronized (member_list){            //list command, print out membership list
                    if(kbInput.equals("list")){
                        if(member_list.size()!=0){
                            System.out.println("List: ");
                            for(int i = 0; i < member_list.size(); i++){
                                System.out.println("IP: " + member_list.get(i) + ", timestamp: " + ts_list.get(i));
                            }
                            continue;
                        }
                        else{
                            System.out.println("Not in group");
                            continue;
                        }
                    }
                }

                if(kbInput.equals("id")){               //command line for showing id
                    if(member_list.size()!=0){
                        try(final DatagramSocket socket = new DatagramSocket()){
                            socket.connect(InetAddress.getByName("8.8.8.8"), 10002);
                            String ip = socket.getLocalAddress().getHostAddress();
                            System.out.println("IP address: " + ip);
                        }
                        System.out.println("Time stamp: " + timestamp);
                        continue;
                    }
                    else{
                        System.out.println("Not in group");
                        continue;
                    }
                }

                if(kbInput.equals("fileinfo")){
                    System.out.println(file_info);
                    continue;
                }

                String ip = null;                      //get your own ip address
                try(final DatagramSocket socket = new DatagramSocket()){
                    socket.connect(InetAddress.getByName("8.8.8.8"), 10002);
                    ip = socket.getLocalAddress().getHostAddress();
                }
                catch(SocketException e){}
                catch(UnknownHostException e){}

                if(kbInput.equals("initiate")){
                    Clock clk = Clock.systemDefaultZone();
                    member_list.addElement(InetAddress.getByName(ip));
                    ts_list.add(clk.millis());
                    continue;
                }

                synchronized (member_list){
                    if(kbInput.equals("join") && member_list.size()==0){
                        for(int k = 0; k < 10; k++){
                            serverAddr = InetAddress.getByName(ip_array[k]);
                            DatagramPacket packet = new DatagramPacket(msg, msg.length, serverAddr, 3490);
                            client.send(packet);
                            client.setSoTimeout(100);
                            byte[] reply = new byte[161];                                       //check if size is 0 to avoid bad request
                            packet = new DatagramPacket(reply, reply.length);
                            try{
                                client.receive(packet);
                            }
                            catch(IOException ex){
                                continue;
                            }

                            String recved = new String(packet.getData(), 0,160);
                            if(recved.substring(0,5).equals("notin"))
                                continue;
                            else{
                                /* Start receiving response from client */
                                int lindex = recved.indexOf('l');
                                int tindex = recved.indexOf('t');
                                for(int i = 0; i <tindex; i++){
                                    char dig = recved.charAt(i);
                                    if(Character.isDigit(dig)) {                           //tell if is digit
                                        int index = Character.getNumericValue(recved.charAt(i));
                                        String ip_string = ip_array[index];
                                        member_list.addElement(InetAddress.getByName(ip_string));      //add the element
                                    }
                                    else{
                                        break;
                                    }
                                }
                                for(int i = tindex + 1; i < lindex; i += 13){               //get the timestamp list elements
                                    String ts = new String();
                                    for(int j = i; j < i + 13; j++){
                                        char dig = recved.charAt(j);
                                        if(Character.isDigit(dig)){
                                            ts += dig;
                                        }
                                        else{
                                            break;
                                        }
                                    }
                                    ts_list.addElement(Long.valueOf(ts));
//
                                }
                                String longs = "";       //get your own timestamp string from the server
                                for(int i = lindex+1; i<recved.length(); i++){
                                    char dig = recved.charAt(i);
                                    if(Character.isDigit(dig)){
                                        longs+=dig;
                                    }
                                    else{
                                        break;
                                    }
                                }
                                timestamp = Long.valueOf(longs);  //give timestamp value
                                break;
                            }
                        }

                    }
                }

                if(kbInput.equals("leav")){                //when list is 2, use the address that is not your own ip
                    String list0 = member_list.get(0).getHostAddress();
                    if(!list0.equals(ip))
                        serverAddr =member_list.get(0);
                    else
                        serverAddr =member_list.get(1);
                }

                //get-versions sdsfn num localfn
                if (kbInput.split("\\s+")[0] .equals("get-versions")){
                    String sdsfilename = kbInput.split("\\s+")[1];
                    int num = Integer.parseInt(kbInput.split("\\s+")[2]);
                    String localfilename = kbInput.split("\\s+")[3];
                    if(file_info.containsKey(sdsfilename)){
                        ArrayList<Integer> list = file_info.get(sdsfilename);
                        Integer temp = list.get(0);
                        InetAddress vmaster = InetAddress.getByName(ip_array[temp]);
                        System.out.println(vmaster);
                        String sending = "ver"+sdsfilename+'\n'+Integer.toString(num);
                        msg = sending.getBytes();
                        DatagramPacket packet = new DatagramPacket(msg, msg.length, vmaster, 3490);
                        client.send(packet);
                        receiver_thread recv = new receiver_thread(localfilename, 3496, vdict);
                        recv.simple_receive();
                        recv.close();
                    }
                    else{
                        System.out.println("Dou Nidaye Wanne?");
                    }
                    continue;
                }


                /* Handle cases of file operations */
                if(kbInput.length() > 3){
                    String op = kbInput.split("\\s+")[0];
                    if(op.equals("put") || op.equals("get") || op.equals("delete")){
                        serverAddr = member_list.get(0);        //route message to master(first on the list)
                        System.out.println("sending file op to " +serverAddr + " port number " + 3490);
                        DatagramPacket packet = new DatagramPacket(msg, msg.length, serverAddr, 3490);
                        client.send(packet);
                        if(op.equals("put")){
                            byte[] reply = new byte[25];
                            packet = new DatagramPacket(reply, reply.length);
                            client.receive(packet);
                            String rep = new String(packet.getData());
                            rep = rep.replace("\0", "");

                            String localname = kbInput.split("\\s+")[1];
                            String sdfsname = kbInput.split("\\s+")[2];

                            String tss = rep.substring(rep.indexOf('n')+1);
                            long ts = Long.parseLong(tss);
                            System.out.println("rep: "+ rep);

                            for(int i = 0; i < rep.indexOf('n'); i++){
                                //System.out.println("Line" + i + " has " + rep.charAt(i));
                                InetAddress target = InetAddress.getByName(ip_array[Character.getNumericValue(rep.charAt(i))]);
                                file_sender fs = new file_sender(target, localname, sdfsname, 3495,ts);
                                fs.sendts();
                            }
                            System.out.println("writing done");
                        }
                        if(op.equals("get")){
                            byte[] reply = new byte[10];
                            packet = new DatagramPacket(reply, reply.length);
                            client.receive(packet);
                            String rep = new String(packet.getData());
                            rep = rep.replace("\0", "");

                            String sdfsname = kbInput.split("\\s+")[1];
                            String localname = kbInput.split("\\s+")[2];
                            int num_rep = member_list.size() >= 4 ? 4 : member_list.size();
                            //System.out.println(rep);
                            if(rep.equals("-1")){
                                System.out.println("File doesn't exist.");
                                continue;
                            }
                            receiver_thread recv = new receiver_thread(localname, 3494, vdict);
                            for(int i = 0; i < rep.length(); i++){
                                //System.out.println("Receiving from " + rep.charAt(i));
                                InetAddress target = InetAddress.getByName(ip_array[Character.getNumericValue(rep.charAt(i))]);
                                String sending = "get_rep " + sdfsname;
                                msg = sending.getBytes();
                                packet = new DatagramPacket(msg, msg.length, target, 3490);
                                client.send(packet);
                                recv.read_receive();
                                //System.out.println("Received from " + rep.charAt(i));
                            }
                            recv.close();
                            System.out.println("reading done");
                        }
                        continue;
                    }
                }

                //code just changed
                //System.out.println("sending to " +serverAddr + " port number " + 3490);
                DatagramPacket packet = new DatagramPacket(msg, msg.length, serverAddr, 3490);

                client.send(packet);

                if(kbInput.equals("leav")){    //type in leave, flush your membership list
                    member_list.clear();
                    ts_list.clear();
                }
            }
            catch(IOException e){
                System.out.println("IOException");
                e.printStackTrace();
            }
        }
    }
}
