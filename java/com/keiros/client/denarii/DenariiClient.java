package com.keiros.client.denarii;

import com.google.protobuf.ByteString;

import com.keiros.common.WalletClass.Wallet;
import com.keiros.common.reactor.EventClass.Event;
import com.keiros.reactor.base.EventAndMessage;
import com.keiros.tools.general.strings.Hex;

public class DenariiClient {
    static {
        System.loadLibrary("denarii_client_so");
    }

    /**
     * Create a Denarii wallet
     * @param wallet The hex encoded string representation of the wallet to create.
     */
    private static native boolean createWallet(String wallet);

    public static boolean createWallet(Wallet wallet) throws Exception {
        return createWallet(Hex.getHexString(wallet.toByteArray()));
    }

    /**
     * Transfer some money between two wallets.
     * @param amount The amount to transfer
     * @param sender The hex encoded string representation of the sender wallet
     * @param receiver The hex encoded string representation of the receiver wallet
     * @return True on success and false otherwise
     */
    private static native boolean transferMoney(Double amount, String sender, String receiver);

    public static boolean transferMoney(double amount, Wallet sender, Wallet receiver) throws Exception {
        return transferMoney(amount, Hex.getHexString(sender.toByteArray()), Hex.getHexString(receiver.toByteArray()));
    }

    /**
     * Get the primary address for the wallet set as the current wallet. Assumes setCurrentWallet has already been called.
     * @param wallet The hex encoded string representation of the wallet passed back.
     */
    private static native boolean getAddress(String wallet);

    public static boolean getAddress(Wallet wallet) throws Exception {
        String hexWallet = "";
        boolean success = getAddress(hexWallet);
        wallet.parseFrom(Hex.hexStringToByteArray(hexWallet));
        return  success;
    }

    /**
     * Get the balance of the wallet passed.
     * @param wallet The hex encoded string representation of the wallet to get the value of.
     * @return The amount of coin in the wallet.
     */
    private static native double getBalanceOfWallet(String wallet);

    public static double getBalanceOfWallet(Wallet wallet) throws Exception {
        return getBalanceOfWallet(Hex.getHexString(wallet.toByteArray()));
    }

    /**
     * Set the current wallet as the one specified.
     * @param wallet The hex encoded string representation of the wallet to get the value of.
     */
    private static native boolean setCurrentWallet(String wallet);

    public static boolean setCurrentWallet(Wallet wallet) throws Exception {
        return setCurrentWallet(Hex.getHexString(wallet.toByteArray()));
    }

    /**
     * Get the block hashing blob and other information about the current block.
     * @param wallet The wallet to attribute any funds from the current block.
     * @param result The json string representation of the information from the response.
     * @return True on success and false otherwise
     */
    private static native boolean getBlockHashingBlob(String wallet, String result);

    public static boolean getBlockHashingBlob(Wallet wallet, StringBuilder result) throws Exception {
        String resultStr = result.toString();
        boolean success = getBlockHashingBlob(Hex.getHexString(wallet.toByteArray()), resultStr);
        result.append(ByteString.copyFrom(Hex.hexStringToByteArray(resultStr)).toStringUtf8());
        return success;
    }

    /**
     * Initialize randomX hashing algorithm
     * @param mode The mode to be in. either "fast" or anything else for "light" mode
     * @param key the byte key used to initialize the algorithm. Can be any length (we think).
     * @return True on success and false otherwise.
     */
    private static native boolean initRandomX(String mode, String key);

    public static boolean initRandomXMethod(String mode, String key) {
        return initRandomX(mode, key);
    }

    /**
     * Shutdown randomX hashing algorithm.
     */
    private static native void shutdownRandomX();

    public static void shutdownRandomXMethod() {
        shutdownRandomX();
    }

    /**
     * Attempt to mine a block.
     * @param nonce The current nonce
     * @param attempts The number of attempts so far
     * @param blockHashingBlob The block hashing blob
     * @param blockTemplateBlob The block template blob
     * @param difficulty The difficulty being mined at
     * @param minedBlock The string to return the newly mined block
     * @return The new nonce to use next round
     */
    private static native int attemptMineBlock(Integer nonce, Integer attempts, String blockHashingBlob,
                                               String blockTemplateBlob, Long difficulty, String minedBlock);

