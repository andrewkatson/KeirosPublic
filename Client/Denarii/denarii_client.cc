#include "denarii_client.h"

using nlohmann::json;

namespace common {

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
    const std::string postField = "{\"jsonrpc\":\"" + std::to_string(2.0) + "\", \"id\":\"" + std::to_string(0) + "\", \"method\":\"" + method + "\", \"params\": " + params + "}";
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

void DenariiClient::sendCommandToOldDaemon(const std::string &method,
                                const std::string &params, std::string *output) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8424/" + method);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    const std::string &postField = params;

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


bool DenariiClient::createWallet(const common::Wallet &wallet) {

  json input;
  input["filename"] = wallet.name();
  input["password"] = wallet.password();
  input["language"] = "English";

  // There is no output expected
  std::string outputStr;
  sendCommand("create_wallet", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }

  return true;
}

bool DenariiClient::getAddress(common::Wallet *wallet) {


  json input;
  input["account_index"] = 0;

  std::string outputStr;
  sendCommand("get_address", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }

  json result = input["result"];

  if (result.contains("address")) {
    wallet->set_address(result["address"]);

    json addresses = result["addresses"];

    for (auto& address : addresses) {
        if (address["label"] != "Primary Account") {
            wallet->add_sub_addresses(address["address"]);
        }
    }
    return true;
  }

  return false;
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

  json amountAndAddress;
  amountAndAddress["amount"] = amount;
  amountAndAddress["address"] = receiver.address();

  input["destinations"] = {amountAndAddress};

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

bool DenariiClient::setCurrentWallet(const common::Wallet &wallet) {

  json input;

  input["filename"] = wallet.name();
  if (!wallet.password().empty()) {
    input["password"] = wallet.password();
  }

  std::string outputStr;
  sendCommand("open_wallet", input.dump(), &outputStr);

  // There is no output expected so we do nothing with it.
  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }
  return true;
}

bool DenariiClient::getBlockHashingBlob(const Wallet &wallet, nlohmann::json *result) {
  json input;
  input["wallet_address"] = wallet.address();

  std::string outputStr;
  sendCommandToDaemon("get_block_template", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }

  *result = output["result"];

  return result->contains("blockhashing_blob");
}

bool DenariiClient::restoreWallet(common::Wallet *wallet) {
  json input;
  input["filename"] = wallet->name();
  input["password"] = wallet->password();
  input["seed"] = wallet->phrase();

  std::string outputStr;
  sendCommandToWalletRPC("restore_deterministic_wallet", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }

  json result = output["result"];

  if (result["info"] != "Wallet has been restored successfully.") {
    return false;
  }

  *wallet->mutable_address() = result["address"];

  return true;
}

bool DenariiClient::querySeed(common::Wallet *wallet) {
  json input;
  input["key_type"] = "mnemonic";

  std::string outputStr;
  sendCommandToWalletRPC("query_key", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  if (!output.contains("result")) {
    return false;
  }

  json result = output["result"];
  wallet->set_phrase(result["key"]);

  return true;
}

bool DenariiClient::createAddress(common::Wallet *wallet) {
    return DenariiClient::createAddress("", wallet);
}

bool DenariiClient::createAddress(const std::string &label, common::Wallet* wallet) {
    json input;
    input["account_index"] = 0;
    input["label"] = label;

    std::string outputStr;
    sendCommandToWalletRPC("create_address", input.dump(), &outputStr);

    json output = json::parse(outputStr);

    if (!output.contains("result")) {
        return false;
    }

    json result = output["result"];

    wallet->add_sub_addresses(result["address"]);

    return true;
}

bool DenariiClient::startMining(bool doBackgroundMining, bool ignoreBattery, int threads) {
    json input;
    input["do_background_mining"] = doBackgroundMining;
    input["ignore_battery"] = ignoreBattery;
    input["threads_count"] = threads;

    std::string outputStr;
    sendCommandToWalletRPC("start_mining", input.dump(), &outputStr);

    json output = json::parse(outputStr);

    return output.contains("result");
}

bool DenariiClient::stopMining() {
  json input;

  std::string outputStr;
  sendCommandToWalletRPC("stop_mining", input.dump(), &outputStr);

  json output = json::parse("result");

  return output.contains("result");
}

bool DenariiClient::initRandomX(std::string &mode, const std::string &key) {
  return initRandomX(mode, const_cast<char*>(key.c_str()), key.length());
}

bool DenariiClient::initRandomX(std::string &mode, char *key, int keySize) {
  return randomx_client::RandomXClient::initRandomX(mode, key, keySize);
}

void DenariiClient::shutdownRandomX() {
  randomx_client::RandomXClient::shutdownRandomX();
}

int DenariiClient::attemptMineBlock(int nonce, int attempts, const std::string &blockHashingBlob,
                                    const std::string &blockTemplateBlob, uint64_t difficulty,
                                    std::string *minedBlock) {
  return randomx_client::RandomXClient::attemptMineBlock(nonce, attempts, blockHashingBlob, blockTemplateBlob, difficulty, minedBlock);
}

bool DenariiClient::attemptSubmitBlock(const std::string &minedBlock) {

  json input = json::array({minedBlock});

  std::string outputStr;
  sendCommandToDaemon("submit_block", input.dump(), &outputStr);

  json output = json::parse(outputStr);

  return !output.contains("error");
}

JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_createWallet(JNIEnv *env, jobject obj, jstring wallet) {
  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));
  bool success = DenariiClient::createWallet(walletObj);

