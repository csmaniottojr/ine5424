from IoT.messages import *
from IoT.objects import *
import struct
import sys

BYTE_ORDER = ((sys.byteorder == "little") and "<" or ">") 

message = register_message.RegisterResponse(True)
print("RegisterResponse Size: %d" % message.get_size())
print("RegisterResponse Id: %d" % message.get_id())
print("RegisterResponse Type: %s\n" % str(message.get_type()))

so = smart_object.SmartObject(101560330, "EPOS")
message = register_message.RegisterObjectRequest(so)
print("RegisterObjectRequest Size: %d" % message.get_size())
print("RegisterObjectRequest Obj Name: %s\n" % message.get_object_name())

serv = service.Service("Atuacao")
message = register_message.RegisterServiceRequest(serv)
print("RegisterServiceRequest Size: %d" % message.get_size())
print("RegisterServiceRequest Serv Name: %s\n" % message.get_service_name())

ftype = parameter_float.ParameterFloat(1, 2)
print(int(ftype.get_type().value))
param = parameter.Parameter("Temperatura", 3, ftype)
message = register_message.RegisterParameterRequest(param)
message.set_id(101559240)
print("RegisterParameterRequest Size: %d" % message.get_size())
print("RegisterParameterRequest Param Name: %s\n" % message.get_parameter_name())

smsg = register_serialization.RegisterSerialization.serialize(message)
print("Serialized: %s" % [ "0x%02x" % b for b in smsg ])
print("")

message = register_serialization.RegisterSerialization.deserialize(smsg)
print("Deserialized Size: %d" % message.get_size())
print("Deserialized Param Name: %s" % message.get_parameter_name())
print("Deserialized min: %s" % [ "0x%02x" % b for b in message.get_min_value() ])
min = struct.unpack((BYTE_ORDER+"f"), message.get_min_value()[1:5])[0]
print("   min: %f" % min)
print("Deserialized max: %s" % [ "0x%02x" % b for b in message.get_max_value() ])
max = struct.unpack((BYTE_ORDER+"f"), message.get_max_value()[1:5])[0]
print("   min: %f" % max)