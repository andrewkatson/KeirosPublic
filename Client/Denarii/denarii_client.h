/*
 * Client code that handles the interaction with the Denarii API on behalf of the caller.
 * See: https://web.getmonero.org/resources/developer-guides/wallet-rpc.html
 */

#ifndef DENARII_CLIENT_H
#define DENARII_CLIENT_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <thread>

#include "absl/memory/memory.h"
#include "curl/curl.h"
#include "gflags/gflags.h"
#include "include/nlohmann/json.hpp"
#include "src/bigint.h"
#include "src/randomx.h"

#include "Proto/wallet.pb.h"
#include "ProtoBuf/TextProto/read_text_proto.h"
#include "System/execute.h"
#include "System/get_pid.h"
#include "Tools/General/Strings/big_int_from_string.h"
#include "Tools/General/Strings/replace.h"

namespace common {

// A flag that can be set pointing to the path to the text proto version of a Wallet
DECLARE_string(walletInfoPath);

// A flag that says where the denariid binary is
DECLARE_string(denariidPath);

// A flag that says where the wallet rpc server binary is
DECLARE_string(walletRpcServerPath);

class DenariiClient {
public:

  ~DenariiClient();

  // Starts up a denarii daemon on local host (127.0.0.1) and port 8424.
  int initDaemon();
  void shutdownDaemon();

  // Connects to the wallet rpc on local host (127.0.0.1) and port 8080.
  // Requires that a wallet file and the password are specified in a text proto file.
  int initWalletRPC();
  void shutdownWalletRPC();

  // Sends a command to the wallet rpc or daemon. Params should be a json object in its string representation.
  // This does not work with non json rpcs.
  // See: https://www.getmonero.org/resources/developer-guides/daemon-rpc.html#get_block_template
  // Or: https://web.getmonero.org/resources/developer-guides/wallet-rpc.html
  void sendCommandToWalletRPC(const std::string& method, const std::string& params, std::string* output);
  void sendCommandToDaemon(const std::string& method, const std::string& params, std::string* output);
  void sendCommand(const std::string& ip, const std::string& port, const std::string& method,
      const std::string& params, std::string* output);
  void sendCommand(const std::string &method, const std::string &params, std::string *output);

  // Creates a wallet with the given wallet info. By default will use English as the language.
  void createWallet(const common::Wallet& wallet);

  // Transfers the specified amount of money from one wallet to the other.
  bool transferMoney(double amount, const common::Wallet &sender, const common::Wallet &receiver);

  // Gets an address for the given wallet info. And fills the address field.
  // TODO support multiple addresses
  void getAddress(common::Wallet *wallet);

  // Retrieves the balance of the wallet.
  double getBalanceOfWallet(const common::Wallet& wallet);

  // Sets the current open wallet to the passed wallet.
  void setCurrentWallet(const common::Wallet& wallet);

  // Initializes RandomX so it can be used to mine. Returns true on success and false otherwise.
  bool initRandomX(std::string& mode, char* key, int keySize);

  // Shuts down RandomX.
  void shutdownRandomX();

  // Attempts to mine a block. This will fail if the block nonce cannot be found after n attempts.
  // If successful then minedBlock will be set. Returns the current nonce.
  // Also specifies the wallet that will receive the funds.
  int attemptMineBlock(int nonce, int attempts, const std::string& blockHashingBlob, const std::string& blockTemplateBlob,
      u_int64_t difficulty, std::string* minedBlock);

  // Packs a nonce into the blob. Be it a block hashing blob or a block template blob.
  static std::string packNonce(const std::string& blob, const std::string& nonce);

  // True if we are meeting the difficulty and false if not.
  bool meetingDifficulty(const Dodecahedron::Bigint& difficulty, char* hash, int hashSize);

  // Gets a block hashing blob for the current block. True if successful. False otherwise
  bool getBlockHashingBlob(const Wallet& wallet, nlohmann::json* result);

  // Attempt to submit a mined block. Returns true on success and false otherwise.
  bool attemptSubmitBlock(const std::string& minedBlock);

private:
  Wallet mWallet;

  std::unique_ptr<std::thread> mDaemon = nullptr;
  std::unique_ptr<std::thread> mWalletRPC = nullptr;

  randomx_cache* mRandomXCache = nullptr;
  randomx_vm* mRandomXVM = nullptr;
  randomx_dataset* mRandomXDataSet = nullptr;

  std::string mMode;

  pid_t mDaemonProcess;
  pid_t mWalletRPCProcess;
};

} // denarii_services



#endif //DENARII_CLIENT_H
