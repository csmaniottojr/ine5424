from managers.serial_port_manager import SerialPortManager
from managers.command_manager import CommandManager
from managers.debug_manager import DebugManager
from managers.register_manager import RegisterManager

if __name__ == '__main__':
    serial_manager = SerialPortManager('/dev/ttyACM0', 115200, 2.0)

    register_manager = RegisterManager()
    serial_manager.register(register_manager)

    command_manager = CommandManager()
    serial_manager.register(command_manager)

    debug_manager = DebugManager()
    serial_manager.register(debug_manager)

    serial_manager.run()
