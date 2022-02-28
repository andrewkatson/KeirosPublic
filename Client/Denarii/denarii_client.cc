#include "denarii_client.h"

using Dodecahedron::Bigint;
using nlohmann::json;

namespace common {

DEFINE_string(walletInfoPath, "", "An absolute file path to a text proto of a Wallet.");

DEFINE_string(denariidPath, "", "An absolute file path to the denarii daemon binary.");

DEFINE_string(walletRpcServerPath, "", "An absolute file path to the denarii wallet rpc server binary");

namespace {
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *) userp)->append((char *) contents, size * nmemb);
  return size * nmemb;
}

} // namespace

DenariiClient::~DenariiClient() = default;

int DenariiClient::initDaemon() {

  // This will make the path an executable command
  const std::string &command = FLAGS_denariidPath;

  // start the process in the background
  std::cout << command << std::endl;


  auto execCommand = [=]() { system(command.c_str()); };
  mDaemon = absl::make_unique<std::thread>(execCommand);
  mDaemon->detach();
  return 0;
}

void DenariiClient::shutdownDaemon() {

  std::vector<int> pids;
  getPid("denariid", &pids);

  for (auto &pid : pids) {
    std::cout << " PID \n \n \n" << pid << std::endl;
    kill(pid, SIGINT);
  }
}

int DenariiClient::initWalletRPC() {

  protobuf::keiros::readTextProto(FLAGS_walletInfoPath, &mWallet);

  // This will make the path an executable command with the needed flags.
  auto userPassword = mWallet.user() + ":" + mWallet.user_password();

  std::string command =
      FLAGS_walletRpcServerPath + " --rpc-bind-port=8080 --wallet-dir=" + "/denarii/wallet" +
      " --rpc-login=" + userPassword;

  std::cout << command << std::endl;

  auto execCommand = [=]() { system(command.c_str()); };
  mWalletRPC = absl::make_unique<std::thread>(execCommand);
  mWalletRPC->detach();
  return 0;
}

void DenariiClient::shutdownWalletRPC() {

  std::vector<int> pids;
  getPid("denarii_wallet", &pids);

  for (auto &pid : pids) {
    std::cout << " PID \n \n \n" << pid << std::endl;
    kill(pid, SIGINT);
  }
}

void DenariiClient::sendCommandToWalletRPC(const std::string &method, const std::string &params, std::string *output) {
  sendCommand("http://127.0.0.1", "8080", method, params, output);
}

void DenariiClient::sendCommandToDaemon(const std::string &method, const std::string &params, std::string *output) {
  sendCommand("http://127.0.0.1", "8424", method, params, output);
}

// See: https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
// See: https://gist.github.com/leprechau/e6b8fef41a153218e1f4
void DenariiClient::sendCommand(const std::string &method, const std::string &params, std::string *output) {

  // This is the old method of doing things so we will save it for backwards compatibility. In this we are default
  // sending to the wallet rpc.
  sendCommandToWalletRPC(method, params, output);
}

void DenariiClient::sendCommand(const std::string &ip, const std::string &port, const std::string &method,
                                const std::string &params, std::string *output) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, ip + ":" + port + "/json_rpc");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    const std::string &postField = R"({"jsonrpc":"2.0","id":"0","method":")" + method +
                                   R"(","params":)" + params + "}";

    std::cout << postField << std::endl;
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postField.c_str());

    auto userPassword = mWallet.user() + ":" + mWallet.user_password();

    std::cout << userPassword << std::endl;

    // Set the authorization to use the username and password.
    curl_easy_setopt(curl, CURLOPT_USERNAME, mWallet.user().c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, mWallet.user_password().c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);

    // Give a call back function and a string to write the callback data into.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, true);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);

    // Set the content type to be application json.
    struct curl_slist *hs = nullptr;
    hs = curl_slist_append(hs, "Accept: application/json");
    hs = curl_slist_append(hs, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  // This is the old command we were using. Going to save for posterity until it gets pushed to source repo.
  const std::string &command =
      R"(curl -u )" + mWallet.user() + ":" + mWallet.user_password() +
      R"( --digest http://127.0.0.1:8080/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":")" + method +
      R"(","params":)" +
      params + R"(}' -H 'Content-Type: application/json')";


  std::cout << command << std::endl;
  std::cout << readBuffer << std::endl;
  *output = readBuffer;
}


void DenariiClient::createWallet(const common::Wallet &wallet) {

  json input;
  input["filename"] = wallet.name();
  input["password"] = wallet.password();
  input["language"] = "English";

  // There is no output expected
  std::string output;
  sendCommand("create_wallet", input.dump(), &output);

}

void DenariiClient::getAddress(common::Wallet *wallet) {


  json input;
  input["account_index"] = 0;

  std::string outputStr;
  sendCommand("get_address", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return;
  }

  json result = input["result"];

  if (result.contains("address")) {
    wallet->set_address(result["address"]);
  }
}

bool DenariiClient::transferMoney(double amount, const common::Wallet &sender, const common::Wallet &receiver) {

  double senderCurrentAmount = getBalanceOfWallet(sender);

  if (senderCurrentAmount < amount) {
    return false;
  }

  // First set the current wallet to the senders wallet.
  setCurrentWallet(sender);

  // Then transfer the money.
  json input;

  input["destinations"] = {{amount, receiver.address()}};

  std::string outputStr;
  sendCommand("transfer", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }

  json result = output["result"];
  // We know its successfull
  return result.contains("amount");
}

