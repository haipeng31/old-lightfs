#include "CSConManager.h"
#include "FsClient.h"

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
	return ipAddr_ < rhs.ipAddr_;
}

FsClient *CSConManager::getOrCreateCSCon(string ipAddr, int port)
{
	ChunkServerAddr csAddr(ipAddr, port);
	getOrCreateCSCon(csAddr);
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
