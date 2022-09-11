A biblioteca Logger é uma lib escrita em C++ muito simples de usar e muito flexível. Ela permite que mensagens de log sejam enviados para mais de um lugar ao mesmo tempo. Você, por exemplo, enviar a mesma mensagem para um arquivo e para o stdout. A biblioteca de log consegue isso através de drivers, quesão especificados na instanciação da classe.

Cada driver trata-se de uma classe que deve dar umd estino adequado para as mensgens de log. A biblioteca, atualemente, contra com três drivers embutidos: Um para escrever no stdou, um para escrever em arquivo e um para a interceptação das mensagens via funções lambada.

Você pode tanto usar o driver lambda para dar um destino personalizado as informações quanto escrever o seu próprio driver para isso. Isso será visto mais abaixo.

A forma mais simples de usar a biblioteca é simplesmente instanciando-a e especificando um ou mais drivers para ela.
No exemplo abaixo, cria-se uma instancia da biblioteca que escreve dados no stdout.


```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.info("main", "this is a log");
```

na linha 1 há a instancia da classe Logger.
na linha 2, ainda na instância da classe Logger, informamos o driver padrão para escrita no stdout.
na linha 5, usamos o método Info para gerar um log de informação. Veremos mais sobre os métodos de log abaixo.

Bom! Feito as apresentações iniciais, abaixo há alguns tópicos interessantes sobre como usar e aproveitar melhor a biblioteca.

# Funções para log de dados

A biblioteca Logger usa algumas funções para facilitar o log das informações. São elas: log, debug, info, warning error e critical. Todas essas funções recebem uma string ou um vetor de strings com os dados que serão salvos nos logs. Há também uma versão para cada uma dessas funções que utilizam o tipo DynamicVar, permitindo uma flexibilidade maior na hora de gerar os logs. O tipo DynamicVar é uma classe que possui um repositório a parte e você poode saber mais sobre ela aqui: http....

## log function
está é a principal função para o log de dados e recebe um logLevel, que é um int indicando o seu nível de log, um nome para o seu log e o dado (ou vetor de dados) a serem guardados.

> Note sobre o log level
> 
> Nota sobre o log name



## debug function

## info function

## warning function

## error function

## critical function
