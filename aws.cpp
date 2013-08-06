#include "aws.h"
#include <QCryptographicHash>
#include <sstream>
#include <QUrl>
#include <chrono> // time
#include <iomanip> // put_time
#include <QMessageAuthenticationCode>

AWS::AWS(AWSCredentials credentials) : UserCredentials(credentials)
{
}

AWS::~AWS() {}

const QString AWS::SignData(const std::string dataMd5, const std::string resourcePath, const std::time_t time)
{
    std::stringstream stream;
    auto date = std::put_time(std::localtime(&time), HttpTimeFormat);

    stream << "PUT"     << "\n"     // we're only uploading shit... for now at least.
       << dataMd5       << "\n"
       << "image/png"   << "\n"
       << date          << "\n"
       << "x-amz-acl:public-read" << "\n"
       << "/" << UserCredentials.BucketName << "/" << resourcePath;

    qDebug("%s", stream.str().c_str());

    //hmac_sha1(QString(UserCredentials.AccessSecretKey.c_str()), QString(stream.str().c_str()));
    return QString(QMessageAuthenticationCode::hash(QByteArray(stream.str().c_str()), QByteArray(UserCredentials.AccessSecretKey.c_str()), QCryptographicHash::Sha1).toBase64());
}

const std::shared_ptr<QNetworkRequest> AWS::MakeRequest(const std::string resourcePath, const QByteArray& data)
{
    std::shared_ptr<QNetworkRequest> request(new QNetworkRequest);
    // The time is required for the "Date" header
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // Data hashing is done here to apply it to the header and send it to the "SignData" method
    QByteArray md5(QCryptographicHash::hash(data, QCryptographicHash::Algorithm::Md5));
    // Format the time into HTTP time format
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), HttpTimeFormat);

    // Set headers
    request->setRawHeader("Date", QByteArray(ss.str().c_str()));
    qDebug("Date: %s", ss.str().c_str());
    request->setRawHeader("Content-MD5", md5.toBase64());
    qDebug("MD5: %s", QString(md5.toBase64()).toStdString().c_str());
    request->setRawHeader("Content-Type", "image/png");
    request->setRawHeader("x-amz-acl", "public-read");
    request->setRawHeader("User-Agent", "SCREEN CAPTURE TOOL YO");
    request->setRawHeader("Content-Length", QString::number(data.size()).toUtf8());

    ss.str("");
    ss << "AWS " << UserCredentials.AccessKeyId << ":" << SignData(QString(md5.toBase64()).toStdString(), resourcePath, time).toStdString();
    request->setRawHeader("Authorization", ss.str().c_str());
    qDebug("Authorization: %s", ss.str().c_str());
    qDebug("Resource: %s", resourcePath.c_str());

    ss.str("");
    ss << "https://" << UserCredentials.BucketName << ".s3.amazonaws.com" << "/" << QString(QUrl::toPercentEncoding(resourcePath.c_str())).toStdString();

    request->setUrl(QUrl(ss.str().c_str()));
    qDebug("%s", QString(request->url().toEncoded()).toStdString().c_str());

    return request;
}
