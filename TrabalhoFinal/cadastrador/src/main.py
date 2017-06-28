# encoding: utf-8
from managers.serial_port_manager import SerialPortManager
from managers.command_manager import CommandManager
from managers.debug_manager import DebugManager
from managers.register_manager import RegisterManager

from app import App

from model_controller.sqlalchemy.smart_object import SmartObjectSQLAController
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
import config_bd

""" 
    Arquivo responsável por iniciar todos os componentes
    necessários para o funcionamento do sistema.
"""
if __name__ == '__main__':
    print("Iniciando o programa...")
    engine = create_engine('mysql+pymysql://{}:{}@{}/{}'.format(
        config_bd.DB_USER,
        config_bd.DB_PASSWORD,
        config_bd.HOST,
        config_bd.DB_NAME))

    Session = sessionmaker(bind=engine)
    session = Session()
    smart_object_controller = SmartObjectSQLAController(session)

    serial_manager = SerialPortManager('/dev/ttyACM0', 115200, 2.0)

    debug_manager = DebugManager()
    serial_manager.register(debug_manager)

    command_manager = CommandManager()
    serial_manager.register(command_manager)
    
    register_manager = RegisterManager(smart_object_controller)
    serial_manager.register(register_manager)

    app = App(smart_object_controller, serial_manager, command_manager)
    app.start()

    serial_manager.run()