    public static int attemptMineBlock(int nonce, int attempts, String blockHashingBlob, String blockTemplateBlob,
                                       long difficulty, StringBuilder minedBlock) throws  Exception {

        String minedBlockStr = "";
        int newNonce = attemptMineBlock(nonce, attempts, Hex.getHexString(blockHashingBlob.getBytes()),
                Hex.getHexString(blockTemplateBlob.getBytes()), difficulty, minedBlockStr);
        minedBlock.append(ByteString.copyFrom(Hex.hexStringToByteArray(minedBlockStr)).toStringUtf8());
        return newNonce;
    }

    /**
     * Attempt to submit a block that was mined.
     * @param minedBlock The newly mined block.
     * @return True on success and false otherwise.
     */
    private static native boolean attemptSubmitBlock(String minedBlock);

    public static boolean attemptSubmitBlockMethod(String minedBlock) throws  Exception {
        return attemptSubmitBlock(Hex.getHexString(minedBlock.getBytes()));
    }

    /**
     * Restore a deterministic wallet. Assumes name and password and phrase are set.
     * @param wallet The wallet to be restored.
     * @return True on succes and false otherwise.
     */
    private static native boolean restoreWallet(String wallet);

    public static boolean restoreWallet(Wallet wallet) throws Exception {
        String walletStr = Hex.getHexString(wallet.toByteArray());

        boolean success = restoreWallet(walletStr);

        wallet.parseFrom(Hex.hexStringToByteArray(walletStr));

        return success;
    }

    /**
     * Get the mnemonic seed for a wallet. Assumes current wallet has been set,
     * @param wallet The wallet to be store the seed phrase for.
     * @return True on success and false otherwise.
     */
    private static native boolean querySeed(String wallet);

    public static boolean querySeed(Wallet wallet) throws Exception {
        String walletStr = Hex.getHexString(wallet.toByteArray());

        boolean success = querySeed(walletStr);

        wallet.parseFrom(Hex.hexStringToByteArray(walletStr));

        return success;
    }

    /**
     * Creates a sub address for the current wallet with no label.
     * @param wallet The wallet to store the sub address in
     * @return True on success and false otherwise
     */
    private static native boolean createNoLabelAddress(String wallet);

    public static boolean createAddress(Wallet wallet) throws Exception {
        String walletStr = Hex.getHexString(wallet.toByteArray());

        boolean success = createNoLabelAddress(walletStr);

        wallet.parseFrom(Hex.hexStringToByteArray(walletStr));

        return success;
    }

    /**
     * Creates a sub address for the current wallet with a label.
     * @param label The label to give the wallet
     * @param wallet The wallet to store the sub address in
     * @return True on success and false otherwise
     */
    private static native boolean createAddress(String label, String wallet);

    public static boolean createAddress(String label, Wallet wallet) throws Exception {
        String walletStr = Hex.getHexString(wallet.toByteArray());

        boolean success = createAddress(label, walletStr);

        wallet.parseFrom(Hex.hexStringToByteArray(walletStr));

        return success;
    }

    /**
     * Start the mining process.
     * @param doBackgroundMining Whether to mine in the background
     * @param ignoreBattery Whether to ignore the battery
     * @param threads The number of threads to use
     * @return True on success and false otherwise
     */
    private static native boolean startMining(Boolean doBackgroundMining, Boolean ignoreBattery, Integer threads);

    public static boolean startMiningMethod(Boolean doBackgroundMining, Boolean ignoreBattery, Integer threads) {
        return startMining(doBackgroundMining, ignoreBattery, threads);
    }

     /**
     * Stop the mining process.
     * @return True on success and false otherwise
     */
    private static native boolean stopMining();

    public static boolean stopMiningMethod() {
        return stopMining();
    }
}