double DenariiClient::getBalanceOfWallet(const common::Wallet &wallet) {

  // First we set the current wallet.
  setCurrentWallet(wallet);

  // Then we get the balance.
  json input;

  input["account_index"] = 0;

  std::string outputStr;
  sendCommand("get_balance", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return 0.0;
  }

  json result = output["result"];
  if (!result.contains("balance")) {
    return 0.0;
  }

  return result["balance"];
}

void DenariiClient::setCurrentWallet(const common::Wallet &wallet) {

  json input;

  input["filename"] = wallet.name();
  if (!wallet.password().empty()) {
    input["password"] = wallet.password();
  }

  std::string outputStr;
  sendCommand("open_wallet", input.dump(), &outputStr);

  // There is no output expected so we do nothing with it.
  json output = json::parse(outputStr);
}


bool DenariiClient::initRandomX(std::string &mode, char *key, int keySize) {
  randomx_flags flags = randomx_get_flags();

  boost::to_lower(mode);

  mMode = mode;
  if (mode == "fast") {
    flags |= RANDOMX_FLAG_LARGE_PAGES;
    flags |= RANDOMX_FLAG_FULL_MEM;
  }

  try {
    mRandomXCache = randomx_alloc_cache(flags);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    mRandomXCache = nullptr;
  } catch (std::exception *e) {
    std::cout << e->what() << std::endl;
    mRandomXCache = nullptr;
  }

  if (mRandomXCache == nullptr) {

    if (mode == "fast") {
      flags = randomx_get_flags();
      flags |= RANDOMX_FLAG_FULL_MEM;
      mRandomXCache = randomx_alloc_cache(flags);
    }

    if (mRandomXCache == nullptr) {
      return false;
    }
  }

  randomx_init_cache(mRandomXCache, key, keySize);

  if (mMode == "fast") {
    mRandomXDataSet = randomx_alloc_dataset(flags);

    if (mRandomXDataSet == nullptr) {
      return false;
    }

    auto datasetItemCount = randomx_dataset_item_count();
    std::thread t1(&randomx_init_dataset, mRandomXDataSet, mRandomXCache, 0, datasetItemCount / 2);
    std::thread t2(&randomx_init_dataset, mRandomXDataSet, mRandomXCache, datasetItemCount / 2,
                   datasetItemCount - datasetItemCount / 2);
    t1.join();
    t2.join();
    randomx_release_cache(mRandomXCache);

  }

  if (mMode == "fast") {
    mRandomXVM = randomx_create_vm(flags, nullptr, mRandomXDataSet);
  } else {
    mRandomXVM = randomx_create_vm(flags, mRandomXCache, NULL);
  }

  if (mRandomXVM == nullptr) {
    return false;
  }

  return true;
}

void DenariiClient::shutdownRandomX() {

  if (mRandomXVM != nullptr) {
    randomx_destroy_vm(mRandomXVM);

    if (mMode == "fast") {
      randomx_release_dataset(mRandomXDataSet);
    } else {
      randomx_release_cache(mRandomXCache);
    }
  }
}

int DenariiClient::attemptMineBlock(int nonce, int attempts, const std::string &blockHashingBlob,
                                    const std::string &blockTemplateBlob, u_int64_t difficulty,
                                    std::string *minedBlock) {

  int currentAttempts = 0;
  do {
    char hash[RANDOMX_HASH_SIZE];

    // Hash it
    std::string blockHashingBlobWithNonce = packNonce(blockHashingBlob, std::to_string(nonce));

    randomx_calculate_hash(mRandomXVM, blockHashingBlobWithNonce.c_str(), blockHashingBlobWithNonce.size(), hash);

    // If it meets difficulty return true
    if (meetingDifficulty(difficulty, hash, RANDOMX_HASH_SIZE)) {

      // Pack the block template blob with our nonce
      *minedBlock = packNonce(blockTemplateBlob, std::to_string(nonce));
      return nonce;
    }

    currentAttempts++;
    nonce++;
    // Otherwise we keep trying if not out of attempts
  } while (currentAttempts < attempts);

  return nonce;
}

std::string DenariiClient::packNonce(const std::string &blob, const std::string &nonce) {
  std::string b = blob.substr(0, 39);
  b = b + nonce;
  b = b + blob.substr(43);
  return b;
}

bool DenariiClient::getBlockHashingBlob(const Wallet &wallet, nlohmann::json *result) {
  json input;
  input["wallet_address"] = wallet.address();

  std::string outputStr;
  sendCommand("get_block_template", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }

  *result = output["result"];

  return result->contains("blockhashing_blob");
}

bool DenariiClient::meetingDifficulty(const Bigint &difficulty, char *hash, int hashSize) {
  // 2^256 -1
  Bigint base = 115792089237316195423570985008687907853269984665640564039457584007913129639935;

  std::string hashAsStr = std::string(hash);
  Bigint hashAsInt = common::tools::fromString(hashAsStr);
  return hashAsInt * difficulty <= base;
}

bool DenariiClient::attemptSubmitBlock(const std::string &minedBlock) {

  json input = json::array({minedBlock});

  std::string outputStr;
  sendCommand("submit_block", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  return output.contains("result");
}


} // common
