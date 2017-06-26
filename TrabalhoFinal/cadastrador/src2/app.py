# encoding: utf-8
from model_controller.sqlalchemy.smart_object import SmartObjectSQLAController
from sqlalchemy import create_engine
from models import *
from sqlalchemy.orm import sessionmaker
import config_bd
import sys

engine = create_engine('mysql+pymysql://{}:{}@{}/{}'.format(
    config_bd.DB_USER,
    config_bd.DB_PASSWORD,
    config_bd.HOST,
    config_bd.DB_NAME))

Session = sessionmaker(bind=engine)
session = Session()

smart_object_controller = SmartObjectSQLAController(session)

def case_1():
    smart_objects = smart_object_controller.list()
    for so in smart_objects:
        print ('Id: {}, nome: {}, dev_id: {}'.format(so.id, so.name, so.device_id))


def case_2():
    _id = int(input('Insira o id do smart object '))

    smart_object = smart_object_controller.get_by_id(_id)
    if not smart_object:
        print ('Smart object não existente')
    else:
        print (smart_object)

def case_3():
    _id = int(input('Insira o id do smart object '))

    smart_object = smart_object_controller.get_by_id(_id)
    if not smart_object:
        print ('Smart object não existente')
    else:
        service_defined = False
        while not service_defined:
            for i in range(len(smart_object.services)):
                print ('{}. {}'.format(i+1, smart_object.services[0].name))

            input_serv = int(input('Informe o número do serviço: '))
            try:
                service = smart_object.services[input_serv - 1]
            except IndexError:
                print ('Serviço não encontrado')
            else:
                service_defined = True

        param_value = []
        for param in service.parameters:
            if isinstance(param, ParameterBoolean):

                valid_value = False

                while not valid_value:
                    try:
                        value = bool(input('Informe valor para parâmetro booleano: {}\n'.format(param.name)))
                    except ValueError:
                        print('Valor inválido!')
                    else:
                        valid_value = True
                        param_value.append((param.name, value))

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
                            param_value.append((param.name, value))
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
                            param_value.append((param.name, value))
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
                        param_value.append((param.name, value))

    print (smart_object.name, service.name, param_value)

def case_4():
    print ('Saindo do programa...\n')
    sys.exit(0)

def default():
    print ('Opção inválida')


cases = {'1': case_1, '2': case_2, '3': case_3, '4': case_4}


def print_menu():
    print ('\n1. Listar dispositivos\n2. Ver dispositivo\n3. Executar serviço\n4. Sair\n')


def start_app():

    while True:
        print_menu()
        op = input('Insira a opção: ')
        cases.get(op, default)()


if __name__ == '__main__':
    start_app()
