# proxy_client-server (Sorket Programming)
proxy_client_server using c



### 개발환경(Development Environment)
- OS: Linux Ubuntu
- IDE: VIM
- Language: C
- browser: crome

### 사용법(Usage)
- use makefile(type in make in terminal)
- create myserver file
- ./myserver 3333(port number)
- run browser
- ip:port number/test.html

### design

1) 컴파일을 하고 실행 하게 되면, 처음 입력 받은 port넘버로 서버 소켓을 만든다.
2) 소켓 주소와 소켓을 bind 한 후, listen을 통해서 client의 신호를 기다린다.
3) 신호가 오면 accept를 통해 해당 client의 정보를 받고 client와 연결한다.
4) client가 보낸 http request header를 파싱하여, http response를 만들도록 한다. 
- 또한, client가 요구하는 파일 정보를 파싱하여, 서버가 찾을 수 있도록 한다.
5) 파싱은 ‘response_generator’ 라는 함수를 만들어서 진행한다.
6) ‘response_generator’는 client 소켓 주소와 파일 이름을 인자로 받는다. 이 함수는 해당 파일을 열고, 파일이 있는지 찾는다. 
7) 그 후, 파일의 유무에 따라 if문을 통해 각 상황에 맞는 버퍼에 http response 정보를 담는다.
8) 파일의 타입은 ‘find_content_type’이라는 함수를 통해 파일이름을 파싱하여 알아낸다.
9) http request 정보는 header 버퍼에 담는다.
10) 그 후, 인자로 받았던 client 소켓 주소를 이용하여 client로 http request와 파일을 전송한다. 
