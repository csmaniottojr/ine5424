#include "IoT/objects/callback.h"
#include "cheats/led.h"
#include "framework/main.h"
#include "IoT/objects/parameter.h"
#include "IoT/objects/parameter_boolean.h"
#include "IoT/objects/smartobject.h"
#include <utility/random.h>


using namespace EPOS;
using namespace IoT;
EPOS::OStream cout;

/*
 *A geladeira vai ter os seguintes parametros:
 *1) Temperatura atual (float) 
 *2) Potencia (Combo)
 *3) Turbo (bool)
 */

//Data variables
bool * turbo = new bool( false );
float * temperatura_atual = new float( 15 );
int * potencia_atual = new int ( 0 );
int indexes_potencia[3];

enum Potencias
{
    BAIXA = 0,
    MEDIA = 1,
    ALTA = 2

} ;

//=========================FUNCOES DE CALLBACK==================================
//Função a ser utilizada no callback da potencia

void atualizar_potencia( ) {
    cout << "[GELADEIRA]Potência recém atualizada com " << *potencia_atual << endl;
    //Aqui faz coisas relativas a potencia atual;
}

//Função a ser utilizada no callback do turbo

void atualizar_turbo ( ) {
    cout << "[GELADEIRA]Modo turbo " << ( *turbo ? "ATIVADO" : "DESATIVADO" ) << endl;
    //Aqui faz o que precisa pra ligar/desligar o turbo!
}

//Função a ser utilizada no callback da temperatura

void atualizar_temperatura( ) {
    cout << "[GELADEIRA]Lendo a temperatura interna da geladeira!" << endl;
    *temperatura_atual = Random::random( ) % 20;
}

//Função do servico

void configurar_geladeira( ) {
    cout << "Configurando a geladeira!" << endl;
    cout << ( *turbo ? "TURBO!" : "Operação normal" ) << endl;
    cout << "Compressor desativando a ";
    if ( *potencia_atual == indexes_potencia[Potencias::BAIXA] ) {
        cout << "22 graus!" << endl;
    } else if ( *potencia_atual == indexes_potencia[Potencias::MEDIA] ) {
        cout << "18 graus!" << endl;
    } else if ( *potencia_atual == indexes_potencia[Potencias::ALTA] ) {
        cout << "12 graus!" << endl;
    }
}
//==============================================================================

int main( ) {
    //Cria o objeto inteligente
    Smart_Object geladeira( "Geladeira" );
    //Cria um servico
    Service configura_geladeira( "Configurar Geladeira", new Callback( &configurar_geladeira ) );
    geladeira.add_service( &configura_geladeira );
    //Cria o parâmetro Turbo
    Parameter_Boolean ptTurbo( new Callback( &atualizar_turbo ), turbo );
    Parameter pTurbo ( "Turbo", 1, &ptTurbo );
    configura_geladeira.addParameter( &pTurbo );
    //Cria o parametro temperatura
    Parameter_Float ptTemperatura ( new Callback( &atualizar_temperatura ), temperatura_atual, -20, 40 );
    Parameter pTemperatura ( "Temperatura Atual", 2, &ptTemperatura );
    configura_geladeira.addParameter( &pTemperatura );
    //Cria o parametro potencia
    Parameter_Combo ptPotencia ( new Callback( &atualizar_potencia ), potencia_atual );
    indexes_potencia[Potencias::BAIXA] = ptPotencia.add_option( "Baixa" );
    indexes_potencia[Potencias::MEDIA] = ptPotencia.add_option( "Média" );
    indexes_potencia[Potencias::ALTA] = ptPotencia.add_option( "Alta" );
    Parameter pPotencia ( "Potencia", 3, &ptPotencia );
    configura_geladeira.addParameter( &pPotencia );
    //ETAPA DE CONFIGURAÇÃO COMPLETA
    //Agora vou simular algus comandos que podem chegar 
    //Chamou o serviço "Configurar geladeira", passando os parametros 
    //  turbo = false
    //  potencia = alta
    //Supondo que deu os gets no objeto->Serviço->Parametros, pra pegar os objs
    //ATUALIZANDO PARAMETROS!
    pTurbo.update( false );
    pPotencia.update( ( int ) 2 );
    configura_geladeira.actuate( );
    //Simulando que deu um "get" na temperatura atual
    float temp = pTemperatura.float_value( );
    //Agora digamos que por alguma razão eu queira verificar se a geladeira está em turbo
    bool turbo = pTurbo.bool_value( );









}