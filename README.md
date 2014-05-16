# Chating v0
#### Visual Studio 2012 C++ project

###### ChatingServer

Server command line
```
server>start
_ (waiting for client connection)
```

**Release server**
```
Project properties -> Configuration properties -> C/C++ -> General -> Additional Include Directories -> ../Core;../Logic;../TUI
```

###### ChatingDll

DLL for client use

Note: only *Autenticar* is implemented for testing.
```cpp
int Autenticar(const TCHAR *login, const TCHAR *pass);
int LerListaUtilizadores(UTILIZADOR *utilizadores);
int LerListaUtilizadoresRegistados(UTILIZADOR *utilizadores);
int IniciarConversa(const TCHAR *utilizador);
int DesligarConversa();
int EnviarMensagemPrivada(const TCHAR *msg);
int EnviarMensagemPublica(const TCHAR *msg);
CHAT LerInformacaoInicial();
MENSAGEM LerMensagensPublicas();
MENSAGEM LerMensagensPrivadas();
int Sair();
int Desligar();
```

**Debugging the ChatingDll.dll**
```
Project properties -> Configuration properties -> Debugging -> Command -> Browse... Chating.exe
```

###### Chating

Client: use DLL

Testing with server running and dll updated:
```
execute Chating.exe
```

```
server>start
Login approved for Admin
Client disconnected
```

###### Core

Core classes

###### Logic

Logic classes

###### TUI

Text User Interface classes
