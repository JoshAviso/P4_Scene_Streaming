#pragma once
#include <String.hpp>
#include <google/protobuf/message.h>
using gRPCMessage = google::protobuf::Message;
class ISerializable {
public:
	virtual String Serialize() const { return String(); };
	virtual bool Deserialize(const String& s) { return false; };
	virtual gRPCMessage* SerializeGRPCMessage() const { return nullptr; };
	virtual bool Deserialize(const gRPCMessage& msg) { return false; }
};