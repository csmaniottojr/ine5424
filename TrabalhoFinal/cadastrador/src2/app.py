# encoding: utf-8
from model_controller.sqlalchemy.smart_object import SmartObjectSQLAController
from sqlalchemy import create_engine
from models import *
from sqlalchemy.orm import sessionmaker
import config_bd
import sys

from command.command_message_type import CommandMessageType
from command.command_serialization import CommandSerialization
from utils.utils import *

from threading import Thread

class App(Thread):
    def __init__(self, so_controller, serial_manager):
        Thread.__init__(self)
        self.so_controller = so_controller
        self.serial_manager = serial_manager
        self.cases = {'1': self.case_1, '2': self.case_2, '3': self.case_3, '4': self.case_4}
    
    def run(self):
        while True:
            self.print_menu()
            op = input('Insira a opção: ')
            self.cases.get(op, self.default)()

    def print_menu(self):
        print ('\n1. Listar dispositivos\n2. Ver dispositivo\n3. Executar serviço\n4. Sair\n')

    def case_1(self):
        smart_objects = self.so_controller.list()
        for so in smart_objects:
            print ('Id: {}, nome: {}, dev_id: {}'.format(so.id, so.name, so.device_id))


    def case_2(self):
        _id = int(input('Insira o id do smart object '))

        smart_object = self.so_controller.get_by_id(_id)
        if not smart_object:
            print ('Smart object não existente')
        else:
            print (smart_object)

    def case_3(self):
        resp = dict()
        resp['msg_type'] = CommandMessageType.COMMAND_WRITE_REQUEST

        _id = int(input('Insira o id do smart object '))

        smart_object = self.so_controller.get_by_id(_id)
        if not smart_object:
            print ('Smart object não existente')
        else:
            resp['emote_id'] = smart_object.device_id
            service_defined = False
            while not service_defined:
                for i in range(len(smart_object.services)):
                    print ('{}. {}'.format(i+1, smart_object.services[i].name))

                input_serv = int(input('Informe o número do serviço: '))
                try:
                    service = smart_object.services[input_serv - 1]
                except IndexError:
                    print ('Serviço não encontrado')
                else:
                    service_defined = True
            
            param_value = []
            for param in service.parameters:
                if param.read_only:
                    continue
                if isinstance(param, ParameterBoolean):
                    
                    valid_value = False

                    while not valid_value:
                        try:
                            value = bool( int(input('Informe valor para parâmetro booleano: {}\n'.format(param.name))) )
                        except ValueError:
                            print('Valor inválido!')
                        else:
                            valid_value = True
                            param_value.append( (param.reg_id, Utils.pack_with_byte_order("?", value)) )

                elif isinstance(param, ParameterInteger):
                    valid_value = False

                    while not valid_value:
                        try:
                            value = int(input('Informe valor para parâmetro inteiro: {}. Min: {}. Max: {}\n'.format(
                                param.name, int(param.min_value), int(param.max_value))))
                            if value < param.min_value or value > param.max_value:
                                print('Valor fora da faixa de valores permitidos!')
                            else:
                                valid_value = True
                                param_value.append( (param.reg_id, Utils.pack_with_byte_order("I", value)) )
                        except ValueError:
                            print('Valor informado não corresponde ao tipo esperado!')


                elif isinstance(param, ParameterFloat):
                    valid_value = False

                    while not valid_value:
                        try:
                            value = float(input('Informe valor para parâmetro pto flutuante: {}. Min: {}. Max: {}\n'.format(
                                param.name, param.min_value, param.max_value)))
                            if value < param.min_value or value > param.max_value:
                                print('Valor fora da faixa de valores permitidos!')
                            else:
                                valid_value = True
                                param_value.append( (param.reg_id, Utils.pack_with_byte_order("f", value)) )
                        except ValueError:
                            print('Valor informado não corresponde ao tipo esperado!')


                elif isinstance(param, ParameterOption):
                    valid_value = False
                    while not valid_value:
                        for i in range(len(param.options)):
                            print ('{}. {}'.format(i+1, param.options[i]))

                        op = int(input('Informe número da opção para o parâmetro {}:\n'.format(param.name)))
                        try:
                            value = param.options[op - 1]
                        except IndexError:
                            print ('Parâmetro não encontrado!')
                        else:
                            valid_value = True
                            param_value.append( (param.reg_id, Utils.pack_with_byte_order("I", op)) )

        for value in param_value:
            resp['reg_id'] = value[0]
            resp['data'] = bytearray(value[1])
            #print("Escrevendo: %s" % resp)
            msg = CommandSerialization.serialize(resp)
            self.serial_manager.write(msg)

    def case_4(self):
        print ('Saindo do programa...\n')
        sys.exit(0)

    def default(self):
        print ('Opção inválida')