  env->ReleaseStringUTFChars(wallet, convertedValue);

  return success;
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
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_getAddress(JNIEnv *env, jobject obj, jstring wallet) {

  common::Wallet walletObj;
  bool success = DenariiClient::getAddress(&walletObj);

  std::string walletAsString = string_to_hex(walletObj.SerializeAsString());

  const char *convertedBack = walletAsString.c_str();
  *wallet = *env->NewStringUTF(convertedBack);

  return success;
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
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_setCurrentWallet(JNIEnv *env, jobject obj, jstring wallet) {

  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));
  bool success = DenariiClient::setCurrentWallet(walletObj);

  env->ReleaseStringUTFChars(wallet, convertedValue);

  return success;
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
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_restoreWallet(JNIEnv *env, jobject obj, jstring wallet) {
  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));

  bool success = DenariiClient::restoreWallet(&walletObj);

  std::string walletAsString = string_to_hex(walletObj.SerializeAsString());

  const char *convertedBack = walletAsString.c_str();
  *wallet = *env->NewStringUTF(convertedBack);

  env->ReleaseStringUTFChars(wallet, convertedValue);

  return success;
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_querySeed(JNIEnv* env, jobject obj, jstring wallet) {
  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));

  bool success = DenariiClient::querySeed(&walletObj);

  std::string walletAsString = string_to_hex(walletObj.SerializeAsString());

  const char *convertedBack = walletAsString.c_str();
  *wallet = *env->NewStringUTF(convertedBack);

  env->ReleaseStringUTFChars(wallet, convertedValue);

  return success;
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_createNoLabelAddress(JNIEnv* env, jobject obj, jstring wallet) {
  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));

  bool success = DenariiClient::createAddress(&walletObj);

  std::string walletAsString = string_to_hex(walletObj.SerializeAsString());

  const char *convertedBack = walletAsString.c_str();
  *wallet = *env->NewStringUTF(convertedBack);

  env->ReleaseStringUTFChars(wallet, convertedValue);

  return success;
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_createAddress(JNIEnv* env, jobject obj, jstring label, jstring wallet) {
  const char *convertedValue = (env)->GetStringUTFChars(wallet, nullptr);
  std::string walletCopy = std::string(convertedValue);

  const char *convertedValueTwo = (env)->GetStringUTFChars(label, nullptr);
  std::string labelCopy = std::string(convertedValueTwo);

  common::Wallet walletObj;
  walletObj.ParseFromString(hex_to_string(walletCopy));

  bool success = DenariiClient::createAddress(labelCopy, &walletObj);

  std::string walletAsString = string_to_hex(walletObj.SerializeAsString());

  const char *convertedBack = walletAsString.c_str();
  *wallet = *env->NewStringUTF(convertedBack);

  env->ReleaseStringUTFChars(wallet, convertedValue);
  env->ReleaseStringUTFChars(label, convertedValueTwo);

  return success;
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_startMining(JNIEnv *env, jobject obj, jboolean doBackgroundMining, jboolean ignoreBattery, jint threads) {
  return DenariiClient::startMining(doBackgroundMining, ignoreBattery, threads);
}
JNIEXPORT jboolean JNICALL Java_com_keiros_client_denarii_DenariiClient_stopMining(JNIEnv *env, jobject obj) {
  return DenariiClient::stopMining();
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
