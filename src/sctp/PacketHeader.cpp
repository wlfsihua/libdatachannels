#include "PacketHeader.h"

namespace sctp
{
	
PacketHeader::PacketHeader(uint16_t sourcePortNumber,uint16_t destinationPortNumber,uint32_t verificationTag)
{
	this->sourcePortNumber = sourcePortNumber;
	this->destinationPortNumber = destinationPortNumber;
	this->verificationTag = verificationTag;
}

static PacketHeader::shared Parse(BufferReader& reader)
{
	//Check size
	if (reader.Assert(8)) return nullptr;
	
	//Get header
	uint16_t sourcePortNumber	= reader.Get2();
	uint16_t destinationPortNumber	= reader.Get2();
	uint32_t verificationTag	= reader.Get4();
	
	//Create PacketHeader
	auto header = std::make_shared<PacketHeader>(sourcePortNumber,destinationPortNumber,verificationTag);
	
	//Done
	return header;
}

size_t PacketHeader::GetSize() const
{
	//ports + tag 
	return 8;
}

size_t PacketHeader::Serialize(BufferWritter& writter) const
{
	//Check size
	if (writter.Assert(8)) return 0;
	
	//Set header
	writter.Set2(sourcePortNumber);
	writter.Set2(destinationPortNumber);
	writter.Set4(verificationTag);
	
	//Done
	return writter.GetLength();
}
	
}; //namespace sctp
