#ifndef KEIROSPUBLIC_RANDOMX_CLIENT_H
#define KEIROSPUBLIC_RANDOMX_CLIENT_H

#include <algorithm>
#include <thread>

#include "src/bigint.h"
#include "src/randomx.h"

#include "Tools/General/Strings/big_int_from_string.h"
namespace randomx_client {

class RandomXClient{
public:
  // Initializes RandomX so it can be used to mine. Returns true on success and false otherwise.
  static bool initRandomX(std::string &mode, char *key, int keySize);

// Shuts down RandomX.
  static void shutdownRandomX();

// Attempts to mine a block. This will fail if the block nonce cannot be found after n attempts.
// If successful then minedBlock will be set. Returns the current nonce.
// Also specifies the wallet that will receive the funds.
  static int
  attemptMineBlock(int nonce, int attempts, const std::string &blockHashingBlob, const std::string &blockTemplateBlob,
                   uint64_t difficulty, std::string *minedBlock);

// Packs a nonce into the blob. Be it a block hashing blob or a block template blob.
  static std::string packNonce(const std::string &blob, const std::string &nonce);

// True if we are meeting the difficulty and false if not.
  static bool meetingDifficulty(const Dodecahedron::Bigint &difficulty, char *hash, int hashSize);
private:
  static randomx_cache *mRandomXCache;
  static randomx_vm *mRandomXVM;
  static randomx_dataset *mRandomXDataSet;

  static std::string mMode;
};
}

#endif //KEIROSPUBLIC_RANDOMX_CLIENT_H
