//Exemplo de Gateway
/*
Um Gateway linux é um computador que se comunicará com o ambiente de internet
das coisas composto por eMotes(esta plaquinha na sua mão).

Este Gateway se comunicará com os eMotes de várias maneiras:
-> Recebendo dados de sensores
-> Enviando mensagens para os eMotes
-> Enviando dados para um banco de dados

Para receber mensagens vindas de eMotes e comunicar com os mesmos, o Gateway
utiliza um eMote conectado em uma de suas portas USB.

A maneira mais simples de conectar o Gateway com um eMote é através de sua
porta serial, utilizando o protocolo  TSTP (apesar de você poder usar o
protocolo que preferir).
*/

/*UTILIZANDO O TSTP
    O TSTP é o protocolo IoT padrão para comunicar entre o eMote e o Gateway.
Uma aplicação EPOS totalmente funcional está disponível no repositório do
EPOS2: Você precisa carregar esta aplicação (este programa aqui) no eMote
que será conectado no gateway.
    A implementação padrão do Gateway Linux (que utiliza TSTP também) pode ser
baixada do repositório
    $ svn checkout https://svn.lisha.ufsc.br/openepos/epos2/trunk/tools/gateway_daemon/ gateway_daemon/
    Agora, o Gateway linux está disponível na pasta gateway_daemon. Após este
download, você precisa instalar as dependências e o própio Gateway. Para
fazer isso, execute os seguintes comandos:

    $ mkdir log
    $ python3 -m venv venv
    $ source venv/bin/activate
    $ pip3 install

E para inicializar o Gateway:

    $ source venv/bin/activate
    $ python3 -u -m gateway_daemon >> log/lehder_server_out.log 2>> log/lehder_server_err.log

    Os logs gerados pelo Gateway serão armazenados na pasta log. enquanto os
erros serão armazenados em log/lehder_server_err.log.
    Apesar de você ter baixado uma versão funcional do Gateway Linux, ele não
irá funcionar corretamente, pois é esperado que esta versão envie os dados re-
cebidos através de uma API HTTP. Para configurar o Gateway Linux junto com
um banco de dados, visite

    http://epos.lisha.ufsc.br/IoT+with+EPOS#Setting_up_the_database


*/



#include <utility/ostream.h>
#include <include/utility/random.h> //Para gerar números aleatórios

using namespace EPOS

const TSTP::Time DATA_PERIOD = 10 * 1000000;
const TSTP::Time DATA_EXPIRY = 2 * DATA_PERIOD;
const TSTP::Time INTEREST_EXPIRY = 5 * 60 * 1000000;

OStream cout;

int main()
{
    while(true)
        static int randomNumber = Random.random();

    return 0;
}
