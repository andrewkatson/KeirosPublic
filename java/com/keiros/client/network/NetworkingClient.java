package com.keiros.client.network;

import com.google.protobuf.ByteString;

import com.keiros.common.network.IPClass.IP;
import com.keiros.common.reactor.EventClass.Event;
import com.keiros.reactor.base.EventAndMessage;
import com.keiros.tools.general.strings.Hex;

public class NetworkingClient {

    static {
        System.loadLibrary("networking_client_so");
    }

    /**
     * Setup this networking client as a client.
     * @param caCertPath The path to the CA certificate of the server this client connects to
     * @return true on success and false otherwise
     */
    private static native boolean setupAsClient(String caCertPath);

    public static boolean setupAsClientMethod(String caCertPath) throws Exception {
        return setupAsClient(Hex.getHexString(ByteString.copyFromUtf8(caCertPath).toByteArray()));
    }

    /**
     * Setup this networking client as a server.
     * @param caCertPath The path to the CA certificate of the server this client connects to
     * @param serverCertPath The path to the server's certificate
     * @param serverKeyPath the path to the server's private key
     * @return true on success and false otherwise
     */
    private static native boolean setupAsServer(String caCertPath, String serverCertPath, String serverKeyPath);

    public static boolean setupAsServerMethod(String caCertPath, String serverCertPath, String serverKeyPath) throws Exception {
        return setupAsServer(Hex.getHexString(ByteString.copyFromUtf8(caCertPath).toByteArray()),
                Hex.getHexString(ByteString.copyFromUtf8(serverCertPath).toByteArray()),
                Hex.getHexString(ByteString.copyFromUtf8(serverKeyPath).toByteArray()));
    }

    /**
     * Send some event over the wire.
     * @param ip The ip and port of the place to send to
     * @param event The hex encoded byte string representation of the event.
     * @return true on success and false otherwise
     */
    private static native boolean send(String ip, String event);

    public static boolean send(IP ip, EventAndMessage event) throws Exception {
        event.event.setStoredEvent(event.message.build().toByteString());
        return send(Hex.getHexString(ip.toByteArray()), Hex.getHexString(event.event.build().toByteArray()));
    }

    /**
     * Receive some event over the wire.
     * @param ip The ip and port of the place to send to
     * @param event The hex encoded byte string representation of the event.
     * @return true on success and false otherwise
     */
    private static native boolean receive(String ip, String event);

    public static boolean receive(IP ip, EventAndMessage event) throws Exception {
        String receivedEvent = "";
        boolean success =  receive(Hex.getHexString(ip.toByteArray()), receivedEvent);
        event.event = event.event.build().parseFrom(Hex.hexStringToByteArray(receivedEvent)).toBuilder();
        return success;
    }

    /**
     * Perform cleanup of wolfssl for a single ip.
     * @param ip The ip and port connection to cleanup
     */
    private static native void cleanupWolfsslWithIp(String ip);

    public static void cleanupWolfssl(IP ip) throws Exception {
        cleanupWolfsslWithIp(Hex.getHexString(ip.toByteArray()));
    }

    /**
     * Perform cleanup of wolfssl for all the rest of it. Should be called after the above function.
     */
    private static native void cleanupWolfssl();

    public static void cleanupWolfsslMethod() {
        cleanupWolfssl();
    }
}
