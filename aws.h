#ifndef AWS_H
#define AWS_H
#include <string>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkRequest>
#include <chrono>

class AWS
{
public:
    struct AWSCredentials
    {
        std::string AccessKeyId; // key id is 20 chars
        std::string AccessSecretKey; // secret key is 40 chars
        std::string BucketName;
        std::string PublicUrl;
    };

    AWS(AWSCredentials credentials);
    ~AWS();

    const AWSCredentials UserCredentials;

    const std::shared_ptr<QNetworkRequest> MakeRequest(const std::string resourcePath, const QByteArray& data);
    const QString SignData(const std::string dataMd5, const std::string resourcePath, const std::time_t time);

private:
    const char* HttpTimeFormat = "%a, %d %h %Y %T %z";
    const QString hmac_sha1(const QString &key, const QString &secret);

};

#endif // AWS_H
