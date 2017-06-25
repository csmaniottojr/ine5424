from IoT.utils.utils import Utils

value = 2.0
packed = Utils.pack_with_byte_order("f", value)
print( Utils.bytearray_array(packed) )

value = Utils.unpack_with_byte_order("f", packed)
print( value )