#include <iostream>
#include <unistd.h>
#include <openssl/sha.h>
#include <memory>

#include "ssl_helper.h"

void encrypt(const char* value, const char* _uid, const char* pcode)
{
    std::shared_ptr<SslHelper> sslHelper = std::make_shared<SslHelper>((const unsigned char *)value);
    const char *key = sslHelper->getLocalPublicKey();
    LOCAL_MS(128, ms);
    // iv is 16 bytes string, it begins with did, other bytes is '1'
    ms << deviceInfo_.did << "1111111111111111";
    sslHelper_->setIv(ms.data());
    std::cout << "base64 key from peer is " << value;
    sslHelper_->dumpKeyAndIv();

    const std::string uid = _uid;
    const std::string secretSource = deviceInfo_.did + deviceInfo_.key + uid;

    cLOG_INFO << "source of secret code is " << secretSource;

    unsigned char encrypted[512] = {0};
    int encryptLength = 0;
    auto secret = sslHelper_->md5(secretSource);
}

int main(int argc, const char *argv[])
{
}