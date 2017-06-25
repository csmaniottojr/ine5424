from IoT.messages import *
import struct
import sys

BYTE_ORDER = ((sys.byteorder == "little") and "<" or ">")

message = command_message.CommandReadResponse()
message.set_id(101560330)
message.set_register_id(2)
data = 2.0
data = struct.pack((BYTE_ORDER+"f"), data)
message.set_data(data, 4)

print("Message size: %d" % message.get_size())
print("Message ID: %d" % message.get_id())
print("Message type: %s" % message.get_type())
print("Message regId: %d" % message.get_register_id())
print("Message data: %s" % [ "0x%02x" % b for b in message.get_data() ])
print("")

smsg = command_serialization.CommandSerialization.serialize(message)
print("Serialized: %s" % [ "0x%02x" % b for b in smsg ])
print("")

message = command_serialization.CommandSerialization.deserialize(smsg)
print("Deserialized size: %d" % message.get_size())
print("Deserialized ID: %d" % message.get_id())
print("Deserialized type: %s" % message.get_type())
print("Deserialized regId: %d" % message.get_register_id())
print("Deserialized data: %s" % [ "0x%02x" % b for b in message.get_data() ])
print("")