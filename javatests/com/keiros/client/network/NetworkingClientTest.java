package com.keiros.client.network;

import static org.junit.Assert.*;

import org.junit.*;

import com.google.protobuf.ByteString;

import com.keiros.common.network.IPClass.IP;
import com.keiros.common.reactor.EventClass.Event;
import com.keiros.client.network.NetworkingClient;
import com.keiros.reactor.base.EventAndMessage;

public class NetworkingClientTest {

    public void before(IP ip) throws Exception {
        NetworkingClient.cleanupWolfssl(ip);
        NetworkingClient.cleanupWolfsslMethod();
    }

    @Test
    public void sendAsServer() throws Exception {
        IP ip = IP.newBuilder().setBlock("127.0.0.1").setPort(8080).build();

        assertTrue(NetworkingClient.setupAsServerMethod("Network/Client/ca.pem", "Network/Client/test-server.pem", "Network/Client/test.key,pem"));

        EventAndMessage eventAndMessage = new EventAndMessage();
        eventAndMessage.message = Event.newBuilder().setStoredEvent(ByteString.copyFromUtf8("something"));

        // We expect this to fail because we have no pending client connections.
        assertFalse(NetworkingClient.send(ip, eventAndMessage));

        before(ip);
    }

    @Test
    public void sendAsClient() throws Exception {
        IP ip = IP.newBuilder().setBlock("127.0.0.1").setPort(8080).build();

        assertTrue(NetworkingClient.setupAsClientMethod("Network/Client/ca.pem"));

        EventAndMessage eventAndMessage = new EventAndMessage();
        eventAndMessage.message = Event.newBuilder().setStoredEvent(ByteString.copyFromUtf8("something"));

        // We expect this to fail because we have no pending client connections.
        assertFalse(NetworkingClient.send(ip, eventAndMessage));

        before(ip);
    }

}
