# Eletrônica

A fim de garantir que o braço não realizará movimentos inesperados quando em modo manual, de muita importância se levarmos em consideração que ele pode receber comandos remotamente, o botão que controla o modo de operação atua diretamente no hardware, através dos pinos de Standby do driver TB6612FNG, utilizado para controlar os motores. Como se observa na Tabela \tb66, o nível lógico baixo (L) nesse pino (STBY) desliga as saídas, de maneira que o motor passa a girar livremente e não oferece nenhum risco. Desse modo, a segurança do modo manual não será comprometida por falhas de código, visto mesmo a indicação visual está atrelada diretamente ao botão e não é influenciada pelo microcontrolador.

