#include "denarii_client.h"

using Dodecahedron::Bigint;
using nlohmann::json;

namespace common {

randomx_cache* DenariiClient::mRandomXCache = nullptr;
randomx_vm* DenariiClient::mRandomXVM = nullptr;
randomx_dataset* DenariiClient::mRandomXDataSet = nullptr;

std::string DenariiClient::mMode = "";

namespace {
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *) userp)->append((char *) contents, size * nmemb);
  return size * nmemb;
}

} // namespace

DenariiClient::~DenariiClient() = default;

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

    // Set the authorization to use the username and password.
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

  std::transform (mode.begin(), mode.end(), mode.begin(), ::tolower);

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
                                    const std::string &blockTemplateBlob, uint64_t difficulty,
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
#ifdef _WIN32
  Bigint base = "115792089237316195423570985008687907853269984665640564039457584007913129639935";
#else
  Bigint base = 115792089237316195423570985008687907853269984665640564039457584007913129639935;
#endif
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

JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_createWallet(JNIEnv *env, jobject obj, jstring wallet) {
  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));
  DenariiClient::createWallet(walletObj);

  env->ReleaseStringUTFChars(wallet, convertedValue);
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_transferMoney(JNIEnv *env, jobject obj, jdouble amount, jstring sender, jstring receiver) {

  const char *convertedValueSender = (env)->GetStringUTFChars(sender, nullptr);
  std::string walletSender = std::string(convertedValueSender);

  common::Wallet senderWallet;
  senderWallet.ParseFromString(hex_to_string(walletSender));

  const char *convertedValueReceiver = (env)->GetStringUTFChars(receiver, nullptr);
  std::string walletReceiver = std::string(convertedValueReceiver);

  common::Wallet receiverWallet;
  receiverWallet.ParseFromString(hex_to_string(walletReceiver));


  bool success = DenariiClient::transferMoney(amount, senderWallet, receiverWallet);

  env->ReleaseStringUTFChars(sender, convertedValueSender);
  env->ReleaseStringUTFChars(receiver, convertedValueReceiver);

  return success;
}
JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_getAddress(JNIEnv *env, jobject obj, jstring wallet) {

  common::Wallet walletObj;
  DenariiClient::getAddress(&walletObj);

  std::string walletAsString = string_to_hex(walletObj.SerializeAsString());

  const char *convertedBack = walletAsString.c_str();
  *wallet = *env->NewStringUTF(convertedBack);
}
JNIEXPORT jdouble JNICALL Java_com_keiros_client_denarii_DenariiClient_getBalanceOfWallet(JNIEnv *env, jobject obj, jstring wallet) {

  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));
  double balance = DenariiClient::getBalanceOfWallet(walletObj);

  env->ReleaseStringUTFChars(wallet, convertedValue);

  return balance;
}
JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_setCurrentWallet(JNIEnv *env, jobject obj, jstring wallet) {

  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));
  DenariiClient::setCurrentWallet(walletObj);

  env->ReleaseStringUTFChars(wallet, convertedValue);
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_getBlockHashingBlob(JNIEnv *env, jobject obj, jstring wallet, jstring result) {

  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));

  json resultJson;

  bool success = DenariiClient::getBlockHashingBlob(walletObj, &resultJson);

  std::string resultJsonHex = string_to_hex(resultJson.dump());

  const char *convertedBack = resultJsonHex.c_str();
  *result = *env->NewStringUTF(convertedBack);

  env->ReleaseStringUTFChars(wallet, convertedValue);

  return success;
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_initRandomX(JNIEnv *env, jobject obj, jstring mode, jstring key) {
  const char *convertedValueMode = (env)->GetStringUTFChars(mode, nullptr);
  std::string modeStr = std::string(hex_to_string(convertedValueMode));

  const char *convertedValueKey = (env)->GetStringUTFChars(key, nullptr);
  std::string keyAsChar = std::string(hex_to_string(convertedValueKey));

  bool success = DenariiClient::initRandomX(modeStr, const_cast<char*>(keyAsChar.c_str()), keyAsChar.length());

  env->ReleaseStringUTFChars(mode, convertedValueMode);
  env->ReleaseStringUTFChars(key, convertedValueKey);

  return success;
}
JNIEXPORT void JNICALL Java_com_keiros_client_denarii_DenariiClient_shutdownRandomX(JNIEnv *env, jobject obj) {
  DenariiClient::shutdownRandomX();
}

JNIEXPORT jint JNICALL Java_com_keiros_client_denarii_DenariiClient_attemptMineBlock(JNIEnv *env, jobject obj, jint nonce,
    jint attempts, jstring blockHashingBlob, jstring blockTemplateBlob, jlong difficulty, jstring minedBlock) {

  const char *convertedValueHashing = (env)->GetStringUTFChars(blockHashingBlob, nullptr);
  std::string hashingCopy = std::string(hex_to_string(convertedValueHashing));

  const char *convertedValueTemplate = (env)->GetStringUTFChars(blockTemplateBlob, nullptr);
  std::string templateCopy = std::string(hex_to_string(convertedValueTemplate));

  std::string minedBlockStr;
  int currentNonce = DenariiClient::attemptMineBlock(nonce, attempts, hashingCopy, templateCopy, difficulty, &minedBlockStr);

  std::string minedBlockHex = string_to_hex(minedBlockStr);

  const char *convertedBack = minedBlockHex.c_str();
  *minedBlock = *env->NewStringUTF(convertedBack);

  env->ReleaseStringUTFChars(blockHashingBlob, convertedValueHashing);
  env->ReleaseStringUTFChars(blockTemplateBlob, convertedValueTemplate);

  return currentNonce;
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_attemptSubmitBlock(JNIEnv *env, jobject obj, jstring minedBlock) {

  const char *convertedValue = (env)->GetStringUTFChars(minedBlock, nullptr);
  std::string minedBlockCopy = std::string(convertedValue);

  bool success = DenariiClient::attemptSubmitBlock(minedBlockCopy);

  env->ReleaseStringUTFChars(minedBlock, convertedValue);

  return success;
}

} // common
