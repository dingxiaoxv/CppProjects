#ifndef DM_SSL_HELPER_H
#define DM_SSL_HELPER_H


#include <openssl/evp.h>
#include <string.h>
#include <vector>
#include <string>

namespace dm {


class  SslHelper {
public:
  SslHelper(const unsigned char* input);
  ~SslHelper();
  const std::string getKey() const { return keyBase64_; }
  int localPublicKeyLength() { return EVP_PKEY_size(local_); }
  void genLocalKey();
  const std::string encode(const std::string& value);
  void setIv(const void* iv) { memcpy(iv_, iv, 16); }
  void dumpKeyAndIv();
  bool encrypt(const std::string& instr, unsigned char *out, int *outlen);
  bool decrypt(const unsigned char *in, int inlen, unsigned char *out, int *outlen);
  std::string md5(const std::string& str);
  void genSecret();
  const char* getSecret() const { return (const char*) secret_.data(); }
  const char* getLocalPublicKey() const { return (const char*) localPubMd5_.data(); }

  // for test only
  void _resetLocalPrivateKey(const unsigned char* data);

private:
  EVP_PKEY* local_;
  EVP_PKEY* localPriv_;
  EVP_PKEY* localPub_;
  EVP_PKEY* peerPub_;
  std::string keyBase64_;

  std::vector<unsigned char> localPubMd5_;
  std::vector<unsigned char> secret_;
  unsigned char iv_[16];
};

} // namespace dm


#endif // DM_SSL_HELPER_H





