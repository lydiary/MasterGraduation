#pragma once

#include <vector>
#include <winsock2.h>

class Utilities
{
public:
	virtual ~Utilities();

	void getPicture(std::vector<std::vector<UCHAR>> &image);
    void getPicture(UCHAR *&image, UINT32 &width, UINT32 &height);

	static Utilities* getInstance();

private:
	Utilities();

	void initializeNetwork();
	void readSocketData(std::vector<std::vector<UCHAR>> &data) const;
	void vectorToCharString(const std::vector<std::vector<UCHAR>> &data, UCHAR *dest) const;
	void getYCbCr422Image(const UCHAR *src, std::vector<std::vector<UCHAR>> &img, UINT32 len) const;	
	void ycbcr422ToRGB(const std::vector<std::vector<UCHAR>> &src, UCHAR *data, UINT32 imgSize) const;
	void ycbcr422ToRGB(const std::vector<std::vector<UCHAR>> &src, std::vector<std::vector<UCHAR>> &dest) const;
	void yCbCrToRGB(UCHAR y, UCHAR cb, UCHAR cr, UCHAR &r, UCHAR &g, UCHAR &b) const;

private:
	static Utilities *instance_;

	SOCKET serverSocket_;
	SOCKADDR_IN serverAddress_;
};

