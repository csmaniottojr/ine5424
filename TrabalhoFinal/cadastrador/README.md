# Cadastrador de objetos inteligentes

### Instalação

##### Configuração do banco de dados

```sh
$ mysql -u <username> -p sodb_novo < dump_model.sql
```

##### Instalação das dependências

```sh
$ sudo apt-get install python3
$ sudo apt install python3-pip
$ sudo pip3 install virtualenv
$ virtualenv -p python3 venv
$ pip3 install -r requirements.txt
$ cd src
$ python3 main.py
```

##### Execução

```sh
$ cd src
$ python3 main.py
```
