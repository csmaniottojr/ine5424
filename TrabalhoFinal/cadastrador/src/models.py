from sqlalchemy.ext.declarative import declarative_base, AbstractConcreteBase
from sqlalchemy import Table, Column, Integer, String, ForeignKey, Float
from sqlalchemy.orm import relationship

Base = declarative_base()


class SmartObject(Base):
    __tablename__ = 'smart_object'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    device_id = Column(Integer)

    services = relationship('Service')


    def __init__(self, name, device_id):
        self.name = name
        self.device_id = device_id
        self.services = []

    def add_service(self, service):
        self.services.append(service)

    def __repr__(self):
        return 'Id: {}, Nome: {}, Device_Id: {}\n\t{}'.\
            format(self.id, self.name, self.device_id, '\t'.join([str(serv) + '\n' for serv in self.services]))


class Service(Base):
    __tablename__ = 'service'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    smart_object_id = Column(Integer, ForeignKey('smart_object.id'))

    parameters = relationship('Parameter')


    def __init__(self, name):
        self.name = name
        self.parameters = []

    def add_parameter(self, parameter):
        self.parameters.append(parameter)

    def __repr__(self):
        return 'Servico: {}\n\t\t{}'.format(self.name, '\t\t'.join([str(p) + '\n' for p in self.parameters]))


class Parameter(AbstractConcreteBase, Base):

    def __init__(self, name, reg_id):
        self.name = name
        self.reg_id = reg_id

    def __repr__(self):
        return 'Parametro: {} ({})'.format(self.name, self.__class__.__name__[9:].lower())


class ParameterBoolean(Parameter):
    __tablename__ = 'parameter_boolean'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    reg_id = Column(Integer)
    service_id = Column(Integer, ForeignKey('service.id'))

    __mapper_args__ = {
        'polymorphic_identity': 'parameter_boolean',
        'concrete': True
    }


    def __init__(self, name, reg_id):
        super(ParameterBoolean, self).__init__(name, reg_id)


association_table = Table('options', Base.metadata,
    Column('parameter_option_id', Integer, ForeignKey('parameter_option.id')),
    Column('option_id', Integer, ForeignKey('option.id'))
)


class ParameterOption(Parameter):
    __tablename__ = 'parameter_option'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    reg_id = Column(Integer)
    service_id = Column(Integer, ForeignKey('service.id'))

    options = relationship('Option', secondary=association_table)

    __mapper_args__ = {
        'polymorphic_identity': 'parameter_option',
        'concrete': True
    }


    def __init__(self, name, reg_id):
        super(ParameterOption, self).__init__(name, reg_id)
        self.options = []

    def add_option(self, option):
        self.options.append(option)

    def __repr__(self):
        return super(ParameterOption, self).__repr__() + ', opcoes: {}'.format(
            ', '.join([str(op) for op in self.options])
        )


class ParameterInteger(Parameter):
    __tablename__ = 'parameter_int'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    reg_id = Column(Integer)
    min_value = Column(Integer)
    max_value = Column(Integer)
    service_id = Column(Integer, ForeignKey('service.id'))

    __mapper_args__ = {
        'polymorphic_identity': 'parameter_int',
        'concrete': True
    }


    def __init__(self, name, reg_id, min_value, max_value):
        super(ParameterInteger, self).__init__(name, reg_id)
        self.min_value = min_value
        self.max_value = max_value

    def __repr__(self):
        return super(ParameterInteger, self).__repr__() + ' min value: {},\
        max value: {}'.format(self.min_value, self.max_value)


class ParameterFloat(Parameter):
    __tablename__ = 'parameter_float'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    reg_id = Column(Integer)
    min_value = Column(Float)
    max_value = Column(Float)
    service_id = Column(Integer, ForeignKey('service.id'))

    __mapper_args__ = {
        'polymorphic_identity': 'parameter_float',
        'concrete': True
    }


    def __init__(self, name, reg_id, min_value, max_value):
        super(ParameterFloat, self).__init__(name, reg_id)
        self.min_value = min_value
        self.max_value = max_value

    def __repr__(self):
        return super(ParameterFloat, self).__repr__() + ' min value: {}, max value: {}'.format(self.min_value, self.max_value)


class Option(Base):
    __tablename__ = 'option'

    id = Column(Integer, primary_key=True)
    name = Column(String)


    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return self.name
