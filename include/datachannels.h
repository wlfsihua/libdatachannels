#ifndef LIBDATACHANNELS_H_
#define LIBDATACHANNELS_H_
#include <stdint.h>
#include <chrono>
#include <memory>
#include <string>
#include <functional>

class TimeService
{
public:
	class Timer
	{
	public:
		using shared = std::shared_ptr<Timer>;
	public:
		virtual void Cancel() = 0;
		virtual void Retry(const std::chrono::milliseconds& ms) = 0;
	};
public:
	uint64_t GetNow();
	Timer::shared CreateTimer(const std::chrono::milliseconds& ms,std::function<void(void) > timeout);
};

class Datachannel
{
public:
	// Type definitions
	
	struct Options
	{
		
	};

	using shared = std::shared_ptr<Datachannel>;
public:
	// Member methods
	
	virtual bool Send(const std::wstring& msg)  = 0;
	virtual bool Send(const uint8_t* data, const uint64_t size)  = 0;
	
	// Stream mode
	virtual bool StartMessage()  = 0;
	virtual bool SendMessageData(const uint8_t* data, const uint64_t size)  = 0;
	virtual bool EndMessage()  = 0;
	
	// Event handlers
	virtual void onMessage(const std::function<void(const std::wstring&)>& callback);
	virtual void onMessage(const std::function<void(const uint8_t*,uint64_t)>& callback);
	
	virtual void onMessageStart(const std::function<void(void)>& callback);
	virtual void onMessageData(const std::function<void(const uint8_t*,uint64_t)>& callback);
	virtual void onMessageEnd(const std::function<void(void)>& callback);
		
	virtual bool Close();
};

class DatachannelEndpoint
{
public:
	// Type definitions
	
	enum Setup
	{
		Client,
		Server
	};	
	
	struct Options
	{
		Setup setup;
	};
	
	using shared = std::shared_ptr<DatachannelEndpoint>;
public:
	// Static methods
	
	// Create new datachannel endpoint
	//	options.setup : Client/Server	
	static DatachannelEndpoint::shared Create(TimeService& timeService, const Options& options) ;
	
public:
	// Type definitions
	virtual bool Start(uint16_t remotePort)  = 0;
	virtual Datachannel::shared CreateDatachannel(const Datachannel::Options& options)  = 0;
	virtual bool Close()  = 0;
	
	// Getters
	virtual uint16_t GetLocalPort() const = 0;
	virtual uint16_t GetRemotePort() const = 0;
	virtual Setup	 GetSetup() const = 0;
	
	//DTLS tranport
	virtual size_t ReadPacket(uint8_t *data, uint32_t size) = 0;
	virtual size_t WritePacket(uint8_t *data, uint32_t size) = 0; 
	
	virtual void OnPendingData(std::function<void(void)> callback) = 0;
};

#endif
