#include "randomx_client.h"

using Dodecahedron::Bigint;

randomx_cache *mRandomXCache = nullptr;
randomx_vm *mRandomXVM = nullptr;
randomx_dataset *mRandomXDataSet = nullptr;

std::string mMode = "";

namespace randomx_client {

bool RandomXClient::initRandomX(std::string &mode, char *key, int keySize) {
  randomx_flags flags = randomx_get_flags();

  std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

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

void RandomXClient::shutdownRandomX() {

  if (mRandomXVM != nullptr) {
    randomx_destroy_vm(mRandomXVM);

    if (mMode == "fast") {
      randomx_release_dataset(mRandomXDataSet);
    } else {
      randomx_release_cache(mRandomXCache);
    }
  }
}

int RandomXClient::attemptMineBlock(int nonce, int attempts, const std::string &blockHashingBlob,
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

std::string RandomXClient::packNonce(const std::string &blob, const std::string &nonce) {
  std::string b = blob.substr(0, 39);
  b = b + nonce;
  b = b + blob.substr(43);
  return b;
}

bool RandomXClient::meetingDifficulty(const Bigint &difficulty, char *hash, int hashSize) {
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

}