package com.keiros.reactor.base;

import com.google.protobuf.Message;

import com.keiros.common.reactor.EventClass.Event;

/**
 * Holds an event and a message (proto).
 */
public class EventAndMessage {
    public Event.Builder event = Event.newBuilder();
    public Message.Builder message = null;
}
