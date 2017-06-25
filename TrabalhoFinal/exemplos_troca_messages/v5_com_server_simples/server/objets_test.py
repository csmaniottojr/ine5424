from IoT.objects import *

so = smart_object.SmartObject(101560330, "EPOS")

atuacao = service.Service("Atuacao")
so.add_service(atuacao)
sensores = service.Service("Sensores")
so.add_service(sensores)

_led = parameter_boolean.ParameterBoolean()
led = parameter.Parameter("Led", 1, _led)
atuacao.add_parameter(led)

_combo = parameter_combo.ParameterCombo()
_combo.add_option("Baixa")
_combo.add_option("Media")
_combo.add_option("Alta")
temperatura1 = parameter.Parameter("Temperatura1", 2, _combo)
atuacao.add_parameter(temperatura1)

_temp = parameter_float.ParameterFloat(1, 2)
temperatura2 = parameter.Parameter("Temperatura2", 3, _temp)
sensores.add_parameter(temperatura2)

print("Obj ID: %d" % so.get_id())
print("Obj Name: %s" % so.get_name())

for s in so.get_services():
    print("   Service Name: %s" % s.get_name())

    for p in s.get_parameters():
        print("      Param Name: %s" % p.get_name())
        print("      Param RegId: %d" % p.get_register_id())
        print("      Param type: %s" % str(p.get_type()))
        print("      Param min: %s" % [ "0x%02x" % b for b in p.get_min_value() ])
        print("      Param max: %s" % [ "0x%02x" % b for b in p.get_max_value() ])
        print("")