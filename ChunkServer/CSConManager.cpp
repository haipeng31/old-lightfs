#include "CSConManager.h"
#include "FsClient.h"
#include "netlib/base/Logging.h"

using mynet::Logger;
using namespace myfs;

ChunkServerAddr::ChunkServerAddr(string ipAddr, int port)
 :  ipAddr_(ipAddr),
	port_(port)
{
}

string ChunkServerAddr::ipAddr()
{
	return ipAddr_;
}

int ChunkServerAddr::port()
{
	return port_;
}

bool ChunkServerAddr::operator<(const ChunkServerAddr &rhs) const
{
	if (ipAddr_ == rhs.ipAddr_) {
		return port_ < rhs.port_;
	} else {
		return ipAddr_ < rhs.ipAddr_;
	}
}

FsClient *CSConManager::getOrCreateCSCon(string ipAddr, int port)
{
	ChunkServerAddr csAddr(ipAddr, port);
	return getOrCreateCSCon(csAddr);
}

FsClient *CSConManager::getOrCreateCSCon(ChunkServerAddr csAddr)
{
	if (CSConns_.find(csAddr) == CSConns_.end()) {
		FsClient *fsClient = new FsClient(csAddr.ipAddr().c_str(), csAddr.ipAddr().c_str(), csAddr.port());
		CSConns_[csAddr] = fsClient;
		return fsClient;
	} else {
		return CSConns_[csAddr];
	}
}
