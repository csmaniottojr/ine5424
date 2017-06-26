from serial_port_manager import SerialPortManager
from model_controller.sqlalchemy.smart_object import SmartObjectSQLAController
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
import config_bd

if __name__ == '__main__':
    print("Iniciando o app...")
    engine = create_engine('mysql+pymysql://{}:{}@{}/{}'.format(
        config_bd.DB_USER,
        config_bd.DB_PASSWORD,
        config_bd.HOST,
        config_bd.DB_NAME))

    Session = sessionmaker(bind=engine)
    session = Session()

    smart_object_controller = SmartObjectSQLAController(session)
    sp_manager = SerialPortManager('/dev/ttyACM0', 115200, smart_object_controller)
    sp_manager.start()
