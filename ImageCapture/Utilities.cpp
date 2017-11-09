#include "Utilities.h"
#include <iostream>
#include <algorithm>
#include <QDebug>


enum { EVEN_LINES = 288, ODD_LINES = 288, IMAGE_WIDTH = 1440,
       IMAGE_HEIGHT = EVEN_LINES + ODD_LINES};


Utilities *Utilities::instance_ = new Utilities;

Utilities::Utilities()
{
    initializeNetwork();

}

void Utilities::initializeNetwork()
{
    int iResult;
    WSADATA wsaData;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: %d\n", iResult;
    }

    serverSocket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket_ == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        throw std::runtime_error("create socket failed.");
    }

    memset(&serverAddress_, 0, sizeof(serverAddress_));
    serverAddress_.sin_addr.S_un.S_addr = INADDR_ANY;
    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_port = htons(8080);

    //Adjust recv buffer to 10MB.
    UINT32 uiRecvBuf = 10 * 1024 * 1024;
    setsockopt(serverSocket_, SOL_SOCKET, SO_RCVBUF, (const CHAR *)&uiRecvBuf, sizeof (uiRecvBuf));

    if (bind(serverSocket_, (sockaddr *)&serverAddress_, sizeof(serverAddress_)) < 0) {
        //std::cout << "bind error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        throw std::runtime_error("bind error.");
    }
}

void Utilities::readSocketData(std::vector<std::vector<UCHAR>>& data) const
{
    const UINT32 kLineSize = 1024;
    const UINT32 kLineCount = 625 * 2 * 2;

    UCHAR *buffer = new UCHAR[kLineSize];

    for (UINT32 lineCount = 0; lineCount < kLineCount; ++lineCount) {
        memset(buffer, 0, sizeof(UCHAR) * kLineSize);
        int len = recvfrom(serverSocket_, reinterpret_cast<CHAR *>(buffer), sizeof(CHAR) * kLineSize,
            0, nullptr, nullptr);

        data.push_back(std::vector<UCHAR>(buffer, buffer + len));
    }
    delete[] buffer;
}

void Utilities::vectorToCharString(const std::vector<std::vector<UCHAR>>& data, UCHAR *dest) const
{
    UINT32 curPos = 0;

    for (const auto &item : data) {
        memcpy(dest + curPos, item.data(), 864);
        curPos += 864;
    }
}

Utilities::~Utilities()
{
    closesocket(serverSocket_);
}

Utilities * Utilities::getInstance()
{
    return instance_;
}

void Utilities::getYCbCr422Image(const UCHAR *src, std::vector<std::vector<UCHAR>> &dest, UINT32 len) const
{
    enum PackageState : UCHAR {
        PREHANDLE = 0, EVENBLANK, EVENACTIVEEVE, EVENACTIVESVE,
        ODDBLANK, ODDACTIVEEVE, ODDACTIVESVE, END
    };

    UINT32 i = 0;
    PackageState curState = PREHANDLE;
    std::vector<unsigned char> lineData;
    UINT32 evenLines = 0;
    std::vector<std::vector<UCHAR>> img;
    while (i < len - 3 && curState != END) {
        switch (curState) {
        case PREHANDLE:
            if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0xF0) {
                curState = EVENBLANK;
                continue;
            }
            break;
        case EVENBLANK:
            if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0xD0) {
                curState = EVENACTIVEEVE;
                lineData.clear();
                continue;
            }
            break;
        case EVENACTIVEEVE:
            if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0xC0) {
                i += 4;
                curState = EVENACTIVESVE;
                continue;
            }
            break;
        case EVENACTIVESVE:
            if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0xD0) {
                i += 4;
                curState = EVENACTIVEEVE;
                img.push_back(lineData);
                evenLines++;
                lineData.clear();
                continue;
            }
            else if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0xB0) {
                i += 4;
                curState = ODDBLANK;
                img.push_back(lineData);
                evenLines++;
                //qDebug() << "evenLines = " << evenLines << endl;

                while (evenLines++ < ODD_LINES) {
                    img.push_back(lineData);
                }
                lineData.clear();
                continue;
            }
            else if (lineData.size() < IMAGE_WIDTH){
                lineData.push_back(src[i]);
            }
            break;
        case ODDBLANK:
            if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0x90) {
                curState = ODDACTIVEEVE;
                continue;
            }
            break;
        case ODDACTIVEEVE:
            if (src[i] == 0xFF &&
                src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0x80) {
                i += 4;
                curState = ODDACTIVESVE;
                lineData.clear();
                continue;
            }
            break;
        case ODDACTIVESVE:
            if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] == 0x90) {
                i += 4;
                curState = ODDACTIVEEVE;
                img.push_back(lineData);
                lineData.clear();
                continue;
            }
            else if (src[i] == 0xFF && src[i + 1] == 0 && src[i + 2] == 0 && src[i + 3] != 0x90) {
                curState = END;
                img.push_back(lineData);
                //qDebug() << "img.size = " << img.size() << endl;

                while (img.size() < IMAGE_HEIGHT) {
                    img.push_back(lineData);
                }
                lineData.clear();
                continue;
            }
            else if (lineData.size() < IMAGE_WIDTH){
                lineData.push_back(src[i]);
            }
            break;
        default:
            curState = PREHANDLE;
            break;
        }
        ++i;
    }

    for (auto &item : img) {
        UINT32 size = item.size();
        if (size >= IMAGE_WIDTH) continue;

        UCHAR ch = item[size - 1];
        for (UINT32 i = item.size(); i < IMAGE_WIDTH; ++i) {
            item.push_back(ch);
        }
    }

