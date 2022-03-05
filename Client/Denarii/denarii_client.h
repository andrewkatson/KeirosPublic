/*
 * Client code that handles the interaction with the Denarii API on behalf of the caller.
 * See: https://web.getmonero.org/resources/developer-guides/wallet-rpc.html
 */

#ifndef DENARII_CLIENT_H
#define DENARII_CLIENT_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <jni.h>
#include <thread>

#include "curl/curl.h"
#include "include/nlohmann/json.hpp"
#include "src/bigint.h"
#include "src/randomx.h"

#include "Proto/wallet.pb.h"
#include "ProtoBuf/TextProto/read_text_proto.h"
#include "Tools/General/Strings/big_int_from_string.h"
#include "Tools/General/Strings/hex.h"
#include "Tools/General/Strings/replace.h"


#ifdef __cplusplus
extern "C" {
#endif
/**
 * JNI calls used to call the equivalent C++ functions below.
 */
JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_createWallet(JNIEnv *, jobject, jstring);
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_transferMoney(JNIEnv *, jobject, jdouble, jstring, jstring);
JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_getAddress(JNIEnv *, jobject, jstring);
JNIEXPORT jdouble JNICALL Java_com_keiros_client_denarii_DenariiClient_getBalanceOfWallet(JNIEnv *, jobject, jstring);
JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_setCurrentWallet(JNIEnv *, jobject, jstring);
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_getBlockHashingBlob(JNIEnv *, jobject, jstring, jstring);
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_initRandomX(JNIEnv *, jobject, jstring, jstring);
JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_shutdownRandomX(JNIEnv *, jobject);
JNIEXPORT jint JNICALL Java_com_keiros_client_denarii_DenariiClient_attemptMineBlock(JNIEnv *, jobject, jint, jint, jstring, jstring, jlong, jstring);
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_attemptSubmitBlock(JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif


namespace common {

class DenariiClient {
public:

  ~DenariiClient();

  // Sends a command to the wallet rpc or daemon. Params should be a json object in its string representation.
  // This does not work with non json rpcs.
  // See: https://www.getmonero.org/resources/developer-guides/daemon-rpc.html#get_block_template
  // Or: https://web.getmonero.org/resources/developer-guides/wallet-rpc.html
  static void sendCommandToWalletRPC(const std::string& method, const std::string& params, std::string* output);
  static void sendCommandToDaemon(const std::string& method, const std::string& params, std::string* output);
  static void sendCommand(const std::string& ip, const std::string& port, const std::string& method,
      const std::string& params, std::string* output);
  static void sendCommand(const std::string &method, const std::string &params, std::string *output);

  // Creates a wallet with the given wallet info. By default will use English as the language.
  static void createWallet(const common::Wallet& wallet);

  // Transfers the specified amount of money from one wallet to the other.
  static bool transferMoney(double amount, const common::Wallet &sender, const common::Wallet &receiver);

  // Gets an address for the given wallet info. And fills the address field.
  // This assumes you have already set the current wallet.
  // TODO support multiple addresses
  static void getAddress(common::Wallet *wallet);

  // Retrieves the balance of the wallet.
  static double getBalanceOfWallet(const common::Wallet& wallet);

  // Sets the current open wallet to the passed wallet.
  static void setCurrentWallet(const common::Wallet& wallet);

  // Initializes RandomX so it can be used to mine. Returns true on success and false otherwise.
  static bool initRandomX(std::string& mode, char* key, int keySize);

  // Shuts down RandomX.
  static void shutdownRandomX();

  // Attempts to mine a block. This will fail if the block nonce cannot be found after n attempts.
  // If successful then minedBlock will be set. Returns the current nonce.
  // Also specifies the wallet that will receive the funds.
  static int attemptMineBlock(int nonce, int attempts, const std::string& blockHashingBlob, const std::string& blockTemplateBlob,
      uint64_t difficulty, std::string* minedBlock);

  // Packs a nonce into the blob. Be it a block hashing blob or a block template blob.
  static std::string packNonce(const std::string& blob, const std::string& nonce);

  // True if we are meeting the difficulty and false if not.
  static bool meetingDifficulty(const Dodecahedron::Bigint& difficulty, char* hash, int hashSize);

  // Gets a block hashing blob for the current block. True if successful. False otherwise
  static bool getBlockHashingBlob(const Wallet& wallet, nlohmann::json* result);

  // Attempt to submit a mined block. Returns true on success and false otherwise.
  static bool attemptSubmitBlock(const std::string& minedBlock);

private:
  static randomx_cache* mRandomXCache;
  static randomx_vm* mRandomXVM;
  static randomx_dataset* mRandomXDataSet;

  static std::string mMode;
};

} // denarii_services



#endif //DENARII_CLIENT_H
