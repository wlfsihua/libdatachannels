#ifndef SCTP_ASSOCIATION_H_
#define SCTP_ASSOCIATION_H_
#include <list>

#include "datachannels.h"
#include "sctp/PacketHeader.h"
#include "BufferWritter.h"
#include "BufferReader.h"

namespace sctp
{

	
class Association
{
public:
	enum State
	{
		Closed,
		CookieWait,
		CookieEchoed,
		Established,
		ShutdownPending,
		ShutDownSent,
		ShutDownReceived,
		ShutDown,
		ShutDownAckSent
	};
public:
	Association(TimeService& timeService);
	
	bool Associate();
	bool Shutdown();
	bool Abort();

	
	
	void SetLocalPort(uint16_t port) 	{ localPort = port;	}
	void SetRemotePort(uint16_t port) 	{ remotePort = port;	}
	uint16_t GetLocalPort() const 		{ return localPort;	}
	uint16_t GetRemotePort() const		{ return remotePort;	}
	
	size_t ReadPacket(uint8_t *data, uint32_t size); 
	size_t WritePacket(uint8_t *data, uint32_t size); 
	
	void OnPendingData(std::function<void(void)> callback)
	{
		onPendingData = callback;
	}
private:
	void Process(const Chunk::shared& chunk);
	void SetState(State state);
	void Enqueue(const Chunk::shared& chunk);
private:
	State state = State::Closed;
	std::list<Chunk::shared> queue;
	TimeService& timeService;
	
	uint16_t localPort = 0;
	uint16_t remotePort = 0;
	
	uint32_t localVerificationTag = 0;
	uint32_t remoteVerificationTag = 0;
	uint32_t initRetransmissions = 0;
	TimeService::Timer::shared initTimer;
	TimeService::Timer::shared cookieEchoTimer;
	
	bool pendingData = false;
	std::function<void(void)> onPendingData;
};

}
#endif