//    for (int i = 0, j = evenLines; i < evenLines && j < img.size(); ++i, ++j) {
//        dest.push_back(img[j]);
//        dest.push_back(img[i]);
//    }

    for (int i = evenLines - 1, j = img.size() - 1; i >= 0 && j >= evenLines; --i, --j) {
        dest.push_back(img[j]);
        dest.push_back(img[i]);
    }
}

void Utilities::getPicture(std::vector<std::vector<UCHAR>> &data)
{
    std::vector<std::vector<UCHAR>> originalData;
    readSocketData(originalData);

    //一次接收到的UDP数据报文长度为864字节
    const UINT32 kBufferSize = originalData.size() * 864;
    UCHAR *buffer = new UCHAR[kBufferSize];
    memset(buffer, 0, originalData.size() * 864);
    vectorToCharString(originalData, buffer);

    std::vector<std::vector<UCHAR>> ycbcr422Data;
    getYCbCr422Image(buffer, ycbcr422Data, kBufferSize);
    delete[] buffer;

    ycbcr422ToRGB(ycbcr422Data, data);
}

void Utilities::getPicture(UCHAR *&image, UINT32 & width, UINT32 & height)
{
    std::vector<std::vector<UCHAR>> originalData;
    readSocketData(originalData);

    //一次接收到的UDP数据报文长度为864字节
    const UINT32 kBufferSize = originalData.size() * 864;
    UCHAR *buffer = new UCHAR[kBufferSize];
    memset(buffer, 0, kBufferSize);
    vectorToCharString(originalData, buffer);

    std::vector<std::vector<UCHAR>> ycbcr422Data;
    getYCbCr422Image(buffer, ycbcr422Data, kBufferSize);
    delete[] buffer;

    if (ycbcr422Data.size() <= 0) {
        width = 0;
        height = 0;
        image = nullptr;
        return;
    }

    const UINT32 kImageSize = ycbcr422Data[0].size() * ycbcr422Data.size();
    image = new UCHAR[kImageSize];
    ycbcr422ToRGB(ycbcr422Data, image, kImageSize);
    width = ycbcr422Data[0].size() / 2;
    height = ycbcr422Data.size();
}

void Utilities::ycbcr422ToRGB(const std::vector<std::vector<UCHAR>>& src, UCHAR * data, UINT32 imgSize) const
{
    UINT32 index = 0;
    for (auto & item : src) {
        for (UINT32 i = 0; i < item.size() - 3 && index < imgSize; i += 4) {
            UCHAR cb = item[i];
            UCHAR y0 = item[i + 1];
            UCHAR cr = item[i + 2];
            UCHAR y1 = item[i + 3];

            UCHAR &r0 = data[index++], &g0 = data[index++], &b0 = data[index++];
            UCHAR &r1 = data[index++], &g1 = data[index++], &b1 = data[index++];

            yCbCrToRGB(y0, cb, cr, r0, g0, b0);
            yCbCrToRGB(y1, cb, cr, r1, g1, b1);
        }
    }
}

void Utilities::ycbcr422ToRGB(const std::vector<std::vector<UCHAR>>& src, std::vector<std::vector<UCHAR>>& img) const
{
    for (const auto & item : src) {
        std::vector<UCHAR> lineData;
        for (UINT32 i = 0; i < item.size() - 3; i += 4) {
            UCHAR cb = item[i];
            UCHAR y0 = item[i + 1];
            UCHAR cr = item[i + 2];
            UCHAR y1 = item[i + 3];

            UCHAR r0, g0, b0;
            UCHAR r1, g1, b1;

            yCbCrToRGB(y0, cb, cr, r0, g0, b0);
            yCbCrToRGB(y1, cb, cr, r1, g1, b1);
            lineData.push_back(r0); lineData.push_back(g0); lineData.push_back(b0);
            lineData.push_back(r1); lineData.push_back(g1); lineData.push_back(b1);
        }
        img.push_back(lineData);
    }
}

inline void Utilities::yCbCrToRGB(UCHAR y, UCHAR cb, UCHAR cr, UCHAR & r, UCHAR & g, UCHAR & b) const
{
    // Transformate TCbCr to RGB.
    r = y + 1.402 * (cr  -128);
    g = y - 0.34414 * (cb - 128) - 0.71414 * (cr -  128);
    b = y + 1.772 * (cb - 128);
}
