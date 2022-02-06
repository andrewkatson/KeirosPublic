package com.keiros.example.strings;

public class Convert {

    static {
        System.loadLibrary("convert");
    }

    private static native void something(String some);
    public static void somethingMethod(String some) {
        something(some);
    }
    public static void main(String[] args) {
        Convert.somethingMethod("Some");
    }
}
