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

A biblioteca Logger usa algumas funções para facilitar o log das informações. São elas: log, debug, info, warning error e critical. Todas essas funções recebem uma string ou um vetor de strings com os dados que serão salvos nos logs. Há também uma versão para cada uma dessas funções que utilizam o tipo DynamicVar, permitindo uma flexibilidade maior na hora de gerar os logs. O tipo DynamicVar é uma classe que possui um repositório a parte e você poode saber mais sobre ela aqui: https://github.com/rafael-tonello/DynamicVar

## log function
está é a principal função para o log de dados e recebe um logLevel, que é um int indicando o seu nível de log, um nome para o seu log e o dado (ou vetor de dados) a serem guardados.

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.log(LOGGER_LOGLEVEL_INFO, "examples", "an example log");
//all vector data will be merge in one log line
logger.log(LOGGER_LOGLEVEL_INFO, "examples", {"a ", "vector", "of", "data"});
logger.log(LOGGER_LOGLEVEL_INFO, "a log with default log name");
```

> Note sobre o log level: O log level é um número inteiro que indica a criciticade (ou importancia) do log. A bibiote Logger já possui 5 níveis de log padrão, sendo eles: Debug (log level 1), Info (log level 2), Warning (log level 3), Error (log level 4) e Critical (log level 4). Os drivers, normalmente, recebem um log level na su construção, indicando que apenas log levels maiores ou iguais à ele serão considerados como válidos e o restante será descartado.
> 
> Nota sobre o log name: O log name é um nome que é utilizado para identificar módulos e grupos de logs. Ele é, normalmente, adicionado pelos drivers no inicio das linhas de log, juntamente com a data e hora e o nível do log. Veja o exemplo abaixo para entender melhor o que é o nome do log.
> ```log
> [2022-05-01T00:02:03-0300] [INFO] [HttpServerService] new data requested to the server
> ```
> No exemplo acima, o log name é "HttpServerService"



## debug function
A função "debug" é uma função que facilita a geração de logs com o level 1 (LOGGER_LOGLEVEL_DEBUG).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.debug("examples", "an example log");
//all vector data will be merge in one log line
logger.debug("examples", {"a ", "vector", "of", "data"});
logger.debug("a log with default log name");
```

## info function
A função "info" é uma função que, assim como a função debug, facilita a geração de logs, porém com o level 2 (LOGGER_LOGLEVEL_INFO).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.info("examples", "an example log");
//all vector data will be merge in one log line
logger.info("examples", {"a ", "vector", "of", "data"});
logger.info("a log with default log name");
```

## warning function
A função "warning" é uma função que facilita a geração de logs com o level 3 (LOGGER_LOGLEVEL_WARNING).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.warning("examples", "an example log");
//all vector data will be merge in one log line
logger.warning("examples", {"a ", "vector", "of", "data"});
logger.warning("a log with default log name");
```

## error function
A função "error" é uma função que facilita a geração de logs com o level 4 (LOGGER_LOGLEVEL_ERROR).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.error("examples", "an example log");
//all vector data will be merge in one log line
logger.error("examples", {"a ", "vector", "of", "data"});
logger.error("a log with default log name");
```

## critical function
A função "critical" é uma função que facilita a geração de logs com o level 5 (LOGGER_LOGLEVEL_CRITICAL).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.critical("examples", "an example log");
//all vector data will be merge in one log line
logger.critical("examples", {"a ", "vector", "of", "data"});
logger.critical("a log with default log name");
```
A função critical possui um parâmetro adicional e opcional (raiseException) que permite lançar automaticamente uma exceção assim que o log for feito. A excessão lançada é uma "runtime_error" que contem como "wath" o próprio texto do log.

# TODO LISTS
## general
  [ ] critical function must wait internal cache be clear before raise the exception