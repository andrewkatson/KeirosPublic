package java.com.keiros.client.network;

import com.google.protobuf.ByteString;

import com.keiros.common.network.IPClass.IP;
import com.keiros.reactor.base.EventAndMessage;
import com.keiros.common.reactor.EventClass.Event;
import com.keiros.tools.general.strings.Hex;

public class NetworkingClient {

    static {
        System.loadLibrary("main-jni");
    }

    // Setup this networking client as a client.
    private native boolean setupAsClient(String caCertPath);

    // Setup this networking client as a server.
    private native boolean setupAsServer(String caCertPath, String serverCertPath, String serverKeyPath);

    // Send some event over the wire.
    private native boolean send(String ip, String event);
    public boolean send(IP ip, EventAndMessage event) throws Exception {
        event.event.setStoredEvent(event.message.build().toByteString());
        return send(Hex.getHexString(ip.toByteArray()), Hex.getHexString(event.event.build().toByteArray()));
    }

    // Receive some event from the wire.
    private native boolean receive(String ip, String event);
    public boolean receive(IP ip, EventAndMessage event) throws Exception {
        event.event.setStoredEvent(event.message.build().toByteString());
        String eventString = Hex.getHexString(event.event.build().toByteArray());
        boolean success = receive(Hex.getHexString(ip.toByteArray()), eventString);

        event.event = Event.parseFrom(ByteString.copyFrom(Hex.hexStringToByteArray(eventString))).toBuilder();

        return success;
    }
}
