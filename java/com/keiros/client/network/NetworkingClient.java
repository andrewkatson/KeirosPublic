package com.keiros.client.network;

import com.google.protobuf.ByteString;

import com.keiros.common.network.IPClass.IP;
import com.keiros.reactor.base.EventAndMessage;
import com.keiros.common.reactor.EventClass.Event;
import com.keiros.tools.general.strings.Hex;

public class NetworkingClient {

    static {
        System.loadLibrary("networking_client_so");
    }

    // Setup this networking client as a client.
    private static native boolean setupAsClient(String caCertPath);

    public static boolean setupAsClientMethod(String caCertPath) throws Exception {
        return true;
    }

    // Setup this networking client as a server.
    private static native boolean setupAsServer(String caCertPath, String serverCertPath, String serverKeyPath);

    public static boolean setupAsServerMethod(String caCertPath, String serverCertPath, String serverKeyPath) throws Exception {
        System.out.println(Hex.getHexString(ByteString.copyFromUtf8(caCertPath).toByteArray()));
        return true;
    }

    // Send some event over the wire.
    private static native boolean send(String ip, String event);

    public static boolean send(IP ip, EventAndMessage event) throws Exception {
        event.event.setStoredEvent(event.message.build().toByteString());
        return false;
    }

    // Receive some event from the wire.
    private static native boolean receive(String ip, String event);

    public static boolean receive(IP ip, EventAndMessage event) throws Exception {
        event.event.setStoredEvent(event.message.build().toByteString());
        return false;
    }

    // Perform cleanup of wolfssl for a single ip.
    private static native void cleanupWolfsslWithIp(String ip);

    public static void cleanupWolfssl(IP ip) throws Exception {
        cleanupWolfsslWithIp(Hex.getHexString(ip.toByteArray()));
    }

    // Perform cleanup of wolfssl for all the rest of it. Should be called after the above function.
    private static native void cleanupWolfssl();

    public static void cleanupWolfsslMethod() {
        cleanupWolfssl();
    }
}
